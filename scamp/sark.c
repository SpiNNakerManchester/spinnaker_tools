/*
 * Copyright (c) 2009 The University of Manchester
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
//! \brief Main entry point for SARK
#include "sark.h"

//! \brief Main entry point.
//!
//! \note user application code will supply their own version of this;
//! this version is just for raw SARK.
void c_main(void)
{
    sark_cpu_state(CPU_STATE_IDLE);
    cpu_sleep();
}
