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

#include <sark.h>

void c_main(void)
{
    uint *rom = (uint *) ROM_BASE;
    uint data = 0x55aa55aa;

    io_printf(IO_STD, "Reading ROM at %08x = %08x\n", (uint) rom, rom[0]);
    io_printf(IO_STD, "Writing ROM at %08x > %08x\n", (uint) rom, data);

    rom[0] = data;

    io_printf(IO_STD, "Reading ROM at %08x = %08x\n", (uint) rom, rom[0]);
}
