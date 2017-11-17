//------------------------------------------------------------------------------
//
// scamp-p2p.c	    P2P packet handling for SC&MP
//
// Copyright (C)    The University of Manchester - 2009-2011
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------


#include "spinnaker.h"
#include "sark.h"
#include "scamp.h"

#include "spinn_net.h"


#define STATS

#define P2P_NUM_STR 8	// Number of streams

#define TX_IDLE 0
#define TX_OPEN_REQ 1
#define TX_OPEN 2
#define TX_RETRY 3
#define TX_CLOSE 4

#define RX_IDLE 0
#define RX_OPEN 1
#define RX_CLOSE_REQ 2


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

    event_t* event;
    uint event_id;
} rx_desc_t;


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

    event_t *event;
    uint event_id;
} tx_desc_t;

//------------------------------------------------------------------------------

tx_desc_t tx_desc;

rx_desc_t rx_desc_table[P2P_NUM_STR];


#ifdef STATS

#define DATA_TO 0
#define ACK_TO 1
#define OPEN_TO 2
#define CLOSE_TO 3

#define P2P_SENDS 4
#define P2P_OPENS 5
#define P2P_BUSY1 6
#define P2P_BUSY2 7

#define P2P_REJECTS 8
#define TCOUNT 9
#define OPEN_DUP 10
#define TX_FAIL 11
#define OPEN_EVENT 12

#define P2P_OPEN_N 16

uint p2p_stats[32];

#endif

//------------------------------------------------------------------------------


uint open_req_retry = 16;
uint open_ack_retry = 4;
uint data_ack_retry = 4;
uint close_req_retry = 4;

uint open_ack_time = 250;
uint data_ack_time = 3000;
uint data_time = 500;
uint close_ack_time = 250;


//------------------------------------------------------------------------------

// #define ASM

#ifdef ASM // !! Needs updating
extern void p2p_send_pkt(uint data, uint addr);

__asm void p2p_send_ctl(uint ctrl, uint addr, uint data)
{
	code32
	import	chksum_32
	import	pkt_tx
	export	p2p_send_pkt

	orr	r0, r0, r2		;// (r0) data = data | ctrl
	mov	r2, r0			;// r2 = data
	mov	r3, lr			;// Save lr
	bl	chksum_32		;// sum = chksum(data)
	mov	lr, r3			;// Restore lr
	orr	r0, r0, r2		;// data |= sum

p2p_send_pkt
	tst	r1, #BIT_31		;// if addr & BIT_31

	moveq	r2, r1			;// r2 = addr
	moveq	r1, r0			;// r1 = data
	moveq	r0, #TCR_P2P_P		;// r0 = TCR_P2P_P
	beq	pkt_tx			;// pkt_tx(TCR_P2P_P, data, addr);

	mov	r2, r1, lsl #18		;// r2 = addr << 18
	mov	r1, r0	    		;// r1 = data
	orr	r0, r2, #TCR_NN_P	;// r0 = TCR_NN_P + (addr << 18)
	mov	r2, #NN_SDP_KEY		;// r2 = NN_SDP_KEY
	b	pkt_tx

}

#else // !ASM

void p2p_send_data(uint data, uint addr)
{
#ifdef STATS
    uint r = pkt_tx(PKT_P2P_PL, data, addr + (P2P_DATA << 16));
    if (r == 0) {
	p2p_stats[TX_FAIL]++;
    }
#else // !STATS
    (void) pkt_tx(PKT_P2P_PL, data, addr + (P2P_DATA << 16));
#endif // STATS
}

void p2p_send_ctl(uint ctrl, uint addr, uint data)
{
    data |= ctrl;
    data |= chksum_32(data);
#ifdef STATS
    uint r = pkt_tx(PKT_P2P_PL, data, addr + (P2P_CTRL << 16));
    if (r == 0) {
	p2p_stats[TX_FAIL]++;
    }
#else // !STATS
    (void) pkt_tx(PKT_P2P_PL, data, addr + (P2P_CTRL << 16));
#endif // STATS
}

#endif // ASM


// Received ACK from receiver. Cancel ack timeout and update tx_desc

/*      +-------+-----+-+---+-+---------+---------------+---------------+
	|       |     | |   |P|         |                               |
  Data	|  Sum  | 001 |1| 00|H|   TID   |           Ack mask            |
  Ack  	|       |     | |   |A|         |                               |
	+-------+-----+-+---+-+---------+---------------+---------------+
*/

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


// Received CLOSE_REQ from receiver. If TX_OPEN then tidy up tx_desc.
// In any case, send a CLOSE_ACK back to receiver.

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


// Timed out waiting for data ACK from receiver. This suggests that the
// receiver has died so close the connection.

void p2p_ack_timeout(uint txd, uint a2)
{
    tx_desc_t *desc = (tx_desc_t *) txd;

#ifdef STATS
    p2p_stats[ACK_TO]++;
#endif

    desc->mask = 0; // Force exit
    desc->done = 1;
    desc->ack = 2;
    desc->rc = RC_P2P_TIMEOUT;
}

/*      +-----+---------+---------------+---------------+---------------+
	|     |         |                                               |
  Data	| RID | Seq_num |                     [24]                      |
       	|     |         |                                               |
	+-----+-+-----+-+---------------+---------------+---------------+
*/


void p2p_open_timeout(uint a, uint b)
{
    tx_desc_t *desc = (tx_desc_t *) a;
    desc->ack = b;
#ifdef STATS
    p2p_stats[OPEN_EVENT]++;
#endif
}


uint p2p_send_msg(uint addr, sdp_msg_t *msg)
{
    uchar *buf = (uchar *) &msg->length;	// Point to len/sum
    uint len = msg->length;			// 'Real' length

    msg->flags |= SDPF_SUM;			// Set checksum bit
    msg->checksum = 0;				// buf[3..2] (BE)

    tx_desc_t *desc = &tx_desc;

    desc->tid = (desc->tid + 1) & 31;

    uint seq_len_log = sv->p2p_sql & 7;
    uint ctrl = (seq_len_log << 5) + desc->tid;

    uint sum = ~ipsum(buf, len+4, 0);		// NB "ctrl" omitted from sum

    buf[2] = sum >> 8;				// Sum (hi)
    buf[3] = sum;				// Sum (lo)

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

#ifdef STATS
    p2p_stats[P2P_SENDS]++;
#endif

    while (1) {
	desc->ack = 0;

	//##event_t* e = event_new(proc_byte_set, (uint) &desc->ack, 2);
	event_t* e = event_new(p2p_open_timeout, (uint) desc, 2);
	desc->event = e;
	if (e != NULL) {
	    desc->event_id = e->ID;
	    timer_schedule(e, open_ack_time);
	} else {
	    sw_error(SW_OPT);
	}

	p2p_send_ctl(P2P_OPEN_REQ, addr, (len << 8) + ctrl);

	while (desc->ack == 0) {	//const - returns 1 for OK, 2 for timeout
	    continue;
        }
	if (desc->ack == 1) {
	    break;
        }

#ifdef STATS
	p2p_stats[OPEN_TO]++;
	p2p_stats[TCOUNT] = desc->tcount;
#endif

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

		    event_t* e = event_new(p2p_ack_timeout, (uint) desc, 0);
		    if (e != NULL) {
			desc->event = e;
			desc->event_id = e->ID;
			timer_schedule(e, data_ack_time);
		    } else {
			sw_error(SW_OPT);
		    }
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


// Timed out waiting for "seq_len" data packets. Send P2P_DATA_ACK
// with current mask and restart timeout.

void p2p_data_timeout(uint rxd, uint a2)
{
    rx_desc_t *desc = (rx_desc_t *) rxd;

    if (desc->state == RX_OPEN) {
#ifdef STATS
	p2p_stats[DATA_TO]++;
#endif

	if (desc->tcount == 0) {
	    sark_msg_free(desc->msg);
	    desc->state = RX_IDLE;
	    return;
	}

	desc->tcount--;

	p2p_send_ctl(P2P_DATA_ACK, desc->srce,
		(desc->phase << 21) + (desc->tid << 16) + desc->mask);

	event_t* e = event_new(p2p_data_timeout, (uint) desc, 0);
	if (e != NULL) {
	    desc->event = e;
	    desc->event_id = e->ID;
	    timer_schedule(e, data_time);
	} else {
	    sw_error(SW_OPT);
	}
    }
}


// Received OPEN_ACK packet. If RX_BUSY do nothing so that timeout
// will expire (and we keep trying). Otherwise update tx_desc and
// cancel timeout.

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
  Open	|  Sum  | 1xx |1|            Length             | SQL |   TID   |
  Req  	|       |     | |                               |     |         |
	+-------+-----+-+---------------+---------------+-----+---------+
        +-------+-----+-+-----+---------+---------------+---------------+
	|       |     | |     |         |               |               |
  Open	|  Sum  | 000 |1| RID |   TID   |               |      RC       |
  Ack  	|       |     | |     |         |               |               |
	+-------+-----+-+-----+---------+---------------+---------------+
*/

void p2p_open_req(uint data, uint addr)
{
    uint len = (data >> 8) & 0xffff; // Real length from SDP hdr on...
    uint ctrl = data & 255;

    uint tid = ctrl & 31;
    uint seq_len = 1 << (ctrl >> 5);

    if (len > (SDP_BUF_SIZE + 8 + 16) || seq_len > 16) { //const
	p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + RC_P2P_REJECT);
#ifdef STATS
	p2p_stats[P2P_REJECTS]++;
#endif
	return;
    }

    rx_desc_t *desc = rx_desc_table;
    uint rid = P2P_NUM_STR;

    for (uint i = 0; i < P2P_NUM_STR; i++) {
	if (desc->state == RX_OPEN && desc->srce == addr
		&& desc->tid == tid) {			// Already open
	    timer_cancel(desc->event, desc->event_id);	// p2p_data_timeout
#ifdef STATS
	    p2p_stats[OPEN_DUP]++;
#endif
	    p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + (i << 22) + RC_OK);

	    desc->tcount = open_ack_retry;

	    event_t* e = event_new(p2p_data_timeout, (uint) desc, 0);
	    if (e != NULL) {
		desc->event = e;
		desc->event_id = e->ID;
		timer_schedule(e, data_time);
	    } else {
		sw_error(SW_OPT);
	    }

	    return;
	}

	if (desc->state == RX_IDLE) {			// Free ?
	    rid = i;
	}
	desc++;
    }

    if (rid == P2P_NUM_STR) {		// No free streams - send busy
	p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + RC_P2P_BUSY);
#ifdef STATS
	p2p_stats[P2P_BUSY1]++;
#endif
	return;
    }

    desc = rx_desc_table + rid;

    sdp_msg_t *msg = sark_msg_get();
    if (msg == NULL) {
	p2p_send_ctl(P2P_OPEN_ACK, addr, (tid << 16) + RC_P2P_BUSY);
#ifdef STATS
	p2p_stats[P2P_BUSY2]++;
#endif
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

#ifdef STATS
    p2p_stats[P2P_OPEN_N + rid]++;
    p2p_stats[P2P_OPENS]++;
#endif

    event_t* e = event_new(p2p_data_timeout, (uint) desc, 0);
    if (e != NULL) {
	desc->event = e;
	desc->event_id = e->ID;
	timer_schedule(e, data_time);
    } else {
	sw_error(SW_OPT);
    }
}


// Timed out waiting for CLOSE_ACK. Repeat P2P_CLOSE_REQ a few times
// then give up
// State should be RX_CLOSE_REQ

void p2p_close_timeout(uint rxd, uint rid)
{
    rx_desc_t *desc = (rx_desc_t *) rxd;

#ifdef STATS
    p2p_stats[CLOSE_TO]++;
#endif

    if (desc->tcount == 0) {
	sark_msg_free(desc->msg);
	desc->state = RX_IDLE;
	return;
    }

    desc->tcount--;

    p2p_send_ctl(P2P_CLOSE_REQ, desc->srce,
	    (rid << 21) + (desc->tid << 16)); //##

    event_t* e = event_new(p2p_close_timeout, (uint) desc, rid);
    if (e != NULL) {
	desc->event = e;
	desc->event_id = e->ID;
	timer_schedule(e, close_ack_time);
    } else {
	sw_error(SW_OPT);
    }
}


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

// May write 1 or 2 bytes beyond end of buffer (buffer has pad word)
// Could write further if error in "seq" field? - check!
// Rearrange base/limit to write at -1, -2, -3  then can
// save if ptr < limit

/*      +-----+---------+---------------+---------------+---------------+
	|     |         |                                               |
  Data	| RID | Seq_num |                     [24]                      |
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

	if (desc->mask == 0) {			// Advance
	    timer_cancel(desc->event, desc->event_id); // p2p_data_timeout

	    if (desc->done) {			// Close
		desc->state = RX_CLOSE_REQ;
		desc->tcount = close_req_retry;

		p2p_send_ctl(P2P_CLOSE_REQ, desc->srce,
			(rid << 21) + (desc->tid << 16)); //##

		// start close timout - mustn't exit until have CLOSE_ACK

		event_t* e = event_new(p2p_close_timeout, (uint) desc, rid);
		if (e != NULL) {
		    desc->event = e;
		    desc->event_id = e->ID;
		    timer_schedule(e, close_ack_time);
		} else {
		    sw_error(SW_OPT);
		}
	    } else {				// more to do
		uint phase = desc->phase ^= 1;
		desc->base += (3 * desc->seq_len);
		desc->mask = desc->new_mask;

		p2p_send_ctl(P2P_DATA_ACK, desc->srce,
			(phase << 21) + (desc->tid << 16) + desc->mask);

		desc->tcount = data_ack_retry;

		event_t* e = event_new(p2p_data_timeout, (uint) desc, 0);
		if (e != NULL) {
		    desc->event = e;
		    desc->event_id = e->ID;
		    timer_schedule(e, data_time);
		} else {
		    sw_error(SW_OPT);
		}
	    }
	}
    }
}

#ifdef ASM // !! Needs updating

__asm void p2p_rcv_pkt(uint data, uint addr)
{
	code32
	import	p2p_data_pkt
	import	p2p_open_req
	import	p2p_open_ack
	import	p2p_data_ack
	import	p2p_close_req
	import	p2p_close_ack

	tst	r0, #P2P_CTRL		;// SDP control packet?
	beq	p2p_data_pkt		;// Deal with data if not

	mov	r3, lr			;// Save lr
	mov	r2, r0			;// Save data in r2 while
	bl	chksum_32		;// we checksum data field
	mov	lr, r3			;// Restore lr

	cmp	r0, #0			;// Non-zero means error
	bxne	lr  			;// so return

	mov	r0, r2			;// Restore data to r0
	and	r2, r2, #0x0f000000	;// and mask ctrl type field

	cmp	r2, #P2P_DATA_ACK	;// Check for each type
	beq	p2p_data_ack

	cmp	r2, #P2P_OPEN_REQ
	beq	p2p_open_req

	cmp	r2, #P2P_OPEN_ACK
	beq	p2p_open_ack

	cmp	r2, #P2P_CLOSE_REQ
	beq	p2p_close_req

	cmp	r2, #P2P_CLOSE_ACK
	beq	p2p_close_ack

	bx	lr			;// None of the above
}
#else
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
    }
}
#endif
