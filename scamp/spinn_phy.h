//------------------------------------------------------------------------------
//
// spinn_phy.h      Spinnaker support routines for SMC PHY chip
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#/*
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

ifndef SPINN_PHY_H
#define SPINN_PHY_H

// PHY registers

#define PHY_CONTROL             0
#define PHY_STATUS              1
#define PHY_ID1                 2
#define PHY_ID2                 3
#define PHY_AUTO_ADV            4
#define PHY_AUTO_LPA            5
#define PHY_AUTO_EXP            6
#define PHY_SI_REV              16
#define PHY_MODE_CSR            17
#define PHY_SP_MODE             18
#define PHY_ERR_COUNT           26
#define PHY_SP_CSIR             27
#define PHY_INT_SRC             29
#define PHY_INT_MASK            30
#define PHY_SP_CSR              31


void phy_reset(void);
void phy_write(uint addr, uint data);
uint phy_read(uint addr);

#endif
