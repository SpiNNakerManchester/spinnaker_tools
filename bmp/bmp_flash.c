//------------------------------------------------------------------------------
//
// bmp_flash.c	    Flash routines for BMP LPC1768
//
// Copyright (C)    The University of Manchester - 2013-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


#include "lpc17xx.h"
#include "bmp.h"


//------------------------------------------------------------------------------

// Check if an area of (flash) memory is blank

uint32_t is_blank(void *buf, uint32_t len)
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

// Convert flash address to sector number (LPC17xx specific)

// Sectors 0..15 are 4096 bytes and sectors 16..29 are 32768 bytes

uint32_t flash_sector(uint32_t addr)
{
    if (addr < 0x10000) {
	return addr / 4096;
    } else {
	return 16 + (addr - 0x10000) / 32768;
    }
}


//------------------------------------------------------------------------------

// Erase sectors given start and end (byte) addresses

uint32_t flash_erase(uint32_t start, uint32_t end)
{
    uint32_t ss = flash_sector(start);
    uint32_t es = flash_sector(end);

    uint32_t iap_cmd[5];
    uint32_t iap_res[4];

    iap_cmd[0] = 50;			// Flash prepare
    iap_cmd[1] = ss;
    iap_cmd[2] = es;

    lpc_iap(iap_cmd, iap_res);

    if (iap_res[0] == 0) {
	iap_cmd[0] = 52;		// Flash erase
	iap_cmd[3] = CCLK / 1000;
	lpc_iap(iap_cmd, iap_res);
    }

    return iap_res[0];
}


//------------------------------------------------------------------------------

// Write to flash from supplied buffer
//
// "addr" is a byte address, "buf" points to uint32, "length" is in bytes

uint32_t flash_write(uint32_t addr, uint32_t length, uint32_t *buffer)
{
    uint32_t ss = flash_sector(addr);

    uint32_t iap_cmd[5];
    uint32_t iap_res[4];

    iap_cmd[0] = 50;			// Flash prepare
    iap_cmd[1] = ss;
    iap_cmd[2] = ss;

    lpc_iap(iap_cmd, iap_res);

    if (iap_res[0] == 0) {
	iap_cmd[0] = 51;		// Flash copy
	iap_cmd[1] = addr;
	iap_cmd[2] = (uint32_t) buffer;
	iap_cmd[3] = length;
	iap_cmd[4] = CCLK / 1000;

	lpc_iap(iap_cmd, iap_res);
    }

    return iap_res[0];
}

//------------------------------------------------------------------------------
