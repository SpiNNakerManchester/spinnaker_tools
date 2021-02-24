/*
 * Copyright (c) 2019-2020 The University of Manchester
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
