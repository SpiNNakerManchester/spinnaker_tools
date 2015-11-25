
//------------------------------------------------------------------------------
//
// sark_pkt.c	    Packet handling routines for SARK
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#include <sark.h>

//------------------------------------------------------------------------------


extern INT_HANDLER txpkt_int_han (void);

void event_register_pkt (uint queue_size, vic_slot slot)
{
  if (queue_size <= 256 &&
      (queue_size & (queue_size - 1)) == 0 &&
      slot <= SLOT_MAX &&
      vic[VIC_CNTL + slot] == 0)
    {
      event.pkt_queue = sark_alloc (queue_size, sizeof (pkt_t));

      if (event.pkt_queue != NULL)
	{
	  event.pkt_insert = 1;
	  event.pkt_size = queue_size;

	  sark_vic_set (slot, CC_TNF_INT, 0, txpkt_int_han);

	  cc[CC_TCR] = PKT_MC;

	  return;
	}
    }

  rt_error (RTE_PKT);
}


//------------------------------------------------------------------------------

// Transmit a packet (4 variants)


uint pkt_tx_kdc (uint key, uint data, uint ctrl)
{
  pkt_t pkt = {(ctrl << 16) + 3, data, key};

  uint cpsr = cpu_int_disable ();

  if (event.pkt_count >= event.pkt_size)
    {
      cpu_int_restore (cpsr);
      return 0;
    }

  if (event.pkt_count == 0)
    vic[VIC_ENABLE] = 1 << CC_TNF_INT;

  event.pkt_count++;

  if (event.pkt_count > event.pkt_max)
    event.pkt_max = event.pkt_count;

  event.pkt_queue[event.pkt_insert] = pkt;
  event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

  cpu_int_restore (cpsr);

  return 1;
}


uint pkt_tx_kd (uint key, uint data)
{
  pkt_t pkt = {2, data, key};

  uint cpsr = cpu_int_disable ();

  if (event.pkt_count >= event.pkt_size)
    {
      cpu_int_restore (cpsr);
      return 0;
    }

  if (event.pkt_count == 0)
    vic[VIC_ENABLE] = 1 << CC_TNF_INT;

  event.pkt_count++;

  if (event.pkt_count > event.pkt_max)
    event.pkt_max = event.pkt_count;

  event.pkt_queue[event.pkt_insert] = pkt;
  event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

  cpu_int_restore (cpsr);

  return 1;
}


uint pkt_tx_k (uint key)
{
  pkt_t pkt = {0, 0, key};

  uint cpsr = cpu_int_disable ();

  if (event.pkt_count >= event.pkt_size)
    {
      cpu_int_restore (cpsr);
      return 0;
    }

  if (event.pkt_count == 0)
    vic[VIC_ENABLE] = 1 << CC_TNF_INT;

  event.pkt_count++;

  if (event.pkt_count > event.pkt_max)
    event.pkt_max = event.pkt_count;

  event.pkt_queue[event.pkt_insert] = pkt;
  event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

  cpu_int_restore (cpsr);

  return 1;
}


uint pkt_tx_kc (uint key, uint ctrl)
{
  pkt_t pkt = {(ctrl << 16) + 1, 0, key};

  uint cpsr = cpu_int_disable ();

  if (event.pkt_count >= event.pkt_size)
    {
      cpu_int_restore (cpsr);
      return 0;
    }

  if (event.pkt_count == 0)
    vic[VIC_ENABLE] = 1 << CC_TNF_INT;

  event.pkt_count++;

  if (event.pkt_count > event.pkt_max)
    event.pkt_max = event.pkt_count;

  event.pkt_queue[event.pkt_insert] = pkt;
  event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

  cpu_int_restore (cpsr);

  return 1;
}


//------------------------------------------------------------------------------
