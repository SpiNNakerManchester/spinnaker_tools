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

// Simple SpiNNaker application to demonstrate use of GDB

#include <spin1_api.h>

// Declare a struct type to contain data for this program

typedef struct my_data {
    uint count;
    uchar list[16];
    struct my_data *self;
} my_data_t;

// Declare an instance of the data struct. This is what we want to inspect
// with GDB. Placing many or all of the module's variables in a single
// struct is useful for debugging as we can display them all with a single
// command in GDB. The compiler is just as efficient at addressing these
// variables whether or not they are located in a struct.

my_data_t app_data;

// A timer function, called every 10ms. This copies the tick count into
// the data struct and updates one of the array elements with a new
// random number each time. It also sets the 'self' pointer to itself
// so that we can see this in GDB.

void timer_proc(uint ticks, uint arg2)
{
    app_data.count = ticks;
    app_data.list[ticks % 16] = spin1_rand();
    app_data.self = &app_data;
}

// Main function which sets up and runs the timer

void c_main(void)
{
    io_printf(IO_STD, ">> %s on %d \n", sark_app_name(), sark_core_id());

    spin1_callback_on(TIMER_TICK, timer_proc, 1);
    spin1_set_timer_tick(10 * 1000);
    spin1_start(SYNC_NOWAIT);
}
