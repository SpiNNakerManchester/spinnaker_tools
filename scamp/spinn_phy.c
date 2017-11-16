//------------------------------------------------------------------------------
//
// spinn_phy.c	    Spinnaker support routines for SMC PHY chip
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

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
