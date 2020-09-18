//------------------------------------------------------------------------------
//
//! \file bmp_flash.c
//! \brief          Flash routines for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2013-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2013-2019 The University of Manchester
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


#include "lpc17xx.h"
#include "bmp.h"

//! (Subset of) commands that can be passed to lpc_iap()
enum bmp_lpc_flash_command {
    FLASH_PREPARE = 50,         //!< Flash prepare
    FLASH_COPY = 51,            //!< Flash copy
    FLASH_ERASE = 52,           //!< Flash erase
    FLASH_SERIAL = 58           //!< Flash get serial number
};


//------------------------------------------------------------------------------

//! \brief Check if an area of memory is blank (filled with `0xFF`)
//! \param[in] buf: The address of the area of memory
//! \param[in] len: The size of the area of memory, in bytes
//! \return True if all bytes are `0xFF`, false otherwise
uint32_t is_blank(const void *buf, uint32_t len)
{
    uint8_t *buffer = (uint8_t *) buf;

    for (uint32_t i = 0; i < len; i++) {
        if (buffer[i] != 0xff) {
            return 0;
        }
    }
    return 1;
}


//------------------------------------------------------------------------------

//! \brief Convert flash address to sector number (LPC17xx specific)
//! \details Sectors 0..15 are 4096 bytes and sectors 16..29 are 32768 bytes
//! \param[in] addr: Address in flash
//! \return Sector number
uint32_t flash_sector(uint32_t addr)
{
    if (addr < 0x10000) {
        return addr / 4096;
    } else {
        return 16 + (addr - 0x10000) / 32768;
    }
}


//------------------------------------------------------------------------------

//! \brief Erase sectors given start and end (byte) addresses
//! \param[in] start: Starting address
//! \param[in] end: Ending address
//! \return Result code
uint32_t flash_erase(uint32_t start, uint32_t end)
{
    uint32_t ss = flash_sector(start);
    uint32_t es = flash_sector(end);

    uint32_t iap_cmd[5];
    uint32_t iap_res[4];

    iap_cmd[0] = FLASH_PREPARE;
    iap_cmd[1] = ss;
    iap_cmd[2] = es;

    lpc_iap(iap_cmd, iap_res);

    if (iap_res[0] == 0) {
        iap_cmd[0] = FLASH_ERASE;
        iap_cmd[3] = CCLK / 1000;
        lpc_iap(iap_cmd, iap_res);
    }

    return iap_res[0];
}


//------------------------------------------------------------------------------

//! \brief Write to flash from supplied buffer. Only writes one sector
//! \param[in] addr: byte address in flash
//! \param[in] length: number of bytes to write
//! \param[in] buffer: data to write
//! \return Result code
uint32_t flash_write(uint32_t addr, uint32_t length, const uint32_t *buffer)
{
    uint32_t ss = flash_sector(addr);

    uint32_t iap_cmd[5];
    uint32_t iap_res[4];

    iap_cmd[0] = FLASH_PREPARE;
    iap_cmd[1] = ss;
    iap_cmd[2] = ss;

    lpc_iap(iap_cmd, iap_res);

    if (iap_res[0] == 0) {
        iap_cmd[0] = FLASH_COPY;
        iap_cmd[1] = addr;
        iap_cmd[2] = (uint32_t) buffer;
        iap_cmd[3] = length;
        iap_cmd[4] = CCLK / 1000;

        lpc_iap(iap_cmd, iap_res);
    }

    return iap_res[0];
}

//------------------------------------------------------------------------------
