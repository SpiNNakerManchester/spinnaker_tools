/****a* sdping.c/sdping_summary
*
* SUMMARY
*  sample SpiNNaker SDP application
*
* AUTHOR
*  Steve Temple - temples@cs.man.ac.uk
*
* DETAILS
*  Created on       : 03 May 2011
*  Version          : $Revision: 1196 $
*  Last modified on : $Date: 2011-06-27 14:32:29 +0100 (Mon, 27 Jun 2011) $
*  Last modified by : $Author: plana $
*  $Id: simple.c 1196 2011-06-27 13:32:29Z plana $
*  $HeadURL: file:///home/amulinks/spinnaker/svn/spinn_api/trunk/examples/simple.c $
*
* COPYRIGHT
*  Copyright (c) The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
*******/


#include <spin1_api.h>


// Swap source and dest fields of an SDP packet to return to sender

void swap_sdp_hdr (sdp_msg_t *msg)
{
  uint dest_port = msg->dest_port;
  uint dest_addr = msg->dest_addr;

  msg->dest_port = msg->srce_port;
  msg->srce_port = dest_port;

  msg->dest_addr = msg->srce_addr;
  msg->srce_addr = dest_addr;
}


// Callback for SDP message received - 
// Print the message via Tubotron then...
//   Port 1 - return to sender
//   Port 2 - return to IPTag 1
//   Port 7 - exit API
// Note that the message buffer must be freed here.

void process_sdp (uint m, uint port)
{
  sdp_msg_t *msg = (sdp_msg_t *) m;

  io_printf (IO_STD, "SDP len %d, port %d - %s\n", msg->length, port, msg->data);

  if (port == 1) // Port 1 - echo to sender
    {
      swap_sdp_hdr (msg);
      (void) spin1_send_sdp_msg (msg, 10);
    }
  else if (port == 2) // Port 2 - send via IPTAG 1
    {
      msg->tag = 1;				// IPTag 1
      msg->dest_port = PORT_ETH;		// Ethernet
      msg->dest_addr = sv->dbg_addr; 		// Root chip
      msg->flags = 0x07;			// Flags = 7
      msg->srce_port = spin1_get_core_id ();	// Source port
      msg->srce_addr = spin1_get_chip_id ();	// Source addr

      (void) spin1_send_sdp_msg (msg, 10);
    }

  spin1_msg_free (msg);

  if (port == 7)
    spin1_exit (0);
}


// "flip_led" is the timer callback.

void flip_led (uint ticks, uint null)
{
  spin1_led_control (LED_INV (1));
}


// Main program just sets up callbacks and then starts the API

void c_main ()
{
  io_printf (IO_STD, ">> sdping\n");

  spin1_set_timer_tick (1000000);

  spin1_callback_on (TIMER_TICK, flip_led, 3);

  spin1_callback_on (SDP_PACKET_RX, process_sdp, 2);

  spin1_start (SYNC_NOWAIT);

  io_printf (IO_STD, ">> sdping done\n");
}
