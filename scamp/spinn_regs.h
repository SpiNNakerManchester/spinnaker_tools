/*
 * Copyright (c) 2009-2023 The University of Manchester
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

//! \file
//! \brief Miscellaneous test registers
//! \deprecated This file seems to be be unused.

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
