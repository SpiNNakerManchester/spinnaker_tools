//------------------------------------------------------------------------------
//! \file
//! \brief     Command handling for SC&MP
//!
//! \copyright &copy; The University of Manchester - 2009-2011
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009-2019 The University of Manchester
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "sark.h"
#include "scamp.h"

#include "spinn_net.h"

//------------------------------------------------------------------------------

//! Version command string for SC&MP

#define SCAMP_ID_STR            "SC&MP/SpiNNaker"

//------------------------------------------------------------------------------

//! \brief Generic nearest-neighbour message injection command handler.
//!
//! ```
//! arg1 = key
//! arg2 = data
//! arg3 = mark[31] : : fwd : retry
//! Bit 31 of arg3 causes an ID to be generated
//! ```
//! \param[in] msg: SCP message
//! \return zero; this command never has any payload in its response
uint cmd_nnp(sdp_msg_t *msg)
{
    ff_nn_send(msg->arg1, msg->arg2, msg->arg3, msg->arg3 & BIT_31);

    return 0;
}

//------------------------------------------------------------------------------

//! \brief Flood fill data command handler.
//!
//! ```
//! msg->arg1 = srce_fwd : srce_rty : 00 : ID
//! msg->arg2 = 00 : blk_num : word_cnt : 00
//! msg->arg3 = load_addr
//! msg->buf -> load data
//! ```
//! \param[in] msg: SCP message
//! \return zero; this command never has any payload in its response
// !! Multiply delay by some factor (2?) to avoid overload
uint cmd_ffd(sdp_msg_t *msg)
{
    uint fwd_rty = msg->arg1 >> 16;
    uint ID = msg->arg1 & 0xfe;
    uint sum = msg->arg3;

    uint key = (NN_CMD_FBS << 24) + (msg->arg2 & 0x00ffff00) + ID;

    ff_nn_send(key, sum, fwd_rty, 0);   //## Changed log 1->0 10may13

    uint words = ((msg->arg2 >> 8) & 255) + 1;

    key = (NN_CMD_FBD << 24) + (msg->arg2 & 0x00ff0000) + ID;

    uint *ptr = (uint *) msg->data;

    for (uint w = 0; w < words; w++) {
        uint data = *ptr++;
        sum += data;
        ff_nn_send(key + (w << 8), data, fwd_rty, 0);
    }

    key = (NN_CMD_FBE << 24) + (msg->arg2 & 0x00ff0000) + ID;

    ff_nn_send(key, -sum, fwd_rty, 0);

    return 0;
}

//------------------------------------------------------------------------------

//! \brief IPTag manipulation command handler.
//!
//! ```
//! arg1 = flags[11:8] : timeout : command : dest_port : tag
//! arg2 = dest_addr : port
//! arg3 = IP
//! ```
//! \param[in,out] msg: SCP message; will be updated with response code and
//!     payload
//! \param[in] srce_ip: Source IP address (needed in some modes)
//! \return the length of the response payload
uint cmd_iptag(sdp_msg_t *msg, uint srce_ip)
{
    uint op = (msg->arg1 >> 16) & 255;
    uint tag = msg->arg1 & 255;

    if (op > IPTAG_MAX || tag >= TAG_TABLE_SIZE) {
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    if (op == IPTAG_NEW || op == IPTAG_SET) {
        if (op == IPTAG_NEW) {
            tag = iptag_new();
        }

        if (tag != TAG_NONE) {
            iptag_t *tt = tag_table + tag;
            iptag_t *source_tag = tag_table + msg->tag;

            sark_word_set(tt, 0, sizeof(iptag_t));

            uint timeout = (msg->arg1 >> 24) & 15;
            uint flags = (msg->arg1 >> 20) & 0x0f00;

            if (timeout != 0) {
                timeout = 1 << (timeout - 1);
            }

            tt->timeout = timeout;
            tt->flags = flags |= timeout;

            tt->dest_addr = msg->arg2 >> 16;
            tt->dest_port = (msg->arg1 >> 8) & 255;

            if (msg->arg3 != 0) {
                uchar *ip_addr = (uchar *) &msg->arg3;
                copy_ip(ip_addr, tt->ip);
            } else if (! (flags & IPFLAG_REV)) {
                if (flags & IPFLAG_USE_SENDER) {
                    copy_ip(source_tag->ip, tt->ip);
                } else {
                    copy_ip((uchar *) &srce_ip, tt->ip);
                }
            }

            if (flags & IPFLAG_REV) {
                tt->rx_port = msg->arg2 & 0xffff;

                tt->flags |= IPFLAG_VALID;
            } else {
                if (flags & IPFLAG_USE_SENDER) {
                    tt->tx_port = source_tag->tx_port;
                } else {
                    tt->tx_port = msg->arg2 & 0xffff;
                }

                tt->flags |= IPFLAG_ARP;
                arp_lookup(tt);
            }
        }

        msg->arg1 = tag;

        return 4;
    } else if (op == IPTAG_GET) {
        iptag_t *tt = tag_table + tag;
        uint size = msg->arg2 * sizeof(iptag_t);

        if (size > SDP_BUF_SIZE) {
            msg->cmd_rc = RC_ARG;
            return 0;
        }

        sark_mem_cpy(&msg->arg1, tt, size);

        return size;
    } else if (op == IPTAG_TTO) {
        msg->arg1 = (TAG_FIXED_SIZE << 24) +
                (TAG_POOL_SIZE << 16) +
                (sizeof(iptag_t) << 8) +
                tag_tto;

        if (msg->arg2 < 16) {
            tag_tto = msg->arg2;
        }
        return 4;
    } else if (op == IPTAG_CLR) {
        tag_table[tag].flags = 0;
    }

    return 0;
}

//-------------------------------------------------------------------------

//! \brief Version command handler
//! \param[in,out] msg: SCP message; will be updated with response code and
//!     payload
//! \return the length of the response payload
uint cmd_ver(sdp_msg_t *msg)
{
    // Report P2P address of 255,255 until boot completes (as an indication
    // of system readiness)
    uint p2p_addr =
            ((netinit_phase == NETINIT_PHASE_DONE) &&
                    (ethinit_phase == ETHINIT_PHASE_DONE))
            ? sv->p2p_addr : 0xFFFF;

    msg->arg1 = (p2p_addr << 16) + (sark.phys_cpu << 8) + sark.virt_cpu;
    msg->arg2 = 0xffff0000 + SDP_BUF_SIZE;
    msg->arg3 = (uint) build_date;

    sark_str_cpy((char *) msg->data, SCAMP_ID_STR);
    sark_str_cpy((char *) msg->data + sizeof(SCAMP_ID_STR), SLLT_VER_STR);

    return 12 + sizeof(SCAMP_ID_STR) + sizeof(SLLT_VER_STR);
}

//-------------------------------------------------------------------------

//! \brief Link read command handler
// arg1=addr
// arg2=len
// arg3=link_num

//! \param[in,out] msg: SCP message, will be updated with result + payload
//! \return The length of the payload of \p msg
uint cmd_link_read(sdp_msg_t *msg)
{
    uint len = msg->arg2;
    uint link = msg->arg3;

    if (len > SDP_BUF_SIZE || link > NUM_LINKS - 1) {
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    uint addr = msg->arg1;
    uint *buf = (uint *) &msg->arg1;

    uint timeout = sv->peek_time;

    for (uint i = 0; i < len / 4; i++) {
        uint rc = link_read_word(addr, link, buf, timeout);
        if (rc != RC_OK) {
            msg->cmd_rc = rc;
            return 0;
        }

        addr += 4;
        buf += 1;
    }

    return len;
}

//------------------------------------------------------------------------------

//! \brief Link write command handler
// arg1=addr
// arg2=len
// arg3=link_num

//! \param[in,out] msg: SCP message, will be updated with result
//! \return Zero; no payload
uint cmd_link_write(sdp_msg_t *msg)
{
    uint len = msg->arg2;
    uint link = msg->arg3;

    if (len > SDP_BUF_SIZE || link > NUM_LINKS - 1) {
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    uint addr = msg->arg1;
    uint *buf = (uint *) msg->data;

    uint timeout = sv->peek_time;

    for (uint i = 0; i < len / 4; i++) {
        uint rc = link_write_word(addr, link, buf, timeout);
        if (rc != RC_OK) {
            msg->cmd_rc = rc;
            return 0;
        }

        addr += 4;
        buf += 1;
    }

    return 0;
}

//------------------------------------------------------------------------------

//! \brief Application start command handler
//! \param[in] msg: SCP message
//! \return Zero; no payload
uint cmd_as(sdp_msg_t *msg)
{
    proc_start_app(msg->arg1, msg->arg2);

    return 0;
}


//------------------------------------------------------------------------------

//! \brief Application reset command handler
//! \param[in,out] msg: SCP message, will be updated with result
//! \return Zero; no payload
uint cmd_ar(sdp_msg_t *msg)
{
    uint id_op_mask = msg->arg1;

    if (id_op_mask & 1) {               // Can't reset monitor
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    proc_start_app((uint) sv->sdram_sys, id_op_mask);

    return 0;
}


//------------------------------------------------------------------------------

//! \brief Router control command handler
// arg1 = (count << 16) + (app_id << 8) + op

// op = 0 - initialise
// op = 1 - clear (entry=arg2, count)
// op = 2 - load (addr=arg2, count, offset=arg3, app_id)
// op = 3 - set/get Fixed Route register (arg2 = route)
//          if bit 31 of arg1 set then return FR reg else set it

//! \param[in,out] msg: SCP message, will be updated with result + payload
//! \return The length of the payload of \p msg
uint cmd_rtr(sdp_msg_t *msg)
{
    uint op = msg->arg1 & 255;
    uint app_id = (msg->arg1 >> 8) & 255;
    uint count = msg->arg1 >> 16;

    if (op == 0) {
        rtr_mc_init(1);
    } else if (op == 1) {
        if (! rtr_mc_clear(msg->arg2, count)) {
            msg->cmd_rc = RC_ARG;
        }
    } else if (op == 2) {
        rtr_entry_t *table = (rtr_entry_t *) msg->arg2;
        uint offset = msg->arg3;

        if (! rtr_mc_load(table, count, offset, app_id)) {
            msg->cmd_rc = RC_ARG;
        }
    } else if (op == 3) {
        if (msg->arg2 & BIT_31) {
            msg->arg1 = rtr_fr_get();
            return 4;
        }
        rtr_fr_set(msg->arg2);
    } else {
        msg->cmd_rc = RC_ARG;
    }

  return 0;
}

//------------------------------------------------------------------------------

//! \brief General chip information command handler
//!
//! Get information about this chip. Intended to support a host probing the
//! machine for its basic information.
//!
//! No input arguments expected.
//!
//! The response will contain arg1-3 as desribed below with an additional data
//! payload indicating the application states of all cores.
//! * arg1:
//!   * Bits  4:0  - The number of working cores (including the monitor)
//!   * Bits 13:8  - A bitmap of links, 1 if responding correctly to PEEK of
//!                  Chip ID in system controller, 0 otherwise. This check is
//!                  performed on demand.
//!   * Bits 24:14 - The number of routing table entries in the
//!                  largest free block.
//!   * Bit 25     - 1 if Ethernet is up, 0 otherwise.
//!   * Bits 31:26 - Undefined
//! * arg2: The size (in bytes) of the largest free block in the SDRAM heap
//! * arg3: The size (in bytes) of the largest free block in the SysRAM heap
//!
//! The data payload consists of (in order):
//! * an 18-byte block which gives the cpu_state_e of each application core with
//!   byte 0 containing core 0's state and so-on.
//! * a short giving the P2P address of the closest chip with an active Ethernet.
//!   Note that this is the chip to which SDP packets will be sent to from this
//!   chip when they contain a destination address indicating that they should be
//!   sent over Ethernet.
//! * a 4-byte block containing the IPv4 address of the Ethernet connection on
//!   this chip, made up of the 4 segments of the IPv4 address, each between
//!   0 and 255.  This might equate to an address of 0.0.0.0 if this chip is not
//!   physically connected to the Ethernet port of a board.
//!
//! \param[in,out] msg: SCP message, will be updated with result + payload
//! \return The length of the payload of \p msg
uint cmd_info(sdp_msg_t *msg)
{
    // Get number of working CPUs
    msg->arg1 = sv->num_cpus & 0x0000001F;

    // Get number of free multicast routing table entries
    msg->arg1 |= rtr_alloc_max() << 14;

    // Is this chip's Ethernet connection up?
    msg->arg1 |= (!!(sv->eth_up)) << 25;

    // Get working link bitmap
    uint timeout = sv->peek_time;
    uint local_chip_id = sc[SC_CHIP_ID];
    for (uint link = 0; link < NUM_LINKS; link++) {
        // A link is determined working if: a link read of the remote system
        // controller's chip ID returns and the chip ID matches this chip's
        // ID (i.e. the remote chip is the same type of chip as this one!).
        // Mark "disabled" links as not working.
        if (link_en & (1 << link)) {
            uint remote_chip_id;
            uint rc = link_read_word((uint) (sc + SC_CHIP_ID), link,
                    &remote_chip_id, timeout);
            if (rc == RC_OK && remote_chip_id == local_chip_id) {
                msg->arg1 |= 1 << (link + 8);
            }
        }
    }

    // Get largest free block in SDRAM
    msg->arg2 = sark_heap_max(sv->sdram_heap, ALLOC_LOCK);

    // Get largest free block in SysRAM
    msg->arg3 = sark_heap_max(sv->sysram_heap, ALLOC_LOCK);

    // Add core states to the message
    uchar *buf = (uchar*) &(msg->data);
    for (uint core = 0; core < NUM_CPUS; core++) {
        *(buf++) = sv_vcpu[core].cpu_state;
    }

    // Add the nearest Ethernet P2P id
    *(buf++) = sv->eth_addr & 0xFF;
    *(buf++) = (sv->eth_addr >> 8) & 0xFF;

    // Add the Ethernet IP address
    *(buf++) = sv->ip_addr[0];
    *(buf++) = sv->ip_addr[1];
    *(buf++) = sv->ip_addr[2];
    *(buf++) = sv->ip_addr[3];

    // Add the link followed to get to root as this is used in signalling
    uint word = p2p_root >> P2P_LOG_EPW;
    uint offset = P2P_BPE * (p2p_root & P2P_EMASK);
    uint p2p_root_data = rtr_p2p[word];
    ushort link_root = (p2p_root_data >> offset) & 0x7;
    // We only need the first byte of the data as the second will always be 0
    // (We use a short here because the packet length has to be even)
    *(buf++) = link_root;
    *(buf++) = 0;

    // Returned packet size
    return 12 + 18 + 2 + 4 + 2;
}

//------------------------------------------------------------------------------

extern level_t levels[4];
extern uint pkt_tx(uint tcr, uint data, uint key);
extern void return_msg(sdp_msg_t *msg, uint rc);

//! State "counting" coalescing mode
enum state_coalesce_mode {
    MODE_OR =       0,  //!< Combine using bitwise OR (any in state?)
    MODE_AND =      1,  //!< Combine using bitwise AND (all in state?)
    MODE_SUM =      2,  //!< Combine using ADD
    MODE_3 =        3   //!< unused
};

//! Operation to use for P2P_LEVEL messages
enum send_reg_ctrl {
    APP_RET =       0,  //!< This is a RETURN (from APP_STAT)
    APP_STAT =      1,  //!< This is a request for statistics/count
    APP_SIG =       2,  //!< This is a request to signal cores
    APP_3 =         3   //!< unused
};

//! \brief Send a P2P packet encoding a value sent in response.
//! \param[in] ctrl: The packet control data (::send_reg_ctrl << 22)
//! \param[in] addr: The packet target address (::ushort)
//! \param[in] data: The packet payload information
static void p2p_send_reg(uint ctrl, uint addr, uint data)
{
    data |= ctrl;
    data |= chksum_32(data);
    addr |= P2P_LEVEL << 16;

    (void) pkt_tx(PKT_P2P_PL, data, addr);
}

//! \brief Timer callback, used to finish cmd_sig() call for state counting.
//! \param[in,out] arg1: SDP message
//! \param[in] level: Which counter are we reporting the value of?
static void proc_ret_msg(uint arg1, uint level)
{
    sdp_msg_t *msg = (sdp_msg_t *) arg1;
    msg->arg1 = levels[level].result;
    msg->length = sizeof(cmd_hdr_t);

    // check that replies actually arrived
    if (levels[level].rcvd != 0) {
        return_msg(msg, 0);
    } else {
        return_msg(msg, RC_P2P_NOREPLY);
    }

    //NB: clear result just in case
    levels[level].result = 0;
}

//! \brief Callback for proc_send(), gathering the results from sending and
//!     passing them on to our parent SCAMP.
//! \param[in] level: Which level are we processing at
//! \param[in] mode: What mode were we operating in
static void proc_gather(uint level, uint mode)
{
    //  uint rcvd = levels[level].rcvd;
    uint srce = levels[level].parent;
    uint d = (mode << 20) + ((level - 1) << 26) + levels[level].result;

    p2p_send_reg(APP_RET << 22, srce, d);

    //NB: clear result just in case
    levels[level].result = 0;
}

//! \brief Callback for p2p_region(), distributing requests to other SCAMPs
//! \param[in] data: The description of exactly what is to be done
//! \param[in] mask: Which bits are relevant
static void proc_send(uint data, uint mask)
{
    uint level = (data >> 26) & 3;
    uint mode = (data >> 20) & 3;

    data &= ~(3 << 26);
    data |= ((level + 1) & 3) << 26;

    levels[level].sent = 0;
    levels[level].rcvd = 0;
    levels[level].result = (mode == MODE_AND) ? (1 << 16) : 0;

    for (uint i = 0; i < 16; i++) {
        if (mask & (1 << i)) {
            uint valid = levels[level].valid[i];

            if (valid) {
                uint addr = levels[level].addr[i];

                levels[level].sent++;
                p2p_send_reg(data & (3 << 22), addr, data); //##
            }
        }
    }

    // schedule the sending of the return message in plenty of time
    if (mask & 0xffff0000) {
        timer_schedule_proc(proc_gather, level, mode, 500 * (4 - level));
    }
}

//! \brief Callback for p2p_region() when statistics are being collected,
//!     used so that low-priority processing does not block a high-priority
//!     interrupt.
//! \param[in] data: The description of exactly what is to be calculated
//! \param[in] srce: Where the data came from
static void proc_process(uint data, uint srce)
{
    uint mode = (data >> 20) & 3;
    uint app_id = data & 255;
    uint app_mask = (data >> 8) & 255;
    uint state = (data >> 16) & 15;
    uint mask = 0;

    for (uint i = 1; i < num_cpus; i++) {
        uint b = (core_app[i] & app_mask) == app_id;
        mask |= b << i;
    }

    uint result = 0;
    if (mode == MODE_SUM) {
        for (uint i = 1; i < num_cpus; i++) {
            if ((mask & (1 << i)) && (sv_vcpu[i].cpu_state == state)) {
                result++;
            }
        }
    } else if (mode == MODE_OR) {
        for (uint i = 1; i < num_cpus; i++) {
            if ((mask & (1 << i)) && (sv_vcpu[i].cpu_state == state)) {
                result = 1 << 16;
            }
        }

        result++;
    } else {            // MODE_AND
        result = 1 << 16;

        for (uint i = 1; i < num_cpus; i++) {
            if ((mask & (1 << i)) && (sv_vcpu[i].cpu_state != state)) {
                result = 0;
            }
        }

        result++;
    }

    uint d = (mode << 20) + (3 << 26) + result;
    p2p_send_reg(APP_RET << 22, srce, d);
}

//! \brief Handler for P2P Region packets (those with type ::P2P_LEVEL)
//! \param[in] data: The payload from the packet
//! \param[in] srce: The sender address (lower 16 bits) from the packet
void p2p_region(uint data, uint srce)
{
    uint t = chksum_32(data);
    if (t != 0) {
        return;
    }

    uint cmd = (data >> 22) & 3;
    uint level = (data >> 26) & 3;
    uint mode = (data >> 20) & 3;

    if (cmd == APP_RET) {
        data &= 0x000fffff;     // trim to 20 bits

        if (mode == MODE_OR) {
            levels[level].result += data & 0xffff;
            levels[level].result |= data & (1 << 16);
        } else if (mode == MODE_AND) {
            uint count = (levels[level].result + data) & 0xffff;
            uint bit = levels[level].result & data & (1 << 16);
            levels[level].result = bit | count;
        } else {
            levels[level].result += data;
        }

        levels[level].rcvd++;
        return;
    }

    if (level != 0) {
        levels[level].parent = srce;
        data &= 0x0fffffff;

        // schedule the sending of packets to subregions
        event_queue_proc(proc_send, data, 0xffffffff, PRIO_0);

        return;
    }

    // Level == 0 - process packet

    uint result = 1;

    if (cmd == APP_STAT) {
        // schedule the processing of the packet
        event_queue_proc(proc_process, data, srce, PRIO_0);
        return;
    } else if (cmd == APP_SIG) {
        signal_app(data);
    }
    // else     // APP_3

    uint d = (mode << 20) + (3 << 26) + result;
    p2p_send_reg(APP_RET << 22, srce, d);
}

//! The types of messages to use to send signals
enum signal_type {
    SIG_TYPE_MC = 0,    //!< Use multicast packets (standard signals)
    SIG_TYPE_P2P = 1,   //!< Use P2P packets (state counting)
    SIG_TYPE_NN = 2     //!< Use nearest neighbour packets (flood machine)
};

//! \brief Signal command handler.
//! \param[in,out] msg: SCP message; will be updated with response code and
//!     payload
//! \return the length of the response payload
uint cmd_sig(sdp_msg_t *msg)
{
    uint type = msg->arg1;
    uint data = msg->arg2;

    if (type == SIG_TYPE_MC) {
        if (p2p_addr != sv->p2p_root) {
            msg->cmd_rc = RC_ARG;
            return 0;
        }
        pkt_tx(PKT_MC_PL, data, SCAMP_MC_SIGNAL_KEY);
    } else if (type == SIG_TYPE_P2P) {
        uint mask = msg->arg3;

        if (mask == 0) {
            msg->cmd_rc = RC_ARG;
            return 0;
        }

        uint level = (data >> 26) & 3;

        proc_send(data, mask);

        // check that packets were actually sent (i.e., chips are listening)
        if (levels[level].sent != 0) {
            // schedule the return message with plenty of time to finish
            timer_schedule_proc(proc_ret_msg, (uint) msg, level, 10000);

            // a 'wrong' message length indicates that the
            // return message should not be sent at this time
            return 0xffff0000;
        } else {
            msg->cmd_rc = RC_ROUTE;
            return 0;
        }
    } else if (type == SIG_TYPE_NN) {
        ff_nn_send((NN_CMD_SIG0 << 24) + 0x3f0000,
                (5 << 28) + data, 0x3f00, 1);
    } else {
        msg->cmd_rc = RC_ARG;
    }

    return 0;
}


//------------------------------------------------------------------------------

//! \brief Resource allocation command handler
// arg1 = (app_id << 8) + op

// op 0 - allocate SDRAM - arg2 = size, arg3 = tag
// op 1 - free SDRAM - arg2 = ptr
// op 2 - free SDRAM by ID
// op 3 - allocate router - arg2 = count
// op 4 - free router - arg2 = entry, arg3 = clear
// op 5 - free router by ID - arg2 = clear
// op 6 - return free bytes in SDRAM heap & largest block size
// op 7 - return block point by AppID and Tag

//! \param[in,out] msg: SCP message, will be updated with result + payload
//! \return The length of the payload of \p msg
uint cmd_alloc(sdp_msg_t *msg)
{
    uint op = msg->arg1 & 255;
    uint app_id = (msg->arg1 >> 8) & 255;
    uint extra_flag = (msg->arg1 >> 16) & 255;
    if (op > ALLOC_MAX) {
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    switch (op) {
    case ALLOC_SDRAM:
        msg->arg1 = (uint) sark_xalloc(sv->sdram_heap,
                msg->arg2,
                msg->arg3,
                ALLOC_LOCK + ALLOC_ID + (app_id << 8) + extra_flag);
        sv->app_data[app_id].clean = 0;
        break;

    case FREE_SDRAM:
        sark_xfree(sv->sdram_heap, (void *) msg->arg2, ALLOC_LOCK);
        return 0;

    case FREE_SDRAM_ID:
        msg->arg1 = sark_xfree_id(sv->sdram_heap, app_id, ALLOC_LOCK);
        break;

    case ALLOC_RTR:
        msg->arg1 = rtr_alloc_id(msg->arg2, app_id);
        sv->app_data[app_id].clean = 0;
        break;

    case FREE_RTR:
        rtr_free(msg->arg2, msg->arg3);
        return 0;

    case FREE_RTR_ID:
        msg->arg1 = rtr_free_id(app_id, msg->arg2);
        break;

    case SDRAM_SPACE:
        msg->arg1 = sv->sdram_heap->free_bytes;
        msg->arg2 = sark_heap_max(sv->sdram_heap, ALLOC_LOCK);
        return 8;

    case HEAP_TAG_PTR:
        msg->arg1 = (uint) sark_tag_ptr(msg->arg2 & 255, app_id);
        break;
    }

    return 4;
}

//------------------------------------------------------------------------------

//! \brief Core remap command handler
// May want to check all cores are idle?
//! \param[in,out] msg: SCP message, will be updated with result
//! \return Always zero; result never has a payload
uint cmd_remap(sdp_msg_t *msg)
{
    uint core = msg->arg1;
    uint flags = msg->arg2;

    uint virt_core, phys_core;

    if (core > NUM_CPUS) {
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    if (flags & 1) {                    // Arg is physical core
        virt_core = sv->p2v_map[core];
        phys_core = core;
    } else {                            // Arg is virtual core
        virt_core = core;
        phys_core = v2p_map[virt_core];
    }

    if (virt_core == 0 || virt_core >= num_cpus) {
        msg->cmd_rc = RC_ARG;
        return 0;
    }

    remap_phys_cores(1 << phys_core);
    return 0;
}


//------------------------------------------------------------------------------

//! \brief General debugging and control command handler
//! \details Delegates to:
//! * cmd_ver()
//! * sark_cmd_read()
//! * sark_cmd_write()
//! * sark_cmd_fill()
//! * sark_led_set()
//! * cmd_srom()
//! * cmd_link_read()
//! * cmd_link_write()
//! * cmd_ffd()
//! * cmd_iptag()
//! * cmd_nnp()
//! * cmd_as()
//! * cmd_ar()
//! * cmd_remap()
//! * cmd_alloc()
//! * cmd_sig()
//! * cmd_rtr()
//! * cmd_info()
//!
//! based on the sdp_msg_t::cmd_rc contents.
//! \param[in,out] msg: The SCP message. Will be updated with response.
//! \param[in] srce_ip: Source IP address. Only used for cmd_iptag()
//! \return the length of the _content_ in the response message. 0 means
//!     there's just a header
uint scamp_debug(sdp_msg_t *msg, uint srce_ip)
{
    uint len = msg->length;
    if (len < 24) {
        msg->cmd_rc = RC_LEN;
        return 0;
    }

    uint t = msg->cmd_rc;
    msg->cmd_rc = RC_OK;

    switch (t) {
    case CMD_VER:
        return cmd_ver(msg);
    case CMD_READ:
        return sark_cmd_read(msg);
    case CMD_WRITE:
        return sark_cmd_write(msg);
    case CMD_FILL:
        return sark_cmd_fill(msg);
    case CMD_LED:
        sark_led_set(msg->arg1);
        return 0;
    case CMD_SROM:
        return cmd_srom(msg);
    case CMD_LINK_READ:
        return cmd_link_read(msg);
    case CMD_LINK_WRITE:
        return cmd_link_write(msg);
    case CMD_FFD:
        return cmd_ffd(msg);
    case CMD_IPTAG:
        return cmd_iptag(msg, srce_ip);
    case CMD_NNP:
        return cmd_nnp(msg);
    case CMD_AS:
        return cmd_as(msg);
    case CMD_AR:
        return cmd_ar(msg);
    case CMD_REMAP:
        return cmd_remap(msg);
    case CMD_ALLOC:
        return cmd_alloc(msg);
    case CMD_SIG:
        return cmd_sig(msg);
    case CMD_RTR:
        return cmd_rtr(msg);
    case CMD_INFO:
        return cmd_info(msg);
    default:
        msg->cmd_rc = RC_CMD;
        return 0;
    }
}

//------------------------------------------------------------------------------
