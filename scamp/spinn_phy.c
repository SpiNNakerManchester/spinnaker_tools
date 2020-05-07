//------------------------------------------------------------------------------
//! \file
//! \brief     Spinnaker support routines for SMC PHY chip
//!
//! \copyright &copy; The University of Manchester - 2009, 2010
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

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

#include "spinnaker.h"
#include "sark.h"


void phy_reset()
{
    er[ETH_PHY_CTRL] = 0;
    sark_delay_us(10);
    er[ETH_PHY_CTRL] = PHY_CTRL_NRST;
}


static void phy_shift_out(uint data, uint len)
{
    uint c = er[ETH_PHY_CTRL];

    while (len--) {
        if (data & 0x80000000) {
            c |= PHY_CTRL_DOUT;
        } else {
            c &= ~PHY_CTRL_DOUT;
        }

        er[ETH_PHY_CTRL] = c;
        sark_delay_us(1);
        er[ETH_PHY_CTRL] = c | PHY_CTRL_CLK;
        sark_delay_us(1);
        er[ETH_PHY_CTRL] = c;

        data = data << 1;
    }
}


static uint phy_shift_in(uint len)
{
    uint c = er[ETH_PHY_CTRL];
    uint r = 0;

    while (len--) {
        sark_delay_us(1);
        r = r << 1;
        if (er[ETH_PHY_CTRL] & PHY_CTRL_DIN) {
            r |= 1;
        }
        er[ETH_PHY_CTRL] = c | PHY_CTRL_CLK;
        sark_delay_us(1);
        er[ETH_PHY_CTRL] = c;
    }

    return r;
}


void phy_write(uint addr, uint data)
{
    uint c = er[ETH_PHY_CTRL];

    data = 0x50020000 | (addr << 18) | data;
    er[ETH_PHY_CTRL] = c | PHY_CTRL_OE;

    phy_shift_out(0xffffffff, 32);
    phy_shift_out(data, 32);

    er[ETH_PHY_CTRL] = c;
}


uint phy_read(uint addr)
{
    uint c = er[ETH_PHY_CTRL];

    er[ETH_PHY_CTRL] = c | PHY_CTRL_OE;

    phy_shift_out(0xffffffff, 32);
    phy_shift_out(0x60000000 | (addr << 18), 14);

    er[ETH_PHY_CTRL] = c;

    phy_shift_out(0, 2);

    return phy_shift_in(16);
}
