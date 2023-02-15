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

#include "spinnaker.h"
#include "sark.h"

void phy_reset(void)
{
    er[ETH_PHY_CTRL] = 0;
    sark_delay_us(10);
    er[ETH_PHY_CTRL] = PHY_CTRL_NRST;
}

//! \brief Sends bits to the PHY chip via the SMC
//! \param[in] data: the data to move, in the _upper_ bits
//! 	(critical when fewer than 32 bits being moved)
//! \param[in] len: the number of bits to move (up to 32)
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

//! \brief Receive some bits from the PHY chip via the SMC
//! \param[in] len: the number of bits to receive (up to 32)
//! \return The received data, in the _low_ bits
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
