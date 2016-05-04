/*
  ring.c - this program demonstrates core-to-core communication using
  multicast packets on a single chip. 16 application cores are
  arranged in a ring and a packet is sent from one to the next.

  A timed delay of 0.25 secs is used to slow the packet transmission
  so that it is visble and the reception of a packet causes a square
  to be updated on the "tubogrid" display.

  The User variables are used to track the execution of events.
*/


#include <sark.h>


uint core;	// Core ID of this core


// This "proc" is called when a scheduled timer delay elapses.  It
// puts a message in the print buffer for the core and sends a packet
// to the next core in the ring. It stops event processing when the
// count exceeds 128.

void timer_proc (uint count, uint none)
{
  sark.vcpu->user0++;

  io_printf (IO_BUF, "# timer_proc %d\n", count);

  if (count > 128)
    event_stop (0);
  else
    pkt_tx_kd (core, count + 1);
}


// This "proc" is called when a multicast packet is received. It sends
// a message to the "tubogrid" containing the count that came in the
// packet's payload. The background changes colour every 16 iterations.

void pkt_proc (uint key, uint data)
{
  sark.vcpu->user1++;

  char *colour = (data & 16) ? "blue" : "red";

  io_printf (IO_STD, "#%s;#fill;#white;%d\n", colour, data);

  timer_schedule_proc (timer_proc, data, 0, 250000);
}


// The main program prints a message to the core's IO buffer and then
// sets up a routing table entry so that packets which contain this
// core's ID in the key field are routed to the next core in the ring.
// Then it sets up callbacks for the elapsed timer and packet received
// events. Core 1 (only) places an initial call to the timer proc on
// the event queue. Finally, it starts event processing but requires a
// SYNC0 signal before operation commences.

void c_main (void)
{
  core = sark_core_id ();
  uint app_id = sark_app_id ();
  char *app_name = (char *) &sark.vcpu->app_name;

  // Say hello...

  io_printf (IO_BUF, "# App \"%s\" on core %d (AppID %d)\n",
	     app_name, core, app_id);

  // Initialise User variables

  sark.vcpu->user0 = sark.vcpu->user1 = 0;

  // Set up a single MC router entry to send packets to the next core
  // in the ring

  uint e = rtr_alloc (1);

  if (e == 0)
    rt_error (RTE_ABORT);

  uint next = (core == 16) ? 1 : core + 1;

  rtr_mc_set (e, core, 0xffffffff, MC_CORE_ROUTE (next));

  // Register the two events we're using and set up packet transmission queue

  event_register_queue (pkt_proc, EVENT_RXPKT, SLOT_0, PRIO_0);
  event_register_pkt (16, SLOT_1);
  event_register_timer (SLOT_2);

  // If we are core 1 then set things going by placing an event on the queue

  if (core == 1)
    event_queue_proc (timer_proc, 0, 0, PRIO_0);

  // Start event processing but wait for SYNC0

  uint rc = event_start (0, 0, SYNC_WAIT);

  // Print RC if we stop...

  io_printf (IO_BUF, "Terminated - RC %d\n", rc);
}
