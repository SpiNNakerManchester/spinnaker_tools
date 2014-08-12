
//------------------------------------------------------------------------------
//
// sark_event.c	    Event handling routines for SARK
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#include <sark.h>

//------------------------------------------------------------------------------


extern int_handler fiq_events[];
extern int_handler irq_events[];
extern int_handler queue_events[];
extern int_handler null_events[];


static const uchar vic_bit[] =
  {
    TIMER1_INT,		// Timer 1
    CC_MC_INT,		// MC pkt
    SARK_MSG_INT,	// SDP msg
    SOFTWARE_INT,	// User event 
    SARK_SIG_INT,	// Signal
    DMA_DONE_INT	// DMA done
  };


enum efail_code {EFAIL_FIQ, EFAIL_IRQ, EFAIL_QUEUE, EFAIL_ALLOC, EFAIL_NEW};

#define EFAIL(x) (1 << (x))


event_data_t event;


//------------------------------------------------------------------------------


uint event_user (uint arg1, uint arg2)
{
  if (event.user)
    return 0;

  event.arg1 = arg1;
  event.arg2 = arg2;
  event.user = 1;

  vic[VIC_SOFT_SET] = 1 << SOFTWARE_INT;

  return 1;
}


//------------------------------------------------------------------------------


void event_register_int (event_proc proc, event_type event_num,
			 vic_slot slot)
{
  uint bit = 1 << vic_bit[event_num];

  if (slot == SLOT_FIQ)
    {
      if (event_num > EVENT_MAX ||
	  event.vic_select != 0 ||
	  event.vic_enable & bit)
	{
	  event.rc |= EFAIL (EFAIL_FIQ);
	  return;
	}

      event.fiq_vector = fiq_events[event_num];

      sark_vec->event[event_num].proc = (uint) proc;
      sark_vec->event[event_num].slot = SLOT_FIQ;
      sark_vec->event[event_num].priority = 0;	// 0 for FIQ

      event.vic_select = bit;
      event.vic_enable |= bit;
    }
  else
    {
      if (event_num > EVENT_MAX ||
	  slot > SLOT_MAX ||
	  event.vic_enable & bit ||
	  vic[VIC_CNTL + slot] != 0)
	{
	  event.rc |= EFAIL (EFAIL_IRQ);
	  return;
	}

      vic[VIC_ADDR + slot] = (uint) irq_events[event_num];
      vic[VIC_CNTL + slot] = 0x20 | vic_bit[event_num];

      sark_vec->event[event_num].proc = (uint) proc;
      sark_vec->event[event_num].slot = slot;
      sark_vec->event[event_num].priority = 1;	// 1 for IRQ

      event.vic_addr[event_num] = irq_events[event_num];
      event.vic_enable |= bit;
    }
}


void event_register_queue (event_proc proc, event_type event_num,
			   vic_slot slot, event_priority priority)
{
  uint bit = 1 << vic_bit[event_num];

  if (event_num > EVENT_MAX ||
      slot > SLOT_MAX ||
      priority > PRIO_MAX ||
      event.vic_enable & bit ||
      vic[VIC_CNTL + slot] != 0)
    {
      event.rc |= EFAIL (EFAIL_QUEUE);
      return;
    }

  vic[VIC_ADDR + slot] = (uint) queue_events[event_num];
  vic[VIC_CNTL + slot] = 0x20 | vic_bit[event_num];

  sark_vec->event[event_num].proc = (uint) proc;
  sark_vec->event[event_num].slot = slot;
  sark_vec->event[event_num].priority = priority;

  event.vic_addr[event_num] = queue_events[event_num];
  event.vic_enable |= bit;
}


void event_enable (event_type event_num, uint enable)
{
  uint slot = sark_vec->event[event_num].slot;

  if (enable)
    {
      if (slot == SLOT_FIQ)
	{
	  if (event.fiq_vector == NULL)
	    rt_error (RTE_ENABLE);

	  sark_vec->fiq_vec = event.fiq_vector;
	}
      else
	{
	  if (event.vic_addr[event_num] == NULL)
	    rt_error (RTE_ENABLE);

	  vic[VIC_ADDR + slot] = (uint) event.vic_addr[event_num];
	}
    }
  else
    {
      if (slot == SLOT_FIQ)
	{
	  sark_vec->fiq_vec = null_events[event_num];
	}
      else
	{
	  vic[VIC_ADDR + slot] = (uint) null_events[event_num];
	}
    }
}


void event_register_pause (event_proc proc, uint arg2)
{
  event.pause_proc = proc;
  event.pause_arg2 = arg2;
}


//------------------------------------------------------------------------------


void event_wait (void)
{
  uint bit = 1 << sark.virt_cpu;

  if (event.wait)
    {
      sark_cpu_state (CPU_STATE_SYNC1);

      while (~sc[SC_FLAG] & bit) 	// Wait 1
	cpu_wfi ();
    }
  else
    {
      sark_cpu_state (CPU_STATE_SYNC0);

      while (sc[SC_FLAG] & bit) 	// Wait 0
	cpu_wfi ();
    }

  event.wait ^= 1;
  sark_cpu_state (CPU_STATE_RUN);
}

//##
void event_run2 (uint restart);

uint event_start (uint period, uint events, sync_bool wait)
{
  // Allocate required number of events

  if (events != 0)
    event_alloc (events);

  // Die if any errors so far

  if (event.rc != 0)
    rt_error (RTE_EVENT, event.rc);

  // Wait for go...

  if (wait)
    event_wait ();

  // Set up timer1 if needed

  if (period)
    {
      tc[T1_CONTROL] = 0xe2;
      tc[T1_LOAD] = sark.cpu_clk * period;
    }

  // Set up VIC enables and FIQ vector for this app.

  uint cpsr = cpu_int_disable ();

  event.old_vector = sark_vec->fiq_vec;
  event.old_select = vic[VIC_SELECT];
  event.old_enable = vic[VIC_ENABLE];

  // Need to check that received packets are still discarded
  // if app doesn't use EVENT_RXPKT?

  sark_vec->fiq_vec = event.fiq_vector;
  vic[VIC_SELECT] = event.vic_select;

  vic[VIC_ENABLE] = event.vic_enable;

  cpu_int_restore (cpsr);

  // Notify new CPU state

  sark_cpu_state (CPU_STATE_RUN);

  // Run the event loop until asked to stop

  while (event.state != EVENT_STOP)
    {
      event_run2 (1);

      do
	{
	  cpu_wfi ();
	}
      while (event.state == EVENT_PAUSE);
    }

  // Notify new CPU state

  sark_cpu_state (CPU_STATE_EXIT);
  sark.vcpu->rt_code = event.exit_rc;

  // Tidy up VIC and FIQ vector

  cpsr = cpu_int_disable ();

  sark_vec->fiq_vec = event.old_vector;

  //  vic[VIC_DISABLE] = event.vic_enable; // Done in "event_stop"
  vic[VIC_SELECT] = event.old_select;
  vic[VIC_ENABLE] = event.old_enable;

  cpu_int_restore (cpsr);

  return event.exit_rc;
}


void event_pause (uint pause)
{
  if (pause)
    {
      event.state = EVENT_PAUSE;
      sark_cpu_state (CPU_STATE_PAUSE);
    }
  else
    {
      event.state = EVENT_RUN;
      sark_cpu_state (CPU_STATE_RUN);
    }

  if (event.pause_proc != NULL)
    event.pause_proc (pause, event.pause_arg2);
}


void event_stop (uint rc)
{
  event.state = EVENT_STOP;

  vic[VIC_DISABLE] = event.vic_enable;

  event.exit_rc = rc;
}


//------------------------------------------------------------------------------

// Adds an event to a list of events which can (all) be executed
// at some later time. Later events are queued at the tail of the queue.

uint event_queue (event_t *e, event_priority priority)
{
  if (priority > PRIO_MAX)
    return 0;

  proc_queue_t *queue = event.proc_queue + priority;

  uint cpsr = cpu_int_disable ();

  if (queue->proc_head == NULL)
    {
      queue->proc_head = queue->proc_tail = e;
    }
  else
    {
      queue->proc_tail->next = e;
      queue->proc_tail = e;
    }

  cpu_int_restore (cpsr);

  return 1;
}


//------------------------------------------------------------------------------

// Adds an "event_proc" with args to the event_queue.

uint event_queue_proc (event_proc proc, uint arg1, uint arg2,
		       event_priority priority)
{
  event_t *e = event_new (proc, arg1, arg2);

  if (e != NULL)
    return event_queue (e, priority);

  return 0;
}


//------------------------------------------------------------------------------

// Execute a list of events (in the order in which they were added
// to the list). Events are returned to the free queue after execution.

void event_run (uint restart)
{
  uint priority = PRIO_0;

  while (priority <= PRIO_MAX && event.state == EVENT_RUN)
    {
      proc_queue_t *queue = event.proc_queue + priority;

      uint cpsr = cpu_int_disable ();

      event_t* e = queue->proc_head;	// Get list of events and
      queue->proc_head = NULL;		// update list head

      cpu_int_restore (cpsr);

      event_t *x = e;			// Non-NULL if any events run

      while (e != NULL)
	{
	  event_t* next = e->next;

	  e->proc (e->arg1, e->arg2);	// No need to check for NULL here

	  uint cpsr = cpu_int_disable ();

	  e->next = event.free;		// Return to free queue
	  event.free = e;
	  e->ID = 0;
	  event.count--;

	  cpu_int_restore (cpsr);

	  e = next;
	}

      if ((x != NULL) && restart)
	priority = PRIO_0;
      else
	priority++;
    }
}


void event_run2 (uint restart)
{
  event_priority priority = PRIO_0;

  while (priority <= PRIO_MAX && event.state == EVENT_RUN)
    {
      proc_queue_t *queue = event.proc_queue + priority;

      uint cpsr = cpu_int_disable ();	// Interrupts off to manipulate queue

      event_t* e = queue->proc_head;	// Get head of queue

      if (e == NULL)			// If no item on queue...
	{
	  cpu_int_restore (cpsr);
	  priority++;
	  continue;
	}

      queue->proc_head = e->next;	// Remove from queue

      cpu_int_restore (cpsr);		// Interrupts on again

      e->proc (e->arg1, e->arg2);	// Execute the "proc"

      cpsr = cpu_int_disable ();	// Return to free queue

      e->next = event.free;
      event.free = e;
      e->ID = 0;
      event.count--;

      cpu_int_restore (cpsr);

      if (restart)			// Back to priority 0 if anything
	priority = PRIO_0;		// executed
    }
}


//------------------------------------------------------------------------------

// Allocate a new event from the free queue and intialise "proc",
// "arg1" and "arg2" fields. The "ID", "next" and "time" fields are also
// set.

event_t* event_new (event_proc proc, uint arg1, uint arg2)
{
  uint cpsr = cpu_int_disable ();

  event_t *e = event.free;

  if (e != NULL)
    {
      uint next_id = event.id + 1;

      if (next_id == 0)
	next_id = 1;

      event.free = e->next;
      e->next = NULL;

      e->ID = event.id = next_id;

      e->proc = proc;
      e->arg1 = arg1;
      e->arg2 = arg2;
      e->time = 0;

      event.count++;
      if (event.count > event.max)
	event.max = event.count;
    }
  else
    {
      event.failed++;
      event.rc |= EFAIL (EFAIL_NEW);
    }

  cpu_int_restore (cpsr);

  return e;
}


//------------------------------------------------------------------------------


// Create a new free pool of events by "sark_alloc". Caller specifies number of
// events to allocate and they will be added to the existing free pool.

uint event_alloc (uint events)
{
  event_t *head = (event_t *) sark_alloc (events, sizeof (event_t));

  if (head != NULL)
    {
      event_t *tail = sark_block_init (head, events, sizeof (event_t));

      uint cpsr = cpu_int_disable ();

      tail->next = event.free;
      event.free = head;

      cpu_int_restore (cpsr);

      return 1;
    }

  event.rc |= EFAIL (EFAIL_ALLOC);
  return 0;
}


//------------------------------------------------------------------------------
