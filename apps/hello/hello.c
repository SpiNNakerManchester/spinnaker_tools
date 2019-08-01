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
  Simple Hello World program which writes output to two different
  places. One is IO_STD, which should appear on a "tubotron" viewer.
  The other is IO_BUF which goes to a memory buffer on the SpiNNaker
  chip and can be viewed with the "iobuf" command in "ybug"
*/


#include <sark.h>


void c_main(void)
{
    char buf[64];

    io_printf(IO_STD, "Hello world! (via SDP)\n");

    io_printf(IO_BUF, "Hello world! (via SDRAM)\n");

    // io_printf can also do sprintf!

    io_printf(buf, "Hello world! (via printf...)\n");
    io_printf(IO_BUF, buf);
}
