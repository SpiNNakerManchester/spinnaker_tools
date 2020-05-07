/*
 * Copyright (c) 2009-2019 The University of Manchester
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

//! \file
//! \brief Miscellaneous test registers

#ifndef SPINN_REGS_H
#define SPINN_REGS_H

uint reg_test(const uint *vec);

extern uint timer_test[];
extern uint cc_test[];
extern uint dma_test[];
extern uint vic_test[];

extern uint sdram_test[];

extern uint router_test[];
extern uint ether_test[];
extern uint wdog_test[];
extern uint sysctl_test[];
extern uint pl340_test[];

#endif
