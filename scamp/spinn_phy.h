//------------------------------------------------------------------------------
//! \file
//! \brief     Spinnaker support routines for SMC PHY (physical layer ethernet) chip
//!
//! \copyright &copy; The University of Manchester - 2009-2019
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009-2023 The University of Manchester
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

#ifndef SPINN_PHY_H
#define SPINN_PHY_H

//! \brief PHY registers
//! \details Documentation guessed based on
//! 	https://github.com/goertzenator/smsc7500/blob/master/smsclan7500.h
enum spinn_phy_registers {
    PHY_CONTROL =           0,  //!< Control register
    PHY_STATUS =            1,  //!< Status register
    PHY_ID1 =               2,  //!< PHY identifier 1 register
    PHY_ID2 =               3,  //!< PHY identifier 2 register
    PHY_AUTO_ADV =          4,  //!< Auto Negotiation Advertisement register
    PHY_AUTO_LPA =          5,  //!< Auto Negotiation Link Partner register
    PHY_AUTO_EXP =          6,  //!< Auto Negotiation Expansion register
    PHY_SI_REV =            16, // !< WTF is this?
    PHY_MODE_CSR =          17, //!< 10/100 Mode Control/Status register
    PHY_SP_MODE =           18, //!< 10/100 Special Modes register
    PHY_ERR_COUNT =         26, //!< Error count register
    PHY_SP_CSIR =           27, //!< Special Control/Status Indication register
    PHY_INT_SRC =           29, //!< Interrupt source register
    PHY_INT_MASK =          30, //!< Interrupt mask register
    PHY_SP_CSR =            31, //!< Special Control/Status register
};

//! Reset the PHY chip
void phy_reset(void);
//! \brief Write to the PHY chip
//! \param[in] addr: The register to write to
//! \param[in] data: The value to write (really a ushort)
void phy_write(uint addr, uint data);
//! \brief Read from the PHY chip
//! \param[in] addr: The register to read from
//! \return The read value (really a ushort)
uint phy_read(uint addr);

#endif
