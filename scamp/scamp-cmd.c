
//------------------------------------------------------------------------------
//
// scamp-cmd.c	    Command handling for SC&MP
//
// Copyright (C)    The University of Manchester - 2009-2011
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


#include "sark.h"
#include "scamp.h"

#include "spinn_net.h"

//------------------------------------------------------------------------------

// Version command string for SC&MP

#define SCAMP_ID_STR  		"SC&MP/SpiNNaker"

//------------------------------------------------------------------------------

// arg1 = key
// arg2 = data
// arg3 = mark[31] : : fwd : retry
// Bit 31 of arg3 causes an ID to be generated

uint cmd_nnp (sdp_msg_t *msg)
{
  ff_nn_send (msg->arg1, msg->arg2, msg->arg3, msg->arg3 & BIT_31);

  return 0;
}

//------------------------------------------------------------------------------

// arg1 = srce_fwd : srce_rty : 00 : ID
// arg2 = 00 : blk_num : word_cnt : 00
// arg3 = load_addr
// buf -> load data

// !! Multiply delay by some factor (2?) to avoid overload

uint cmd_ffd (sdp_msg_t *msg)
{
  uint fwd_rty = msg->arg1 >> 16;
  uint ID = msg->arg1 & 0xfe;
  uint sum = msg->arg3;

  uint key = (NN_CMD_FBS << 24) + (msg->arg2 & 0x00ffff00) + ID;

  ff_nn_send (key, sum, fwd_rty, 0);	//## Changed log 1->0 10may13

  uint words = ((msg->arg2 >> 8) & 255) + 1;

  key = (NN_CMD_FBD << 24) + (msg->arg2 & 0x00ff0000) + ID;

  uint *ptr = (uint *) msg->data;

  for (uint w = 0; w < words; w++)
    {
      uint data = *ptr++;

      sum += data;

      ff_nn_send (key + (w << 8), data, fwd_rty, 0);
    }

  key = (NN_CMD_FBE << 24) + (msg->arg2 & 0x00ff0000) + ID;

  ff_nn_send (key, -sum, fwd_rty, 0);

  return 0;
}

//------------------------------------------------------------------------------

// arg1 = flags[11:8] : timeout : command : dest_port : tag
// arg2 = dest_addr : port
// arg3 = IP

uint cmd_iptag (sdp_msg_t *msg, uint srce_ip)
{
  uint op = (msg->arg1 >> 16) & 255;
  uint tag = msg->arg1 & 255;

  if (op > IPTAG_MAX || tag >= TAG_TABLE_SIZE)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  if (op == IPTAG_NEW || op == IPTAG_SET)
    {
      if (op == IPTAG_NEW)
        tag = iptag_new ();

      if (tag != TAG_NONE)
        {
          iptag_t *tt = tag_table + tag;

          sark_word_set (tt, 0, sizeof (iptag_t));

          uint timeout = (msg->arg1 >> 24) & 15;
          uint flags = (msg->arg1 >> 20) & 0x0f00;

          if (timeout != 0)
            timeout = 1 << (timeout - 1);

          tt->timeout = timeout;
          tt->flags = flags |= timeout;

          tt->dest_addr = msg->arg2 >> 16;
          tt->dest_port = (msg->arg1 >> 8) & 255;

          if (msg->arg3 != 0)
            {
              uchar *ip_addr = (uchar *) &msg->arg3;
              copy_ip (ip_addr, tt->ip);
            }
          else if (! (flags & IPFLAG_REV))
            {
              copy_ip ((uchar *) &srce_ip, tt->ip);
            }

          if (flags & IPFLAG_REV)
            {
              tt->rx_port = msg->arg2 & 0xffff;

              tt->flags |= IPFLAG_VALID;
            }
          else
            {
              tt->tx_port = msg->arg2 & 0xffff;

              tt->flags |= IPFLAG_ARP;
              arp_lookup (tt);
            }
        }

      msg->arg1 = tag;

      return 4;
    }
  else if (op == IPTAG_GET)
    {
      iptag_t *tt = tag_table + tag;
      uint size = msg->arg2 * sizeof (iptag_t);

      if (size > SDP_BUF_SIZE)
        {
          msg->cmd_rc = RC_ARG;
          return 0;
        }

      sark_mem_cpy (&msg->arg1, tt, size);

      return size;
    }
  else if (op == IPTAG_TTO)
    {
      msg->arg1 = (TAG_FIXED_SIZE << 24) +
                  (TAG_POOL_SIZE << 16) +
                  (sizeof (iptag_t) << 8) +
                  tag_tto;

      if (msg->arg2 < 16)
        tag_tto = msg->arg2;

      return 4;
    }
  else // IPTAG_CLR
    {
      tag_table[tag].flags = 0;
    }

  return 0;
}

//------------------------------------------------------------------------------

uint cmd_ver (sdp_msg_t *msg)
{
  // Report P2P address of 255,255 until boot completes (as an indication of
  // system readiness)
  uint p2p_addr = (netinit_phase == NETINIT_PHASE_DONE) ? sv->p2p_addr
                                                        : 0xFFFF;

  msg->arg1 = (p2p_addr << 16) + (sark.phys_cpu << 8) + sark.virt_cpu;
  msg->arg2 = 0xffff0000 + SDP_BUF_SIZE;
  msg->arg3 = (uint) build_date;

  sark_str_cpy ((char *) msg->data, SCAMP_ID_STR);
  sark_str_cpy ((char *) msg->data + sizeof (SCAMP_ID_STR), SLLT_VER_STR);

  return 12 + sizeof (SCAMP_ID_STR) + sizeof (SLLT_VER_STR);
}

//------------------------------------------------------------------------------

// arg1=addr
// arg2=len
// arg3=link_num

uint cmd_link_read (sdp_msg_t *msg)
{
  uint len = msg->arg2;
  uint link = msg->arg3;

  if (len > SDP_BUF_SIZE || link > NUM_LINKS - 1)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  uint addr = msg->arg1;
  uint *buf = (uint *) &msg->arg1;

  uint timeout = sv->peek_time;

  for (uint i = 0; i < len / 4; i++)
    {
      uint rc = link_read_word (addr, link, buf, timeout);

      if (rc != RC_OK)
        {
          msg->cmd_rc = rc;
          return 0;
        }

      addr += 4;
      buf += 1;
    }

  return len;
}

//------------------------------------------------------------------------------

// arg1=addr
// arg2=len
// arg3=link_num

uint cmd_link_write (sdp_msg_t *msg)
{
  uint len = msg->arg2;
  uint link = msg->arg3;

  if (len > SDP_BUF_SIZE || link > NUM_LINKS - 1)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  uint addr = msg->arg1;
  uint *buf = (uint *) msg->data;

  uint timeout = sv->peek_time;

  for (uint i = 0; i < len / 4; i++)
    {
      uint rc = link_write_word (addr, link, buf, timeout);

      if (rc != RC_OK)
        {
          msg->cmd_rc = rc;
          return 0;
        }

      addr += 4;
      buf += 1;
    }

  return 0;
}

//------------------------------------------------------------------------------


uint cmd_as (sdp_msg_t *msg)
{
  proc_start_app (msg->arg1, msg->arg2);

  return 0;
}


//------------------------------------------------------------------------------


uint cmd_ar (sdp_msg_t *msg)
{
  uint id_op_mask = msg->arg1;

  if (id_op_mask & 1) // Can't reset monitor
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  proc_start_app ((uint) sv->sdram_sys, id_op_mask);

  return 0;
}


//------------------------------------------------------------------------------

// arg1 = (count << 16) + (app_id << 8) + op

// op = 0 - initialise
// op = 1 - clear (entry=arg2, count)
// op = 2 - load (addr=arg2, count, offset=arg3, app_id)
// op = 3 - set/get Fixed Route register (arg2 = route)
//          if bit 31 of arg1 set then return FR reg else set it

uint cmd_rtr (sdp_msg_t *msg)
{
  uint op = msg->arg1 & 255;
  uint app_id = (msg->arg1 >> 8) & 255;
  uint count = msg->arg1 >> 16;

  if (op == 0)
    {
      rtr_mc_init (1);
    }
  else if (op == 1)
    {
      if (! rtr_mc_clear (msg->arg2, count))
        msg->cmd_rc = RC_ARG;
    }
  else if (op == 2)
    {
      rtr_entry_t *table = (rtr_entry_t *) msg->arg2;
      uint offset = msg->arg3;

      if (! rtr_mc_load (table, count, offset, app_id))
        msg->cmd_rc = RC_ARG;
    }
  else if (op == 3)
    {
      if (msg->arg2 & BIT_31)
        {
          msg->arg1 = rtr_fr_get ();
          return 4;
        }
      rtr_fr_set (msg->arg2);
    }
  else
    {
      msg->cmd_rc = RC_ARG;
    }

  return 0;
}

//------------------------------------------------------------------------------

// Get information about this chip. Intended to support a host probing the
// machine for its basic information.
//
// An optional arg1 is taken:
// * If not specified or 0, basic information is returned
// * If specified, the bits indicate the items to be returned
//   * Bit  0    - set to include the link destinations
//   * Bit  1    - set to include the vcpu_t structure base address
//   * Bit  2    - set to include the router table copy addresses
//   * Bit  3    - set to include the nearest Ethernet p2p address
//   * Bit  4    - set to include the Ethernet IP address
//                 (if the Ethernet is up, otherwise this is ignored)
//   * Bit  5    - set to include the machine dimensions
//   * Bit  6    - set to include the size of an iobuf buffer
//   * Bits 7-31 - undefined
//
// The response will contain arg1-3 as described below with an additional data
// payload indicating the application states of all cores.  If requested, it
// will also contain further information.
// * arg1:
//   * Bits  4:0  - The number of working cores (including the monitor)
//   * Bits 13:8  - A bitmap of links, 1 if responding correctly to PEEK of
//                  Chip ID in system controller, 0 otherwise. This check is
//                  performed on demand.
//   * Bits 24-14 - The number of routing table entries in the
//                  largest free block.
//   * Bit  25    - 1 if Ethernet is up, 0 otherwise.
//   * Bit  26    - 1 if any extra information is included
//   * Bits 27-31 - undefined
// * arg2: The size (in bytes) of the largest free block in the SDRAM heap
// * arg3: The size (in bytes) of the largest free block in the SysRAM heap
//
// The data payload starts with an 18-byte block which gives the cpu_state_e
// of each application core with byte 0 containing core 0's state and so-on.
//
// If bit 26 is 0, no further data follows.  If bit 26 is 1, the next halfword
// of the data payload is a bit field that indicates the information contained
// in the rest of the data payload.  The items in order are:
// * If bit 0 is 1, a 6-halfword block which gives the p2p id of the chip down
//   each of the links, with halfword 0 containing link 0's p2p id and so on
//   (disabled links will contain a p2p id of 0)
// * If bit 1 is 1, a word containing the base address of the vcpu_t data
//   structure
// * If bit 2 is 1, a word containing the base address of the multicast router
//   table copy followed by a word containing the base address of the fixed
//   router table copy
// * If bit 3 is 1, a halfword containing the nearest Ethernet p2p id
// * If bit 4 is 1, a 4-byte block containing the Ethernet ip address
// * If bit 5 is 1, a halfword containing the machine dimensions
// * If bit 6 is 1, a word containing the size of an iobuf buffer
// * Bits 7-15 can be ignored. (Future implementation note: if any more than 15
//                              bits are required, use bit 15 to indicate the
//                              inclusion of another bit field following the
//                              rest of the data)

uint cmd_info (sdp_msg_t *msg)
{
  uint extra_info_flags = 0;
  if (msg->length >= 20)
    {
      extra_info_flags = msg->arg1;
    }

  // Get number of working CPUs
  msg->arg1 = sv->num_cpus & 0x0000001F;

  // Get number of free multicast routing table entries
  msg->arg1 |= rtr_alloc_max() << 14;

  // Is this chip's Ethernet connection up?
  msg->arg1 |= (!!(sv->eth_up)) << 25;

  // Get working link bitmap
  uint timeout = sv->peek_time;
  uint local_chip_id = sc[SC_CHIP_ID];
  for (uint link = 0; link < NUM_LINKS; link++)
    {
      // A link is determined working if: a link read of the remote system
      // controller's chip ID returns and the chip ID matches this chip's ID
      // (i.e. the remote chip is the same type of chip as this one!). Mark
      // "disabled" links as not working.
      if (link_en & (1 << link))
        {
          uint remote_chip_id;
          uint rc = link_read_word (
            (uint)(sc + SC_CHIP_ID), link, &remote_chip_id, timeout);
          if (rc == RC_OK && remote_chip_id == local_chip_id)
            {
            msg->arg1 |= 1 << (link + 8);
            }
        }
    }

  // Get largest free block in SDRAM
  msg->arg2 = sark_heap_max (sv->sdram_heap, ALLOC_LOCK);

  // Get largest free block in SysRAM
  msg->arg3 = sark_heap_max (sv->sysram_heap, ALLOC_LOCK);

  // Add core states to the message
  uchar *buf = (uchar*) &(msg->data);
  for (uint core = 0; core < NUM_CPUS; core++)
    {
      *(buf++) = sv_vcpu[core].cpu_state;
    }

  // The size so far is the 3 args plus the 18 cores
  uint size = 12 + 18;

  // If any additional information was requested, set bit 26 in arg1, and
  // add a word indicating what information is returned
  ushort* shortbuf = (ushort*) buf;
  ushort* return_flags = shortbuf++;
  if (extra_info_flags != 0)
    {
      msg->arg1 |= (1 << 26);
      *return_flags = 0;
      size += 2;
    }

  if ((extra_info_flags & 1) != 0)
    {

      *return_flags |= 1;

      // Add the link remote p2p ids to the message
      for (uint link = 0; link < NUM_LINKS; link++)
        {
          if (msg->arg1 & (1 << (link + 8)))
            {
              uint remote_chip_p2p = 0;
              uint rc = link_read_word (
                (uint)(sv), link, &remote_chip_p2p, timeout);
              if (rc == RC_OK)
                {
                  *(shortbuf++) = (ushort) (remote_chip_p2p & 0xFFFF);
                }
              else
                {
                  *(shortbuf++) = 0;
                }
            }
          else
            {
              *(shortbuf++) = 0;
            }
        }
        size += 12;
    }

  if ((extra_info_flags & (1 << 1)) != 0)
    {
      *return_flags |= (1 << 1);

      // Add the vcpu_t base address
      uint sv_cpu_addr = (uint) &sv_vcpu;
      *(shortbuf++) = (ushort) (sv_cpu_addr & 0xFFFF);
      *(shortbuf++) = (ushort) ((sv_cpu_addr >> 16) & 0xFFFF);
      size += 4;
    }

  if ((extra_info_flags & (1 << 2)) != 0)
    {
      *return_flags |= (1 << 2);

      // Add the multicast router copy address
      uint rtr_copy = (uint) sv->rtr_copy;
      *(shortbuf++) = (ushort) (rtr_copy & 0xFFFF);
      *(shortbuf++) = (ushort) ((rtr_copy >> 16) & 0xFFFF);
      size += 4;

      // Add the fixed router copy address
      uint fr_copy = (uint) &(sv->fr_copy);
      *(shortbuf++) = (ushort) (fr_copy & 0xFFFF);
      *(shortbuf++) = (ushort) ((fr_copy >> 16) & 0xFFFF);
      size += 4;
    }

  if ((extra_info_flags & (1 << 3)) != 0)
    {
      *return_flags |= (1 << 3);

      // Add the nearest Ethernet P2P id
      *(shortbuf++) = sv->eth_addr;
      size += 2;
    }

  if (sv->eth_up && ((extra_info_flags & (1 << 4)) != 0))
    {
      *return_flags |= (1 << 4);

      // Add the Ethernet IP address
      buf = (uchar*) shortbuf;
      *(buf++) = sv->ip_addr[0];
      *(buf++) = sv->ip_addr[1];
      *(buf++) = sv->ip_addr[2];
      *(buf++) = sv->ip_addr[3];
      size += 4;
      shortbuf = (ushort*) buf;
    }

  if ((extra_info_flags & (1 << 5)) != 0)
    {
      *return_flags |= (1 << 5);

      // Add the P2P dims
      msg->arg1 |= (1 << 27);
      *(shortbuf++) = sv->p2p_dims;
      size += 2;
    }

  if ((extra_info_flags & (1 << 6)) != 0)
    {
      *return_flags |= (1 << 6);

      // Add the iobuf size
      *(shortbuf++) = (ushort) (sv->iobuf_size & 0xFFFF);
      *(shortbuf++) = (ushort) ((sv->iobuf_size >> 16) & 0xFFFF);
      size += 4;
    }

  // Returned packet size
  return size;
}

//------------------------------------------------------------------------------

extern level_t levels[4];
extern uint pkt_tx (uint tcr, uint data, uint key);


#define MODE_OR  	0
#define MODE_AND 	1
#define MODE_SUM 	2
#define MODE_3   	3

#define APP_RET		0
#define APP_STAT	1
#define APP_SIG		2
#define APP_3		3


void p2p_send_reg (uint ctrl, uint addr, uint data)
{
  data |= ctrl;
  data |= chksum_32 (data);
  addr |= P2P_LEVEL << 16;

  (void) pkt_tx (PKT_P2P_PL, data, addr);
}


void proc_send (uint data, uint mask)
{
  uint level = (data >> 26) & 3;
  uint mode = (data >> 20) & 3;

  data &= ~(3 << 26);
  data |= ((level + 1) & 3) << 26;

  levels[level].sent = 0;
  levels[level].rcvd = 0;
  levels[level].result = (mode == MODE_AND) ? (1 << 16) : 0;

  for (uint i = 0; i < 16; i++)
    {
      if (mask & (1 << i))
        {
          uint valid = levels[level].valid[i];

          if (valid)
            {
              uint addr = levels[level].addr[i];

              levels[level].sent++;
              p2p_send_reg (data & (3 << 22), addr, data); //##
            }
        }
    }
}


void proc_gather (uint level, uint mode)
{
  //  uint rcvd = levels[level].rcvd;
  uint srce = levels[level].parent;
  uint d = (mode << 20) + ((level - 1) << 26) + levels[level].result;

  p2p_send_reg (APP_RET << 22, srce, d);
}


// Handler for P2P Region packets

void p2p_region (uint data, uint srce)
{
  uint t = chksum_32 (data);

  if (t != 0)
    return;

  uint cmd = (data >> 22) & 3;
  uint level = (data >> 26) & 3;
  uint mode = (data >> 20) & 3;

  if (cmd == APP_RET)
    {
      data &= 0x000fffff;	// trim to 20 bits

      if (mode == MODE_OR)
        {
          levels[level].result += data & 0xffff;
          levels[level].result |= data & (1 << 16);
        }
      else if (mode == MODE_AND)
        {
          uint count = (levels[level].result + data) & 0xffff;
          uint bit = levels[level].result & data & (1 << 16);
          levels[level].result = bit | count;
        }
      else
        levels[level].result += data;

      levels[level].rcvd++;

      return;
    }

  if (level != 0)
    {
      levels[level].parent = srce;
      data &= 0x0fffffff;
      timer_schedule_proc (proc_send, data, 0xffffffff, 5);
      timer_schedule_proc (proc_gather, level, mode, 200 * (4 - level));

      return;
    }

  // Level == 0 - process packet

  uint result = 1;

  if (cmd == APP_STAT)
    {
      uint app_id = data & 255;
      uint app_mask = (data >> 8) & 255;
      uint state = (data >> 16) & 15;
      uint mask = 0;

      for (uint i = 1; i < num_cpus; i++)
        {
          uint b = (core_app[i] & app_mask) == app_id;
          mask |= b << i;
        }

      result = 0;

      if (mode == MODE_SUM)
        {
          for (uint i = 1; i < num_cpus; i++)
            if ((mask & (1 << i)) && (sv_vcpu[i].cpu_state == state))
              result++;
        }
      else if (mode == MODE_OR)
        {
          for (uint i = 1; i < num_cpus; i++)
            if ((mask & (1 << i)) && (sv_vcpu[i].cpu_state == state))
              result = 1 << 16;

          result++;
        }
      else // MODE_AND
        {
          result = 1 << 16;

          for (uint i = 1; i < num_cpus; i++)
            if ((mask & (1 << i)) && (sv_vcpu[i].cpu_state != state))
              result = 0;

          result++;
        }
    }
  else if (cmd == APP_SIG)
    {
      signal_app (data);
    }
  // else	// APP_3

  uint d = (mode << 20) + (3 << 26) + result;
  p2p_send_reg (APP_RET << 22, srce, d);
}


uint cmd_sig (sdp_msg_t *msg)
{
  uint type = msg->arg1;
  uint data = msg->arg2;

  if (type == 0)
    {
      if (p2p_addr != sv->p2p_root)
        {
          msg->cmd_rc = RC_ARG;
          return 0;
        }
      pkt_tx (PKT_MC_PL, data, 0xffff5555);
    }
  else if (type == 1)
    {
      uint mask = msg->arg3;

      if (mask == 0)
        {
          msg->cmd_rc = RC_ARG;
          return 0;
        }

      uint level = (data >> 26) & 3;

      proc_send (data, mask);

      sark_delay_us (10000);

      msg->arg1 = levels[level].result;

      return 4;
    }
  else if (type == 2)
    {
      ff_nn_send ((NN_CMD_SIG0 << 24) + 0x3f0000,
                  (5 << 28) + data,
                  0x3f00,
                  1);
    }
  else
    {
      msg->cmd_rc = RC_ARG;
    }

  return 0;
}


//------------------------------------------------------------------------------

// arg1 = (app_id << 8) + op

// op 0 - allocate SDRAM - arg2 = size, arg3 = tag
// op 1 - free SDRAM - arg2 = ptr
// op 2 - free SDRAM by ID
// op 3 - allocate router - arg2 = count
// op 4 - free router - arg2 = entry, arg3 = clear
// op 5 - free router by ID - arg2 = clear
// op 6 - return free bytes in SDRAM heap & largest block size
// op 7 - return block point by AppID and Tag

uint cmd_alloc (sdp_msg_t *msg)
{
  uint op = msg->arg1 & 255;
  uint app_id = (msg->arg1 >> 8) & 255;

  if (op > ALLOC_MAX)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  switch (op)
    {
    case ALLOC_SDRAM:
      msg->arg1 = (uint) sark_xalloc (sv->sdram_heap,
                                      msg->arg2,
                                      msg->arg3,
                                      ALLOC_LOCK + ALLOC_ID + (app_id << 8));
      sv->app_data[app_id].clean = 0;
      break;

    case FREE_SDRAM:
      sark_xfree (sv->sdram_heap, (void *) msg->arg2, ALLOC_LOCK);
      return 0;

    case FREE_SDRAM_ID:
      msg->arg1 = sark_xfree_id (sv->sdram_heap, app_id, ALLOC_LOCK);
      break;

    case ALLOC_RTR:
      msg->arg1 = rtr_alloc_id (msg->arg2, app_id);
      sv->app_data[app_id].clean = 0;
      break;

    case FREE_RTR:
      rtr_free (msg->arg2, msg->arg3);
      return 0;

    case FREE_RTR_ID:
      msg->arg1 = rtr_free_id (app_id, msg->arg2);
      break;

    case SDRAM_SPACE:
      msg->arg1 = sv->sdram_heap->free_bytes;
      msg->arg2 = sark_heap_max (sv->sdram_heap, ALLOC_LOCK);
      return 8;

    case HEAP_TAG_PTR:
      msg->arg1 = (uint) sark_tag_ptr (msg->arg2 & 255, app_id);
      break;
    }

  return 4;
}

//------------------------------------------------------------------------------


// May want to check all cores are idle?

uint cmd_remap (sdp_msg_t *msg)
{
  uint core = msg->arg1;
  uint flags = msg->arg2;

  uint virt_core;
  uint phys_core;

  if (core > NUM_CPUS)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  if (flags & 1) // Arg is physical core
    {
      virt_core = sv->p2v_map[core];
      phys_core = core;
    }
  else //  Arg is virtual core
    {
      virt_core = core;
      phys_core = v2p_map[virt_core];
    }

  if (virt_core == 0 || virt_core >= num_cpus)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  remap_phys_cores(1 << phys_core);

  return 0;
}


//------------------------------------------------------------------------------


uint scamp_debug (sdp_msg_t *msg, uint srce_ip)
{
  uint len = msg->length;

  if (len < 24)
    {
      msg->cmd_rc = RC_LEN;
      return 0;
    }

  uint t = msg->cmd_rc;
  msg->cmd_rc = RC_OK;

  switch (t)
    {
    case CMD_VER:
      return cmd_ver (msg);

    case CMD_READ:
      return sark_cmd_read (msg);

    case CMD_WRITE:
      return sark_cmd_write (msg);

    case CMD_FILL:
      return sark_cmd_fill (msg);

    case CMD_LED:
      sark_led_set (msg->arg1);
      return 0;

    case CMD_SROM:
      return cmd_srom (msg);

    case CMD_LINK_READ:
      return cmd_link_read (msg);

    case CMD_LINK_WRITE:
      return cmd_link_write (msg);

    case CMD_FFD:
      return cmd_ffd (msg);

    case CMD_IPTAG:
      return cmd_iptag (msg, srce_ip);

    case CMD_NNP:
      return cmd_nnp (msg);

    case CMD_AS:
      return cmd_as (msg);

    case CMD_AR:
      return cmd_ar (msg);

    case CMD_REMAP:
      return cmd_remap (msg);

    case CMD_ALLOC:
      return cmd_alloc (msg);

    case CMD_SIG:
      return cmd_sig (msg);

    case CMD_RTR:
      return cmd_rtr (msg);

    case CMD_INFO:
      return cmd_info (msg);

    default:
      msg->cmd_rc = RC_CMD;
      return 0;
    }
}

//------------------------------------------------------------------------------
