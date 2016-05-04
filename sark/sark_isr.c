
//------------------------------------------------------------------------------
//
// sark_isr.c	    Interrupt handlers for SARK
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#include <sark.h>

//------------------------------------------------------------------------------


extern void timer2_int (void);


INT_HANDLER timer2_int_han (void)
{
  timer2_int ();
}


INT_HANDLER sark_fiq_han (void)
{
  (void) cc[CC_RXKEY];
  sark.pkt_count++;
}

//------------------------------------------------------------------------------


INT_HANDLER txpkt_int_han ()
{
  event.pkt_remove = (event.pkt_remove + 1) & (event.pkt_size - 1);

  pkt_t *pkt = event.pkt_queue + event.pkt_remove;

  if (pkt->ctrl & 1)
    cc[CC_TCR] = pkt->ctrl;

  if (pkt->ctrl & 2)
    cc[CC_TXDATA] = pkt->data;

  cc[CC_TXKEY] = pkt->key;

  event.pkt_count--;

  if (event.pkt_count == 0)
    vic[VIC_DISABLE] = 1 << CC_TNF_INT;

  vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


INT_HANDLER user_null (void)
{
  vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

  uint slot = sark_vec->event[EVENT_USER].slot;

  if (slot != SLOT_FIQ)
    vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER user_irq (void)
{
  vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_USER].proc;

  event.user = 0;
  proc (event.arg1, event.arg2);

  vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER user_fiq (void)
{
  vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_USER].proc;

  event.user = 0;
  proc (event.arg1, event.arg2);
}


INT_HANDLER user_queue (void)
{
  vic[VIC_SOFT_CLR] = 1 << SOFTWARE_INT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_USER].proc;
  event_priority priority = (event_priority) sark_vec->event[EVENT_USER].priority;

  event.user = 0;
  event_queue_proc (proc, event.arg1, event.arg2, priority);

  vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


INT_HANDLER sdp_null (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

  uint slot = sark_vec->event[EVENT_USER].slot;

  if (slot != SLOT_FIQ)
    vic[VIC_VADDR] = (uint) vic;
}

INT_HANDLER sdp_irq (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

  sdp_msg_t *msg = event.msg;
  uint port = msg->dest_port >> PORT_SHIFT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SDP].proc;

  proc ((uint) msg, port);

  vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER sdp_fiq (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

  sdp_msg_t *msg = event.msg;
  uint port = msg->dest_port >> PORT_SHIFT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SDP].proc;

  proc ((uint) msg, port);
}


INT_HANDLER sdp_queue (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_MSG_INT;

  sdp_msg_t *msg = event.msg;
  uint port = msg->dest_port >> PORT_SHIFT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SDP].proc;
  event_priority priority = (event_priority) sark_vec->event[EVENT_SDP].priority;

  event_queue_proc (proc, (uint) msg, port, priority);

  vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


INT_HANDLER rxpkt_null (void)
{
  uint key = cc[CC_RXKEY];

  uint slot = sark_vec->event[EVENT_RXPKT].slot;

  if (slot != SLOT_FIQ)
    vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER rxpkt_irq (void)
{
  uint data = cc[CC_RXDATA];
  uint key = cc[CC_RXKEY];

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_RXPKT].proc;

  proc (key, data);

  vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER rxpkt_fiq (void)
{
  uint data = cc[CC_RXDATA];
  uint key = cc[CC_RXKEY];

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_RXPKT].proc;

  proc (key, data);
}


INT_HANDLER rxpkt_queue (void)
{
  uint data = cc[CC_RXDATA];
  uint key = cc[CC_RXKEY];

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_RXPKT].proc;
  event_priority priority = (event_priority) sark_vec->event[EVENT_RXPKT].priority;

  event_queue_proc (proc, key, data, priority);

  vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


INT_HANDLER timer_null (void)
{
  tc[T1_INT_CLR] = (uint) tc;
  vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

  uint slot = sark_vec->event[EVENT_TIMER].slot;

  if (slot != SLOT_FIQ)
    vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER timer_irq (void)
{
  tc[T1_INT_CLR] = (uint) tc;
  vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

  event.ticks++;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_TIMER].proc;

  proc (event.ticks, 0);

  vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER timer_fiq (void)
{
  tc[T1_INT_CLR] = (uint) tc;
  vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

  event.ticks++;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_TIMER].proc;

  proc (event.ticks, 0);
}


INT_HANDLER timer_queue (void)
{
  tc[T1_INT_CLR] = (uint) tc;
  vic[VIC_SOFT_CLR] = 1 << TIMER1_INT;

  event.ticks++;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_TIMER].proc;
  event_priority priority = (event_priority) sark_vec->event[EVENT_TIMER].priority;

  event_queue_proc (proc, event.ticks, 0, priority);

  vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


INT_HANDLER sig_null (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

  uint slot = sark_vec->event[EVENT_SIG].slot;

  if (slot != SLOT_FIQ)
    vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER sig_irq (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SIG].proc;

  proc (event.signal, 0);

  vic[VIC_VADDR] = (uint) vic;
}


INT_HANDLER sig_fiq (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SIG].proc;

  proc (event.signal, 0);
}


INT_HANDLER sig_queue (void)
{
  vic[VIC_SOFT_CLR] = 1 << SARK_SIG_INT;

  event_proc proc = (event_proc) (uint) sark_vec->event[EVENT_SIG].proc;
  event_priority priority = (event_priority) sark_vec->event[EVENT_SIG].priority;

  event_queue_proc (proc, event.signal, 0, priority);

  vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


const int_handler fiq_events[] =
  {
    timer_fiq,
    rxpkt_fiq,
    sdp_fiq,
    user_fiq,
    sig_fiq
  };

const int_handler irq_events[] =
  {
    timer_irq,
    rxpkt_irq,
    sdp_irq,
    user_irq,
    sig_irq
  };

const int_handler queue_events[] =
  {
    timer_queue,
    rxpkt_queue,
    sdp_queue,
    user_queue,
    sig_queue
  };

const int_handler null_events[] =
  {
    timer_null,
    rxpkt_null,
    sdp_null,
    user_null,
    sig_null
  };

//------------------------------------------------------------------------------
