/*
 * Copyright (c) 2017-2019 The University of Manchester
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

//! \file
//! \brief Interrupt handlers for Spin1API
#include <sark.h>

#include <spin1_api.h>
#include <spin1_api_params.h>
#include <scamp_spin1_sync.h>

//! \brief Wrapper for schedule_sysmode() that handles interrupts
//! \param[in] event_id: ID of the event triggering a callback
//! \param[in] arg0: argument to be passed to the callback
//! \param[in] arg1: argument to be passed to the callback
extern void schedule(uchar event_id, uint arg0, uint arg1);

extern uchar user_pending;
extern uint user_arg0;
extern uint user_arg1;

extern uint ticks;
extern uint timer_tick;
extern uint timer_tick_clocks;
extern int drift;
extern int drift_accum;
extern int drift_sign;
extern int time_to_next_drift_update;

extern dma_queue_t dma_queue;
extern tx_packet_queue_t tx_packet_queue;
extern user_event_queue_t user_event_queue;


/*! \brief Handles an incoming multicast packet.
*
*  This interrupt service routine is called in response to receipt of a packet
*  from the router. Chips are configured such that fascicle processors receive
*  only multicast neural event packets. In response to receipt of a MC packet
*  a callback is scheduled to process the corresponding routing key and data.
*
*  Checking for parity and framing errors is not performed. The VIC is
*  configured so that the interrupts raised by erroneous packets prompt
*  execution of cc_rx_error_isr() which clears them.
*/
INT_HANDLER cc_rx_ready_isr(void)
{
    uint rx_status = cc[CC_RSR];        // Get Rx status

    if (rx_status & PKT_PL) {           // Has payload?
        uint rx_data = cc[CC_RXDATA];
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        // If application callback registered schedule it

        if (callback[MCPL_PACKET_RECEIVED].cback != NULL) {
            schedule(MCPL_PACKET_RECEIVED, rx_key, rx_data);
        } else {
#if (API_DEBUG == TRUE) || (API_DIAGNOSTICS == TRUE)
            diagnostics.discarded_mc_packets++;
        }
#endif
    } else {
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        if (callback[MC_PACKET_RECEIVED].cback != NULL) {
            schedule(MC_PACKET_RECEIVED, rx_key, 0);
        } else {
#if (API_DEBUG == TRUE) || (API_DIAGNOSTICS == TRUE)
            diagnostics.discarded_mc_packets++;
#endif
        }
    }

    // TODO: maybe clear error flags (sticky) in CC

    // Ack VIC
    vic[VIC_VADDR] = (uint) vic;
}
/*
*******/


/*! \brief Handles an incoming multicast packet. (FIQ)
*
*  This interrupt service routine is called in response to receipt of a packet
*  from the router. Chips are configured such that fascicle processors receive
*  only multicast neural event packets. In response to receipt of a MC packet
*  a callback is scheduled to process the corresponding routing key and data.
*
*  Checking for parity and framing errors is not performed. The VIC is
*  configured so that the interrupts raised by erroneous packets prompt
*  execution of cc_rx_error_isr() which clears them.
*/
INT_HANDLER cc_rx_ready_fiqsr(void)
{
    uint rx_status = cc[CC_RSR];        // Get Rx status

    if (rx_status & PKT_PL) {           // Has payload?
        uint rx_data = cc[CC_RXDATA];
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        // Execute preeminent callback

        callback[MCPL_PACKET_RECEIVED].cback(rx_key, rx_data);
    } else {
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        callback[MC_PACKET_RECEIVED].cback(rx_key, 0);
    }

    // TODO: maybe clear error flags (sticky) in CC
}
/*
*******/


/*! \brief Handles an incoming fixed-route packet.
*
*  This interrupt service routine is called in response to receipt of a packet
*  from the router. Chips are configured such that fascicle processors receive
*  only multicast neural event packets. In response to receipt of a MC packet
*  a callback is scheduled to process the corresponding routing key and data.
*
*  Checking for parity and framing errors is not performed. The VIC is
*  configured so that the interrupts raised by erroneous packets prompt
*  execution of cc_rx_error_isr() which clears them.
*/
INT_HANDLER cc_fr_ready_isr(void)
{
    uint rx_status = cc[CC_RSR];        // Get Rx status

    if (rx_status & PKT_PL) {           // Has payload?
        uint rx_data = cc[CC_RXDATA];
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        // If application callback registered schedule it

        if (callback[FRPL_PACKET_RECEIVED].cback != NULL) {
            schedule(FRPL_PACKET_RECEIVED, rx_key, rx_data);
        } else {
#if (API_DEBUG == TRUE) || (API_DIAGNOSTICS == TRUE)
            diagnostics.discarded_fr_packets++;
#endif
        }
    } else {
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        if (callback[FR_PACKET_RECEIVED].cback != NULL) {
            schedule(FR_PACKET_RECEIVED, rx_key, 0);
        } else {
#if (API_DEBUG == TRUE) || (API_DIAGNOSTICS == TRUE)
            diagnostics.discarded_fr_packets++;
#endif
        }
    }

    // TODO: maybe clear error flags (sticky) in CC

    // Ack VIC
    vic[VIC_VADDR] = (uint) vic;
}
/*
*******/


/*! \brief Handles an incoming fixed-route packet. (FIQ)
*
*  This interrupt service routine is called in response to receipt of a packet
*  from the router. Chips are configured such that fascicle processors receive
*  only multicast neural event packets. In response to receipt of a MC packet
*  a callback is scheduled to process the corresponding routing key and data.
*
*  Checking for parity and framing errors is not performed. The VIC is
*  configured so that the interrupts raised by erroneous packets prompt
*  execution of cc_rx_error_isr() which clears them.
*/
INT_HANDLER cc_fr_ready_fiqsr(void)
{
    uint rx_status = cc[CC_RSR];        // Get Rx status

    if (rx_status & PKT_PL) {           // Has payload?
        uint rx_data = cc[CC_RXDATA];
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        // Execute preeminent callback

        callback[FRPL_PACKET_RECEIVED].cback(rx_key, rx_data);
    } else {
        uint rx_key = cc[CC_RXKEY];     // also clears interrupt

        callback[FR_PACKET_RECEIVED].cback(rx_key, 0);
    }

    // TODO: maybe clear error flags (sticky) in CC
}
/*
*******/


/*! \brief Clears errors from the communications controller
*
*  This interrupt service routine is called in response to receipt of a packet
*  from the router with either parity or framing errors. The routine simply
*  clears the error and disposes of the packet. The monitor processor may
*  observe packet errors by reading from the router diagnostic registers.
*/
INT_HANDLER cc_rx_error_isr(void)
{
    // Consume erroneous packet (also clears interrupt)

    uint sink = cc[CC_RXKEY];

    // clear error flags (sticky) in CC and ack VIC

    cc[CC_RSR] = 0;
    vic[VIC_VADDR] = 1;
}
/*
*******/


/*! \brief Comms controller ready to send interrupt handler.
*
*  This interrupt service function is called when the comms controller
*  transmit buffer is empty. The function dequeues packets queued for
*  transmission by spin1_send_mc_packet() function and writes them to the
*  comms controller hardware, until either the packet queue is empty or the
*  comms controller is full.
*
*  This interrupt is only enabled when there is at least one message to send
*  in the transmit queue.
*/
INT_HANDLER cc_tx_empty_isr(void)
{
    //TODO: should use TX_not_full interrupt in SpiNNaker -- more efficient!

    // Clear the sticky TX full bit
    cc[CC_TCR] = TX_TCR_MCDEFAULT;
    (void) cc[CC_TCR];  // needed to avoid a RAW hazard accessing CC_TCR

    // Drain queue: send packets while queue not empty and CC not full

    while (tx_packet_queue.start != tx_packet_queue.end
            && ~cc[CC_TCR] & 0x40000000) {
        // Dequeue packet

        uint key = tx_packet_queue.queue[tx_packet_queue.start].key;
        uint data = tx_packet_queue.queue[tx_packet_queue.start].data;
        uint TCR = tx_packet_queue.queue[tx_packet_queue.start].TCR;

        tx_packet_queue.start = (tx_packet_queue.start + 1) % TX_PACKET_QUEUE_SIZE;

        // Send the packet

        cc[CC_TCR] = TCR;

        if (TCR & PKT_PL) {
            cc[CC_TXDATA] = data;
        }

        cc[CC_TXKEY] = key;
        (void) cc[CC_TCR];  // needed to avoid a RAW hazard accessing CC_TCR
    }

    // If queue empty turn off tx_empty interrupt

    if (tx_packet_queue.start == tx_packet_queue.end) {
        vic[VIC_DISABLE] = 1 << CC_TMT_INT;
    }

    // ack VIC
    vic[VIC_VADDR] = 1;
}
/*
*******/


/*! \brief DMA complete interrupt handler.
*
*  This interrupt service routine is called upon completion of a DMA transfer.
*  A user callback is scheduled (with two parameters, the ID of the completed
*  transfer and the user-provided transfer tag) and the next DMA transfer
*  request is dequeued and fulfilled. The completion and subsequent scheduling
*  of transfers must be atomic (as they are in this uninterruptable ISR)
*  otherwise transfer requests may not be completed in the order they were
*  made.
*/
INT_HANDLER dma_done_isr(void)
{
    // Clear transfer done interrupt in DMAC
    dma[DMA_CTRL] = 0x8;

    // Prepare data for callback before triggering a new DMA transfer

    uint completed_id  = dma_queue.queue[dma_queue.start].id;
    uint completed_tag = dma_queue.queue[dma_queue.start].tag;

    //TODO: can schedule up to 2 transfers if DMA free
    // Update queue pointer and trigger new transfer if queue not empty

    dma_queue.start = (dma_queue.start + 1) % DMA_QUEUE_SIZE;

    if (dma_queue.start != dma_queue.end) {
        uint *system_address = dma_queue.queue[dma_queue.start].system_address;
        uint *tcm_address = dma_queue.queue[dma_queue.start].tcm_address;
        uint  description = dma_queue.queue[dma_queue.start].description;

        dma[DMA_ADRS] = (uint) system_address;
        dma[DMA_ADRT] = (uint) tcm_address;
        dma[DMA_DESC] = description;
    }

    // If application callback registered schedule it

    if (callback[DMA_TRANSFER_DONE].cback != NULL) {
        schedule(DMA_TRANSFER_DONE, completed_id, completed_tag);
    }

    // Ack VIC
    vic[VIC_VADDR] = 1;
}
/*
*******/


/*! \brief DMA complete interrupt handler. (FIQ)
*
*  This interrupt service routine is called upon completion of a DMA transfer.
*  A user callback is scheduled (with two parameters, the ID of the completed
*  transfer and `1' indicating transfer success) and the next DMA transfer
*  request is dequeued and fulfilled. The completion and subsequent scheduling
*  of transfers must be atomic (as they are in this uninterruptable ISR)
*  otherwise transfer requests may not be completed in the order they were
*  made.
*/
INT_HANDLER dma_done_fiqsr(void)
{
    // Clear transfer done interrupt in DMAC

    dma[DMA_CTRL]  = 0x8;

    // Prepare data for callback before triggering a new DMA transfer

    uint completed_id  = dma_queue.queue[dma_queue.start].id;
    uint completed_tag = dma_queue.queue[dma_queue.start].tag;

    //TODO: can schedule up to 2 transfers if DMA free

    // Update queue pointer and trigger new transfer if queue not empty

    dma_queue.start = (dma_queue.start + 1) % DMA_QUEUE_SIZE;

    if (dma_queue.start != dma_queue.end) {
        uint *system_address = dma_queue.queue[dma_queue.start].system_address;
        uint *tcm_address = dma_queue.queue[dma_queue.start].tcm_address;
        uint  description = dma_queue.queue[dma_queue.start].description;

        dma[DMA_ADRS] = (uint) system_address;
        dma[DMA_ADRT] = (uint) tcm_address;
        dma[DMA_DESC] = description;
    }

    // Execute preeminent callback

    callback[DMA_TRANSFER_DONE].cback(completed_id, completed_tag);
}
/*
*******/


/*! \brief DMA error interrupt handler.
*
*  This interrupt service function is called when a DMA transfer error arises.
*  Currently, such an event causes termination of the simulation.
*/
INT_HANDLER dma_error_isr(void)
{
    //TODO: update to other dma error sources when supported
    // deal with write buffer errors
#if (API_WARN == TRUE) && (USE_WRITE_BUFFER == TRUE)
    // increase error count
    wberrors++;
#endif

    // clear write buffer error interrupt in DMAC and ack VIC
    dma[DMA_CTRL]  = 0x20;
    vic[VIC_VADDR] = 1;
}
/*
*******/


/*! \brief Timer 1 interrupt handler.
*
*  This interrupt service routine is called upon countdown of the processor's
*  primary timer to zero. In response, a callback is scheduled.
*/
INT_HANDLER timer1_isr(void)
{
    // Clear timer interrupt
    tc[T1_INT_CLR] = 1;

    // Increment simulation "time"
    ticks++;

    // Update drift if needed
    time_to_next_drift_update -= timer_tick;
    if (time_to_next_drift_update <= 0) {
        drift = timer_tick * sv->clock_drift;
        drift_sign = 1;
        if (drift < 0) {
            drift = -drift;
            drift_sign = -1;
        }
        time_to_next_drift_update += TIME_BETWEEN_SYNC_US;
    }

    // Each timer tick we add on the integer number of clock cycles accumulated
    // and subtract this from the accumulator.
    drift_accum += drift;
    int drift_int = drift_accum & DRIFT_INT_MASK;
    drift_accum -= drift_int;
    drift_int = (drift_int >> DRIFT_FP_BITS) * drift_sign;
    tc[T1_BG_LOAD] = timer_tick_clocks + drift_int;

    // If application callback registered schedule it
    if (callback[TIMER_TICK].cback != NULL) {

        // check for timer tic overload and store in diagnostics
        if (diagnostics.in_timer_callback != 0) {

            // if in timer tic callback already, add to tracker for total failures
            diagnostics.total_times_tick_tic_callback_overran += 1;

            // if number of timer callbacks in queue is greater than previously seen
            if (diagnostics.number_timer_tic_in_queue >
                    diagnostics.largest_number_of_concurrent_timer_tic_overruns) {
                diagnostics.largest_number_of_concurrent_timer_tic_overruns =
                        diagnostics.number_timer_tic_in_queue;
            }
        }
        schedule(TIMER_TICK, ticks, NULL);
    }

    // Ack VIC
    vic[VIC_VADDR] = 1;
}
/*
*******/


/*! \brief Timer 1 interrupt handler. (FIQ)
*
*  This interrupt service routine is called upon countdown of the processor's
*  primary timer to zero. In response, a callback is scheduled.
*/
INT_HANDLER timer1_fiqsr(void)
{
    // clear timer interrupt
    tc[T1_INT_CLR] = 1;

    // Increment simulation "time"
    ticks++;

    // Execute preeminent callback
    callback[TIMER_TICK].cback(ticks, NULL);
}
/*
*******/


/*! \brief Software-initiated interrupt handler.
*
*  This interrupt service routine is called upon receipt of software
*  controller interrupt, triggered by a "USER EVENT".
*/
INT_HANDLER soft_int_isr(void)
{
    uint arg0  = user_event_queue.queue[user_event_queue.start].arg0;
    uint arg1  = user_event_queue.queue[user_event_queue.start].arg1;

    // Update queue pointer
    user_event_queue.start = (user_event_queue.start + 1) % USER_EVENT_QUEUE_SIZE;

    // If the queue is now empty, clear the interrupt
    if (user_event_queue.start == user_event_queue.end) {
        vic[VIC_SOFT_CLR] = (1 << SOFTWARE_INT);
    }

    // If application callback registered schedule it
    if (callback[USER_EVENT].cback != NULL) {
        schedule(USER_EVENT, arg0, arg1);
    }

    // Ack VIC
    vic[VIC_VADDR] = 1;
}
/*
*******/


/*! \brief Software-initiated interrupt handler. (FIQ)
*
*  This interrupt service routine is called upon receipt of software
*  controller interrupt, triggered by a "USER EVENT".
*/
INT_HANDLER soft_int_fiqsr(void)
{
    uint arg0  = user_event_queue.queue[user_event_queue.start].arg0;
    uint arg1  = user_event_queue.queue[user_event_queue.start].arg1;

    // Update queue pointer and trigger new event if queue not empty
    user_event_queue.start = (user_event_queue.start + 1) % USER_EVENT_QUEUE_SIZE;

    // If the queue is now empty, clear the interrupt
    if (user_event_queue.start == user_event_queue.end) {
        vic[VIC_SOFT_CLR] = (1 << SOFTWARE_INT);
    }

    // Execute preeminent callback
    callback[USER_EVENT].cback(arg0, arg1);
}
/*
*******/
