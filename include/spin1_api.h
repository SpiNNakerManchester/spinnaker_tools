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

#ifndef __SPIN1_API_H__
#define __SPIN1_API_H__

#include "sark.h"

// ------------------------------------------------------------------------
// Useful SpiNNaker parameters
// ------------------------------------------------------------------------
/* shared memory */
/* system RAM address and size */
#define SPINN_SYSRAM_BASE     SYSRAM_BASE
#define SPINN_SYSRAM_SIZE     SYSRAM_SIZE
/* SDRAM address and size */
#define SPINN_SDRAM_BASE      SDRAM_BASE
#define SPINN_SDRAM_SIZE      SDRAM_SIZE

// ------------------------------------------------------------------------
// general parameters and definitions
// ------------------------------------------------------------------------
/* boolean constants */
#define TRUE               (0 == 0)
#define FALSE              (0 != 0)
/* function results   */
#define SUCCESS            (uint) 1
#define FAILURE            (uint) 0

#ifndef NULL
/* Null pointer value */
#define NULL               0
#endif // NULL

// ------------------------------------------------------------------------
// event definitions
// ------------------------------------------------------------------------
// event-related parameters
#define NUM_EVENTS            8
#define MC_PACKET_RECEIVED    0
#define DMA_TRANSFER_DONE     1
#define TIMER_TICK            2
#define SDP_PACKET_RX         3
#define USER_EVENT            4
#define MCPL_PACKET_RECEIVED  5
#define FR_PACKET_RECEIVED    6
#define FRPL_PACKET_RECEIVED  7

// ------------------------------------------------------------------------
// DMA transfer parameters
// ------------------------------------------------------------------------
// DMA transfer direction (from core point of view)
#define DMA_READ           0
#define DMA_WRITE          1


// ------------------------------------------------------------------------
// packet parameters
// ------------------------------------------------------------------------
// payload presence
#define NO_PAYLOAD         0
#define WITH_PAYLOAD       1


// ------------------------------------------------------------------------
// type definitions
// ------------------------------------------------------------------------

typedef void (*callback_t) (uint, uint);  // callbacks
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// simulation control functions
// ------------------------------------------------------------------------
uint spin1_start (sync_bool sync);
uint spin1_start_paused (void);
void spin1_exit (uint error);
void spin1_set_timer_tick_and_phase(uint time, uint phase);

static inline void spin1_set_timer_tick(uint time)
{
  spin1_set_timer_tick_and_phase(time, 0);
}

uint spin1_get_simulation_time(void);
void spin1_pause (void);
void spin1_resume (sync_bool sync);
void spin1_rte (rte_code code);

/****f* spin1_api.h/spin1_delay_us
*
* SUMMARY
*  This function implements a delay measured in microseconds
*  The function busy waits to implement the delay.
*
* SYNOPSIS
*  void spin1_delay_us(uint n)
*
* INPUTS
*  uint n: requested delay (in microseconds)
*
* SOURCE
*/
static inline void spin1_delay_us(uint n) {
    sark_delay_us(n);
}

// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// callback and task functions
// ------------------------------------------------------------------------
__attribute__((nonnull)) void
spin1_callback_on(uint event_id, callback_t cback, int priority);
void spin1_callback_off(uint event_id);
__attribute__((nonnull)) uint
spin1_schedule_callback(callback_t cback, uint arg0, uint arg1, uint priority);
uint spin1_trigger_user_event(uint arg0, uint arg1);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  data transfer functions
// ------------------------------------------------------------------------
__attribute__((nonnull)) uint
spin1_dma_transfer(uint tag, void *system_address, void *tcm_address,
        uint direction, uint length);
__attribute__((nonnull)) void
spin1_memcpy(void *dst, void const *src, uint len);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  communications functions
// ------------------------------------------------------------------------

void spin1_flush_rx_packet_queue(void);
void spin1_flush_tx_packet_queue(void);
uint spin1_send_packet(uint key, uint data, uint TCR);

/****f* spin1_api.h/spin1_send_mc_packet
*
* SUMMARY
*  This function enqueues a request to send a multicast packet. If
*  the software buffer is full then a failure code is returned. If the comms
*  controller hardware buffer and the software buffer are empty then the
*  the packet is sent immediately, otherwise it is placed in a queue to be
*  consumed later by cc_tx_empty interrupt service routine.
*
* SYNOPSIS
*  uint spin1_send_mc_packet(uint key, uint data, uint load)
*
* INPUTS
*  uint key: packet routing key
*  uint data: packet payload
*  uint load: 0 = no payload (ignore data param), 1 = send payload
*
* OUTPUTS
*  1 if packet is enqueued or sent successfully, 0 otherwise
*
* SOURCE
*/

static inline uint spin1_send_mc_packet(uint key, uint data, uint load) {
    uint tcr = (load) ? PKT_MC_PL : PKT_MC;

    return spin1_send_packet(key, data, tcr);
}

/****f* spin1_api.h/spin1_send_ft_packet
*
* SUMMARY
*  This function enqueues a request to send a fixed-route packet. If
*  the software buffer is full then a failure code is returned. If the comms
*  controller hardware buffer and the software buffer are empty then the
*  the packet is sent immediately, otherwise it is placed in a queue to be
*  consumed later by cc_tx_empty interrupt service routine.
*
* SYNOPSIS
*  uint spin1_send_fr_packet(uint key, uint data, uint load)
*
* INPUTS
*  uint key: packet routing key
*  uint data: packet payload
*  uint load: 0 = no payload (ignore data param), 1 = send payload
*
* OUTPUTS
*  1 if packet is enqueued or sent successfully, 0 otherwise
*
* SOURCE
*/

static inline uint spin1_send_fr_packet(uint key, uint data, uint load) {
    uint tcr = (load) ? PKT_FR_PL : PKT_FR;

    return spin1_send_packet(key, data, tcr);
}

// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// SDP related functions
// ------------------------------------------------------------------------

static inline __attribute__((nonnull)) void spin1_msg_free(sdp_msg_t *msg) {
    sark_msg_free(msg);
}

static inline sdp_msg_t* spin1_msg_get(void) {
    return sark_msg_get();
}

static inline __attribute__((nonnull)) uint
spin1_send_sdp_msg(sdp_msg_t *msg, uint timeout) {
    return sark_msg_send(msg, timeout);
}

// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  interrupt control functions
// ------------------------------------------------------------------------

#ifndef SPIN1_INLINE
#ifdef __GNUC__
#define SPIN1_INLINE static __inline __attribute__((always_inline))
#else
#define SPIN1_INLINE static __forceinline
#endif
#endif

SPIN1_INLINE void spin1_store_barrier(void)
{
#ifdef __GNUC__
    // GCC only really has one kind of barrier, and this is how to do it
    asm volatile("" : : : "cc", "memory");
#else
    __force_stores();
#endif
}

SPIN1_INLINE void spin1_memory_barrier(void)
{
#ifdef __GNUC__
    // GCC only really has one kind of barrier, and this is how to do it
    asm volatile("" : : : "cc", "memory");
#else
    __memory_changed();
#endif
}

/****f* spin1_api.h/spin1_irq_disable
*
* SUMMARY
*  This function sets the I bit in the CPSR in order to disable IRQ
*  interrupts to the processor.
*
* SYNOPSIS
*  uint spin1_irq_disable()
*
* OUTPUTS
*  state of the CPSR before the interrupt disable
*
* SOURCE
*/
#ifdef THUMB
// Implemented in sark_alib.s; needs to be in ARM mode, not THUMB
extern uint spin1_irq_disable(void);
#else
SPIN1_INLINE uint spin1_irq_disable(void)
{
    uint old_val, new_val;

#ifdef __GNUC__
    asm volatile (
    "mrs	%[old_val], cpsr \n\
     orr	%[new_val], %[old_val], #0x80 \n\
     msr	cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );
#else
    __asm { mrs old_val, cpsr }
    __asm { orr new_val, old_val, 0x80 }
    __asm { msr cpsr_c, new_val }
#endif

    spin1_memory_barrier();
    return old_val;
}
#endif

/****f* spin1_api.h/spin1_fiq_disable
*
* SUMMARY
*  This function sets the F bit in the CPSR in order to disable
*  FIQ interrupts in the processor.
*
* SYNOPSIS
*  uint spin1_fiq_disable()
*
* OUTPUTS
*  state of the CPSR before the interrupts disable
*
* SOURCE
*/
#ifdef THUMB
// Implemented in sark_alib.s; needs to be in ARM mode, not THUMB
extern uint spin1_fiq_disable(void);
#else
SPIN1_INLINE uint spin1_fiq_disable(void)
{
    uint old_val, new_val;

#ifdef __GNUC__
    asm volatile (
    "mrs	%[old_val], cpsr \n\
     orr	%[new_val], %[old_val], #0x40 \n\
     msr	cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );
#else
    __asm { mrs old_val, cpsr }
    __asm { orr new_val, old_val, 0x40 }
    __asm { msr cpsr_c, new_val }
#endif

    spin1_memory_barrier();
    return old_val;
}
#endif

/****f* spin1_api.h/spin1_int_disable
*
* SUMMARY
*  This function sets the F and I bits in the CPSR in order to disable
*  FIQ and IRQ interrupts in the processor.
*
* SYNOPSIS
*  uint spin1_int_disable()
*
* OUTPUTS
*  state of the CPSR before the interrupts disable
*
* SOURCE
*/
#ifdef THUMB
// Implemented in sark_alib.s; needs to be in ARM mode, not THUMB
extern uint spin1_int_disable(void);
#else
SPIN1_INLINE uint spin1_int_disable(void)
{
    uint old_val, new_val;

#ifdef __GNUC__
    asm volatile (
    "mrs	%[old_val], cpsr \n\
     orr	%[new_val], %[old_val], #0xc0 \n\
     msr	cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );
#else
    __asm { mrs old_val, cpsr }
    __asm { orr new_val, old_val, 0xc0 }
    __asm { msr cpsr_c, new_val }
#endif

    spin1_memory_barrier();
    return old_val;
}
#endif

/****f* spin1_api.h/spin1_mode_restore
*
* SUMMARY
*  This function sets the CPSR to the value given in parameter sr, in order to
*  restore the CPSR following a call to spin1_irq_disable.
*
* SYNOPSIS
*  void spin1_mode_restore(uint sr)
*
* INPUTS
*  uint sr: value with which to set the CPSR
*
* SOURCE
*/
#ifdef THUMB
// Implemented in sark_alib.s; needs to be in ARM mode, not THUMB
extern void spin1_mode_restore(uint cpsr);
#else
SPIN1_INLINE void spin1_mode_restore(uint sr)
{
    spin1_store_barrier();

#ifdef __GNUC__
    asm volatile (
    "msr	cpsr_c, %[cpsr]"
    :
    : [cpsr] "r" (sr)
    :);
#else
    __asm { msr cpsr_c, sr }
#endif
}
#endif

/****f* spin1_api.c/spin1_irq_enable
*
* SUMMARY
*  This function clears the I bit in the CPSR in order to enable IRQ
*  interrupts to the processor.
*
* SYNOPSIS
*  uint spin1_irq_enable()
*
* OUTPUTS
*  state of the CPSR before the interrupt enable
*
* SOURCE
*/
#ifdef THUMB
// Implemented in sark_alib.s; needs to be in ARM mode, not THUMB
extern uint spin1_irq_enable(void);
#else
SPIN1_INLINE uint spin1_irq_enable(void)
{
    uint old_val, new_val;

    spin1_store_barrier();

#if defined(__GNUC__)
    asm volatile (
    "mrs        %[old_val], cpsr \n\
     bic        %[new_val], %[old_val], #0x80 \n\
     msr        cpsr_c, %[new_val] \n"
     : [old_val] "=r" (old_val), [new_val] "=r" (new_val)
     :
     : );
#else
    __asm { mrs old_val, cpsr }
    __asm { bic new_val, old_val, 0x80 }
    __asm { msr cpsr_c, new_val }
#endif

    return old_val;
}
#endif
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  system resources access functions
// ------------------------------------------------------------------------
uint  spin1_get_id(void);
uint  spin1_get_core_id(void);
uint  spin1_get_chip_id(void);
uint  spin1_set_mc_table_entry(uint entry, uint key, uint mask, uint route);

/****f* spin1_api.h/spin1_malloc
*
* SUMMARY
*  This function returns a pointer to a block of memory of size "bytes".
*
* SYNOPSIS
*  void * spin1_malloc(uint bytes)
*
* INPUTS
*  uint bytes: size, in bytes, of the requested memory block
*
* OUTPUTS
*  pointer to the requested memory block or 0 if unavailable
*
* SOURCE
*/
static inline SARK_IS_A_MALLOC(1) void*
spin1_malloc(uint bytes) {
    return sark_alloc(bytes, 1);
}

/****f* spin1_api.h/spin1_led_control
*
* SUMMARY
*  This function controls LEDs according to an input pattern.
*  Macros for turning LED number N on, off or inverted are
*  defined in spinnaker.h.
*
*  To turn LEDs 0 and 1 on, then invert LED2 and finally
*  turn LED 0 off:
*
*   spin1_led_control (LED_ON (0) + LED_ON (1));
*   spin1_led_control (LED_INV (2));
*   spin1_led_control (LED_OFF (0));
*
* SYNOPSIS
*  void spin1_set_leds(uint p);
*
* INPUTS
*  uint p: led control word
*
* SOURCE
*/
static inline void spin1_led_control(uint p) {
    sark_led_set(p);
}
// ------------------------------------------------------------------------


// ----------------------------------
// pseudo-random number generation
// ----------------------------------

/****f* spin1_api.h/spin1_srand
*
* SUMMARY
*  This function is used to initialize the seed for the
*  pseudo-random number generator.
*
* SYNOPSIS
*  void spin1_srand (uint seed)
*
* SOURCE
*/
static inline void spin1_srand(uint seed) {
    sark_srand(seed);
}

/****f* spin1_api.h/spin1_rand
*
* SUMMARY
*  This function generates a pseudo-random 32-bit integer.
*  Taken from "Programming Techniques"
*  ARM document ARM DUI 0021A
*
* SYNOPSIS
*  uint spin1_rand (void)
*
* OUTPUTS
*  32-bit pseudo-random integer
*
* SOURCE
*/
static inline uint spin1_rand(void) {
    return sark_rand();
}


// ------------------------------------------------------------------------
//  diagnostic data available to the application
// ------------------------------------------------------------------------
typedef struct {
    uint exit_code;                     // simulation exit code
    uint warnings;                      // warnings type bit map
    uint total_mc_packets;              // total routed MC packets during simulation
    uint dumped_mc_packets;             // total dumped MC packets by the router
    volatile uint discarded_mc_packets; // total discarded MC packets by API
    uint dma_transfers;                 // total DMA transfers requested
    uint dma_bursts;                    // total DMA bursts completed
    uint dma_queue_full;                // dma queue full count
    uint task_queue_full;               // task queue full count
    uint tx_packet_queue_full;          // transmitter packet queue full count
    uint writeBack_errors;              // write-back buffer error count
    uint total_fr_packets;              // total routed FR packets during simulation
    uint dumped_fr_packets;             // total dumped FR packets by the router
    uint discarded_fr_packets;          // total discarded FR packets by API
    uint in_timer_callback;             // bool which states if currently in timer callback
    uint number_timer_tic_in_queue;     // the number of timer tic callbacks in the queue
    uint largest_number_of_concurrent_timer_tic_overruns;
                                        // the max number of timer tics callbacks being queued at any time
    uint total_times_tick_tic_callback_overran;
                                        // the total number of times the timer tic callback overran
} diagnostics_t;

extern diagnostics_t diagnostics;
extern uchar         leadAp;   // lead appl. core has special functions
// ------------------------------------------------------------------------


#endif /* __SPIN1_API_H__ */
