
//------------------------------------------------------------------------------
//
// sark_timer.c	    Timer routines for SARK
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#include <sark.h>

//------------------------------------------------------------------------------


extern INT_HANDLER timer2_int_han (void);

void event_register_timer (vic_slot slot)
{
  if (vic[VIC_CNTL + slot] == 0)
    {
      tc[T2_CONTROL] = 0;			// Disable timer

      event.vic_enable |= 1 << TIMER2_INT;	// Disabled by event_stop

      sark_vic_set (slot, TIMER2_INT, 1, timer2_int_han);
    }
  else
    rt_error (RTE_TIMER);
}


//------------------------------------------------------------------------------

// Schedule an event to occur at some time in the future (measured in
// microseconds).


void timer_schedule (event_t *e, uint time)
{
  time *= sark.cpu_clk;		// Convert us to timer ticks

  uint cpsr = cpu_int_disable ();

  if (event.timer_queue == NULL)// Queue empty - just insert
    {
      event.timer_queue = e;

      tc[T2_LOAD] = time;
      tc[T2_CONTROL] = 0xe3;

      cpu_int_restore (cpsr);
      return;
    }

  uint t2c = tc[T2_COUNT];

  if (time < t2c) 		// Earlier than current head - replace
    {
      tc[T2_LOAD] = time;	// Set counter to new time

      event.timer_queue->time = t2c - time;	// Adjust old head time

      e->next = event.timer_queue;		// and insert new timer at head
      event.timer_queue = e;

      cpu_int_restore (cpsr);
      return;
    }

  // (time >= t2c) - Insert in correct place in queue (tricky!)

  uint total = 0;
  uint ptotal = 0;

  event_t *pq = event.timer_queue;
  event_t *q = pq->next;

  time -= t2c;

  while (q != NULL)
    {
      total += q->time;

      if (total > time)
	{
	  q->time = total - time;
	  break;
	}

      pq = q;
      ptotal += q->time;
      q = q->next;
    }

  e->time = time - ptotal;
  
  e->next = pq->next;
  pq->next = e;

  cpu_int_restore (cpsr);
}


//------------------------------------------------------------------------------

// Schedule an "event_proc" to occur at some time in the future (measured in
// microseconds). Returns zero if an event could not be allocated, one
// otherwise.


uint timer_schedule_proc (event_proc proc, uint arg1, uint arg2, uint time)
{
  event_t *e = event_new (proc, arg1, arg2);

  if (e == NULL)
    return 0;

  timer_schedule (e, time);
  return 1;
}


//------------------------------------------------------------------------------


// Cancel a timer that was previously scheduled. The ID that was
// allocated when the timer was created must be given in case the
// timer has already executed and possibly been recycled.

// It is potentially quite difficult to cancel an timer at the head
// of the timer queue so in this case the "proc" is made NULL and
// the timer left to terminate on the timer interrupt.


void timer_cancel (event_t *e, uint ID)
{
  if (e == NULL)
    rt_error (RTE_NULL);

  if (e->ID != ID)		// Still active (return if not)
    return;

  uint cpsr = cpu_int_disable ();
  event_t* q = event.timer_queue;

  if (q == e) 		// At head of queue - nullify proc
    {
      e->proc = NULL;
    }
  else 			// Further down queue - remove
    {
      event_t* pq = q;
      q = q->next;

      while (q != NULL)
	{
	  if (q == e)
	    {
	      pq->next = e->next;

	      if (e->next != NULL)	// Add our time to next in queue
		e->next->time += e->time;

	      e->next = event.free;	// Return to free queue
	      event.free = e;
	      e->ID = 0;
	      event.count--;

	      break;
	    }

	  pq = q;
	  q = q->next;
	}
    }

  cpu_int_restore (cpsr);
}


//------------------------------------------------------------------------------


// Interrupt handler for the timer which is handling timers.

void timer2_int (void)
{
  tc[T2_INT_CLR] = (uint) tc;		// Clear interrupt in timer

  event_t *e = event.timer_queue;	// Pointer to timer queue
  event_t *eq = NULL;			// Accumulates list of events to execute

  while (e != NULL && e->time == 0)	// Process all in queue with time=0
    {
      event_t* next = e->next;		// Remove head of event queue
      event.timer_queue = next;

      e->next = eq;			// Move to exec queue
      eq = e;

      e = next;				// Move to next entry
    }

  if (e != NULL)			// If entries remain in queue
    {
      tc[T2_LOAD] = e->time;		// Put first one into timer
      e->time = 0;			// and set its time to zero
    }

  while (eq != NULL)			// Run the execute queue
    {
      if (eq->proc != NULL)		// Execute proc if non-NULL
      	eq->proc (eq->arg1, eq->arg2);

      event_t* next = eq->next;

      eq->next = event.free;		// Return to free queue
      event.free = eq;
      eq->ID = 0;
      event.count--;

      eq = next;
    }

  vic[VIC_VADDR] = (uint) vic;		// Ack the VIC
}


//------------------------------------------------------------------------------
