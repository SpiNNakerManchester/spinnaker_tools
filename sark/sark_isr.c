//------------------------------------------------------------------------------
//! \file
//! \brief     Interrupt handlers for SARK
//!
//! \copyright &copy; The University of Manchester - 2009-2019
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//! \details
//! In general, for the classes of interrupt with in this code, the options
//! for handling are:
//!
//! 1. to dump the interrupt (clear the hardware but otherwise do nothing
//! 2. to process the interrupt as a fast interrupt, using the FIQ (a very
//!    limited number of event types can use this at a time)
//! 3. to process the interrupt immediately as a standard interrupt.
//! 4. to clear the hardware rapidly and enqueue a user-level callback for
//!    handling the results at a later point.
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sark.h>

//------------------------------------------------------------------------------


extern void timer2_int(void);

//! Interrupt handler for timer 2. Delegates to timer2_int()
INT_HANDLER timer2_int_han(void)
{
    timer2_int();
}

//! \details Just throws the packet away.
INT_HANDLER sark_fiq_han(void)
{
    (void) cc[CC_RXKEY];
    sark.pkt_count++;
}

//------------------------------------------------------------------------------

//! \brief SpiNNaker packet may be transmitted interrupt handler.
//!
//! Sends the next packet on the event_data_t::pkt_queue. Disables itself if
//! no packet is waiting.
INT_HANDLER txpkt_int_han(void)
{
    event.pkt_remove = (event.pkt_remove + 1) & (event.pkt_size - 1);

    pkt_t *pkt = event.pkt_queue + event.pkt_remove;

    if (pkt->ctrl & 1) {
        cc[CC_TCR] = pkt->ctrl;
    }
    if (pkt->ctrl & 2) {
        cc[CC_TXDATA] = pkt->data;
    }

    cc[CC_TXKEY] = pkt->key;

    event.pkt_count--;
    if (event.pkt_count == 0) {
        vic[VIC_DISABLE] = 1 << CC_TNF_INT;
    }

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Dump handler for user-requested interrupts.
static INT_HANDLER user_null(void)
{
    vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

    uint slot = sark_vec->event[EVENT_USER].slot;

    if (slot != SLOT_FIQ) {
        vic[VIC_VADDR] = (uint) vic;
    }
}

//! Interrupt handler for user-requested interrupts.
static INT_HANDLER user_irq(void)
{
    vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_USER].proc;

    event.user = 0;
    proc(event.arg1, event.arg2);

    vic[VIC_VADDR] = (uint) vic;
}

//! FIQ handler for user-requested interrupts.
static INT_HANDLER user_fiq(void)
{
    vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_USER].proc;

    event.user = 0;
    proc(event.arg1, event.arg2);
}

//! Queue handler for user-requested interrupts.
static INT_HANDLER user_queue(void)
{
    vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_USER].proc;
    event_priority priority = (event_priority) sark_vec->event[EVENT_USER].priority;

    event.user = 0;
    event_queue_proc(proc, event.arg1, event.arg2, priority);

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Dump handler for SDP reception interrupts.
static INT_HANDLER sdp_null(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

    uint slot = sark_vec->event[EVENT_USER].slot;

    if (slot != SLOT_FIQ) {
        vic[VIC_VADDR] = (uint) vic;
    }
}

//! Interrupt handler for SDP reception interrupts.
static INT_HANDLER sdp_irq(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

    sdp_msg_t *msg = event.msg;
    uint port = msg->dest_port >> PORT_SHIFT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SDP].proc;

    proc((uint) msg, port);

    vic[VIC_VADDR] = (uint) vic;
}

//! FIQ handler for SDP reception interrupts.
static INT_HANDLER sdp_fiq(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

    sdp_msg_t *msg = event.msg;
    uint port = msg->dest_port >> PORT_SHIFT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SDP].proc;

    proc((uint) msg, port);
}

//! Queue handler for SDP reception interrupts.
static INT_HANDLER sdp_queue(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

    sdp_msg_t *msg = event.msg;
    uint port = msg->dest_port >> PORT_SHIFT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SDP].proc;
    event_priority priority = (event_priority) sark_vec->event[EVENT_SDP].priority;

    event_queue_proc(proc, (uint) msg, port, priority);

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Dump handler for SpiNNaker packet reception interrupts.
static INT_HANDLER rxpkt_null(void)
{
    uint key = cc[CC_RXKEY];

    uint slot = sark_vec->event[EVENT_RXPKT].slot;

    if (slot != SLOT_FIQ) {
        vic[VIC_VADDR] = (uint) vic;
    }
}

//! Interrupt handler for SpiNNaker packet reception interrupts.
static INT_HANDLER rxpkt_irq(void)
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_RXPKT].proc;

    proc(key, data);

    vic[VIC_VADDR] = (uint) vic;
}

//! FIQ handler for SpiNNaker packet reception interrupts.
static INT_HANDLER rxpkt_fiq(void)
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_RXPKT].proc;

    proc(key, data);
}

//! Queue handler for SpiNNaker packet reception interrupts.
static INT_HANDLER rxpkt_queue(void)
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_RXPKT].proc;
    event_priority priority = (event_priority) sark_vec->event[EVENT_RXPKT].priority;

    event_queue_proc(proc, key, data, priority);

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Dump handler for timer 1 interrupts.
static INT_HANDLER timer_null(void)
{
    tc[T1_INT_CLR] = (uint) tc;
    vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

    uint slot = sark_vec->event[EVENT_TIMER].slot;

    if (slot != SLOT_FIQ) {
        vic[VIC_VADDR] = (uint) vic;
    }
}

//! Interrupt handler for timer 1 interrupts.
static INT_HANDLER timer_irq(void)
{
    tc[T1_INT_CLR] = (uint) tc;
    vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

    event.ticks++;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_TIMER].proc;

    proc(event.ticks, 0);

    vic[VIC_VADDR] = (uint) vic;
}

//! FIQ handler for timer 1 interrupts.
static INT_HANDLER timer_fiq(void)
{
    tc[T1_INT_CLR] = (uint) tc;
    vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

    event.ticks++;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_TIMER].proc;

    proc(event.ticks, 0);
}

//! Queue handler for timer 1 interrupts.
static INT_HANDLER timer_queue(void)
{
    tc[T1_INT_CLR] = (uint) tc;
    vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

    event.ticks++;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_TIMER].proc;
    event_priority priority = (event_priority) sark_vec->event[EVENT_TIMER].priority;

    event_queue_proc(proc, event.ticks, 0, priority);

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Dump handler for signals.
static INT_HANDLER sig_null(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

    uint slot = sark_vec->event[EVENT_SIG].slot;

    if (slot != SLOT_FIQ) {
        vic[VIC_VADDR] = (uint) vic;
    }
}

//! Interrupt handler for signals.
static INT_HANDLER sig_irq(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SIG].proc;

    proc(event.signal, 0);

    vic[VIC_VADDR] = (uint) vic;
}

//! FIQ handler for signals.
static INT_HANDLER sig_fiq(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SIG].proc;

    proc(event.signal, 0);
}

//! Queue handler for signals.
static INT_HANDLER sig_queue(void)
{
    vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

    event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SIG].proc;
    event_priority priority = (event_priority) sark_vec->event[EVENT_SIG].priority;

    event_queue_proc(proc, event.signal, 0, priority);

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! FIQ interrupt handlers, indexed by ::event_type_e
const int_handler fiq_events[] = {
    timer_fiq,
    rxpkt_fiq,
    sdp_fiq,
    user_fiq,
    sig_fiq
};

//! Standard interrupt handlers, indexed by ::event_type_e
const int_handler irq_events[] = {
    timer_irq,
    rxpkt_irq,
    sdp_irq,
    user_irq,
    sig_irq
};

//! Event queuers, indexed by ::event_type_e
const int_handler queue_events[] = {
    timer_queue,
    rxpkt_queue,
    sdp_queue,
    user_queue,
    sig_queue
};

//! Null interrupt handlers, indexed by ::event_type_e
const int_handler null_events[] = {
    timer_null,
    rxpkt_null,
    sdp_null,
    user_null,
    sig_null
};

//------------------------------------------------------------------------------
