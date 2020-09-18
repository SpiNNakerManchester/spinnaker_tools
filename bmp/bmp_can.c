//------------------------------------------------------------------------------
//
//! \file bmp_can.c
//! \brief          CAN hardware interface code for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
//  Email           steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012-2019 The University of Manchester
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

// TODO
//
// **** Tidy up/implement remaining timeouts
// **** Act on tx_desc->delay
//

#include "lpc17xx.h"
#include "lpc17xx_clkpwr.h"

#include "bmp.h"

//------------------------------------------------------------------------------

//! CAN SDP command codes
enum can_sdp_command {
    CAN_DATA = 0,       //!< Payload when sending SDP over CAN
    CAN_NACK = 1,       //!< Restart sequence
    CAN_OPEN_REQ = 2,   //!< Request to start sending
    CAN_OPEN_ACK = 3,   //!< Permission to start sending
    CAN_CLOSE_REQ = 4,  //!< Message complete
    CAN_CLOSE_ACK = 5,  //!< Message complete acknowledge

    CAN_PROC = 6,       //!< Initiate a "proc"
    CAN_EXEC = 7,       //!< Immediate command
};

//! CAN SDP sender states
enum can_sdp_sender_states {
    TX_IDLE = 0,
    TX_OPEN_REQ = 1,
    TX_DATA = 2,
};

//! CAN SDP receiver states
enum can_sdp_receiver_states {
    RX_IDLE = 0,
    RX_DATA = 1,
    RX_CLOSE_REQ = 2,
};

//! SDP result codes that we care about (others are just passed through)
enum can_sdp_results {
    RC_SDP_NOREPLY = 0x8b,      //!< No reply to open
    RC_SDP_REJECT = 0x8c,       //!< Open rejected
    RC_SDP_BUSY = 0x8d,         //!< Dest busy
    RC_SDP_TIMEOUT = 0x8e,      //!< Dest died?
};

#define CAN_NUM_STR     8       //!< Number of CAN receive streams

#define TX_OPEN_RETRY   4       //!< Number of retries to open
#define TX_DATA_RETRY   4       //!< Number of retries to send data item
#define RX_CLOSE_RETRY  4       //!< Number of retries to close

#define CAN_OPEN_TO     1000    //!< Time for receiver to return CAN_OPEN_ACK
#define CAN_CLOSE_TO    1000    //!< Time for sender to return CAN_CLOSE_ACK


typedef struct rx_desc {
    event_t* event;     //!< Event and ID must be first in struct
    uint32_t event_id;

    uint8_t state;      //!< State variable
    uint8_t tid;        //!< Tx ID of sender
    uint8_t srce;       //!< Source address of sender
    uint8_t last;       //!< Sequence number of last packet in message
    uint8_t seq;        //!< Expected sequence number
    uint8_t retry;      //!< Counts retries for can_close_req

    uint32_t *buf;      //!< Pointer to buffer in SDP message
    sdp_msg_t *msg;     //!< Pointer to SDP message
} rx_desc_t;


typedef struct {
    event_t *event;     //!< Event and ID must be first in struct
    uint32_t event_id;

    uint8_t state;      //!< State variable
    uint8_t rid;        //!< Rx ID at receiver
    uint8_t tid;        //!< Tx ID
    volatile uint8_t seq; //!< Sequence number for sent DATA packets

    uint8_t dest;       //!< Address of receiver
    uint8_t rc;         //!< Return code
    uint8_t delay;      //!< Packet delay (not used yet)
    volatile uint8_t ack; //!< Ack variable set by interrupt routines
} tx_desc_t;


//! Collects statistics about CAN messages
typedef struct {
    uint32_t send_msg;
    uint32_t rc_route;
    uint32_t timeout;
    uint32_t can_nack;
    uint32_t close_req;
    uint32_t data_timeout;
    uint32_t open_timeout;
    uint32_t open_ack;
    uint32_t reject;
    uint32_t close_ack;
    uint32_t open_fail;
    uint32_t rxseq_error;
} stat_t;


//------------------------------------------------------------------------------

uint32_t config1;       //!< Argument \p d1 to pass to proc_setup()
uint32_t config2;       //!< Argument \p d2 to pass to proc_setup()

//! Whether a particular board is talking to the CAN bus
uint8_t can_status[CAN_SIZE];

//! Receive descriptors
static rx_desc_t rx_desc_table[CAN_NUM_STR];
//! Transmit descriptor storage
static tx_desc_t tx_desc_str;
//! Transmit descriptor
static tx_desc_t *tx_desc = &tx_desc_str;

//! Used to work out when to show the LED indicating a bus timeout
static uint32_t bus_timeout;
//! Used to work out when to show the LED indicating a CAN timeout
static uint32_t can_timeout;

//! Whether we had a request on the CAN bus
static bool had_CAN_req;

//! Increment the named field in ::stat
#define STAT(x) stat.x++;
//! Collects statistics about CAN messages
stat_t stat;
//#define STAT(x)


//------------------------------------------------------------------------------

#if 0
#define IO_CAN  IO_DBG

void can_dump(void)
{
    io_printf(IO_CAN,
            "TX - state %d, rid %d, tid %d, seq %d, dest %d, rc %d, delay %d, ack %d\n",
            tx_desc->state, tx_desc->rid, tx_desc->tid, tx_desc->seq, tx_desc->dest,
            tx_desc->rc, tx_desc->delay, tx_desc->ack);

    for (int i = 0; i < 8; i++) {
        rx_desc_t *rx_desc = rx_desc_table + i;
        io_printf(IO_CAN,
                "RX %d - state %d, tid %d, srce %d, last %d, seq %d, retry %d\n",
                i, rx_desc->state, rx_desc->tid, rx_desc->srce, rx_desc->last,
                rx_desc->seq, rx_desc->retry);
    }
}
#endif

//------------------------------------------------------------------------------

//! \brief Transmit using CAN buffer 1
//! \param[in] dest: who to transmit to
//! \param[in] id: operation code?
//! \param[in] d1: payload value 1
//! \param[in] d2: payload value 2
static void can_tx1(uint32_t dest, uint32_t id, uint32_t d1, uint32_t d2)
{
    while ((LPC_CAN1->SR & (1<<2)) == 0) {      // Wait for TxBuf1
        continue;
    }
    LPC_CAN1->TID1 = (dest << 24) | (can_ID << 19) | id;
    LPC_CAN1->TDA1 = d1;
    LPC_CAN1->TDB1 = d2;

    while ((LPC_CAN1->SR & (1<<3)) == 0) {      // Wait for Tx1 complete
        continue;
    }
    LPC_CAN1->CMR = 0x21;                       // Send TxBuf1
}

//! \brief Transmit using CAN buffer 2
//! \param[in] dest: who to transmit to
//! \param[in] id: operation code?
//! \param[in] d1: payload value 1
//! \param[in] d2: payload value 2
static void can_tx2(uint32_t dest, uint32_t id, uint32_t d1, uint32_t d2)
{
    while ((LPC_CAN1->SR & (1<<10)) == 0) {     // Wait for TxBuf2
        continue;
    }
    LPC_CAN1->TID2 = (dest << 24) | (can_ID << 19) | id;
    LPC_CAN1->TDA2 = d1;
    LPC_CAN1->TDB2 = d2;

    while ((LPC_CAN1->SR & (1<<11)) == 0) {     // Wait for Tx2 complete
        continue;
    }
    LPC_CAN1->CMR = 0x41;                       // Send TxBuf2
}

//! \brief Transmit using CAN buffer 3; _no retries_
//! \param[in] dest: who to transmit to
//! \param[in] id: operation code?
//! \param[in] d1: payload value 1
//! \param[in] d2: payload value 2
static void can_tx3_nr(uint32_t dest, uint32_t id, uint32_t d1, uint32_t d2)
{
    while ((LPC_CAN1->SR & (1<<18)) == 0) {     // Wait for TxBuf3
        continue;
    }
    LPC_CAN1->TID3 = (dest << 24) | (can_ID << 19) | id;
    LPC_CAN1->TDA3 = d1;
    LPC_CAN1->TDB3 = d2;

    // while ((LPC_CAN1->SR & (1<<19)) == 0) {  // Wait for Tx3 complete
    //    continue;
    // }

    LPC_CAN1->CMR = 0x83;                       // Send TxBuf3 (no retries)
}


//------------------------------------------------------------------------------

//! \brief Schedule an event when a timeout occurs
//! \param[in] desc: A transmit descriptor or a receive descriptor.
//!     (Uses first two common fields only.)
//! \param[in] proc: Callback to handle timeout
//! \param[in] arg2: Second argument to callback
//! \param[in] time: How far ahead to schedule the timeout
static void timeout_event(void *desc, event_proc proc, uint32_t arg2,
        uint32_t time)
{
    rx_desc_t *d = (rx_desc_t *) desc;
    event_t *e = event_new(proc, (uint32_t) desc, arg2);

    if (e != NULL) {
        d->event = e;
        d->event_id = e->ID;
        event_schedule(e, time);
    }
}

//------------------------------------------------------------------------------


//! \brief Received an CAN_OPEN_ACK packet from receiver.
//! \details Unless return code says "busy" move to state TX_DATA and fill in
//! "tx_desc". If busy, set a packet delay and allow more requests to be sent.
//! \param[in] id: Describes what is going on
static void can_open_ack(uint32_t id)
{
    uint32_t srce = (id >> 19) & 31;
    uint32_t tid = (id >> 11) & 31;

    if (tx_desc->state == TX_OPEN_REQ &&
            tx_desc->tid == tid &&
            tx_desc->dest == srce) {
        uint32_t rc = id & 255;

        if (rc != RC_SDP_BUSY) {        // Must be OK or REJECT
            tx_desc->rid = (id >> 8) & 7;
            tx_desc->rc = rc;
            tx_desc->state = TX_DATA;
            tx_desc->ack = 1;
        } else {
            tx_desc->delay = 100;
        }
    }
}


//! \brief Received CAN_CLOSE_REQ from receiver.
//!
//! If TX_DATA then signal in "ack".
//! In any case, send a CAN_CLOSE_ACK back to receiver.
//! \param[in] id: Describes what is going on
static void can_close_req(uint32_t id)
{
    uint32_t tid = (id >> 11) & 31;
    uint32_t rid = (id >> 8) & 7;
    uint32_t srce = (id >> 19) & 31;

    if (tx_desc->state == TX_DATA &&
            tx_desc->tid == tid &&
            tx_desc->dest == srce) {
        STAT(close_req);
        tx_desc->ack = 1;
    }

    can_tx1(srce, (CAN_CLOSE_ACK << 16) + (tid << 11) + (rid << 8), 0, 0);
}


//! \brief Received CAN_NACK from receiver because it found a packet out of
//!     sequence.
//! \details Restart from the sequence number provided by receiver.
//! \param[in] id: Describes what is going on
static void can_nack(uint32_t id)
{
    uint32_t srce = (id >> 19) & 31;
    uint32_t tid = (id >> 11) & 31;

    if (tx_desc->state == TX_DATA &&
            tx_desc->tid == tid &&
            tx_desc->dest == srce) {
        STAT(can_nack);
        tx_desc->ack = 2;
        tx_desc->seq = id & 255;
    }
}


//! \brief Timed out waiting for CAN_CLOSE_REQ or CAN_NACK.
//! \details Set "tx_desc->ack" to 3
//! \param[in,out] a1: Transmission descriptor
//! \param[in] a2: Marker value (3)
static void can_data_timeout(uint32_t a1, uint32_t a2)
{
    tx_desc_t *tx_desc = (tx_desc_t *) a1;
    tx_desc->ack = a2;
    STAT(data_timeout);
}


//! \brief Timed out waiting for CAN_OPEN_ACK.
//! \details Set "tx_desc->ack" to 2
//! \param[in,out] a1: Transmission descriptor
//! \param[in] a2: Marker value (2)
static void can_open_timeout(uint32_t a1, uint32_t a2)
{
    tx_desc_t *tx_desc = (tx_desc_t *) a1;
    tx_desc->ack = a2;
    STAT(open_timeout);
}

//! \brief Send an SDP message over the CAN
//! \param[in] dest: Which board to send to
//! \param[in] msg: The message to send
//! \return SDP response code
uint32_t can_send_msg(uint32_t dest, sdp_msg_t *msg)
{
    uint32_t len = msg->length;
    uint32_t pkts = (len + 7) / 8;

    uint32_t can_dest = board2can[dest];

    if (can_status[can_dest] == 0) {
        STAT(rc_route);
        return RC_ROUTE;
    }

    STAT(send_msg);

    tx_desc->tid = (tx_desc->tid + 1) & 31;
    tx_desc->dest = can_dest;
    tx_desc->state = TX_OPEN_REQ;
    tx_desc->rc = RC_SDP_NOREPLY;

#if 0
    io_printf(IO_CAN, "can_send_msg - dest %d len %d pkts %d TID %d\n",
            dest, len, pkts, tx_desc->tid);
#endif
    uint32_t retries = TX_OPEN_RETRY; //##

    // Send CAN_OPEN_REQ and await CAN_OPEN_ACK (or timeout)
    while (retries--) {
        tx_desc->ack = 0;
        timeout_event(tx_desc, can_open_timeout, 2, CAN_OPEN_TO);
        can_tx1(can_dest, (CAN_OPEN_REQ << 16) + (tx_desc->tid << 11) + len,
                0, 0);

        uint32_t ack;
        while ((ack = tx_desc->ack) == 0) {
            continue;
        }
        if (ack == 1) { // Received CAN_OPEN_ACK
            STAT(open_ack);
            event_cancel(tx_desc->event, tx_desc->event_id); // can_open_timeout
            break;
        }
    }

    // Here when timed out (rc = RC_SDP_NOREPLY) or received CAN_OPEN_ACK
    if (tx_desc->rc != RC_OK) { // RC_SDP_NOREPLY or RC_SDP_REJECT
        tx_desc->state = TX_IDLE;
        STAT(open_fail);
        return tx_desc->rc;
    }

    // Prepare to send DATA packets
    uint32_t *buf = (uint32_t *) &msg->flags;

    tx_desc->ack = 0;
    tx_desc->seq = 0;
    retries = TX_DATA_RETRY;

    timeout_event(tx_desc, can_data_timeout, 3, 1000 * pkts);

    // Loop below sending DATA packets while "tx_desc->seq" is less than
    // packet count. Also check for three possible events
    // 1) NACK received - restart timeout, continue sending
    // 2) CAN_CLOSE_REQ received - exit loop
    // 3) Timeout - backup and send last DATA again

    while (1) {
        uint32_t seq = tx_desc->seq;
        if (seq < pkts) {
            uint32_t id = (CAN_DATA << 16) + (tx_desc->tid << 11) +
                    (tx_desc->rid << 8) + seq;
            uint32_t d1 = buf[2 * seq];
            uint32_t d2 = buf[2 * seq + 1];

            can_tx1(can_dest, id, d1, d2);
            tx_desc->seq++;
        }

        uint32_t ack = tx_desc->ack;
        if (ack) {
            event_cancel(tx_desc->event, tx_desc->event_id); // can_data_timeout
            tx_desc->ack = 0;
        }

        if (ack == 1) {         // CLOSE_REQ received
            break;
        } else if (ack == 2) {  // NACK received (tx_desc->seq updated)
            timeout_event(tx_desc, can_data_timeout, 3,
                    1000 * (pkts - tx_desc->seq));
        } else if (ack == 3) {  // Data timeout
            if (retries-- == 0) {
                tx_desc->rc = RC_SDP_TIMEOUT;
                break;
            }

            tx_desc->seq--;     // Force a data packet transmission
            timeout_event(tx_desc, can_data_timeout, 3,
                    1000 * (pkts - tx_desc->seq));
        }
    }

    tx_desc->state = TX_IDLE;
    return tx_desc->rc;
}


//------------------------------------------------------------------------------

//! \brief Someone wishes to send us an SDP packet; received CAN_OPEN_REQ
//! \param[in] id: Describes what is going on
static void can_open_req(uint32_t id)
{
    uint32_t len = id & 0x7ff;
    uint32_t srce = (id >> 19) & 31;
    uint32_t tid = (id >> 11) & 31;
#if 0
    io_printf(IO_CAN, "can_open_req - len %d, srce %d, tid %d\n",
            len, srce, tid);
#endif

    // Reject if length is too big
    if (len > SDP_BUF_SIZE + 8 + 16) {
        can_tx2(srce, (CAN_OPEN_ACK << 16) + (tid << 11) + RC_SDP_REJECT,
                0, 0);
        STAT(reject);
        return;
    }

    // Now search the RxDesc table for a free slot or a duplicate request
    rx_desc_t *rx_desc = rx_desc_table;
    uint32_t rid = CAN_NUM_STR;

    for (uint32_t i = 0; i < CAN_NUM_STR; i++) {
        if (rx_desc->state == RX_DATA &&        // Already open (duplicate request)
                rx_desc->srce == srce &&
                rx_desc->tid == tid) {
//##        event_cancel(rx_desc->event, rx_desc->event_id); // can_data_timeout
            can_tx2(srce, (CAN_OPEN_ACK << 16) + (tid << 11) + (i << 8) +
                    RC_OK, 0, 0);
            //    timeout_event(rx_desc, can_xxx_timeout, 0, 1000);
            return;
        }

        if (rx_desc->state == RX_IDLE) {        // Free slot
            rid = i;
        }
        rx_desc++;
    }

    // Check for no free streams - send busy if so
    if (rid == CAN_NUM_STR) {
        can_tx2(srce, (CAN_OPEN_ACK << 16) + (tid << 11) + RC_SDP_BUSY, 0, 0);
        return;
    }

    // Allocate a message buffer - send busy on failure
    sdp_msg_t *msg = msg_get();
    if (msg == NULL) {
        can_tx2(srce, (CAN_OPEN_ACK << 16) + (tid << 11) + RC_SDP_BUSY, 0, 0);
        return;
    }

    // Set up rest of descriptor
    rx_desc = rx_desc_table + rid;

    rx_desc->state = RX_DATA;
    rx_desc->srce = srce;
    rx_desc->tid = tid;
    rx_desc->seq = 0;

    rx_desc->msg = msg;
    rx_desc->buf = (uint32_t *) &msg->flags;

    rx_desc->last = (len - 1) / 8;      // Seq num of last data packet
    msg->length = len;

    can_tx2(srce, (CAN_OPEN_ACK << 16) + (tid << 11) + (rid << 8) + RC_OK, 0, 0);

    // timeout_event(rx_desc, can_xxx_timeout, 0, 1000);
}


//! \brief Received CAN_CLOSE_ACK from sender.
//! \details Cancel timeout and go to IDLE
//! \param[in] id: Describes what is going on
static void can_close_ack(uint32_t id)
{
    uint32_t srce = (id >> 19) & 31;
    uint32_t rid = (id >> 8) & 7;
    uint32_t tid = (id >> 11) & 31;

    rx_desc_t *rx_desc = rx_desc_table + rid;

    if (rx_desc->state == RX_CLOSE_REQ &&
            rx_desc->tid == tid &&
            rx_desc->srce == srce) {
        STAT(close_ack);
        rx_desc->state = RX_IDLE;

        event_cancel(rx_desc->event, rx_desc->event_id); // can_close_timeout
    }
}


//! \brief Receiver timed out waiting for CAN_CLOSE_ACK.
//! \details Retry a few times and then give up, returning to IDLE
//! \param[in] arg1: Receive descriptor
//! \param[in] arg2: Transaction ID
static void can_close_timeout(uint32_t arg1, uint32_t arg2)
{
    rx_desc_t *rx_desc = (rx_desc_t *) arg1;
    uint32_t rid = arg2;

    rx_desc->retry--;
    if (rx_desc->retry == 0) {
        rx_desc->state = RX_IDLE;
        return;
    }

    can_tx2(rx_desc->srce, (CAN_CLOSE_REQ << 16) + (rx_desc->tid << 11) +
            (rid << 8), 0, 0);

    timeout_event(rx_desc, can_close_timeout, rid, CAN_CLOSE_TO);
}


//! \brief Called when CAN_DATA packet arrives
//! \details Adds the payload to the SDP packet being reassembled if the
//!     message is meant for us and expected. If the packet has been fully
//!     received, add it into the main message system with msg_queue_insert()
//! \param[in] id: Describes what is going on
//! \param[in] d1: First payload word
//! \param[in] d2: Second payload word
static void can_data(uint32_t id, uint32_t d1, uint32_t d2)
{
    uint32_t srce = (id >> 19) & 31;
    uint32_t tid = (id >> 11) & 31;
    uint32_t rid = (id >> 8) & 7;

    rx_desc_t *rx_desc = rx_desc_table + rid;

    if (rx_desc->state == RX_DATA &&
            rx_desc->srce == srce &&
            rx_desc->tid == tid) {
        uint32_t seq = id & 255;

        if (rx_desc->seq == seq) {
            rx_desc->seq++;
            rx_desc->buf[2 * seq] = d1;
            rx_desc->buf[2 * seq + 1] = d2;

            if (seq == rx_desc->last) {
                // can_data_timeout
//##            event_cancel(rx_desc->event, rx_desc->event_id);

                rx_desc->state = RX_CLOSE_REQ;
                rx_desc->retry = RX_CLOSE_RETRY;

                msg_queue_insert(rx_desc->msg);

                LPC_GPIO0->FIOSET = LED_5;
                can_timeout = 25;

                can_tx2(srce, (CAN_CLOSE_REQ << 16) + (tid << 11) +
                        (rid << 8), 0, 0);

                timeout_event(rx_desc, can_close_timeout, rid, CAN_CLOSE_TO);
            }
        } else {        // Send NACK
            STAT(rxseq_error);
            can_tx2(srce, (CAN_NACK << 16) + (tid << 11) + (rid << 8) +
                    rx_desc->seq, 0, 0);
        }
    }
}


//------------------------------------------------------------------------------

//! Operations that can be run "immediately" by can_proc()
static const event_proc proc_list[] = {
    proc_reset,
    proc_power,
    proc_led            // inaccessible?
};

#define MAX_PROC 1      //!< UPB of "proc_list" array

//! \brief Add a command immediately; handles CAN_PROC events
//! \details Must be one of the ops listed in ::proc_list
//! \param[in] id: Describes what is going on
//! \param[in] d1: First payload word
//! \param[in] d2: Second payload word
static void can_proc(uint32_t id, uint32_t d1, uint32_t d2)
{
    uint32_t op = id & 255;

    if (op <= MAX_PROC) {
        event_t *e = event_new(proc_list[op], d1, d2);
        if (e != NULL) {
            proc_queue_add(e);
        }
    }
}


//------------------------------------------------------------------------------


extern void proc_setup(uint32_t d1, uint32_t d2);

//! Sub-opcodes used by can_exec()
enum can_exec_opcodes {
    CAN_EXEC_NOP = 0,
    CAN_EXEC_REQ = 1,
    CAN_EXEC_ACK = 2,
};

uint32_t can_req[CAN_SIZE];     //!< Count CAN_EXEC_REQ
uint32_t can_ack[CAN_SIZE];     //!< Count CAN_EXEC_ACK

//! \brief Immediate execute. Arranges for proc_setup() to be called
//! \param[in] id: Describes what is going on
//! \param[in] d1: First payload word
//! \param[in] d2: Second payload word
static void can_exec(uint32_t id, uint32_t d1, uint32_t d2)
{
    uint32_t srce = (id >> 19) & 31;
    uint32_t seq = (id >> 8) & 255;
    uint32_t op = id & 255;

    can_status[srce] = 5;                       //##

    if (op == CAN_EXEC_NOP) {                   // NOP
        // Do nothing
    } else if (op == CAN_EXEC_REQ) {            // Request for ACK
        if (! had_CAN_req) {
            event_t *e = event_new(proc_setup, d1, d2);
            if (e != NULL) {
                proc_queue_add(e);
                had_CAN_req = true;
            }
        }

        can_req[srce]++;

        if (srce == 0) {
            LPC_GPIO0->FIOPIN ^= LED_4;
            LPC_GPIO0->FIOSET = LED_3;
            bus_timeout = 100;  // 1 second
        }

        uint32_t *bs = (uint32_t *) &board_stat[can_ID];

        can_tx3_nr(31, (CAN_EXEC << 16) + (seq << 8) + CAN_EXEC_ACK,
                bs[seq + 0], bs[seq + 1]);
    } else if (op == CAN_EXEC_ACK) {            //++ Process ACK (can_ID == 0)
        can_ack[srce]++;

        if (can_ID == 0) {
            LPC_GPIO0->FIOSET = LED_3;
            bus_timeout = 100;
        }

        if (srce < CAN_SIZE) {
            uint32_t *bs = (uint32_t *) &board_stat[srce];
            bs[seq + 0] = d1;
            bs[seq + 1] = d2;
        }
    }
}

//! \brief Asks another BMP to run a proc from ::proc_list
//! \param[in] dest: Who to ask
//! \param[in] op: Describes what operation to run
//! \param[in] arg1: First payload word
//! \param[in] arg2: Second payload word
void can_proc_cmd(uint32_t dest, uint32_t op, uint32_t arg1, uint32_t arg2)
{
    can_tx1(dest, (CAN_PROC << 16) + op, arg1, arg2);
}


//------------------------------------------------------------------------------

//! The index of the next board to talk to in ::can2board
static uint32_t can_next;
//! Sequence number for CAN requests
static uint32_t can_seq;

//! \brief This is called every 10ms on all boards.
//! \details
//! Non-zero boards just use it to timeout the CAN LED. The zero board uses
//! it to ping all other boards and send them "config1" and "config2"
void can_timer(void)
{
    if (bus_timeout && --bus_timeout == 0) {
        LPC_GPIO0->FIOCLR = LED_3;
    }
    if (can_timeout && --can_timeout == 0) {
        LPC_GPIO0->FIOCLR = LED_5;
    }
    if (can_ID != 0) {
        return;
    }
    can_status[0] = 1;

    while (1) {
        can_next++;

        if (can_next == CAN_SIZE) {
            can_next = 1;

            can_seq += 2;
            if (can_seq == sizeof(board_stat_t) / 4) {
                can_seq = 0;
            }
        }

        if (can2board[can_next] != 0) {         // Skip unused CAN addresses
            break;
        }
    }

#if 0
    if (can_status[can_next] && --can_status[can_next] == 0) {
        io_printf(IO_CAN, ">> %u\n", can_next);
        LPC_GPIO0->FIOSET = LED_6;
    }
#endif

    can_req[can_next]++;

    can_tx3_nr(can_next, (CAN_EXEC << 16) + (can_seq << 8) + CAN_EXEC_REQ,
            config1, config2);
}


//------------------------------------------------------------------------------

//! \brief Handler for receiving a message off the CAN
//! \details Delegates to:
//! * can_data()
//! * can_open_req()
//! * can_open_ack()
//! * can_nack()
//! * can_close_req()
//! * can_close_ack()
//! * can_proc()
//! * can_exec()
void CAN_IRQHandler(void)
{
    uint32_t icr = LPC_CAN1->ICR;

    if (icr & 1) {              // RxBuf
        uint32_t id = LPC_CAN1->RID;
        uint32_t op = (id >> 16) & 7;

        uint32_t rda = LPC_CAN1->RDA;
        uint32_t rdb = LPC_CAN1->RDB;

        LPC_CAN1->CMR = 4;      // Release RxBuf

        switch (op) {
        case CAN_DATA:
            can_data(id, rda, rdb);
            break;

        case CAN_OPEN_REQ:
            can_open_req(id);
            break;

        case CAN_OPEN_ACK:
            can_open_ack(id);
            break;

        case CAN_NACK:
            can_nack(id);
            break;

        case CAN_CLOSE_REQ:
            can_close_req(id);
            break;

        case CAN_CLOSE_ACK:
            can_close_ack(id);
            break;

        case CAN_PROC:
            can_proc(id, rda, rdb);
            break;

        case CAN_EXEC:
            can_exec(id, rda, rdb);
            break;

        default:
            break;
        }
    }
}


//------------------------------------------------------------------------------

//! Clock divisor for the CAN
// Double speed CAN clock!
#define CAN_CLOCK       CLKPWR_PCLKSEL_CCLK_DIV_2
//#define CAN_CLOCK     CLKPWR_PCLKSEL_CCLK_DIV_1

//! \brief Initialises the CAN controller
//! \param[in] id: Our ID
void configure_can(uint32_t id)
{
    clock_div(CLKPWR_PCLKSEL_CAN1, CAN_CLOCK);
    clock_div(CLKPWR_PCLKSEL_CAN2, CAN_CLOCK);
    clock_div(CLKPWR_PCLKSEL_ACF, CAN_CLOCK);

    // Set up address filters

    for (uint32_t i = 0; i < 512; i++) {
        LPC_CANAF_RAM->mask[i] = 0;
    }
    LPC_CANAF_RAM->mask[0] = id << 24;
    LPC_CANAF_RAM->mask[1] = (id << 24) + 0xffffff;
    LPC_CANAF_RAM->mask[2] = 31 << 24;
    LPC_CANAF_RAM->mask[3] = (31 << 24) + 0xffffff;

    LPC_CANAF->ENDofTable = 16;
    LPC_CANAF->AFMR = 0;

    // Set up CAN1 controller

    // Params for 1MHz CAN bus with 50MHz PCLK or 2MHz CAN bus with 100MHz PCLK
    const uint32_t TSEG1 = 5;   // 12 (1MHz CAN - 100MHz PCLK)
    const uint32_t TSEG2 = 2;   // 5
    const uint32_t BRP   = 4;   // 4
    const uint32_t SJW   = 2;   // 3

    LPC_CAN1->BTR = (TSEG2 << 20) | (TSEG1 << 16) | (SJW << 14) | BRP;
    LPC_CAN1->MOD = 0x08;               // Enable - prio mode
    LPC_CAN1->TFI1 = 0x80080001;        // Set up TFI1-3
    LPC_CAN1->TFI2 = 0x80080001;        // TX3 has highest priority
    LPC_CAN1->TFI3 = 0x80080000;        // TX1-2 have equal lower priority
    LPC_CAN1->IER = 1;                  // Enable receive interrupt

    NVIC_SetPriority(CAN_IRQn, 1);
    NVIC_EnableIRQ(CAN_IRQn);
}
