/*
 * Copyright (c) 2019 The University of Manchester
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

//! Clock drift fixed-point number definition in terms of the number of
//! fractional bits and the mask to get the fractional bits
#define DRIFT_FP_BITS 17U
#define DRIFT_INT_MASK (((1U << (32 - DRIFT_FP_BITS)) - 1) << DRIFT_FP_BITS)
#define DRIFT_FRAC_MASK ((1U << DRIFT_FP_BITS) - 1)
#define DRIFT_ONE (1U << DRIFT_FP_BITS)
#define TIME_BETWEEN_SYNC_US 2000000U // !< The time between sync signals in us
//! For some calculations a signed int is better
static const int DRIFT_FP_FACTOR = 1 << DRIFT_FP_BITS;
static const int TIME_BETWEEN_SYNC_INT = TIME_BETWEEN_SYNC_US;
static const int DRIFT_HALF = 1 << (DRIFT_FP_BITS - 1);
