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

/*
  This program demonstrates a very simple bare-metal interrupt
  handler for SpiNNaker.
*/


#include <sark.h>

uint ticks;


// This is the interrupt handler. It needs to acknowledge the
// interrupt, do any interrupt processing (flip a LED and write a
// message) and finally tell the VIC that the routine is finished.

// Note that it's not normally a good idea to do "io_printf" inside
// an interrupt handler as it can be quite slow. This instance is
// OK because the interrupt is only occurring a couple of times
// per second.

INT_HANDLER timer_int_han(void)
{
    tc[T1_INT_CLR] = (uint) tc;                 // Clear interrupt in timer

    sark_led_set(LED_FLIP(1));                  // Flip a LED

    io_printf(IO_BUF, "Tick %d\n", ++ticks);    // Write message to buffer

    vic[VIC_VADDR] = (uint) vic;                // Tell VIC we're done
}


// This routine sets up the core's Timer1 hardware. It loads control
// registers to configure the timer and then sets up the VIC (vectored
// interrupt controller) to direct Timer1 interrupts to the handler
// above

void timer_setup(uint period)
{
    tc[T1_CONTROL] = 0xe2;                      // Set up count-down mode
    tc[T1_LOAD] = sark.cpu_clk * period;        // Load time in microsecs

    sark_vic_set(SLOT_0, TIMER1_INT, 1, timer_int_han); // Set VIC slot 0
}


// "c_main" calls the routine to set up the interrupt handler and
// then goes to sleep permanently, but still waking for the interrupt.

void c_main(void)
{
    io_printf(IO_STD, "Timer interrupt example\n");     // Say hello

    timer_setup(500000);                // This is 500,000 us (0.5 secs)

    cpu_sleep();                        // Send core to sleep
}
