/*
 * Copyright (c) 2016-2023 The University of Manchester
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
