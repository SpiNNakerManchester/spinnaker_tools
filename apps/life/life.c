/*
  life.c - written by Steve Furber - Aug 2013

  This example program demonstrates the use of Tubogrid to get simple
  per-core animation. It implements Conway's Life on an XMAX x YMAX
  chip array grid with 4*XMAX x 4*YMAX cells.

  The only tricky part of the program is setting up the Router tables
  - everything thereafter is pretty simple!
  Each core sends a packet to each of its 8 neighbours every time it
  recomputes its state (whether or not the state changes).
  The update time is skewed from core to core to reduce SDP packet
  contention, so a generation "parity" bit is used to keep the overall
  model synchronous.
  Connections to on-chip neighbours are direct, but when a core is on
  the edge of the 4x4 array on a chip it must communicate with neighbours
  on one or more other chips. The core routes its packet to horizontal
  and vertical neighbours as required, where the corresponding core
  sets up the route to the relevant cores on that chip.
  For example, a packet from core (3,3) on chip (0,0) needs to go to:
  - cores (2,2), (3,2) and (2,3) on chip(0.0) [core(3,3) on chip(0,0)]
  - cores (2,0) and (3,0) on chip(0,1)        [core(3,3) on chip(0,1)]
  - cores (0,2) and (0,3) on chip(1,0)        [core(3,3) on chip(1,0)]
  - core (0,0) on chip(1,1)                   [core(3,3) on chip(1,1)]
  Here the core responsible for local routing is indicated in [..].
  Note that the diagonal connection to chip(1,1) is implemented via
  chip(1,0) - the packet is routed horizontally and then vertically as
  not all diagonal paths are available, so none are used.
*/

#include <spin1_api.h>

#define XMAX 2							// chip array width in chips
#define YMAX 2							// chip array height in chips
#define TOROID FALSE						// chip array = toroid?

#define EAST 1							// inter-chip link definitions
#define NORTH 4							// NB diagonal links not used!
#define WEST 8
#define SOUTH 32

uint alive;							// this cell's state
uint last_alive = 2;						// previous state
uint count [2];							// live neighbour count [for each gen]
uint gen;							// generation (used for parity)
uint myKey;							// key for sending


void tick_callback (uint ticks, uint dummy)
{
  if (ticks % 250 == (myKey & 255))				// Every 0.25 secs, time skewed for SDP
  {
    alive = (count[gen] | alive) == 3;				// Life automaton rule
    count[gen] = 0;						// clear count for next gen
    gen = !gen;							// onto next generation

    spin1_send_mc_packet (myKey, gen+(alive<<1), WITH_PAYLOAD);	// send state to neighbours

    if (alive != last_alive)
      {
	char *s = (alive) ? "white" : "black";			// Make a colour string
	io_printf (IO_STD, "#%s;#fill;\n", s);        		// And print it
	last_alive = alive;
      }
  }
}


void packet_in (uint key, uint data)				// data = gen + 2*alive
{
  count [data & 1] += data >> 1;				// count[gen mod 2] += alive
}


uint toH []   = {WEST, EAST};					// edge directions
uint toV []   = {SOUTH, NORTH};					// to & from, Horizontal & Vertical
uint fromH [] = {EAST, WEST};
uint fromV [] = {NORTH, SOUTH};

uint route = 0;							// route bit vector
int x, y;							// core x, y coordinates


uint edge (uint z)						// check for core at edge of chip
{
  return (z == 0) || (z == 3);
}


void add_route_to_core (int i, int j)				// filters out off-chip neighbours
{
  if (i >- 1 && i < 4  && j >- 1 && j < 4			// check for end of row & col		
      && !(i == x && j == y))
    route += 1<<(i+4*j+7);					// don't route to self
}


uint next_chip (uint chip, uint dir)				// find next chip, modulo array size
{
  uint cX = chip >> 8;
  uint cY = chip & 255;

  // do the modulo sums

  if (dir & SOUTH)
    cY = cY ? cY - 1 : YMAX - 1;
  if (dir & NORTH)
    cY = (cY + 1) % YMAX;
  if (dir & WEST)
    cX = cX ? cX - 1 : XMAX - 1;
  if (dir & EAST)
    cX = (cX + 1) % XMAX;

  return (cX << 8) + cY;
}


void add_route_to_chip (uint chip, uint dir)			// dir specifies possible directions
{
  if (TOROID)
    route += dir;
  else
  {
    uint nc = next_chip (chip, dir);				// toroidal routing on...
    uint cX  = chip >> 8;
    uint cY  = chip & 255;			// ...non-toroidal system
    uint ncX = nc >> 8;
    uint ncY = nc & 255;			// uses default routing to get back

    if (cX != ncX)
      route += toH[ncX > cX];	

    if (cY != ncY)
      route += toV[ncY > cY];		
  }
}


void set_mc_table_entry (uint chip, uint core)			// route from (chip, core)
{
  uint e = rtr_alloc (1);

  if (e == 0)
    rt_error (RTE_ABORT);

  rtr_mc_set (e, (chip << 8) + core, 0xffffffff, route);
  route = 0;							// clear route for next use
}


void set_up_routing_tables (uint chip, uint core)		// this is the tricky bit!
{
  x = (core - 1) & 3;						// core x, y coordinates
  y = (core - 1) >> 2;

// first set up routes for internally-generated packets

  for (int i = x - 1; i < x + 2; i++)				// column left, same, right
    for (int j = y - 1; j < y + 2; j++)
      add_route_to_core (i, j);					// row below, same, above

  if (edge (x))
    add_route_to_chip (chip, toH[x & 1]);			// send off-chip where appropriate

  if (edge (y))
    add_route_to_chip (chip, toV[y & 1]);

  set_mc_table_entry (chip, core);				// write local Router table entry

// then handle incoming packets from other chips...
// ...each core handles routes from corresponding cores on other chips
 
  if (edge (y))							// top & bottom
  {
    for (int i = x - 1; i < x + 2; i++)				// route to opposite row
      add_route_to_core (i, 3 - y);

    if (edge (x))
      add_route_to_chip (chip, toH[x & 1]);			// forward diagonal connection

    set_mc_table_entry (next_chip (chip, fromV[y & 1]), core);
  }
	
  if (edge (x))							// left & right
  {
    for (int j = y - 1; j < y + 2; j++)				// route to opposite col
      add_route_to_core (3 - x, j);

    set_mc_table_entry (next_chip (chip, fromH[x & 1]), core);
  }

  if (edge (x) && edge (y))					// incoming diagonal connection
  {
    add_route_to_core (3 - x, 3 - y);				// receive from opposite corner
    set_mc_table_entry (next_chip (chip, fromH[x & 1] + fromV[y & 1]), core);
  }
}


void init_Life_state (uint chip, uint core)
{
  if (chip == 0 && (core == 5 || core == 6 || core == 7
    || core == 11 || core == 14)) count[0] = 3;			// initial state
}


void c_main (void)
{
  uint chip = spin1_get_chip_id ();				// get chip ID
  uint core = spin1_get_core_id ();				// ...& core ID
  myKey = (chip << 8) + core;					// key for my output packets

  if ((chip >> 8) < XMAX && (chip & 255) < YMAX)		// only start required cores
  {
    io_printf (IO_BUF, "Started core %d %d %d\n",
	  	chip >> 8, chip & 255, core);			// signal core running

    set_up_routing_tables (chip, core);				// configure routing
    init_Life_state (chip, core);				// Initialise Life state

    spin1_set_timer_tick (1000);				// 1ms timer tick
    spin1_callback_on (TIMER_TICK, tick_callback, 1);		// timer callback
    spin1_callback_on (MCPL_PACKET_RECEIVED, packet_in, -1);	// incoming packet callback

    spin1_start (SYNC_NOWAIT);					// start event-driven operation

    io_printf (IO_BUF, "Terminated core %d %d %d\n",
		chip >> 8, chip & 255, core);			// print if event_exit used...
  }
}
