/*
 * Copyright (c) 2017-2023 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
