#include <sark.h>

#include <spin1_api.h>
#include <spin1_api_params.h>


// ---------------------
/* simulation control */
// ---------------------

uchar leadAp;                    	// lead appl. core has special functions

static volatile uint run;           	// controls simulation start/stop
static volatile uint paused;            // indicates when paused
static volatile uint resume_sync;       // controls re-synchronisation
uint ticks;              		// number of elapsed timer periods
static uint timer_tick;  	        // timer tick period

// default fiq handler -- restored after simulation
isr_t old_vector;
uint old_select;
uint old_enable;

int fiq_event = -1;
int mc_pkt_prio = -2;
int fr_pkt_prio = -2;


// ---------------
/* dma transfer */
// ---------------
dma_queue_t dma_queue;
// ---------------


// -----------------
/* communications */
// -----------------
tx_packet_queue_t tx_packet_queue;
// -----------------


// -----------------------
/* scheduler/dispatcher */
// -----------------------
static task_queue_t task_queue[NUM_PRIORITIES-1];  // priority <= 0 is non-queueable
cback_t callback[NUM_EVENTS];
uchar user_pending = FALSE;
uint user_arg0;
uint user_arg1;


// -----------------------------------------
// Configure API debug info (Send to IO_BUF)
// -----------------------------------------

#define IO_API IO_BUF
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

extern INT_HANDLER cc_rx_error_isr (void);
extern INT_HANDLER cc_rx_ready_isr (void);
extern INT_HANDLER cc_fr_ready_isr (void);
extern INT_HANDLER cc_tx_empty_isr (void);
extern INT_HANDLER dma_done_isr (void);
extern INT_HANDLER dma_error_isr (void);
extern INT_HANDLER timer1_isr (void);
extern INT_HANDLER sys_controller_isr (void);
extern INT_HANDLER soft_int_isr (void);
extern INT_HANDLER cc_rx_ready_fiqsr (void);
extern INT_HANDLER cc_fr_ready_fiqsr (void);
extern INT_HANDLER dma_done_fiqsr (void);
extern INT_HANDLER timer1_fiqsr (void);
extern INT_HANDLER soft_int_fiqsr (void);
extern INT_HANDLER sark_int_han (void);

// ----------------------------
/* intercore synchronisation */
// ----------------------------

extern void spin1_wfi (void);
uint spin1_int_enable (void);

// !! ST replaced by SARK routine

void spin1_msg_free (sdp_msg_t *msg)
{
  sark_msg_free (msg);
}

sdp_msg_t* spin1_msg_get (void)
{
  return sark_msg_get ();
}

uint spin1_send_sdp_msg (sdp_msg_t *msg, uint timeout)
{
  return sark_msg_send (msg, timeout);
}

// ------------------------------------------------------------------------
// pseudo-random number generation functions
// ------------------------------------------------------------------------
/****f* spin1_isr.c/spin1_srand
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

// !! ST now calls SARK srand

void spin1_srand (uint seed)
{
  sark_srand (seed);
}
/*
*******/


/****f* spin1_isr.c/spin1_rand
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
/*
*******/

// !! ST now uses equivalent SARK routine

uint spin1_rand  (void)
{
  return sark_rand ();
}

/*
*******/


// ------------------------------------------------------------------------
// hardware support functions
// ------------------------------------------------------------------------
/****f* spin1_api.c/configure_communications_controller
*
* SUMMARY
*  This function configures the communications controller by clearing out
*  any pending packets from the RX buffer and clearing sticky error bits.
*
* SYNOPSIS
*  void configure_communications_controller()
*
* SOURCE
*/
void configure_communications_controller()
{
  // initialize transmitter control to send MC packets
  cc[CC_TCR] = 0x00000000;

  //TODO: clear receiver status
//  cc[CC_RSR] = 0x00000000;
}
/*
*******/



/****f* spin1_api.c/configure_dma_controller
*
* SUMMARY
*  This function configures the DMA controller by aborting any previously-
*  queued or currently-executing transfers and clearing any corresponding
*  interrupts then enabling all interrupts sources.
*
* SYNOPSIS
*  void configure_dma_controller()
*
* SOURCE
*/
void configure_dma_controller()
{
  dma[DMA_CTRL] = 0x3f; // Abort pending and active transfers
  dma[DMA_CTRL] = 0x0d; // clear possible transfer done and restart

  // TODO: needs updating when error support is completed
  // enable interrupt sources
  #if USE_WRITE_BUFFER == TRUE
    dma[DMA_GCTL] = 0x100c01; // enable dma done & write buffer ints.
  #else
    dma[DMA_GCTL] = 0x000c00; // enable dma done interrupt
  #endif

  #if API_DEBUG == TRUE
    /* initialise dmac counters */
    /* dmac counts transfer bursts */
    dma[DMA_SCTL] = 3; // clear and enable counters
  #endif
}
/*
*******/



/****f* spin1_api.c/configure_timer1
*
* SUMMARY
*  This function configures timer 1 to raise an interrupt with a pediod
*  specified by `time'. Firstly, timer 1 is disabled and any pending
*  interrupts are cleared. Then timer 1 load and background load
*  registers are loaded with the core clock frequency (set by the monitor and
*  recorded in system RAM MHz) multiplied by `time' and finally timer 1 is
*  loaded with the configuration below.
*
*    [0]   One-shot/wrapping     Wrapping
*    [1]   Timer size            32 bit
*    [3:2] Input clock divide    1
*    [5]   IRQ enable            Enabled
*    [6]   Mode                  Periodic
*    [7]   Timer enable          Disabled
*
* SYNOPSIS
*  void configure_timer1(uint time)
*
* INPUTS
*  uint time: timer period in microseconds, 0 = timer disabled
*
* SOURCE
*/
void configure_timer1 (uint time)
{
  // do not enable yet!
  tc[T1_CONTROL] = 0;
  tc[T1_INT_CLR] = 1;
  tc[T1_LOAD] = sv->cpu_clk * time;
  tc[T1_BG_LOAD] = sv->cpu_clk * time;
}
/*
*******/



/****f* spin1_api.c/configure_vic
*
* SUMMARY
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
* SYNOPSIS
*  void configure_vic(uint enable_timer)
*
* SOURCE
*/
void configure_vic (uint enable_timer)
{
  uint fiq_select = 0;
  uint int_select = ((1 << TIMER1_INT)   |
                     (1 << SOFTWARE_INT) |
                     (1 << CC_MC_INT) |
                     (1 << CC_FR_INT) |
                     (1 << DMA_ERR_INT)  |
                     (1 << DMA_DONE_INT)
                    );

  // disable the relevant interrupts while configuring the VIC

  vic[VIC_DISABLE] = int_select;

  // remember default fiq handler

  old_vector = sark_vec->fiq_vec;
  old_select = vic[VIC_SELECT];
  old_enable = vic[VIC_ENABLE];

  // configure fiq -- if requested by user
  switch (fiq_event)
  {
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

  vic_controls[sark_vec->sark_slot] = 0;	  // Disable previous slot

  vic_vectors[SARK_PRIORITY]  = sark_int_han;
  vic_controls[SARK_PRIORITY] = 0x20 | CPU_INT;

  // Configure API callback interrupts
  vic_vectors[RX_READY_PRIORITY] = cc_rx_ready_isr;
  vic_controls[RX_READY_PRIORITY] = 0x20 | CC_MC_INT;

  vic_vectors[FR_READY_PRIORITY] = cc_fr_ready_isr;
  vic_controls[FR_READY_PRIORITY] = 0x20 | CC_FR_INT;

  vic_vectors[DMA_DONE_PRIORITY]  = dma_done_isr;
  vic_controls[DMA_DONE_PRIORITY] = 0x20 | DMA_DONE_INT;

  vic_vectors[TIMER1_PRIORITY]  = timer1_isr;
  vic_controls[TIMER1_PRIORITY] = 0x20 | TIMER1_INT;

  // configure the TX empty interrupt but don't enable it yet!
  vic_vectors[CC_TMT_PRIORITY] = cc_tx_empty_isr;
  vic_controls[CC_TMT_PRIORITY] = 0x20 | CC_TMT_INT;

  // configure the software interrupt
  vic_vectors[SOFT_INT_PRIORITY]  = soft_int_isr;
  vic_controls[SOFT_INT_PRIORITY] = 0x20 | SOFTWARE_INT;

#if USE_WRITE_BUFFER == TRUE
    /* configure the DMA error interrupt */
  vic_vectors[DMA_ERR_PRIORITY]  = dma_error_isr;
  vic_controls[DMA_ERR_PRIORITY] = 0x20 | DMA_ERR_INT;
#endif

  vic[VIC_SELECT] = fiq_select;

  if (!enable_timer)
  {
    int_select = int_select & ~(1 << TIMER1_INT);
  }

  #if USE_WRITE_BUFFER == TRUE
    vic[VIC_ENABLE] = int_select;
  #else
    // don't enable the dma error interrupt
    vic[VIC_ENABLE] = int_select & ~(1 << DMA_ERR_INT);
  #endif
}
/*
*******/


void spin1_pause()
{
  vic[VIC_DISABLE] = (1 << TIMER1_INT);
  configure_timer1(timer_tick);
  sark_cpu_state (CPU_STATE_PAUSE);
  paused = 1;
}


void resume()
{
  if (resume_sync == 1)
  {
    resume_sync = 0;
    event.wait ^= 1;
  }
  paused = 0;
  sark_cpu_state (CPU_STATE_RUN);
  vic[VIC_ENABLE] = (1 << TIMER1_INT);
  tc[T1_CONTROL] = 0xe2;
}


void spin1_resume(sync_bool sync)
{
  if (sync == SYNC_NOWAIT)
  {
    resume();
  }
  else
  {
    resume_sync = 1;
    if (event.wait)
    {
      sark_cpu_state(CPU_STATE_SYNC1);
    }
    else
    {
      sark_cpu_state(CPU_STATE_SYNC0);
    }
  }
}


uint resume_wait()
{
  uint bit = 1 << sark.virt_cpu;

  if (event.wait)
  {
    return (~sc[SC_FLAG] & bit);    // Wait 1
  }
  return (sc[SC_FLAG] & bit);     // Wait 0
}


// ------------------------------------------------------------------------
// scheduler/dispatcher functions
// ------------------------------------------------------------------------
/****f* spin1_api.c/dispatch
*
* SUMMARY
*  This function executes callbacks which are scheduled in response to events.
*  Callbacks are completed firstly in order of priority and secondly in the
*  order in which they were enqueued.
*
*  The dispatcher is the sole consumer of the scheduler queues and so can
*  safely run with interrupts enabled. Note that deschedule(uint event_id)
*  modifies the scheduler queues which naturally influences the callbacks
*  that are dispatched by this function but not in such a way as to allow the
*  dispatcher to move the processor into an invalid state such as calling a
*  NULL function.
*
*  Upon emptying the scheduling queues the dispatcher goes into wait for
*  interrupt mode.
*
*  Potential hazard: It is possible that an event will occur -and result in
*  a callback being scheduled- AFTER the last check on the scheduler queues
*  and BEFORE the wait for interrupt call. In this case, the scheduled
*  callback would not be handled until the next event occurs and causes the
*  wait for interrupt call to return.
*
*  This hazard is avoided by calling wait for interrupt with interrupts
*  disabled! Any interrupt will still wake up the core and then
*  interrupts are enabled, allowing the core to respond to it.
*
* SYNOPSIS
*  void dispatch()
*
* SOURCE
*/
void dispatch()
{
  uint i;
  uint cpsr;
  task_queue_t *tq;
  volatile callback_t cback;
  resume_sync = 0;

  // dispatch callbacks from queues until spin1_stop () or
  // spin1_kill () are called (run = 0)
  while (run)
  {
    i = 0;

    // disable interrupts to avoid concurrent
    // scheduler/dispatcher accesses to queues
    cpsr = spin1_int_disable ();

    while (run && i < (NUM_PRIORITIES-1))
    {
      tq = &task_queue[i];

      i++;  // prepare for next priority queue

      if(tq->start != tq->end)
      {
        cback = tq->queue[tq->start].cback;
        uint arg0 = tq->queue[tq->start].arg0;
        uint arg1 = tq->queue[tq->start].arg1;

        tq->start = (tq->start + 1) % TASK_QUEUE_SIZE;

        if(cback != NULL)
        {
          // run callback with interrupts enabled
          spin1_mode_restore (cpsr);

          // check for if its a timer callback, if it is, update tracker values
          if (cback == callback[TIMER_TICK].cback)
          {
            diagnostics.in_timer_callback = 1;
          }

          // execute callback
          if (cback != callback[TIMER_TICK].cback || !paused) {
              cback (arg0, arg1);
          }

          // update queue size
          if (cback == callback[TIMER_TICK].cback)
          {
            if (diagnostics.number_timer_tic_in_queue > 0)
            {
              diagnostics.number_timer_tic_in_queue -= 1;
            }
            diagnostics.in_timer_callback = 0;
          }

          cpsr = spin1_int_disable ();

          // re-start examining queues at highest priority
          i = 0;
        }
      }
    }

    if (run)
    {
      // go to sleep with interrupts disabled to avoid hazard!
      // an interrupt will still wake up the dispatcher
      spin1_wfi ();

      // Handle resume
      if (resume_sync == 1) {
          if (!resume_wait()) {
              resume();
          }
      }

      spin1_mode_restore (cpsr);
    }
  }
}
/*
*******/


// ------------------------------------------------------------------------
// simulation control and event management functions
// ------------------------------------------------------------------------
/****f* spin1_api.c/spin1_callback_on
*
* SUMMARY
*  This function sets the given callback to be scheduled on occurrence of the
*  specified event. The priority argument dictates the order in which
*  callbacks are executed by the scheduler.
*
* SYNOPSIS
*  void spin1_callback_on(uchar event_id, callback_t cback, int priority)
*
* INPUTS
*  uint event_id: event for which callback should be enabled
*  callback_t cback: callback function
*  int priority:   0 = non-queueable callback (associated to irq)
*                > 0 = queueable callback
*                < 0 = preeminent callback (associated to fiq)
*
* SOURCE
*/
void spin1_callback_on (uint event_id, callback_t cback, int priority)
{

  // set up the callback
  callback[event_id].cback = cback;
  callback[event_id].priority = priority;

  // Enforce a single callback on FIQ

  if (priority < 0)
  {
    if (fiq_event == -1 ||
    (event_id == MC_PACKET_RECEIVED && fiq_event == MCPL_PACKET_RECEIVED) ||
    (event_id == MCPL_PACKET_RECEIVED && fiq_event == MC_PACKET_RECEIVED) ||
    (event_id == FR_PACKET_RECEIVED && fiq_event == FRPL_PACKET_RECEIVED) ||
    (event_id == FRPL_PACKET_RECEIVED && fiq_event == FR_PACKET_RECEIVED))
      fiq_event = event_id;
    else
      rt_error (RTE_API);
  }

  // Enforce same interrupt handler for both packet callbacks

  if (event_id == MC_PACKET_RECEIVED || event_id == MCPL_PACKET_RECEIVED)
    {
      if (mc_pkt_prio == -2)
        mc_pkt_prio = priority;
      else if (mc_pkt_prio == -1 && priority != -1)
        rt_error (RTE_API);
    }
  else if (event_id == FR_PACKET_RECEIVED || event_id == FRPL_PACKET_RECEIVED)
    {
      if (fr_pkt_prio == -2)
        fr_pkt_prio = priority;
      else if (fr_pkt_prio == -1 && priority != -1)
        rt_error (RTE_API);
    }

}
/*
*******/


/****f* spin1_api.c/spin1_callback_off
*
* SUMMARY
*  This function disables the callback for the specified event.
*
* SYNOPSIS
*  void spin1_callback_off(uint event_id)
*
* INPUTS
*  uint event_id: event for which callback should be disabled
*
* SOURCE
*/

void spin1_callback_off(uint event_id)
{
  callback[event_id].cback = NULL;

  if (callback[event_id].priority < 0)
    fiq_event = -1;
}
/*
*******/


/****f* spin1_api.c/deschedule
*
* SUMMARY
*  This function deschedules all callbacks corresponding to the given event
*  ID. One use for this function is to effectively discard all received
*  packets which are yet to be processed by calling
*  deschedule(MC_PACKET_RECEIVED). Note that this function cannot guarantee that
*  all callbacks pertaining to the given event ID will be descheduled: once a
*  callback has been prepared for execution by the dispatcher it is immune to
*  descheduling and will be executed upon return to the dispatcher.
*
* SYNOPSIS
*  void deschedule(uint event_id)
*
* INPUTS
*  uint event_id: event ID of the callbacks to be descheduled
*
* SOURCE
*/
void deschedule(uint event_id)
{
  uint cpsr = spin1_irq_disable ();

  task_queue_t *tq = &task_queue[callback[event_id].priority-1];

  for(uint i = 0; i < TASK_QUEUE_SIZE; i++)
  {
    if(tq->queue[i].cback == callback[event_id].cback) tq->queue[i].cback = NULL;
  }

  spin1_mode_restore(cpsr);
}
/*
*******/


/****f* spin1_api.c/spin1_get_simulation_time
*
* SUMMARY
*  This function returns the number of timer periods which have elapsed since
*  the beginning of the simulation.
*
* SYNOPSIS
*  uint spin1_get_simulation_time()
*
* OUTPUTS
*  Timer ticks since beginning of simulation.
*
* SOURCE
*/
uint spin1_get_simulation_time()
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
void spin1_exit (uint error)
{
  // Disable API-enabled interrupts to allow simulation to stop,

  vic[VIC_DISABLE] = (1 << CC_MC_INT)   |
                     (1 << CC_FR_INT)   |
                     (1 << TIMER1_INT)   |
                     (1 << SOFTWARE_INT) |
                     (1 << DMA_ERR_INT)  |
                     (1 << DMA_DONE_INT);

  // Report back the return code and stop the simulation

  run = 0;
  diagnostics.exit_code = error;
}
/*
*******/


/****f* spin1_api.c/spin1_set_timer_tick
*
* SUMMARY
*  This function sets the period of the timer tick
*
* SYNOPSIS
*  void set_timer_tick(uint time)
*
* INPUTS
*  uint time: timer tick period (in microseconds)
*
* SOURCE
*/
void spin1_set_timer_tick (uint time)
{
  timer_tick = time;
}
/*
*******/


/****f* spin1_api.c/clean_up
*
* SUMMARY
*  This function is called after simulation stops to configure
*  hardware for idle operation. It cleans up interrupt lines.
*
* SYNOPSIS
*  void clean_up ()
*
* SOURCE
*/
void clean_up ()
{
  uint cpsr = spin1_int_disable ();

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

  spin1_mode_restore (cpsr);
}
/*
*******/


/****f* spin1_api.c/report_debug
*
* SUMMARY
*  This function reports warnings if requested
*  at compile time
*
* SYNOPSIS
*  void report_debug ()
*
* SOURCE
*/
void report_debug ()
{
  #if API_DEBUG == TRUE

    if (leadAp) // Only the leader appl. core reports router data
    {
      // Report router counters

      io_printf (IO_API, "\t\t[api_debug] RTR mc     packets: %d\n",
                 rtr[RTR_DGC0] + rtr[RTR_DGC1]);
      io_delay (API_PRINT_DELAY);

      io_printf (IO_API, "\t\t[api_debug] RTR dpd mc packets: %d\n",
                 rtr[RTR_DGC8]);
      io_delay (API_PRINT_DELAY);
    }

    io_printf (IO_API, "\t\t[api_debug] ISR thrown packets: %d\n",
               diagnostics.discarded_mc_packets);
    io_delay (API_PRINT_DELAY);

    io_printf (IO_API, "\t\t[api_debug] ISR thrown FR packets: %d\n",
               diagnostics.discarded_fr_packets);
    io_delay (API_PRINT_DELAY);

    // Report DMAC counters

    io_printf (IO_API, "\t\t[api_debug] DMA bursts:  %d\n", dma[DMA_STAT0]);
    io_delay (API_PRINT_DELAY);
  #endif
}
/*
*******/



/****f* spin1_api.c/report_warns
*
* SUMMARY
*  This function reports warnings if requested
*  at compile time
*
* SYNOPSIS
*  void report_warns ()
*
* SOURCE
*/
void report_warns ()
{
#if API_WARN == TRUE	    // report warnings
  if (diagnostics.warnings & TASK_QUEUE_FULL)
    {
      io_printf (IO_API, "\t\t[api_warn] warning: task queue full (%u)\n",
                 diagnostics.task_queue_full);
      io_delay (API_PRINT_DELAY);
    }
  if (diagnostics.warnings & DMA_QUEUE_FULL)
    {
      io_printf (IO_API, "\t\t[api_warn] warning: DMA queue full (%u)\n",
                 diagnostics.dma_queue_full);
      io_delay (API_PRINT_DELAY);
    }
  if (diagnostics.warnings & PACKET_QUEUE_FULL)
    {
      io_printf (IO_API, "\t\t[api_warn] warning: packet queue full (%u)\n",
                 diagnostics.tx_packet_queue_full);
      io_delay (API_PRINT_DELAY);
    }
# if USE_WRITE_BUFFER == TRUE
  if (diagnostics.warnings & WRITE_BUFFER_ERROR)
    {
      io_printf (IO_API,
        "\t\t[api_warn] warning: write buffer errors (%u)\n",
             diagnostics.writeBack_errors);
      io_delay (API_PRINT_DELAY);
    }
#endif
#endif
}
/*
*******/


void spin1_rte(rte_code code)
{

  // Don't actually shutdown, just set the CPU into an RTE code and
  // stop the timer
  clean_up();
  sark_cpu_state(CPU_STATE_RTE);
#ifdef __GNUC__
  register uint lr asm("lr");
#else
  register uint lr __asm("lr");
#endif
  sv_vcpu->lr = lr;
  sv_vcpu->rt_code = code;
  sv->led_period = 8;
}

uint start (sync_bool sync, uint start_paused)
{
  paused = start_paused;
  if (paused)
  {
    sark_cpu_state (CPU_STATE_PAUSE);
  }
  else
  {
    sark_cpu_state (CPU_STATE_RUN);
  }

  // Initialise hardware

  configure_communications_controller();
  configure_dma_controller();
  configure_timer1 (timer_tick);
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
#endif

  // synchronise with other application cores

  if (sync == SYNC_WAIT)
    event_wait ();

  // initialise counter and ticks for simulation
  // 32-bit, periodic counter, interrupts enabled

  if (timer_tick && !paused) {
    tc[T1_CONTROL] = 0xe2;
  }

  ticks = 0;
  run = 1;

  // simulate!
  dispatch ();

  // simulation finished - clean up before returning to c_main
  clean_up ();

  // re-enable interrupts for sark
  // only CPU_INT enabled in the VIC
  spin1_int_enable ();

  // provide diagnostics data to application
  #if (API_DIAGNOSTICS == TRUE)
    diagnostics.total_mc_packets     = rtr[RTR_DGC0] + rtr[RTR_DGC1];
    diagnostics.dumped_mc_packets    = rtr[RTR_DGC8];
    diagnostics.dma_bursts           = dma[DMA_STAT0];
    diagnostics.total_fr_packets     = rtr[RTR_DGC6] + rtr[RTR_DGC7];
    diagnostics.dumped_fr_packets    = rtr[RTR_DGC11];
  #endif

  // report problems if requested!
  #if (API_DEBUG == TRUE) || (API_WARN == TRUE)
    // avoid sending output at the same time as other chips!
    io_delay (10000 * my_chip);

    #if API_DEBUG == TRUE	// report debug information
      report_debug();
    #endif

    #if API_WARN == TRUE       	// report warnings
      report_warns ();
    #endif
  #endif

  return diagnostics.exit_code;
}
/*
*******/


/****f* spin1_api.c/spin1_start
*
* SUMMARY
*  This function begins a simulation by enabling the timer (if called for) and
*  beginning the dispatcher loop.
*
* SYNOPSIS
*  void spin1_start (sync_bool_t sync)
*
* SOURCE
*/

uint spin1_start (sync_bool sync)
{
  return start(sync, 0);
}

uint spin1_start_paused()
{
  return start(SYNC_NOWAIT, 1);
}


/****f* spin1_api.c/spin1_delay_us
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
// !! ST replaced by SARK routine
void spin1_delay_us (uint n)
{
  sark_delay_us (n);
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
uint spin1_dma_transfer (uint tag, void *system_address, void *tcm_address,
            uint direction, uint length)
{
  uint id = 0;
  uint cpsr = spin1_int_disable ();

  uint new_end = (dma_queue.end + 1) % DMA_QUEUE_SIZE;

  if (new_end != dma_queue.start)
  {
    id = diagnostics.dma_transfers++;

    uint desc = DMA_WIDTH << 24 | DMA_BURST_SIZE << 21
      | direction << 19 | length;

    dma_queue.queue[dma_queue.end].id = id;
    dma_queue.queue[dma_queue.end].tag = tag;
    dma_queue.queue[dma_queue.end].system_address = system_address;
    dma_queue.queue[dma_queue.end].tcm_address = tcm_address;
    dma_queue.queue[dma_queue.end].description = desc;

    /* if dmac is available and dma_queue empty trigger transfer now */
    if(!(dma[DMA_STAT] & 4) && (dma_queue.start == dma_queue.end))
    {
      dma[DMA_ADRS] = (uint) system_address;
      dma[DMA_ADRT] = (uint) tcm_address;
      dma[DMA_DESC] = desc;
    }

    dma_queue.end = new_end;
  }
  else
  {
    #if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
      diagnostics.warnings |= DMA_QUEUE_FULL;
      diagnostics.dma_queue_full++;
    #endif
  }

  spin1_mode_restore(cpsr);

  return id;
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
void spin1_memcpy (void *dst, void const *src, uint len)
{
    char *pDst = (char *) dst;
    char const *pSrc = (char const *) src;

    while (len--)
    {
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
void spin1_flush_rx_packet_queue()
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
void spin1_flush_tx_packet_queue()
{
  uint cpsr = spin1_irq_disable ();

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

  uint cpsr = spin1_irq_disable ();

  /* clear sticky TX full bit and check TX state */
  cc[CC_TCR] = TX_TCR_MCDEFAULT;

  if (cc[CC_TCR] & TX_FULL_MASK)
  {
    if((tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE == tx_packet_queue.start)
    {
      /* if queue full cannot do anything -- report failure */
      rc = FAILURE;
      #if (API_WARN == TRUE) || (API_DIAGNOSTICS == TRUE)
        diagnostics.warnings |= PACKET_QUEUE_FULL;
        diagnostics.tx_packet_queue_full++;
      #endif
    }
    else
    {
      /* if not full queue packet */
      tx_packet_queue.queue[tx_packet_queue.end].key = key;
      tx_packet_queue.queue[tx_packet_queue.end].data = data;
      tx_packet_queue.queue[tx_packet_queue.end].TCR = TCR;

      tx_packet_queue.end = (tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE;

      /* turn on tx_empty interrupt (in case it was off) */
      vic[VIC_ENABLE] = (1 << CC_TMT_INT);
    }
  }
  else
  {
    if((tx_packet_queue.end + 1) % TX_PACKET_QUEUE_SIZE == tx_packet_queue.start)
    {
      /* if queue full, dequeue and send packet at the */
      /* head of the queue to make room for new packet */
      uint hkey  = tx_packet_queue.queue[tx_packet_queue.start].key;
      uint hdata = tx_packet_queue.queue[tx_packet_queue.start].data;
      uint hTCR = tx_packet_queue.queue[tx_packet_queue.start].TCR;

      tx_packet_queue.start = (tx_packet_queue.start + 1) % TX_PACKET_QUEUE_SIZE;

      cc[CC_TCR] = hTCR;

      if (hTCR & PKT_PL)
        cc[CC_TXDATA] = hdata;

      cc[CC_TXKEY]  = hkey;
    }

    if(tx_packet_queue.start == tx_packet_queue.end)
    {
      // If queue empty send packet

      cc[CC_TCR] = TCR;

      if (TCR & PKT_PL)
        cc[CC_TXDATA] = data;

      cc[CC_TXKEY]  = key;

      // turn off tx_empty interrupt (in case it was on)
      vic[VIC_DISABLE] = 1 << CC_TMT_INT;
    }
    else
    {
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

/****f* spin1_api.c/spin1_send_mc_packet
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
*  uint key: packet routining key
*  uint data: packet payload
*  uint load: 0 = no payload (ignore data param), 1 = send payload
*
* OUTPUTS
*  1 if packet is enqueued or sent successfully, 0 otherwise
*
* SOURCE
*/

uint spin1_send_mc_packet(uint key, uint data, uint load)
{
  uint tcr = (load) ? PKT_MC_PL : PKT_MC;

  return spin1_send_packet (key, data, tcr);
}
/*
*******/

/****f* spin1_api.c/spin1_send_ft_packet
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
*  uint key: packet routining key
*  uint data: packet payload
*  uint load: 0 = no payload (ignore data param), 1 = send payload
*
* OUTPUTS
*  1 if packet is enqueued or sent successfully, 0 otherwise
*
* SOURCE
*/

uint spin1_send_fr_packet(uint key, uint data, uint load)
{
  uint tcr = (load) ? PKT_FR_PL : PKT_FR;

  return spin1_send_packet (key, data, tcr);
}

/*
*******/


/****f* spin1_api.c/spin1_irq_disable
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
extern uint spin1_irq_disable (void);
#else
#ifdef __GNUC__
__inline uint spin1_irq_disable (void)
{
  uint old, new;

  asm volatile (
    "mrs	%[old], cpsr \n\
     orr	%[new], %[old], #0x80 \n\
     msr	cpsr_c, %[new] \n"
     : [old] "=r" (old), [new] "=r" (new)
     :
     : );

  return old;
}
#else
__forceinline uint spin1_irq_disable (void)
{
  uint old, new;

  __asm { mrs old, cpsr }
  __asm { orr new, old, 0x80 }
  __asm { msr cpsr_c, new }

  return old;
}
#endif
#endif
/*
*******/


/****f* spin1_api.c/spin1_mode_restore
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
extern void spin1_mode_restore (uint cpsr);
#else
#ifdef __GNUC__
__inline void spin1_mode_restore (uint cpsr)
{
  asm volatile (
    "msr	cpsr_c, %[cpsr]"
    :
    : [cpsr] "r" (cpsr)
    :);
}
#else
__forceinline void spin1_mode_restore (uint sr)
{
  __asm { msr cpsr_c, sr }
}
#endif
#endif
/*
*******/


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
extern uint spin1_irq_enable (void);
#else
#ifdef __GNUC__
__inline uint spin1_irq_enable (void)
{
  uint old, new;

  asm volatile (
    "mrs	%[old], cpsr \n\
     bic	%[new], %[old], #0x80 \n\
     msr	cpsr_c, %[new] \n"
     : [old] "=r" (old), [new] "=r" (new)
     :
     : );

  return old;
}
#else
__forceinline uint spin1_irq_enable (void)
{
  uint old, new;

  __asm { mrs old, cpsr }
  __asm { bic new, old, 0x80 }
  __asm { msr cpsr_c, new }

  return old;
}
#endif
#endif
/*
*******/


/****f* spin1_api.c/spin1_fiq_disable
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
extern uint spin1_fiq_disable (void);
#else
#ifdef __GNUC__
__inline uint spin1_fiq_disable (void)
{
  uint old, new;

  asm volatile (
    "mrs	%[old], cpsr \n\
     orr	%[new], %[old], #0x40 \n\
     msr	cpsr_c, %[new] \n"
     : [old] "=r" (old), [new] "=r" (new)
     :
     : );

  return old;
}
#else
__forceinline uint spin1_fiq_disable (void)
{
  uint old, new;

  __asm { mrs old, cpsr }
  __asm { orr new, old, 0x40 }
  __asm { msr cpsr_c, new }

  return old;
}
#endif
#endif
/*
*******/


/****f* spin1_api.c/spin1_int_disable
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
extern uint spin1_int_disable (void);
#else
#ifdef __GNUC__
__inline uint spin1_int_disable (void)
{
  uint old, new;

  asm volatile (
    "mrs	%[old], cpsr \n\
     orr	%[new], %[old], #0xc0 \n\
     msr	cpsr_c, %[new] \n"
     : [old] "=r" (old), [new] "=r" (new)
     :
     : );

  return old;
}
#else
__forceinline uint spin1_int_disable (void)
{
  uint old, new;

  __asm { mrs old, cpsr }
  __asm { orr new, old, 0xc0 }
  __asm { msr cpsr_c, new }

  return old;
}
#endif
#endif
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
uint spin1_get_core_id (void)
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
uint spin1_get_chip_id (void)
{
  return (uint) sv->p2p_addr;
}
/*
*******/



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

//## This routine has been removed - use "rtr_alloc", "rtr_mc_set" instead

#if 0
uint spin1_set_mc_table_entry(uint entry, uint key, uint mask, uint route)
{
  if (entry >= APP_MC_ENTRIES) // top priority entries reserved for the system
    return FAILURE;

  entry += SYS_MC_ENTRIES;
  rtr_mc_set (entry, key, mask, route);

#if API_DEBUG == TRUE
  io_printf (IO_API,
        "\t\t[api_debug] MC entry %d: k 0x%8z m 0x%8z r 0x%8z\n",
        entry, key, mask, route);
  io_delay (API_PRINT_DLY);
#endif

  return SUCCESS;
}
#endif

/****f* spin1_api.c/spin1_led_control
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

// !! ST replaced by SARK routine

void spin1_led_control (uint p)
{
  sark_led_set (p);
}
/*
*******/


// ------------------------------------------------------------------------
// memory allocation functions
// ------------------------------------------------------------------------
/****f* spin1_api.c/spin1_malloc
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

void* spin1_malloc (uint bytes)
{
  return sark_alloc (bytes, 1);
}

/*
*******/


/****f* spin1_api.c/schedule_sysmode
*
* SUMMARY
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
* SYNOPSIS
*  void schedule_sysmode(uchar event_id, uint arg0, uint arg1)
*
* INPUTS
*  uchar event_id: ID of the event triggering a callback
*  uint arg0: argument to be passed to the callback
*  uint arg1: argument to be passed to the callback
*
* SOURCE
*/
void schedule_sysmode (uchar event_id, uint arg0, uint arg1)
{
  if(callback[event_id].priority <= 0)
  {
    callback[event_id].cback(arg0, arg1);
  }
  else
  {
    task_queue_t *tq = &task_queue[callback[event_id].priority-1];

    if((tq->end + 1) % TASK_QUEUE_SIZE != tq->start)
    {
      tq->queue[tq->end].cback = callback[event_id].cback;
      tq->queue[tq->end].arg0 = arg0;
      tq->queue[tq->end].arg1 = arg1;

      tq->end = (tq->end + 1) % TASK_QUEUE_SIZE;

      if (event_id == TIMER_TICK)
      {
        diagnostics.number_timer_tic_in_queue += 1;
      }
    }
    else      // queue is full
    {
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
uint spin1_schedule_callback (callback_t cback, uint arg0, uint arg1,
                uint priority)
{
  uchar result = SUCCESS;

  /* disable interrupts for atomic access to task queues */
  uint cpsr = spin1_irq_disable ();

  task_queue_t *tq = &task_queue[priority-1];

  if((tq->end + 1) % TASK_QUEUE_SIZE != tq->start)
  {
    tq->queue[tq->end].cback = cback;
    tq->queue[tq->end].arg0 = arg0;
    tq->queue[tq->end].arg1 = arg1;

    tq->end = (tq->end + 1) % TASK_QUEUE_SIZE;
  }
  else
  {
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


/****f* spin1_api.c/spin1_trigger_user_event
*
* SUMMARY
*  This function triggers a USER EVENT, i.e., a software interrupt.
*  The function returns FAILURE if a previous trigger attempt
*  is still pending.
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
  if (!user_pending)
  {
    /* remember callback arguments */
    user_arg0 = arg0;
    user_arg1 = arg1;
    user_pending = TRUE;

    /* trigger software interrupt in the VIC */
    vic[VIC_SOFT_SET] = (1 << SOFTWARE_INT);

    return (SUCCESS);
  }
  else
  {
    return (FAILURE);
  }
}
/*
*******/


// ------------------------------------------------------------------------
// rts initialization function
// called before the application program starts!
// ------------------------------------------------------------------------
/****f* spin1_api.c/rts_init
*
* SUMMARY
*  This function is a stub for the run-time system.
*  initializes peripherals in the way the RTS
*  is expected to do
*
* SYNOPSIS
*  void sark_pre_main (void)
*
* SOURCE
*/
void sark_pre_main (void)
{
  sark_call_cpp_constructors();

  sark_cpu_state (CPU_STATE_SARK);
  sark_vec->api = 1;

  leadAp = (sark_app_lead () == sark.virt_cpu);
}
/*
*******/


// ------------------------------------------------------------------------
// rts cleanup function
// called after the application program finishes!
// ------------------------------------------------------------------------
/****f* spin1_api.c/rts_cleanup
*
* SUMMARY
*  This function is a stub for the run-time system.
*  makes sure that application returns cleanly to the RTS.
*
* SYNOPSIS
*  void sark_post_main (void)
*
* SOURCE
*/
void sark_post_main (void)
{
  sark_cpu_state (CPU_STATE_EXIT);
}
/*
*******/
