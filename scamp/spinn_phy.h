//------------------------------------------------------------------------------
//
// spinn_phy.h	    Spinnaker support routines for SMC PHY chip
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#ifndef SPINN_PHY_H
#define SPINN_PHY_H

// PHY registers

#define	PHY_CONTROL		0
#define	PHY_STATUS		1
#define	PHY_ID1			2
#define PHY_ID2			3
#define PHY_AUTO_ADV		4
#define PHY_AUTO_LPA		5
#define PHY_AUTO_EXP		6
#define PHY_SI_REV		16
#define PHY_MODE_CSR		17
#define PHY_SP_MODE		18
#define PHY_ERR_COUNT		26
#define PHY_SP_CSIR		27
#define PHY_INT_SRC		29
#define	PHY_INT_MASK		30
#define	PHY_SP_CSR		31


void phy_reset(void);
void phy_write(uint addr, uint data);
uint phy_read(uint addr);

#endif
