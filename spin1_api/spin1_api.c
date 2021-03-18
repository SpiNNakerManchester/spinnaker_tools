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

//! \dir
//! \brief SpiNNaker Higher-Level Runtime Foundation
//! \file
//! \brief The SpiNNaker API runtime environment
#include <sark.h>

#include <spin1_api.h>
#include <spin1_api_params.h>
#include <scamp_spin1_sync.h>


// ---------------------
/* simulation control */
// ---------------------

uchar leadAp;                       //!< lead appl. core has special functions

static volatile uint run;           //!< controls simulation start/exit
static volatile uint paused;        //!< indicates when paused
static volatile uint resume_sync;   //!< controls re-synchronisation
uint ticks;                         //!< number of elapsed timer periods
uint timer_tick;                    //!< timer tick period
uint timer_tick_clocks;             //!< timer tick period in clock cycles
int drift;                          //!< drift in clock cycles per timer tick
int drift_sign;                     //!< sign of the drift
int drift_accum;                    //!< accumulation of drifts
int time_to_next_drift_update;      //!< timer ticks until drift needs to be updated
static uint timer_phase;            //!< additional phase on starting the timer

//! Default FIQ handler. Restored after simulation
static isr_t old_vector;
//! Default FIQ select. Restored after simulation
static uint old_select;
//! Default interrupt enable. Restored after simulation
static uint old_enable;
//! Interrupt select to be used based on user selections
static uint user_int_select = 0;
//! All VIC interrupts that are handled by the API
static const uint ALL_HANDLED_INTERRUPTS =
        ((1 << TIMER1_INT) | (1 << SOFTWARE_INT) | (1 << CC_MC_INT) |
         (1 << CC_FR_INT) | (1 << DMA_ERR_INT)  | (1 << DMA_DONE_INT));
//! The highest priority selected
static int highest_priority = -1;

//! \brief Which event is to be handled by the FIQ.
//! \details Used to enforce that only one type of basic interrupt handler
//!     can use the FIQ. Special cased for packet received handlers, where
//!     the with- and without-payload variants are actually using the same
//!     interrupt handler.
//!
//!     We usually expect the FIQ to be used for multicast packets, as they
//!     have the most stringent reception performance requirements in
//!     practice.
static int fiq_event = -1;
//! \brief Sanity check state for multicast packet handlers.
//! \details Used to enforce that both the without- and with-payload versions
//!     of the handlers use the same basic interrupt handler. The `-2` means
//!     "not yet assigned".
static int mc_pkt_prio = -2;
//! \brief Sanity check state for fixed-route packet handlers.
//! \details Used to enforce that both the without- and with-payload versions
//!     of the handlers use the same basic interrupt handler. The `-2` means
//!     "not yet assigned".
static int fr_pkt_prio = -2;


// ---------------
/* dma transfer */
// ---------------
//! The pending DMA transfers.
dma_queue_t dma_queue;
// ---------------


// -----------------
/* communications */
// -----------------
//! The pending SpiNNaker packets to transmit.
tx_packet_queue_t tx_packet_queue;
// -----------------

// --------------
/* user events */
// --------------
//! \brief The pending user event callbacks to call.
//! \details Registered by spin1_trigger_user_event()
user_event_queue_t user_event_queue;
// --------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
//! The queue of scheduled tasks.
static task_queue_t task_queue[NUM_PRIORITIES-1];  // priority <= 0 is non-queueable
//! \brief The registered callbacks for each event type.
//! \warning SARK knows about this variable!
cback_t callback[NUM_EVENTS];


// -----------------------------------------
// Configure API debug info (Send to IO_BUF)
// -----------------------------------------

//! API information will be reported via #IO_BUF
#define IO_API IO_BUF
//! \brief Delay between API I/O messages.
//! \param us: Time (in &mu;s) to delay
//! \details Does nothing
#define io_delay(us)
// Uncomment next two for IO_STD
// #define io_delay(us) sark_delay_us (us)
// static uint my_chip;         // chip address in core_map coordinates

// ----------------
/* debug, warning and diagnostics support */
// ----------------
diagnostics_t diagnostics;

// ------------------------------------------------------------------------
// functions
// ------------------------------------------------------------------------
// -----------------------------
/* interrupt service routines */
// -----------------------------

extern INT_HANDLER cc_rx_error_isr(void);
extern INT_HANDLER cc_rx_ready_isr(void);
extern INT_HANDLER cc_fr_ready_isr(void);
extern INT_HANDLER cc_tx_empty_isr(void);
extern INT_HANDLER dma_done_isr(void);
extern INT_HANDLER dma_error_isr(void);
extern INT_HANDLER timer1_isr(void);
extern INT_HANDLER soft_int_isr(void);
extern INT_HANDLER cc_rx_ready_fiqsr(void);
extern INT_HANDLER cc_fr_ready_fiqsr(void);
extern INT_HANDLER dma_done_fiqsr(void);
extern INT_HANDLER timer1_fiqsr(void);
extern INT_HANDLER soft_int_fiqsr(void);
//! \brief Interrupt handler for messages from SCAMP.
extern INT_HANDLER sark_int_han(void);

// ----------------------------
/* intercore synchronisation */
// ----------------------------

//! \brief Wait for interrupt.
//! \details Puts the CPU to sleep until an interrupt occurs.
extern void spin1_wfi(void);
//! \brief Enable interrupts. Alias for cpu_int_enable()
//! \return the old value of the CPSR
uint spin1_int_enable(void);

// ------------------------------------------------------------------------
// hardware support functions
// ------------------------------------------------------------------------
/*! \brief Configures the communications controller.
*
*  This function configures the communications controller by clearing out
*  any pending packets from the RX buffer and clearing sticky error bits.
*/
static void configure_communications_controller(void)
{
    // initialize transmitter control to send MC packets
    cc[CC_TCR] = 0x00000000;

    //TODO: clear receiver status
    // cc[CC_RSR] = 0x00000000;
}
/*
*******/

/*! \brief Configures the DMA controller.
*
*  This function configures the DMA controller by aborting any previously-
*  queued or currently-executing transfers and clearing any corresponding
*  interrupts then enabling all interrupts sources.
*/
static void configure_dma_controller(void)
{
    dma[DMA_CTRL] = 0x3f; // Abort pending and active transfers
    dma[DMA_CTRL] = 0x0d; // clear possible transfer done and restart

    // TODO: needs updating when error support is completed
    // enable interrupt sources
#if USE_WRITE_BUFFER == TRUE
    dma[DMA_GCTL] = 0x100c01; // enable dma done & write buffer ints.
#else
    dma[DMA_GCTL] = 0x000c00; // enable dma done interrupt
#endif // USE_WRITE_BUFFER == TRUE

#if API_DEBUG == TRUE
    /* initialise dmac counters */
    /* dmac counts transfer bursts */
    dma[DMA_SCTL] = 3; // clear and enable counters
#endif // API_DEBUG == TRUE
}
/*
*******/

/*! \brief Configures Timer 1.
*
*  This function configures timer 1 to raise an interrupt with a period
*  specified by `time'. Firstly, timer 1 is disabled and any pending
*  interrupts are cleared. Then timer 1 load and background load
*  registers are loaded with the core clock frequency (set by the monitor and
*  recorded in system RAM MHz) multiplied by `time' and finally timer 1 is
*  loaded with the configuration below.
*
*  ```
*    [0]   One-shot/wrapping     Wrapping
*    [1]   Timer size            32 bit
*    [3:2] Input clock divide    1
*    [5]   IRQ enable            Enabled
*    [6]   Mode                  Periodic
*    [7]   Timer enable          Disabled
*  ```
*
*  \param[in] time: timer period in microseconds, 0 = timer disabled
*  \param[in] phase: timer offset in microseconds
*/
static void configure_timer1(uint time, uint phase)
{
    // Work out the drift per timer tick
    drift = time * sv->clock_drift;

    // Keep the drift positive as the maths is then easier
    drift_sign = 1;
    if (drift < 0) {
        drift = -drift;
        drift_sign = -1;
    }

    // Each timer tick we add on the integer number of clock cycles accumulated
    // and subtract this from the accumulator.
    drift_accum = drift;
    int drift_int = drift_accum & DRIFT_INT_MASK;
    drift_accum -= drift_int;
    drift_int = (drift_int >> DRIFT_FP_BITS) * drift_sign;
    time_to_next_drift_update = TIME_BETWEEN_SYNC_US;

    // do not enable yet!
    timer_tick_clocks = sv->cpu_clk * time;
    tc[T1_CONTROL] = 0;
    tc[T1_INT_CLR] = 1;
    tc[T1_LOAD] = timer_tick_clocks + (sv->cpu_clk * phase) + drift_int;
    tc[T1_BG_LOAD] = timer_tick_clocks + drift_int;
}
/*
*******/

#ifndef VIC_ENABLE_VECTOR
//! Enable flag for VIC vector control (see #VIC_CNTL)
#define VIC_ENABLE_VECTOR (0x20)
#endif //VIC_ENABLE_VECTOR

/*! \brief Configures the VIC.
*
*  This function configures the Vectored Interrupt Controller. Firstly, all
*  interrupts are disabled and then the addresses of the interrupt service
*  routines are placed in the VIC vector address registers and the
*  corresponding control registers are configured. Priority is set in such a
*  manner that the first interrupt source configured by the function is the
*  first priority, the second interrupt configured is the second priority and
*  so on. This mechanism allows future programmers to reorder the IRQ
*  priorities by simply switching around the order in which they are set (a
*  simple copy+paste operation). Finally, the interrupt sources are enabled.
*
*  \param[in] enable_timer: True if the timer interrupt should be enabled.
*/
static void configure_vic(uint enable_timer)
{
    uint fiq_select = 0;

    // disable the relevant interrupts while configuring the VIC
    vic[VIC_DISABLE] = user_int_select;

    // remember default fiq handler

    old_vector = sark_vec->fiq_vec;
    old_select = vic[VIC_SELECT];
    old_enable = vic[VIC_ENABLE];

    // configure irq based on user request

    // configure fiq -- if requested by user
    switch (fiq_event) {
    case MC_PACKET_RECEIVED:
    case MCPL_PACKET_RECEIVED:
        sark_vec->fiq_vec = cc_rx_ready_fiqsr;
        fiq_select = (1 << CC_MC_INT);
        break;
    case FR_PACKET_RECEIVED:
    case FRPL_PACKET_RECEIVED:
        sark_vec->fiq_vec = cc_fr_ready_fiqsr;
        fiq_select = (1 << CC_FR_INT);
        break;
    case DMA_TRANSFER_DONE:
        sark_vec->fiq_vec = dma_done_fiqsr;
        fiq_select = (1 << DMA_DONE_INT);
        break;
    case TIMER_TICK:
        sark_vec->fiq_vec = timer1_fiqsr;
        fiq_select = (1 << TIMER1_INT);
        break;
    case USER_EVENT:
        sark_vec->fiq_vec = soft_int_fiqsr;
        fiq_select = (1 << SOFTWARE_INT);
        break;
    }

    // Move the SARK interrupt to chosen slot

    vic_controls[sark_vec->sark_slot] = 0;        // Disable previous slot

    vic_vectors[SARK_PRIORITY]  = sark_int_han;
    vic_controls[SARK_PRIORITY] = VIC_ENABLE_VECTOR | CPU_INT;

    // Configure API callback interrupts
    vic_vectors[RX_READY_PRIORITY] = cc_rx_ready_isr;
    vic_controls[RX_READY_PRIORITY] = VIC_ENABLE_VECTOR | CC_MC_INT;

    vic_vectors[FR_READY_PRIORITY] = cc_fr_ready_isr;
    vic_controls[FR_READY_PRIORITY] = VIC_ENABLE_VECTOR | CC_FR_INT;

    vic_vectors[DMA_DONE_PRIORITY]  = dma_done_isr;
    vic_controls[DMA_DONE_PRIORITY] = VIC_ENABLE_VECTOR | DMA_DONE_INT;

    vic_vectors[TIMER1_PRIORITY]  = timer1_isr;
    vic_controls[TIMER1_PRIORITY] = VIC_ENABLE_VECTOR | TIMER1_INT;

    // configure the TX empty interrupt but don't enable it yet!
    vic_vectors[CC_TMT_PRIORITY] = cc_tx_empty_isr;
    vic_controls[CC_TMT_PRIORITY] = VIC_ENABLE_VECTOR | CC_TMT_INT;

    // configure the software interrupt
    vic_vectors[SOFT_INT_PRIORITY]  = soft_int_isr;
    vic_controls[SOFT_INT_PRIORITY] = VIC_ENABLE_VECTOR | SOFTWARE_INT;

#if USE_WRITE_BUFFER == TRUE
    /* configure the DMA error interrupt */
    vic_vectors[DMA_ERR_PRIORITY]  = dma_error_isr;
    vic_controls[DMA_ERR_PRIORITY] = VIC_ENABLE_VECTOR | DMA_ERR_INT;
#endif // USE_WRITE_BUFFER == TRUE

    vic[VIC_SELECT] = fiq_select;

    uint int_select = user_int_select;
    if (!enable_timer) {
        int_select = int_select & ~(1 << TIMER1_INT);
    }

#if USE_WRITE_BUFFER == TRUE
    // Enable the DMA error interrupt
    vic[VIC_ENABLE] = int_select | (1 << DMA_ERR_INT);
#else
    vic[VIC_ENABLE] = int_select;
#endif // USE_WRITE_BUFFER == TRUE
}
/*
*******/


void spin1_pause(void)
{
    vic[VIC_DISABLE] = (1 << TIMER1_INT);
    configure_timer1(timer_tick, timer_phase);
    sark_cpu_state(CPU_STATE_PAUSE);
    paused = 1;
}

//! Resumes simulating by enabling the main timer.
static void resume(void)
{
    if (resume_sync == 1) {
        resume_sync = 0;
        event.wait ^= 1;
    }
    paused = 0;
    sark_cpu_state(CPU_STATE_RUN);
    if (callback[TIMER_TICK].cback != NULL) {
        vic[VIC_ENABLE] = (1 << TIMER1_INT);
    }
    tc[T1_CONTROL] = 0xe2;
}


void spin1_resume(sync_bool sync)
{
    if (sync == SYNC_NOWAIT) {
        resume();
    } else {
        resume_sync = 1;
        if (event.wait) {
            sark_cpu_state(CPU_STATE_SYNC1);
        } else {
            sark_cpu_state(CPU_STATE_SYNC0);
        }
    }
}

//! \brief Determines if we are waiting for SYNC flip
//! \return true if we are waiting, false if not
//! \internal Referred to by FEC/simulation.c; must not be static
uint resume_wait(void)
{
    uint bit = 1 << sark.virt_cpu;

    if (event.wait) {
        return (~sc[SC_FLAG] & bit);    // Wait 1
    }
    return (sc[SC_FLAG] & bit);         // Wait 0
}


// ------------------------------------------------------------------------
// scheduler/dispatcher functions
// ------------------------------------------------------------------------
/*! \brief Main simulation event dispatch loop.
*
*  This function executes callbacks which are scheduled in response to events.
*  Callbacks are completed firstly in order of priority and secondly in the
*  order in which they were enqueued.
*
*  The dispatcher is the sole consumer of the scheduler queues and so can
*  safely run with interrupts enabled. Note that deschedule()
*  modifies the scheduler queues which naturally influences the callbacks
*  that are dispatched by this function but not in such a way as to allow the
*  dispatcher to move the processor into an invalid state such as calling a
*  NULL function.
*
*  Upon emptying the scheduling queues the dispatcher goes into wait for
*  interrupt mode.
*
*  Potential hazard: It is possible that an event will occur _and result in
*  a callback being scheduled_ AFTER the last check on the scheduler queues
*  and BEFORE the wait for interrupt call. In this case, the scheduled
*  callback would not be handled until the next event occurs and causes the
*  wait for interrupt call to return.
*
*  This hazard is avoided by calling wait for interrupt with interrupts
*  disabled! Any interrupt will still wake up the core and then
*  interrupts are enabled, allowing the core to respond to it.
*/
static void dispatch(void)
{
    uint i;
    uint cpsr;
    task_queue_t *tq;
    volatile callback_t cback;
    resume_sync = 0;

    // dispatch callbacks from queues until
    // spin1_exit () is called (run = 0)
    while (run) {
        i = 0;

        // disable interrupts to avoid concurrent
        // scheduler/dispatcher accesses to queues
        cpsr = spin1_int_disable();

        while (run && i <= highest_priority) {
            tq = &task_queue[i];

            i++;  // prepare for next priority queue

            if (tq->start != tq->end) {
                cback = tq->queue[tq->start].cback;
                uint arg0 = tq->queue[tq->start].arg0;
                uint arg1 = tq->queue[tq->start].arg1;

                tq->start = (tq->start + 1) % TASK_QUEUE_SIZE;

                if (cback != NULL) {
                    // run callback with interrupts enabled
                    spin1_mode_restore(cpsr);

                    // check for if its a timer callback, if it is, update
                    // tracker values
                    if (cback == callback[TIMER_TICK].cback) {
                        diagnostics.in_timer_callback = 1;
                    }

                    // execute callback
                    if (cback != callback[TIMER_TICK].cback || !paused) {
                        cback(arg0, arg1);
                    }

                    // update queue size
                    if (cback == callback[TIMER_TICK].cback) {
                        if (diagnostics.number_timer_tic_in_queue > 0) {
                            diagnostics.number_timer_tic_in_queue -= 1;
                        }
                        diagnostics.in_timer_callback = 0;
                    }

                    cpsr = spin1_int_disable();

                    // re-start examining queues at highest priority
                    i = 0;
                }
            }
        }

        if (run) {
            // go to sleep with interrupts disabled to avoid hazard!
            // an interrupt will still wake up the dispatcher
            spin1_wfi();

            // Handle resume
            if (resume_sync == 1) {
                if (!resume_wait()) {
                    resume();
                }
            }

            spin1_mode_restore(cpsr);
        }
    }
}
/*
*******/


// ------------------------------------------------------------------------
// simulation control and event management functions
// ------------------------------------------------------------------------
void spin1_callback_on(uint event_id, callback_t cback, int priority)
{
    // set up the callback
    callback[event_id].cback = cback;
    callback[event_id].priority = priority;

    // Enforce a single callback on FIQ

    if (priority < 0) {
        if (fiq_event == -1 ||
                (event_id == MC_PACKET_RECEIVED && fiq_event == MCPL_PACKET_RECEIVED) ||
                (event_id == MCPL_PACKET_RECEIVED && fiq_event == MC_PACKET_RECEIVED) ||
                (event_id == FR_PACKET_RECEIVED && fiq_event == FRPL_PACKET_RECEIVED) ||
                (event_id == FRPL_PACKET_RECEIVED && fiq_event == FR_PACKET_RECEIVED)) {
            fiq_event = event_id;
        } else {
            rt_error(RTE_API);
        }
    }

    // Enforce same interrupt handler for both packet callbacks
    if (event_id == MC_PACKET_RECEIVED || event_id == MCPL_PACKET_RECEIVED) {
        if (mc_pkt_prio == -2) {
            mc_pkt_prio = priority;
        } else if (mc_pkt_prio == -1 && priority != -1) {
            rt_error(RTE_API);
        }
    } else if (event_id == FR_PACKET_RECEIVED || event_id == FRPL_PACKET_RECEIVED) {
        if (fr_pkt_prio == -2) {
            fr_pkt_prio = priority;
        } else if (fr_pkt_prio == -1 && priority != -1) {
            rt_error(RTE_API);
        }
    }

    // Update which events we are to handle
    user_int_select |= VIC_EVENTS[event_id];
    if (run) {
        vic[VIC_ENABLE] = VIC_EVENTS[event_id];
    }
    if (priority > highest_priority) {
        highest_priority = priority;
    }
}

void spin1_callback_off(uint event_id)
{
    callback[event_id].cback = NULL;

    if (callback[event_id].priority < 0) {
        fiq_event = -1;
    }

    // Updates which events we are to handle; note MC and FR have two which
    // use same VIC handler, so only remove if needed
    uint update_vic = 0;
    if (event_id == MC_PACKET_RECEIVED || event_id == MCPL_PACKET_RECEIVED) {
        if (callback[MC_PACKET_RECEIVED].cback == NULL
                && callback[MCPL_PACKET_RECEIVED].cback == NULL) {
            user_int_select &= ~VIC_EVENTS[event_id];
            update_vic = 1;
        }
    } else if (event_id == FR_PACKET_RECEIVED || event_id == FRPL_PACKET_RECEIVED) {
        if (callback[FR_PACKET_RECEIVED].cback == NULL
                && callback[FRPL_PACKET_RECEIVED].cback == NULL) {
            user_int_select &= ~VIC_EVENTS[event_id];
            update_vic = 1;
        }
    } else {
        user_int_select &= ~VIC_EVENTS[event_id];
        update_vic = 1;
    }
    if (run && update_vic) {
        vic[VIC_DISABLE] = VIC_EVENTS[event_id];
    }
}

/*! \brief Deschedules all callbacks for an event.
*
*  This function deschedules all callbacks corresponding to the given event
*  ID. One use for this function is to effectively discard all received
*  packets which are yet to be processed by calling
*  `deschedule(MC_PACKET_RECEIVED)`.
*
*  \note this function cannot guarantee
*  that all callbacks pertaining to the given event ID will be descheduled:
*  once a callback has been prepared for execution by the dispatcher it is
*  immune to descheduling and will be executed upon return to the dispatcher.
*
*  \param[in] event_id: event ID of the callbacks to be descheduled
*/
static void deschedule(uint event_id)
{
    uint cpsr = spin1_int_disable();

    task_queue_t *tq = &task_queue[callback[event_id].priority-1];

    for (uint i = 0; i < TASK_QUEUE_SIZE; i++) {
        if (tq->queue[i].cback == callback[event_id].cback) {
            tq->queue[i].cback = NULL;
        }
    }

    spin1_mode_restore(cpsr);
}
/*
*******/


uint spin1_get_simulation_time(void)
{
    return ticks;
}
/*
*******/


/****f* spin1_api.c/spin1_exit
*
* SUMMARY
*  This function terminates a simulation passing a return code.
*
* SYNOPSIS
*  void spin1_exit (uint error)
*
* INPUTS
*  uint error: error exit code
*
* SOURCE
*/
void spin1_exit(uint error)
{
    // Disable API-enabled interrupts to allow simulation to exit,

    vic[VIC_DISABLE] = ALL_HANDLED_INTERRUPTS;

    // Report back the return code and exit the simulation

    run = 0;
    diagnostics.exit_code = error;
}
/*
*******/


/****f* spin1_api.c/spin1_set_timer_tick_and_phase
*
* SUMMARY
*  This function sets the period and phase of the timer tick
*
* SYNOPSIS
*  void set_timer_tick_and_phase(uint time)
*
* INPUTS
*  uint time: timer tick period (in microseconds)
*  uint phase: timer phase offset (in microseconds)
*
* SOURCE
*/
void spin1_set_timer_tick_and_phase(uint time, uint phase)
{
    timer_tick = time;
    timer_phase = phase;
}
/*
*******/


/*! \brief Post-exit cleanup.
*
*  This function is called after simulation exits to configure
*  hardware for idle operation. It cleans up interrupt lines.
*/
static void clean_up(void)
{
    uint cpsr = spin1_int_disable();

    // Restore old SARK FIQ handler

    sark_vec->fiq_vec = old_vector;
    vic[VIC_SELECT] = old_select;
    vic[VIC_ENABLE] = old_enable;

    // Restore old SARK IRQ handler
    vic_controls[sark_vec->sark_slot] = 0x20 | CPU_INT;

    // timer1
    tc[T1_INT_CLR] = 1;   // clear possible interrupt

    // dma controller
    dma[DMA_GCTL] = 0;    // disable all IRQ sources
    dma[DMA_CTRL] = 0x3f; // Abort pending and active transfers
    dma[DMA_CTRL] = 0x0d; // clear possible transfer done and restart

    spin1_mode_restore(cpsr);
}

/*! \brief Report debugging diagnostic information.
*
*  This function reports provenance if requested at compile time.
*/
static void report_debug(void)
{
#if API_DEBUG == TRUE
    if (leadAp) {       // Only the leader appl. core reports router data
        // Report router counters

        io_printf(IO_API, "\t\t[api_debug] RTR mc     packets: %d\n",
                rtr[RTR_DGC0] + rtr[RTR_DGC1]);
        io_delay(API_PRINT_DELAY);

        io_printf(IO_API, "\t\t[api_debug] RTR dpd mc packets: %d\n",
                rtr[RTR_DGC8]);
        io_delay(API_PRINT_DELAY);
    }

    io_printf(IO_API, "\t\t[api_debug] ISR thrown packets: %d\n",
            diagnostics.discarded_mc_packets);
    io_delay(API_PRINT_DELAY);

    io_printf(IO_API, "\t\t[api_debug] ISR thrown FR packets: %d\n",
            diagnostics.discarded_fr_packets);
    io_delay(API_PRINT_DELAY);

    // Report DMAC counters

    io_printf(IO_API, "\t\t[api_debug] DMA bursts:  %d\n", dma[DMA_STAT0]);
    io_delay(API_PRINT_DELAY);
#endif // API_DEBUG == TRUE
}

/*! \brief Report debugging warnings.
*
*  This function reports warnings if requested at compile time
*/
static void report_warns(void)
{
#if API_WARN == TRUE        // report warnings
    if (diagnostics.warnings & TASK_QUEUE_FULL) {
        io_printf(IO_API, "\t\t[api_warn] warning: task queue full (%u)\n",
                diagnostics.task_queue_full);
        io_delay(API_PRINT_DELAY);
    }
    if (diagnostics.warnings & DMA_QUEUE_FULL) {
        io_printf(IO_API, "\t\t[api_warn] warning: DMA queue full (%u)\n",
                diagnostics.dma_queue_full);
        io_delay(API_PRINT_DELAY);
    }
    if (diagnostics.warnings & PACKET_QUEUE_FULL) {
        io_printf(IO_API, "\t\t[api_warn] warning: packet queue full (%u)\n",
                diagnostics.tx_packet_queue_full);
        io_delay(API_PRINT_DELAY);
    }
#if USE_WRITE_BUFFER == TRUE
    if (diagnostics.warnings & WRITE_BUFFER_ERROR) {
        io_printf(IO_API,
                "\t\t[api_warn] warning: write buffer errors (%u)\n",
                diagnostics.writeBack_errors);
        io_delay(API_PRINT_DELAY);
    }
#endif // USE_WRITE_BUFFER == TRUE
#endif // API_WARN == TRUE
}


#ifdef __GNUC__
    register uint _lr asm("lr");
#else
    register uint _lr __asm("lr");
#endif // __GNUC__

void spin1_rte(rte_code code)
{
    // Don't actually shutdown, just set the CPU into an RTE code and
    // stop the timer
    clean_up();
    sark_cpu_state(CPU_STATE_RTE);
    sv_vcpu->lr = _lr;        // Report the link register (calling address)
    sv_vcpu->rt_code = code;
    sv->led_period = 8;
}

//! \brief Begins a simulation by enabling the timer (if called for) and
//!     beginning the dispatcher loop.
//!     Only returns once told to exit (with spin1_exit()).
//! \param[in] sync: Whether to synchronise with other cores
//! \param[in] start_paused: Whether to start in a paused state
//! \return the exit code
static uint start(sync_bool sync, uint start_paused)
{
    paused = start_paused;
    if (paused) {
        sark_cpu_state(CPU_STATE_PAUSE);
    } else {
        sark_cpu_state(CPU_STATE_RUN);
    }

    // Initialise hardware

    configure_communications_controller();
    configure_dma_controller();
    configure_timer1 (timer_tick, timer_phase);
    configure_vic(!paused);

#if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
    diagnostics.warnings = NO_ERROR;
    diagnostics.dma_queue_full = 0;
    diagnostics.task_queue_full = 0;
    diagnostics.tx_packet_queue_full = 0;
    diagnostics.dma_transfers = 1;
    diagnostics.exit_code = NO_ERROR;     // simulation return value
    diagnostics.in_timer_callback = 0;
    diagnostics.number_timer_tic_in_queue = 0;
    diagnostics.total_times_tick_tic_callback_overran = 0;
    diagnostics.largest_number_of_concurrent_timer_tic_overruns = 0;
#if USE_WRITE_BUFFER == TRUE
    diagnostics.writeBack_errors = 0;
#endif
#endif // (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)

    // synchronise with other application cores

    if (sync == SYNC_WAIT) {
        event_wait();
    }

    // initialise counter and ticks for simulation
    // 32-bit, periodic counter, interrupts enabled

    if (timer_tick && !paused) {
        tc[T1_CONTROL] = 0xe2;
    }

    ticks = 0;
    run = 1;

    // simulate!
    dispatch();

    // simulation finished - clean up before returning to c_main
    clean_up();

    // re-enable interrupts for sark
    // only CPU_INT enabled in the VIC
    spin1_int_enable();

    // provide diagnostics data to application
#if (API_DIAGNOSTICS == TRUE)
    diagnostics.total_mc_packets     = rtr[RTR_DGC0] + rtr[RTR_DGC1];
    diagnostics.dumped_mc_packets    = rtr[RTR_DGC8];
    diagnostics.dma_bursts           = dma[DMA_STAT0];
    diagnostics.total_fr_packets     = rtr[RTR_DGC6] + rtr[RTR_DGC7];
    diagnostics.dumped_fr_packets    = rtr[RTR_DGC11];
#endif // API_DIAGNOSTICS == TRUE

    // report problems if requested!
#if (API_DEBUG == TRUE) || (API_WARN == TRUE)
    // avoid sending output at the same time as other chips!
    io_delay(10000 * my_chip);

#if API_DEBUG == TRUE   // report debug information
    report_debug();
#endif

#if API_WARN == TRUE            // report warnings
    report_warns();
#endif // API_WARN
#endif // API_DEBUG || API_WARN

    return diagnostics.exit_code;
}

uint spin1_start(sync_bool sync)
{
    return start(sync, 0);
}

uint spin1_start_paused(void)
{
    return start(SYNC_NOWAIT, 1);
}
/*
*******/


// ------------------------------------------------------------------------
// data transfer functions
// ------------------------------------------------------------------------
/****f* spin1_api.c/spin1_dma_transfer
*
* SUMMARY
*  This function enqueues a DMA transfer request. Requests are consumed by
*  dma_done_isr, which schedules a user callback with the ID of the completed
*  transfer and fulfils the next transfer request. If the DMA controller
*  hardware buffer is not full (which also implies that the request queue is
*  empty, given the consumer operation) then a transfer request is fulfilled
*  immediately.
*
* SYNOPSIS
*  uint spin1_dma_transfer(uint tag, void *system_address, void *tcm_address,
*                          uint direction, uint length)
*
* INPUTS
*  uint *system_address: system NOC address of the transfer
*  uint *tcm_address: processor TCM address of the transfer
*  uint direction: 0 = transfer to TCM, 1 = transfer to system
*  uint length: length of transfer in bytes
*
* OUTPUTS
*   uint: 0 if the request queue is full, DMA transfer ID otherwise
*
* SOURCE
*/
uint spin1_dma_transfer(uint tag, void *system_address, void *tcm_address,
        uint direction, uint length)
{
    uint id = 0;
    uint cpsr = spin1_int_disable();

    uint new_end = (dma_queue.end + 1) % DMA_QUEUE_SIZE;

    if (new_end != dma_queue.start) {
        id = diagnostics.dma_transfers++;

        uint desc = DMA_WIDTH << 24 | DMA_BURST_SIZE << 21
                | direction << 19 | length;

        dma_queue.queue[dma_queue.end].id = id;
        dma_queue.queue[dma_queue.end].tag = tag;
        dma_queue.queue[dma_queue.end].system_address = system_address;
        dma_queue.queue[dma_queue.end].tcm_address = tcm_address;
        dma_queue.queue[dma_queue.end].description = desc;

        /* if dmac is available and dma_queue empty trigger transfer now */
        if (!(dma[DMA_STAT] & 4) && (dma_queue.start == dma_queue.end)) {
            dma[DMA_ADRS] = (uint) system_address;
            dma[DMA_ADRT] = (uint) tcm_address;
            dma[DMA_DESC] = desc;
        }

        dma_queue.end = new_end;
#if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
    } else {
        diagnostics.warnings |= DMA_QUEUE_FULL;
        diagnostics.dma_queue_full++;
#endif // (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
    }

    spin1_mode_restore(cpsr);
    return id;
}
/*
*******/

/****f* spin1_api.c/spin1_dma_flush
*
* SUMMARY
*  This function:
*    flushes the hardware queue in the DMA controller,
*    aborts any ongoing transfer in the DMA controller,
*    clears any pending DMA_COMPLETE interrupts in the DMA controller and
*    purges any queued DMA_COMPLETE callbacks in the callback queues.
*    flushes the software DMA queue,
*
* SYNOPSIS
*  void spin1_dma_flush(void);
*
 SOURCE
*/
void spin1_dma_flush(void)
{
    // flush the hardware queue in the DMA controller,
    // abort any ongoing transfer in the DMA controller,
    // and clear any pending DMA_COMPLETE interrupts in the DMA controller
    dma[DMA_CTRL] = 0x1f;
    dma[DMA_CTRL] = 0x0d;

    // purge any queued DMA_COMPLETE callbacks in the callback queues
    if (callback[DMA_TRANSFER_DONE].priority > 0) {
        task_queue_t *tq = &task_queue[callback[DMA_TRANSFER_DONE].priority-1];

        // check only if the queue is not empty
        uint cpsr = spin1_int_disable();
        if (tq->end != tq->start) {
            callback_t cb = callback[DMA_TRANSFER_DONE].cback;
            uint mtp = tq->start;
            uint end = tq->end;

            // find first queued DMA_COMPLETE callback
            while ((mtp != end) && (tq->queue[mtp].cback != cb)) {
                mtp = (mtp + 1) % TASK_QUEUE_SIZE;
            }

            // if found, remove it and move up the rest of the queue
            if (mtp != end) {
                uint mfp = (mtp + 1) % TASK_QUEUE_SIZE;
                while (mfp != end) {
                    // jump over other queued DMA_COMPLETE callbacks
                    if (tq->queue[mfp].cback != cb) {
                        tq->queue[mtp].cback = tq->queue[mfp].cback;
                        tq->queue[mtp].arg0  = tq->queue[mfp].arg0;
                        tq->queue[mtp].arg1  = tq->queue[mfp].arg1;
                        mtp = (mtp + 1) % TASK_QUEUE_SIZE;
                    }
                    mfp = (mfp + 1) % TASK_QUEUE_SIZE;
                }

                // update queue end pointer
                tq->end = mtp;
            }
        }
        spin1_mode_restore(cpsr);
    }

    // and flush the software DMA transfer queue
    uint cpsr = spin1_int_disable();
    dma_queue.start = 0;
    dma_queue.end   = 0;
    spin1_mode_restore(cpsr);
}
/*
*******/


/****f* spin1_api.c/spin1_memcpy
*
* SUMMARY
*  This function copies a block of memory
*
* SYNOPSIS
*  void spin1_memcpy(void *dst, void const *src, uint len)
*
* INPUTS
*  void *dst: destination of the transfer
*  void const *src: source of the transfer
*  uint len: number of bytes to transfer
*
* SOURCE
*/
void spin1_memcpy(void *dst, void const *src, uint len)
{
    char *pDst = (char *) dst;
    char const *pSrc = (char const *) src;

    while (len--) {
        *pDst++ = *pSrc++;
    }
}
/*
*******/


// ------------------------------------------------------------------------
// communications functions
// ------------------------------------------------------------------------
/****f* spin1_api.c/spin1_flush_rx_packet_queue
*
* SUMMARY
*  This function effectively discards all received packets which are yet
*  to be processed by calling deschedule(MC_PACKET_RECEIVED).
*
* SYNOPSIS
*  void spin1_flush_rx_packet_queue()
*
* SOURCE
*/
//! \details
//!     Works by calling deschedule() for each queue.
void spin1_flush_rx_packet_queue(void)
{
    deschedule(MC_PACKET_RECEIVED);
    deschedule(MCPL_PACKET_RECEIVED);
    deschedule(FR_PACKET_RECEIVED);
    deschedule(FRPL_PACKET_RECEIVED);
}
/*
*******/


/****f* spin1_api.c/spin1_flush_tx_packet_queue
*
* SUMMARY
*  This function flushes the outbound packet queue by adjusting the
*  queue pointers to make it appear empty to the consumer.
*
* SYNOPSIS
*  void spin1_flush_tx_packet_queue()
*
* SOURCE
*/
//! \details
//!     Works by adjusting the queue pointers to make it appear empty to the
//!     consumer
void spin1_flush_tx_packet_queue(void)
{
    uint cpsr = spin1_int_disable();

    tx_packet_queue.start = tx_packet_queue.end;

    spin1_mode_restore(cpsr);
}
/*
*******/

uint spin1_send_packet(uint key, uint data, uint TCR)
{
    // TODO: This need to be re-written for SpiNNaker using the
    // TX_nof_full flag instead -- much more efficient!

    uint rc = SUCCESS;
    uint cpsr = spin1_int_disable();

    /* clear sticky TX full bit and check TX state */
    cc[CC_TCR] = TX_TCR_MCDEFAULT;
    (void) cc[CC_TCR];  // needed to avoid a RAW hazard accessing CC_TCR

    if (cc[CC_TCR] & TX_FULL_MASK) {
        if ((tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE
                == tx_packet_queue.start) {
            /* if queue full cannot do anything -- report failure */
            rc = FAILURE;
#if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
            diagnostics.warnings |= PACKET_QUEUE_FULL;
            diagnostics.tx_packet_queue_full++;
#endif
        } else {
            /* if not full queue packet */
            tx_packet_queue.queue[tx_packet_queue.end].key = key;
            tx_packet_queue.queue[tx_packet_queue.end].data = data;
            tx_packet_queue.queue[tx_packet_queue.end].TCR = TCR;

            tx_packet_queue.end = (tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE;

            /* turn on tx_empty interrupt (in case it was off) */
            vic[VIC_ENABLE] = (1 << CC_TMT_INT);
        }
    } else {
        if ((tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE
                == tx_packet_queue.start) {
            /* if queue full, dequeue and send packet at the */
            /* head of the queue to make room for new packet */
            uint hkey  = tx_packet_queue.queue[tx_packet_queue.start].key;
            uint hdata = tx_packet_queue.queue[tx_packet_queue.start].data;
            uint hTCR = tx_packet_queue.queue[tx_packet_queue.start].TCR;

            tx_packet_queue.start = (tx_packet_queue.start + 1) % TX_PACKET_QUEUE_SIZE;

            cc[CC_TCR] = hTCR;

            if (hTCR & PKT_PL) {
                cc[CC_TXDATA] = hdata;
            }
            cc[CC_TXKEY]  = hkey;
        }

        if (tx_packet_queue.start == tx_packet_queue.end) {
            // If queue empty send packet
            cc[CC_TCR] = TCR;

            if (TCR & PKT_PL) {
                cc[CC_TXDATA] = data;
            }
            cc[CC_TXKEY]  = key;

            // turn off tx_empty interrupt (in case it was on)
            vic[VIC_DISABLE] = 1 << CC_TMT_INT;
        } else {
            /* if not empty queue packet */
            tx_packet_queue.queue[tx_packet_queue.end].key = key;
            tx_packet_queue.queue[tx_packet_queue.end].data = data;
            tx_packet_queue.queue[tx_packet_queue.end].TCR = TCR;

            tx_packet_queue.end = (tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE;
        }
    }

    spin1_mode_restore(cpsr);

    return rc;
}

/*
*******/


/****f* spin1_api.c/spin1_get_id
*
* SUMMARY
*  This function returns a global ID for the processor.
*
* SYNOPSIS
*  uint spin1_get_id()
*
* OUTPUTS
*  Chip ID in bits [20:5], core ID in bits [4:0].
*
* SOURCE
*/
uint spin1_get_id(void)
{
    return (uint) ((sv->p2p_addr << 5) | sark.virt_cpu);
}
/*
*******/

/****f* spin1_api.c/spin1_get_core_id
*
* SUMMARY
*  This function returns the core ID
*
* SYNOPSIS
*  uint spin1_get_core_id()
*
* OUTPUTS
*  core ID in the bottom 5 bits.
*
* SOURCE
*/
uint spin1_get_core_id(void)
{
    return sark.virt_cpu;
}
/*
*******/

/****f* spin1_api.c/spin1_get_chip_id
*
* SUMMARY
*  This function returns the chip ID
*
* SYNOPSIS
*  uint spin1_get_chip_id()
*
* OUTPUTS
*  chip ID in the bottom 16 bits.
*
* SOURCE
*/
uint spin1_get_chip_id(void)
{
    return (uint) sv->p2p_addr;
}
/*
*******/

#if 0
//## This routine has been removed - use "rtr_alloc", "rtr_mc_set" instead

/****f* spin1_api.c/spin1_set_mc_table_entry
*
* SUMMARY
*  This function sets up an entry in the multicast routing table
*
* SYNOPSIS
*  void spin1_set_mc_table_entry(uint entry, uint key, uint mask,
*                                uint route)
*
* INPUTS
*  uint entry: table entry
*  uint key: entry routing key field
*  uint mask: entry mask field
*  uint route: entry route field
*
* SOURCE
*/

uint spin1_set_mc_table_entry(uint entry, uint key, uint mask, uint route)
{
    if (entry >= APP_MC_ENTRIES) { // top priority entries reserved for the system
        return FAILURE;
    }

    entry += SYS_MC_ENTRIES;
    rtr_mc_set(entry, key, mask, route);

#if API_DEBUG == TRUE
    io_printf(IO_API,
            "\t\t[api_debug] MC entry %d: k 0x%8z m 0x%8z r 0x%8z\n",
            entry, key, mask, route);
    io_delay(API_PRINT_DLY);
#endif

    return SUCCESS;
}
#endif

/*
*******/


// ------------------------------------------------------------------------
// memory allocation functions
// ------------------------------------------------------------------------

/*
*******/


/*! \brief Schedule a callback (called with interrupts disabled)
*
*  This function places a callback into the scheduling queue corresponding
*  to its priority, which is set at configuration time by on_callback(...).
*
*  Non-queueable callbacks (those of priority 0) are executed immediately
*  and atomically.
*
*  Interrupts are not explicitly disabled during this routine: it is assumed
*  that the function is only called by interrupt service routines responding
*  to events, and these ISRs execute with interrupts disabled.
*
* \note Also called from sark_base.c and sark_alib.s via weak linking.
*
* \param[in] event_id: ID of the event triggering a callback
* \param[in] arg0: argument to be passed to the callback
* \param[in] arg1: argument to be passed to the callback
*/
void schedule_sysmode(uchar event_id, uint arg0, uint arg1)
{
    if (callback[event_id].priority <= 0) {
        callback[event_id].cback(arg0, arg1);
    } else {
        task_queue_t *tq = &task_queue[callback[event_id].priority-1];

        if ((tq->end + 1) % TASK_QUEUE_SIZE != tq->start) {
            tq->queue[tq->end].cback = callback[event_id].cback;
            tq->queue[tq->end].arg0 = arg0;
            tq->queue[tq->end].arg1 = arg1;

            tq->end = (tq->end + 1) % TASK_QUEUE_SIZE;

            if (event_id == TIMER_TICK) {
                diagnostics.number_timer_tic_in_queue += 1;
            }
        } else {                // queue is full
#if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
            diagnostics.warnings |= TASK_QUEUE_FULL;
            diagnostics.task_queue_full++;
#endif
        }
    }
}
/*
*******/


/****f* spin1_api.c/spin1_schedule_callback
*
* SUMMARY
*  This function places a cback into the scheduling queue corresponding
*  to its priority
*
* SYNOPSIS
*  uint spin1_schedule_callback(callback_t cback, uint arg0, uint arg1,
                                uint priority)
*
* INPUTS
*  callback_t cback: callback to be scheduled
*  uint arg0: argument to be passed to the callback
*  uint arg1: argument to be passed to the callback
*  uint priority: cback priority
*
* SOURCE
*/
uint spin1_schedule_callback(callback_t cback, uint arg0, uint arg1,
        uint priority)
{
    uchar result = SUCCESS;

    /* disable interrupts for atomic access to task queues */
    uint cpsr = spin1_int_disable();

    task_queue_t *tq = &task_queue[priority-1];
    if (priority > highest_priority) {
        highest_priority = priority;
    }

    if ((tq->end + 1) % TASK_QUEUE_SIZE != tq->start) {
        tq->queue[tq->end].cback = cback;
        tq->queue[tq->end].arg0 = arg0;
        tq->queue[tq->end].arg1 = arg1;

        tq->end = (tq->end + 1) % TASK_QUEUE_SIZE;
    } else {
        // queue is full
        result = FAILURE;
#if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
        diagnostics.warnings |= TASK_QUEUE_FULL;
        diagnostics.task_queue_full++;
#endif
    }

    /* restore interrupt status */
    spin1_mode_restore(cpsr);

    return result;
}
/*
*******/

void spin1_enable_timer_schedule_proc(void) {
    event_register_timer(TIMER2_PRIORITY);
}


/****f* spin1_api.c/spin1_trigger_user_event
*
* SUMMARY
*  This function triggers a USER EVENT i.e. a software interrupt.
*  If a previous trigger event is still pending or executing, the event will
*  be queued until that callback completes.
*  The function returns FAILURE if the queue of user events to be called is
*  already full.
*
*  NOTE: The callback handler should be able to cope with the fact that a
*  second call to the function may result in no work to do should a callback
*  be queued whilst the first is still running and the first deals with
*  "pending tasks".
*
* SYNOPSIS
*  __irq void spin1_trigger_user_event(uint arg0, uint arg1)
*
* INPUTS
*  uint arg0: argument to be passed to the callback
*  uint arg1: argument to be passed to the callback
*
* OUTPUTS
*   uint: 0 = FAILURE, 1 = SUCCESS
*
* SOURCE
*/
uint spin1_trigger_user_event(uint arg0, uint arg1)
{
    uint cpsr = spin1_int_disable();

    uint new_end = (user_event_queue.end + 1) % USER_EVENT_QUEUE_SIZE;

    if (new_end == user_event_queue.start) {
#if (API_DIAGNOSTICS == TRUE)
        diagnostics.user_event_queue_full++;
#endif // (API_DIAGNOSTICS == TRUE)
        spin1_mode_restore(cpsr);
        return (FAILURE);
    }

    user_event_queue.queue[user_event_queue.end].arg0 = arg0;
    user_event_queue.queue[user_event_queue.end].arg1 = arg1;

    // Trigger a user event (doesn't matter if already set)
    vic[VIC_SOFT_SET] = (1 << SOFTWARE_INT);

    user_event_queue.end = new_end;
    spin1_mode_restore(cpsr);
    return (SUCCESS);
}
/*
*******/


/*! \brief Runtime initialisation; called before the application program starts!
*
*  This function is a stub for the run-time system.
*  It initialises peripherals in the way the RTS
*  is expected to do
*/
void sark_pre_main(void)
{
    sark_call_cpp_constructors();

    sark_cpu_state(CPU_STATE_SARK);
    sark_vec->api = 1;

    leadAp = (sark_app_lead() == sark.virt_cpu);
}

/*! \brief Runtime cleanup, called after the application program finishes!
*
*  This function is a stub for the run-time system.
*  It makes sure that application returns cleanly to the RTS.
*/
void sark_post_main(void)
{
    sark_cpu_state(CPU_STATE_EXIT);
}
/*
*******/
