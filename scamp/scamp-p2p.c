//------------------------------------------------------------------------------
//! \file
//! \brief     P2P packet handling for SC&MP
//!
//! \copyright &copy; The University of Manchester - 2009-2019
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "spinnaker.h"
#include "sark.h"
#include "scamp.h"

#include "spinn_net.h"

#define P2P_NUM_STR 8   //!< Number of streams

//! Possible states in P2P protocol: transmitter side
enum p2p_tx_state {
    TX_IDLE = 0,
    TX_OPEN_REQ = 1,
    TX_OPEN = 2,
    TX_RETRY = 3,
    TX_CLOSE = 4
};

//! Possible states in P2P protocol: receiver side
enum p2p_rx_state {
    RX_IDLE = 0,
    RX_OPEN = 1,
    RX_CLOSE_REQ = 2
};

//! P2P channel state structure: receiver
typedef struct rx_desc {
    uchar state;
    uchar seq_len;
    uchar done;
    uchar tid;

    uchar tcount;
    uchar ctrl;
    uchar phase;

    ushort mask;
    ushort new_mask;

    uint srce;

    uchar *base;
    uchar *limit;
    sdp_msg_t *msg;

    event_t* event;     //!< pre-allocated reused event
    uint event_id;
} rx_desc_t;

//! P2P channel state structure: transmitter
typedef struct tx_desc {
    uchar seq_len;
    uchar seq;
    uchar odd;
    uchar done;

    uchar state;
    uchar rid;
    uchar tid;
    uchar tcount;

    uchar rc;
    uchar phase;
    volatile uchar ack;

    ushort mask;

    uint dest;

    uchar *base;
    uchar *limit;

    event_t *event;     //!< pre-allocated reused event
    uint event_id;
} tx_desc_t;

//------------------------------------------------------------------------------

//! Transmitter state descriptor. Transmission is a foreground task
tx_desc_t tx_desc;

//! Receiver state descriptors. Reception is a background task
rx_desc_t rx_desc_table[P2P_NUM_STR];

//! Indices into ::p2p_stats
enum p2p_stats_indices {
    DATA_TO = 0,        //!< Timeouts waiting for data
    ACK_TO = 1,         //!< Timeouts waiting for acknowledgements
    OPEN_TO = 2,        //!< Timeouts during channel open (request)
    CLOSE_TO = 3,       //!< Timeouts during channel close

    P2P_SENDS = 4,      //!< Number of sends
    P2P_OPENS = 5,      //!< Number of opens
    P2P_BUSY1 = 6,      //!< Number of times busy due to no more streams
    P2P_BUSY2 = 7,      //!< Number of times busy due to no more SDP buffers

    P2P_REJECTS = 8,    //!< Number of times message was rejected
    TCOUNT = 9,         //!< Most recent value of tx_desc_t::tcount
    OPEN_DUP = 10,      //!< Number of times a channel was already open
    TX_FAIL = 11,       //!< Number of transmission failures
    OPEN_EVENT = 12,    //!< Timeouts during open (timeout handler)

    P2P_OPEN_N = 16     //!< Base for array of counters for tracking channel usage
};

//! Peer-to-peer statistics table
uint p2p_stats[32];

//------------------------------------------------------------------------------

//! Number of retries for an open request
const uint open_req_retry = 16;
//! Number of retries for an open acknowledge
const uint open_ack_retry = 4;
//! Number of retries for a data acknowledge
const uint data_ack_retry = 4;
//! Number of retries for a close request
const uint close_req_retry = 4;

//! Timeout on open acknowledge, in &mu;s
const uint open_ack_time = 250;
//! Timeout on data acknowledge, in &mu;s
const uint data_ack_time = 3000;
//! Timeout on data, in &mu;s
const uint data_time = 500;
//! Timeout on close acknowledge, in &mu;s
const uint close_ack_time = 250;

//! Reserved for performing P2P pings
extern volatile uint pp_ping_count[NUM_LINKS];

//------------------------------------------------------------------------------

//! initialise TX and RX descriptors

void desc_init(void)
{
    tx_desc_t *tdesc = &tx_desc;

    // initialise TX descriptor reusable event
    // proc_byte_set used as default (to avoid compiler warning)
    event_t* e = event_new(proc_byte_set, 0, 0);
    tdesc->event = e;
    tdesc->event_id = e->ID;
    e->ID = 0;                          // mark event as inactive
    e->reuse = 1;                       // mark event as reused

    rx_desc_t *rdesc = rx_desc_table;

    // initialise RX descriptor reusable events
    // proc_byte_set used as default (to avoid compiler warning)
    for (uint i = 0; i < P2P_NUM_STR; i++) {
        e = event_new(proc_byte_set, 0, 0);
        rdesc->event = e;
        rdesc->event_id = e->ID;
        e->ID = 0;                      // mark event as inactive
        e->reuse = 1;                   // mark event as reused

        rdesc++;
    }

    // update number of reserved event IDs
    uint cpsr = cpu_int_disable();

    event.id_rsvd += P2P_NUM_STR + 1;

    cpu_int_restore(cpsr);
}


//------------------------------------------------------------------------------

// #define ASM

#ifdef ASM // !! Needs updating
extern void p2p_send_pkt(uint data, uint addr);

__asm void p2p_send_ctl(uint ctrl, uint addr, uint data)
{
        code32
        import  chksum_32
        import  pkt_tx
        export  p2p_send_pkt

        orr     r0, r0, r2              ;// (r0) data = data | ctrl
        mov     r2, r0                  ;// r2 = data
        mov     r3, lr                  ;// Save lr
        bl      chksum_32               ;// sum = chksum(data)
        mov     lr, r3                  ;// Restore lr
        orr     r0, r0, r2              ;// data |= sum

p2p_send_pkt
        tst     r1, #BIT_31             ;// if addr & BIT_31

        moveq   r2, r1                  ;// r2 = addr
        moveq   r1, r0                  ;// r1 = data
        moveq   r0, #TCR_P2P_P          ;// r0 = TCR_P2P_P
        beq     pkt_tx                  ;// pkt_tx(TCR_P2P_P, data, addr);

        mov     r2, r1, lsl #18         ;// r2 = addr << 18
        mov     r1, r0                  ;// r1 = data
        orr     r0, r2, #TCR_NN_P       ;// r0 = TCR_NN_P + (addr << 18)
        mov     r2, #NN_SDP_KEY         ;// r2 = NN_SDP_KEY
        b       pkt_tx

}

#else // !ASM

//! \brief Send a P2P data packet. Delegates to pkt_tx()
//! \param[in] data: Payload of the message
//! \param[in] addr: Where to send to
static void p2p_send_data(uint data, uint addr)
{
    uint r = pkt_tx(PKT_P2P_PL, data, addr + (P2P_DATA << 16));
    if (r == 0) {
        p2p_stats[TX_FAIL]++;
    }
}

//! \brief Send a P2P control packet. Delegates to pkt_tx()
//! \param[in] ctrl: What message to send
//! \param[in] addr: Where to send to
//! \param[in] data: Payload of the message
static uint p2p_send_ctl(uint ctrl, uint addr, uint data)
{
    data |= ctrl;
    data |= chksum_32(data);
    uint r = pkt_tx(PKT_P2P_PL, data, addr + (P2P_CTRL << 16));
    if (r == 0) {
        p2p_stats[TX_FAIL]++;
        return 0;
    }
    return 1;
}

#endif // ASM


//! \brief Received ACK from receiver. Cancel ack timeout and update tx_desc
//!
//! ```
//!      +-------+-----+-+---+-+---------+---------------+---------------+
//!      |       |     | |   |P|         |                               |
//! Data |  Sum  | 001 |1| 00|H|   TID   |           Ack mask            |
//! Ack  |       |     | |   |A|         |                               |
//!      +-------+-----+-+---+-+---------+---------------+---------------+
//! ```
//! \param[in] data: The payload from the packet
//! \param[in] srce: The sender address (lower 16 bits) from the packet
void p2p_data_ack(uint data, uint srce)
{
    uint tid = (data >> 16) & 31;
    uint phase = (data >> 21) & 1;
    tx_desc_t *desc = &tx_desc;

    if (desc->state == TX_OPEN && desc->tid == tid && desc->dest == srce) {
        timer_cancel(desc->event, desc->event_id); // p2p_ack_timeout

        if (phase != desc->phase) {
            desc->phase = phase;
            desc->base += 3 * desc->seq_len;
        }

        desc->mask = data;
        desc->ack = 1;
    }
}


//! \brief Received CLOSE_REQ from receiver.
//!
//! If TX_OPEN then tidy up tx_desc.
//! In any case, send a CLOSE_ACK back to receiver.
//! \param[in] data: The payload from the packet
//! \param[in] srce: The sender address (lower 16 bits) from the packet
void p2p_close_req(uint data, uint srce)
{
    uint tid = (data >> 16) & 31;
    tx_desc_t *desc = &tx_desc;

    if (desc->state == TX_OPEN && desc->tid == tid && desc->dest == srce) {
        timer_cancel(desc->event, desc->event_id); // p2p_ack_timeout

        desc->mask = 0;
        desc->state = TX_IDLE;
        desc->ack = 3; //const
    }

    p2p_send_ctl(P2P_CLOSE_ACK, srce, data & 0x00ffffff);
}


//! \brief Timed out waiting for data ACK from receiver.
//!
//! This suggests that the receiver has died so close the connection.
//! \param[out] txd: Transmit descriptor
//! \param[in] a2: ignored
void p2p_ack_timeout(uint txd, uint a2)
{
    tx_desc_t *desc = (tx_desc_t *) txd;

    p2p_stats[ACK_TO]++;

    desc->mask = 0; // Force exit
    desc->done = 1;
    desc->ack = 2;
    desc->rc = RC_P2P_TIMEOUT;
}

/*      +-----+---------+---------------+---------------+---------------+
        |     |         |                                               |
  Data  | RID | Seq_num |                     [24]                      |
        |     |         |                                               |
        +-----+-+-----+-+---------------+---------------+---------------+
*/


//! \brief Timeout handler during channel opening phase
//! \param[out] a: pointer to transmit descriptor
//! \param[in] b: code to write to tx_desc_t::ack on timeout
void p2p_open_timeout(uint a, uint b)
{
    tx_desc_t *desc = (tx_desc_t *) a;
    desc->ack = b;
    p2p_stats[OPEN_EVENT]++;
}

//! \brief Send an SDP message to another SCAMP instance
//! \param[in] addr: the P2P address of the SCAMP to send to
//! \param[in] msg: the message to send, presumably for either the target
//!     SCAMP or a core that that SCAMP can deliver to
//! \return result code saying whether message sending was successful
uint p2p_send_msg(uint addr, sdp_msg_t *msg)
{
    uchar *buf = (uchar *) &msg->length;        // Point to len/sum
    uint len = msg->length;                     // 'Real' length

    msg->flags |= SDPF_SUM;                     // Set checksum bit
    msg->checksum = 0;                          // buf[3..2] (BE)

    tx_desc_t *desc = &tx_desc;

    desc->tid = (desc->tid + 1) & 31;

    uint seq_len_log = sv->p2p_sql & 7;
    uint ctrl = (seq_len_log << 5) + desc->tid;

    uint sum = ~ipsum(buf, len+4, 0);           // NB "ctrl" omitted from sum

    buf[2] = sum >> 8;                          // Sum (hi)
    buf[3] = sum;                               // Sum (lo)

    uint seq_len = desc->seq_len = 1 << seq_len_log;

    desc->seq = 0;
    desc->dest = addr;
    desc->base = buf + 2;
    desc->limit = buf + len - 3 + 4;
    desc->done = 0;
    desc->phase = 0;

    desc->state = TX_OPEN_REQ;
    desc->tcount = open_req_retry;
    desc->rc = RC_P2P_NOREPLY;

    p2p_stats[P2P_SENDS]++;

    event_t *e = desc->event;                   // pre-allocated reused event

    while (1) {
        desc->ack = 0;

        // configure event for timeout
        event_config(e, p2p_open_timeout, (uint) desc, 2);
        e->ID = desc->event_id;

        // schedule timeout
        timer_schedule(e, open_ack_time);

        p2p_send_ctl(P2P_OPEN_REQ, addr, (len << 8) + ctrl);

        while (desc->ack == 0) {        //const - returns 1 for OK, 2 for timeout
            continue;
        }
        if (desc->ack == 1) {
            break;
        }

        p2p_stats[OPEN_TO]++;
        p2p_stats[TCOUNT] = desc->tcount;

        if (desc->tcount == 0) {
            return desc->rc;
        }

        desc->tcount--;
    }

    if (desc->rc != RC_OK) {
        return desc->rc;
    }

    //-------

    desc->state = TX_OPEN;
    uint mask = desc->mask = (1 << seq_len) - 1;

    // !! needed  desc->tcount = RETRY; //const

    while (1) {
        for (uint seq = 0; seq < seq_len; seq++) {
            uchar *p = desc->base + 3 * seq;

            uint is_last_seq = (p >= desc->limit);
            uint next_mask = mask >> 1;

            if (mask & 1) {
                if (is_last_seq || (next_mask == 0)) {
                    desc->ack = 0;

                    // configure event for timeout
                    event_config(e, p2p_ack_timeout, (uint) desc, 0);
                    e->ID = desc->event_id;

                    // schedule timeout
                    timer_schedule(e, data_ack_time);
                }

                uint data = (desc->rid << 29) + (desc->phase << 28) //##
                         + (seq << 24) + (p[2] << 16) + (p[1] << 8) + p[0];

                p2p_send_data(data, addr);
            }

            if (is_last_seq) {
                desc->done = 1;
                break;
            }

            mask = next_mask;
            if (mask == 0) {
                break;
            }
        }

        while (desc->ack == 0) { //const - 1 data_ack, 2 timeout, 3 close_req
            continue;
        }

        mask = desc->mask;
        if (desc->done && mask == 0) {
            break;
        }
    }

    return desc->rc;
}


//------------------------------------------------------------------------------


//! \brief Timed out waiting for "seq_len" data packets. Send P2P_DATA_ACK
//!     with current mask and restart timeout.
//! \param[in,out] rxd: channel receiver descriptor
//! \param[in] a2: ignored
void p2p_data_timeout(uint rxd, uint a2)
{
    rx_desc_t *desc = (rx_desc_t *) rxd;

    if (desc->state == RX_OPEN) {
        p2p_stats[DATA_TO]++;

        if (desc->tcount == 0) {
            sark_msg_free(desc->msg);
            desc->state = RX_IDLE;
            return;
        }

        desc->tcount--;

        p2p_send_ctl(P2P_DATA_ACK, desc->srce,
                (desc->phase << 21) + (desc->tid << 16) + desc->mask);

        event_t* e = desc->event;   // pre-allocated reused event

        // configure event for timeout
        event_config(e, p2p_data_timeout, (uint) desc, 0);
        e->ID = desc->event_id;

        // schedule timeout
        timer_schedule(e, data_time);
    }
}


//! \brief Received OPEN_ACK packet.
//!
//! If RX_BUSY do nothing so that timeout will expire (and we keep trying).
//! Otherwise update tx_desc and cancel timeout.
//! \param[in] data: The payload from the packet
//! \param[in] srce: The sender address (lower 16 bits) from the packet
void p2p_open_ack(uint data, uint srce)
{
    uint tid = (data >> 16) & 31;
    uint rc = data & 255;

    tx_desc_t *desc = &tx_desc;

    if (desc->state == TX_OPEN_REQ && desc->tid == tid &&
            desc->dest == srce && rc != RC_P2P_BUSY) {
        uint rid = (data >> 21) & 7; //##

        desc->rid = rid;
        desc->rc = rc;
        desc->ack = 1; //const

        timer_cancel(desc->event, desc->event_id);
    }
}

/*      +-------+-----+-+---------------+---------------+-----+---------+
        |       |     | |                               |     |         |
  Open  |  Sum  | 1xx |1|            Length             | SQL |   TID   |
  Req   |       |     | |                               |     |         |
        +-------+-----+-+---------------+---------------+-----+---------+
        +-------+-----+-+-----+---------+---------------+---------------+
        |       |     | |     |         |               |               |
  Open  |  Sum  | 000 |1| RID |   TID   |               |      RC       |
  Ack   |       |     | |     |         |               |               |
        +-------+-----+-+-----+---------+---------------+---------------+
*/

//! \brief Another SCAMP has asked for an open channel
//! \param[in] data: The payload from the packet
//! \param[in] addr: The sender address (lower 16 bits) from the packet
void p2p_open_req(uint data, uint addr)
{
    uint len = (data >> 8) & 0xffff; // Real length from SDP hdr on...
    uint ctrl = data & 255;

    uint tid = ctrl & 31;
    uint seq_len = 1 << (ctrl >> 5);

    if (len > (SDP_BUF_SIZE + 8 + 16) || seq_len > 16) { //const
        p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + RC_P2P_REJECT);
        p2p_stats[P2P_REJECTS]++;
        return;
    }

    rx_desc_t *desc = rx_desc_table;
    uint rid = P2P_NUM_STR;

    // find free stream - unless it is a repeated request
    // NOTE: make sure to check all streams before exiting the loop!
    for (uint i = 0; i < P2P_NUM_STR; i++) {
        if (desc->state == RX_OPEN && desc->srce == addr
                && desc->tid == tid) {                  // Already open
            timer_cancel(desc->event, desc->event_id);  // p2p_data_timeout
            p2p_stats[OPEN_DUP]++;
            p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + (i << 22) + RC_OK);

            desc->tcount = open_ack_retry;

            event_t* e = desc->event;   // pre-allocated reused event

            // configure event for timeout
            event_config(e, p2p_data_timeout, (uint) desc, 0);
            e->ID = desc->event_id;

            // schedule timeout
            timer_schedule(e, data_time);

            return;
        }

        if (desc->state == RX_IDLE) {                   // Free ?
            rid = i;
        }
        desc++;
    }

    if (rid == P2P_NUM_STR) {           // No free streams - send busy
        p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + RC_P2P_BUSY);
        p2p_stats[P2P_BUSY1]++;
        return;
    }

    desc = rx_desc_table + rid;

    sdp_msg_t *msg = sark_msg_get();
    if (msg == NULL) {
        p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + RC_P2P_BUSY);
        p2p_stats[P2P_BUSY2]++;
        return;
    }

    desc->msg = msg;
    desc->state = RX_OPEN;

    //  desc->len = len;
    desc->ctrl = ctrl;
    desc->srce = addr;
    desc->tid = tid;
    desc->seq_len = seq_len;
    desc->done = 0;
    desc->phase = 0;

    desc->mask = desc->new_mask = (1 << seq_len) - 1;

    uchar *ptr = desc->base = (uchar *) &msg->flags - 2;
    desc->limit = ptr + len + 4 - (2+3); // 2 for length & 3 per packet

    ptr[-2] = len;
    ptr[-1] = len >> 8;

    p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + (rid << 21) + RC_OK); //##

    desc->tcount = open_ack_retry;

    p2p_stats[P2P_OPEN_N + rid]++;
    p2p_stats[P2P_OPENS]++;

    event_t* e = desc->event;           // pre-allocated reused event

    // configure event for timeout
    event_config(e, p2p_data_timeout, (uint) desc, 0);
    e->ID = desc->event_id;

    // schedule timeout
    timer_schedule(e, data_time);
}


//! \brief Timed out waiting for CLOSE_ACK. Repeat P2P_CLOSE_REQ a few times
//!     then give up.
//!
//! State should be RX_CLOSE_REQ
//! \param[in,out] rxd: Receive descriptor
//! \param[in] rid: Receiver ID
void p2p_close_timeout(uint rxd, uint rid)
{
    rx_desc_t *desc = (rx_desc_t *) rxd;

    p2p_stats[CLOSE_TO]++;

    if (desc->tcount == 0) {
        sark_msg_free(desc->msg);
        desc->state = RX_IDLE;
        return;
    }

    desc->tcount--;

    p2p_send_ctl(P2P_CLOSE_REQ, desc->srce,
            (rid << 21) + (desc->tid << 16)); //##

    event_t* e = desc->event;

    // configure event for timeout
    event_config(e, p2p_close_timeout, (uint) desc, rid);
    e->ID = desc->event_id;

    // schedule timeout
    timer_schedule(e, close_ack_time);
}

//! \brief Received an acknowledge that a channel has been closed
//! \param[in] data: The payload from the packet
//! \param[in] srce: The sender address (lower 16 bits) from the packet
void p2p_close_ack(uint data, uint srce)
{
    uint rid = (data >> 21) & 7; //##
    rx_desc_t *desc = rx_desc_table + rid;

    uint tid = (data >> 16) & 31;

    if (desc->state == RX_CLOSE_REQ && desc->tid == tid
            && desc->srce == srce) {
        desc->state = RX_IDLE;
        timer_cancel(desc->event, desc->event_id); // p2p_close_timeout

        msg_queue_insert(desc->msg, 0);
    }
}

//! \brief Receive data on channel
//!
//! May write 1 or 2 bytes beyond end of buffer (buffer has pad word)
//! \param[in] data: The payload from the packet
//! \param[in] addr: The sender address (lower 16 bits) from the packet
// Could write further if error in "seq" field? - check!
// Rearrange base/limit to write at -1, -2, -3  then can
// save if ptr < limit

/*      +-----+---------+---------------+---------------+---------------+
        |     |         |                                               |
  Data  | RID | Seq_num |                     [24]                      |
        |     |         |                                               |
        +---+-+---------+---------------+---------------+---------------+
*/

void p2p_rcv_data(uint data, uint addr)
{
    uint rid = (data >> 29) & 7; //##
    uint phase = (data >> 28) & 1; //##
    rx_desc_t *desc = rx_desc_table + rid;

    if (desc->state == RX_OPEN && desc->srce == addr
            && desc->phase == phase) {
        uint seq = (data >> 24) & (desc->seq_len - 1); //##
        uchar *ptr = desc->base + (3 * seq);

        ptr[0] = data;
        ptr[1] = data >> 8;
        ptr[2] = data >> 16;

        uint seq_bit = 1 << seq;
        desc->mask &= ~seq_bit;

        if (ptr >= desc->limit) {
            desc->mask &= seq_bit - 1;
            desc->done = 1;
        }

        if (desc->mask == 0) {                  // Advance
            timer_cancel(desc->event, desc->event_id); // p2p_data_timeout

            if (desc->done) {                   // Close
                desc->state = RX_CLOSE_REQ;
                desc->tcount = close_req_retry;

                p2p_send_ctl(P2P_CLOSE_REQ, desc->srce,
                        (rid << 21) + (desc->tid << 16)); //##

                // start close timeout - mustn't exit until have CLOSE_ACK

                event_t* e = desc->event;

                // configure event for timeout
                event_config(e, p2p_close_timeout, (uint) desc, rid);
                e->ID = desc->event_id;

                // schedule timeout
                timer_schedule(e, close_ack_time);
            } else {                            // more to do
                uint phase = desc->phase ^= 1;
                desc->base += (3 * desc->seq_len);
                desc->mask = desc->new_mask;

                p2p_send_ctl(P2P_DATA_ACK, desc->srce,
                        (phase << 21) + (desc->tid << 16) + desc->mask);

                desc->tcount = data_ack_retry;

                event_t* e = desc->event;

                // configure event for timeout
                event_config(e, p2p_data_timeout, (uint) desc, 0);
                e->ID = desc->event_id;

                // schedule timeout
                timer_schedule(e, data_time);
            }
        }
    }
}

uint p2p_send_ping(uint addr, uint link) {
    return p2p_send_ctl(P2P_PING, addr, link);
}

void p2p_ping(uint data, uint addr) {
    // Count pings received from each neighbor
    uint link = data & 0x7;
    if (link < 6) {
        pp_ping_count[link] += 1;
    }
}

#ifdef ASM // !! Needs updating

__asm void p2p_rcv_pkt(uint data, uint addr)
{
        code32
        import  p2p_data_pkt
        import  p2p_open_req
        import  p2p_open_ack
        import  p2p_data_ack
        import  p2p_close_req
        import  p2p_close_ack

        tst     r0, #P2P_CTRL           ;// SDP control packet?
        beq     p2p_data_pkt            ;// Deal with data if not

        mov     r3, lr                  ;// Save lr
        mov     r2, r0                  ;// Save data in r2 while
        bl      chksum_32               ;// we checksum data field
        mov     lr, r3                  ;// Restore lr

        cmp     r0, #0                  ;// Non-zero means error
        bxne    lr                      ;// so return

        mov     r0, r2                  ;// Restore data to r0
        and     r2, r2, #0x0f000000     ;// and mask ctrl type field

        cmp     r2, #P2P_DATA_ACK       ;// Check for each type
        beq     p2p_data_ack

        cmp     r2, #P2P_OPEN_REQ
        beq     p2p_open_req

        cmp     r2, #P2P_OPEN_ACK
        beq     p2p_open_ack

        cmp     r2, #P2P_CLOSE_REQ
        beq     p2p_close_req

        cmp     r2, #P2P_CLOSE_ACK
        beq     p2p_close_ack

        bx      lr                      ;// None of the above
}
#else
//! \brief Received P2P control packet
//!
//! Delegates to:
//! * p2p_open_req()
//! * p2p_open_ack()
//! * p2p_data_ack()
//! * p2p_close_req()
//! * p2p_close_ack()
//!
//! \param[in] data: The payload from the packet
//! \param[in] addr: The sender address (lower 16 bits) from the packet
void p2p_rcv_ctrl(uint data, uint addr)
{
    uint t = chksum_32(data);
    if (t != 0) {
        return;
    }

    uint cmd = (data >> 24) & 15;
    if (cmd == P2P_OPEN_REQ >> 24) {
        p2p_open_req(data, addr);
    } else if (cmd == P2P_OPEN_ACK >> 24) {
        p2p_open_ack(data, addr);
    } else if (cmd == P2P_DATA_ACK >> 24) {
        p2p_data_ack(data, addr);
    } else if (cmd == P2P_CLOSE_REQ >> 24) {
        p2p_close_req(data, addr);
    } else if (cmd == P2P_CLOSE_ACK >> 24) {
        p2p_close_ack(data, addr);
    } else if (cmd == P2P_PING >> 24) {
        p2p_ping(data, addr);
    }
}
#endif
