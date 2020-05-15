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
//! \brief Header file describing the SpiNNaker API runtime environment
#ifndef __SPIN1_API_H__
#define __SPIN1_API_H__

#include "sark.h"

// ------------------------------------------------------------------------
// Useful SpiNNaker parameters
// ------------------------------------------------------------------------
/* shared memory */
/* system RAM address and size */
//! System RAM base address
#define SPINN_SYSRAM_BASE     SYSRAM_BASE
//! System RAM size
#define SPINN_SYSRAM_SIZE     SYSRAM_SIZE
/* SDRAM address and size */
//! SDRAM base address
#define SPINN_SDRAM_BASE      SDRAM_BASE
//! SDRAM size
#define SPINN_SDRAM_SIZE      SDRAM_SIZE

// ------------------------------------------------------------------------
// general parameters and definitions
// ------------------------------------------------------------------------
/* boolean constants */
//! Boolean true
#define TRUE               (0 == 0)
//! Boolean false
#define FALSE              (0 != 0)
/* function results   */
//! Function result: Success
#define SUCCESS            (uint) 1
//! Function result: Failure
#define FAILURE            (uint) 0

#ifndef NULL
/*! Null pointer value */
#define NULL               0
#endif // NULL

// ------------------------------------------------------------------------
// event definitions
// ------------------------------------------------------------------------
//! event-related parameters
enum {
    NUM_EVENTS =           8, //!< Count of possible events
    MC_PACKET_RECEIVED =   0, //!< Multicast packet received
    DMA_TRANSFER_DONE =    1, //!< DMA transfer complete
    TIMER_TICK =           2, //!< Regular timer tick
    SDP_PACKET_RX =        3, //!< SDP message received
    USER_EVENT =           4, //!< User-triggered interrupt
    MCPL_PACKET_RECEIVED = 5, //!< Multicast packet with payload received
    FR_PACKET_RECEIVED =   6, //!< Fixed route packet received
    FRPL_PACKET_RECEIVED = 7  //!< Fixed route packet with payload received
};

// ------------------------------------------------------------------------
// DMA transfer parameters
// ------------------------------------------------------------------------
//! DMA transfer direction (from core point of view)
enum {
    DMA_READ =          0, //!< DMA read from SDRAM to TCM
    DMA_WRITE =         1  //!< DMA write from TCM to SDRAM
};

// ------------------------------------------------------------------------
// packet parameters
// ------------------------------------------------------------------------
//! Spinnaker packet payload presence flag
enum {
    NO_PAYLOAD =        0, //!< No payload word present
    WITH_PAYLOAD =      1  //!< Payload word present
};

// ------------------------------------------------------------------------
// type definitions
// ------------------------------------------------------------------------

//! User callback for event handling
typedef void (*callback_t) (uint, uint);  // callbacks
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// simulation control functions
// ------------------------------------------------------------------------
//! \brief Begins a simulation by enabling the timer (if called for) and
//!     beginning the dispatcher loop.
//!     Only returns once told to exit (with spin1_exit()).
//! \param[in] sync: Whether to synchronise with other cores
//! \return the exit code
uint spin1_start(sync_bool sync);
//! \brief Begins a simulation by enabling the timer (if called for) and
//!     beginning the dispatcher loop once the system is told (via
//!     spin1_resume()) to resume.
//!     Only returns once told to exit (with spin1_exit()).
//! \return the exit code
uint spin1_start_paused(void);
//! \brief Terminates a simulation passing a return code.
//! \note This function returns! This technically terminates the
//!     simulation loop, not the application.
//! \param[in] error: error exit code
void spin1_exit(uint error);

//! \brief Set the timer tick rate and phase offset.
//! \param[in] time: The timer tick rate
//! \param[in] phase: The phase offset
void spin1_set_timer_tick_and_phase(uint time, uint phase);

//! \brief Set the timer tick rate. No phase offset.
//! \param[in] time: The timer tick rate
static inline void spin1_set_timer_tick(uint time)
{
    spin1_set_timer_tick_and_phase(time, 0);
}

//! \brief Returns the number of timer periods which have elapsed since
//!     the beginning of the simulation.
//! \return Timer ticks since beginning of simulation.
uint spin1_get_simulation_time(void);
//! \brief Pause the simulation on this core. Waits for spin1_resume().
void spin1_pause(void);
//! \brief Resume the simulation on this core that was paused with spin1_pause().
//! \param[in] sync: Whether to synchronise the resume with other cores
void spin1_resume(sync_bool sync);
//! \brief Soft-RTEs the core.
//! \details
//!     Sets the CPU into an RTE code and stops the timer.
//! \note This may return!
//! \param[in] code: the error code
void spin1_rte(rte_code code);

/*!
* \brief This function implements a delay measured in microseconds.
* \details
*  The function _busy waits_ to implement the delay.
* \param[in] n: requested delay (in microseconds)
*/
static inline void spin1_delay_us(uint n) {
    sark_delay_us(n);
}

// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// callback and task functions
// ------------------------------------------------------------------------
//! \brief Enables a callback for a class of event
//! \details
//!     This function sets the given callback to be scheduled on occurrence
//!     of the specified event. The priority argument dictates the order in
//!     which callbacks are executed by the scheduler.
//! \param[in] event_id: Which event to enable
//! \param[in] cback: The callback handler
//! \param[in] priority: The priority of interrupt handling.
//!      0 = non-queueable callback (associated to irq)
//!    > 0 = queueable callback
//!    < 0 = preeminent callback (associated to fiq)
NONNULL void
spin1_callback_on(uint event_id, callback_t cback, int priority);

//! \brief Disables a callback for a class of event
//! \details This function disables the callback for the specified event.
//! \param[in] event_id: Which event to disable
void spin1_callback_off(uint event_id);

//! \brief This function places a cback into the scheduling queue
//!     corresponding to its priority
//! \param[in] cback: callback to be scheduled
//! \param[in] arg0: argument to be passed to the callback
//! \param[in] arg1: argument to be passed to the callback
//! \param[in] priority: cback priority
//! \return #SUCCESS or #FAILURE
NONNULL uint
spin1_schedule_callback(callback_t cback, uint arg0, uint arg1, uint priority);

//! \brief This function triggers a USER EVENT, i.e., a software interrupt.
//! \param[in] arg0: argument to be passed to the callback
//! \param[in] arg1: argument to be passed to the callback
//! \return #SUCCESS or #FAILURE
uint spin1_trigger_user_event(uint arg0, uint arg1);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  data transfer functions
// ------------------------------------------------------------------------
//! \brief Initiates a DMA transfer.
//! \param[in] tag: A label for the transfer. Can be used by DMA complete
//! 	callback to work out what to do.
//! \param system_address: Address in SDRAM. May be source or destination.
//! 	Must be word-aligned.
//! \param tcm_address: Address in TCM. May be source or destination.
//! 	Must be word-aligned.
//! \param[in] direction: Which direction is data being moved in.
//! \param[in] length: How much data is being moved.
//! 	Must be a whole number of words.
//! \return a locally-unique DMA transfer ID.
//! 	Can be used by DMA complete callback to identify what finished.
NONNULL uint
spin1_dma_transfer(uint tag, void *system_address, void *tcm_address,
        uint direction, uint length);

//! \brief Flushes any current transfers in the DMA controller.
//! \details
//!     flushes the hardware queue in the DMA controller,
//!     aborts any ongoing transfer in the DMA controller,
//!     clears any pending DMA_COMPLETE interrupts in the DMA controller and
//!     purges any queued DMA_COMPLETE callbacks in the callback queues.
//!     flushes the software DMA queue,
void spin1_dma_flush(void);

//! \brief Directly copies data from \p src to \p dst.
//! \details Does not require alignment.
//! \param[out] dst: Where to copy to
//! \param[in] src: Where to copy from
//! \param[in] len: Number of bytes to copy
NONNULL void
spin1_memcpy(void *dst, void const *src, uint len);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  communications functions
// ------------------------------------------------------------------------

//! \brief Discards all received packets which are yet to be processed.
void spin1_flush_rx_packet_queue(void);
//! \brief Flushes the outbound packet queue.
void spin1_flush_tx_packet_queue(void);
//! \brief Send a packet. Do not normally call directly;
//! 	use spin1_send_mc_packet() and spin1_send_fr_packet() instead.
//! \param[in] key: The key of the packet. Determines destinations.
//! \param[in] data: The payload (if any).
//! \param[in] TCR: Transmission control.
//!     Especially includes the payload-present bit and the packet type.
//! \return #SUCCESS or #FAILURE
uint spin1_send_packet(uint key, uint data, uint TCR);

/*!
* \brief This function enqueues a request to send a multicast packet.
* \details
*  If the software buffer is full then a failure code is returned. If the
*  comms controller hardware buffer and the software buffer are empty then the
*  the packet is sent immediately, otherwise it is placed in a queue to be
*  consumed later by cc_tx_empty() interrupt service routine.
*
* \param[in] key: packet routing key
* \param[in] data: packet payload
* \param[in] load: 0 = no payload (ignore data param), 1 = send payload
*
* \return #SUCCESS or #FAILURE
*/

static inline uint spin1_send_mc_packet(uint key, uint data, uint load) {
    uint tcr = (load) ? PKT_MC_PL : PKT_MC;

    return spin1_send_packet(key, data, tcr);
}

/*!
* \brief This function enqueues a request to send a fixed-route packet.
* \details
*  If the software buffer is full then a failure code is returned. If the
*  comms controller hardware buffer and the software buffer are empty then the
*  the packet is sent immediately, otherwise it is placed in a queue to be
*  consumed later by cc_tx_empty() interrupt service routine.
*
* \param[in] key: packet routing key
* \param[in] data: packet payload
* \param[in] load: 0 = no payload (ignore data param), 1 = send payload
*
* \return #SUCCESS or #FAILURE
*/

static inline uint spin1_send_fr_packet(uint key, uint data, uint load) {
    uint tcr = (load) ? PKT_FR_PL : PKT_FR;

    return spin1_send_packet(key, data, tcr);
}

// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// SDP related functions
// ------------------------------------------------------------------------

//! \brief Frees a received SDP message.
//! \param[in] msg: The message to free.
static inline NONNULL void spin1_msg_free(sdp_msg_t *msg) {
    sark_msg_free(msg);
}

//! \brief Gets an SDP message that has been sent to this core.
//! \return The SDP message. Must be freed with spin1_msg_free()
static inline sdp_msg_t* spin1_msg_get(void) {
    return sark_msg_get();
}

//! \brief Sends an SDP message.
//! \param[in] msg: The message to send.
//! \param[in] timeout: How long to wait for the message to be enqueued by
//!     SC&MP.
//! \return #SUCCESS or #FAILURE
static inline NONNULL uint
spin1_send_sdp_msg(sdp_msg_t *msg, uint timeout) {
    return sark_msg_send(msg, timeout);
}

// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  interrupt control functions
// ------------------------------------------------------------------------
/*!
* \brief
*  Sets the I bit in the CPSR in order to disable IRQ
*  interrupts to the processor.
*
* \return
*  state of the CPSR before the interrupt disable
*/
#ifdef THUMB
extern uint spin1_irq_disable(void);
#elif defined(__GNUC__)
__inline __attribute__((always_inline)) uint spin1_irq_disable(void)
{
    uint old_val, new_val;

    asm volatile (
    "mrs	%[old_val], cpsr \n\
     orr	%[new_val], %[old_val], #0x80 \n\
     msr	cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );

    return old_val;
}
#else
__forceinline uint spin1_irq_disable(void)
{
    uint old_val, new_val;

    __asm { mrs old_val, cpsr }
    __asm { orr new_val, old_val, 0x80 }
    __asm { msr cpsr_c, new_val }

    return old_val;
}
#endif

/*!
* \brief
*  Sets the F bit in the CPSR in order to disable
*  FIQ interrupts in the processor.
*
* \return
*  state of the CPSR before the interrupts disable
*/
#ifdef THUMB
extern uint spin1_fiq_disable(void);
#elif defined(__GNUC__)
__inline __attribute__((always_inline)) uint spin1_fiq_disable(void)
{
    uint old_val, new_val;

    asm volatile (
    "mrs	%[old_val], cpsr \n\
     orr	%[new_val], %[old_val], #0x40 \n\
     msr	cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );

    return old_val;
}
#else
__forceinline uint spin1_fiq_disable(void)
{
    uint old_val, new_val;

    __asm { mrs old_val, cpsr }
    __asm { orr new_val, old_val, 0x40 }
    __asm { msr cpsr_c, new_val }

    return old_val;
}
#endif

/*!
* \brief
*  Sets the F and I bits in the CPSR in order to disable
*  FIQ and IRQ interrupts in the processor.
*
* \return
*  state of the CPSR before the interrupts disable
*/
#ifdef THUMB
extern uint spin1_int_disable(void);
#elif defined(__GNUC__)
__inline __attribute__((always_inline)) uint spin1_int_disable(void)
{
    uint old_val, new_val;

    asm volatile (
    "mrs	%[old_val], cpsr \n\
     orr	%[new_val], %[old_val], #0xc0 \n\
     msr	cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );

    return old_val;
}
#else
__forceinline uint spin1_int_disable(void)
{
    uint old_val, new_val;

    __asm { mrs old_val, cpsr }
    __asm { orr new_val, old_val, 0xc0 }
    __asm { msr cpsr_c, new_val }

    return old_val;
}
#endif

/*!
* \brief
*  Sets the CPSR to the value given in \p sr, in order to
*  restore the CPSR following a call to spin1_irq_disable().
*
* \param[in] value: value with which to set the CPSR
*/
#ifdef THUMB
extern void spin1_mode_restore(uint value);
#elif defined(__GNUC__)
__inline __attribute__((always_inline)) void spin1_mode_restore(uint value)
{
    asm volatile (
    "msr	cpsr_c, %[value]"
    :
    : [value] "r" (value)
    :);
}
#else
__forceinline void spin1_mode_restore(uint value)
{
    __asm { msr cpsr_c, value }
}
#endif
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  system resources access functions
// ------------------------------------------------------------------------
//! \brief Returns a global (machine-wide) ID for the processor.
//! \return Chip ID in bits [20:5], core ID in bits [4:0].
uint  spin1_get_id(void);
//! \brief Returns the core ID.
//! \return Core ID in the bottom 5 bits.
uint  spin1_get_core_id(void);
//! \brief Returns the (machine-wide) chip ID.
//! \return Chip ID in the bottom 16 bits.
uint  spin1_get_chip_id(void);

#if 0
//## This routine has been removed - use "rtr_alloc", "rtr_mc_set" instead
uint  spin1_set_mc_table_entry(uint entry, uint key, uint mask, uint route);
#endif

/*!
* \brief
*  Returns a pointer to a newly-allocated block of memory of size "bytes" in
*  DTCM.
*
* \param[in] bytes: size, in bytes, of the requested memory block
*
* \return
*  pointer to the requested memory block, or 0 if unavailable
*/
static inline SARK_IS_A_MALLOC(1) void*
spin1_malloc(uint bytes) {
    return sark_alloc(bytes, 1);
}

/*!
* \brief
*  This function controls LEDs according to an input pattern.
* \details
*  Macros for turning LED number N on, off or inverted are
*  defined in spinnaker.h.
*
*  To turn LEDs 0 and 1 on, then invert LED2 and finally
*  turn LED 0 off:
*
*      spin1_led_control(LED_ON(0) + LED_ON(1));
*      spin1_led_control(LED_INV(2));
*      spin1_led_control(LED_OFF(0));
*
* \param[in] p: led control word
*/
static inline void spin1_led_control(uint p) {
    sark_led_set(p);
}
// ------------------------------------------------------------------------


// ----------------------------------
// pseudo-random number generation
// ----------------------------------

/*!
* \brief
*  This function is used to initialize the seed for the
*  pseudo-random number generator.
*
* \param[in] seed: The seed for the PRNG
*/
static inline void spin1_srand(uint seed) {
    sark_srand(seed);
}

/*!
* \brief
*  This function generates a pseudo-random 32-bit integer.
* \details
*  Taken from "Programming Techniques"
*  ARM document ARM DUI 0021A
*
* \return
*  32-bit pseudo-random integer
*/
static inline uint spin1_rand(void) {
    return sark_rand();
}


// ------------------------------------------------------------------------
//!  diagnostic data available to the application
// ------------------------------------------------------------------------
typedef struct {
    uint exit_code;                     //!< simulation exit code
    uint warnings;                      //!< warnings type bit map
    uint total_mc_packets;              //!< total routed MC packets during simulation
    uint dumped_mc_packets;             //!< total dumped MC packets by the router
    volatile uint discarded_mc_packets; //!< total discarded MC packets by API
    uint dma_transfers;                 //!< total DMA transfers requested
    uint dma_bursts;                    //!< total DMA bursts completed
    uint dma_queue_full;                //!< dma queue full count
    uint task_queue_full;               //!< task queue full count
    uint tx_packet_queue_full;          //!< transmitter packet queue full count
    uint writeBack_errors;              //!< write-back buffer error count
    uint total_fr_packets;              //!< total routed FR packets during simulation
    uint dumped_fr_packets;             //!< total dumped FR packets by the router
    uint discarded_fr_packets;          //!< total discarded FR packets by API
    uint in_timer_callback;             //!< bool which states if currently in timer callback
    uint number_timer_tic_in_queue;     //!< the number of timer tic callbacks in the queue
    uint largest_number_of_concurrent_timer_tic_overruns;
                                        //!< the max number of timer tics callbacks being queued at any time
    uint total_times_tick_tic_callback_overran;
                                        //!< the total number of times the timer tic callback overran
} diagnostics_t;

//! Miscellaneous diagnostic information, available to the application.
extern diagnostics_t diagnostics;
extern uchar         leadAp;   //!< lead appl. core has special functions
// ------------------------------------------------------------------------


#endif /* __SPIN1_API_H__ */
