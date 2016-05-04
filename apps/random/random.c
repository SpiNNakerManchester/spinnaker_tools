
/*
  This example program demonstrates the use of Tubogrid to get simple
  per-core animation. It also shows scheduled timer events and the use
  of SARK's event queueing.
*/


#include <sark.h>


// It's not safe to do output to IO_STD inside an interrupt routine
// so this application queues an io_printf call on the event queue
// which is processed as a foreground activity. "queue_proc" is the
// "event_proc" which is queued.


void queue_proc (uint colour, uint time)
{
  char *s = (colour) ? "white" : "black";	// Make a colour string

  io_printf (IO_STD, "#%s;#fill;#red;%d\n", s, time); // And print it
}


// This "event_proc" is called each time the timer elapses. It is
// effectively an interrupt handler. It flips colour (black/white)
// each time it is called and schedules a queued event to print the
// new colour to the Tubogrid. It also writes a message to an IO
// buffer and reschedules itself to occur again at some random time
// in the future.


void timer_proc (uint colour, uint b)
{
  uint rand = sark_rand () & 255;		// Get next random period

  colour = !colour;				// Flip the colour

  event_queue_proc (queue_proc, colour, rand, PRIO_0); // Queue printing

  io_printf (IO_BUF, "Timer next %d\n", rand);	// Do buffered printing

  // Reschedule ourselves again

  timer_schedule_proc (timer_proc, colour, 0, 10000 * (rand + 1));
}


// The main program finds out which core it is running on and prints
// a welcome message showing the core. Then it seeds the random
// number generator with a combination of chip ID, core ID and the
// time in milliseconds.

// Then it registers a timer event and places the first activation
// of the timer event on the event queue

// It then starts the event handling loop and, if that ever terminates,
// prints a closing message before exiting.


void c_main (void)
{
  uint core = sark_core_id ();				// Get core ID

  io_printf (IO_BUF, "Started core %d\n", core);	// and print it

  sark_srand ((sark_chip_id () << 8) + core * sv->time_ms); // Init randgen

  event_register_timer (SLOT_0);		// Set up the timer event mechanism

  event_queue_proc (timer_proc, 0, 0, PRIO_0); // Queue the first timer call

  uint rc = event_start (0, 0, SYNC_NOWAIT);	// Start event handling

  io_printf (IO_BUF, "Terminated rc %d\n", rc);	// Printed if event_stop used...
}
