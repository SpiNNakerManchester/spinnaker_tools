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

//------------------------------------------------------------------------------
//! \file
//! \brief     Functions to handle division by 0
//!
//! \copyright &copy; The University of Manchester - 2017-2019
//!
//------------------------------------------------------------------------------

#include <sark.h>

// Functions to handle division by 0 (simply RTE)

//! Handles divide by zero by doing an RTE
void __aeabi_idiv0(void) {
    rt_error(RTE_DIV0);
}

//! Handles divide by zero by doing an RTE
void __aeabi_ldiv0(void) {
    rt_error(RTE_DIV0);
}
