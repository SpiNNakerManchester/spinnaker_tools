//------------------------------------------------------------------------------
//
// bmp_ssp.c	    SSP peripheral handling for BC&MP
//
// Copyright (C)    The University of Manchester - 2012-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


#include "lpc17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_ssp.h"

#include "bmp.h"

//------------------------------------------------------------------------------

// SSP1 is connected to the FPGAs and is used to load their bitmaps. It
// can also be used to communicate with the FPGAs when the FPGA code supports
// this function.

// SSP0 is connected to a serial Flash device which currently holds two areas
// of data. The area at 0 is the SpiNNaker IP address block which is read by
// the root chip when it is reset. A buffer and software multiplexer dual-port
// the serial Flash. It can currently only be written by the BMP.
// The second area holds the FPGA bitmap and starts at 0x10000. This is loaded
// whenever power is applied to the FPGAs

//------------------------------------------------------------------------------

static void delay(uint32_t n)		// 30ns per loop?
{
    while (n--) {
	continue;
    }
}

void ssp1_copy(uint32_t count, uint8_t *buf)
{
    while (count--) {
	while ((LPC_SSP1->SR & 2) == 0)	{// Loop while full
	    continue;
	}

	LPC_SSP1->DR = *buf++;
    }
}

//------------------------------------------------------------------------------

void fpga_word(uint32_t addr, uint32_t fpga, uint32_t *buf, uint32_t dir)
{
    // Assert select

    if (fpga == 0) {
	LPC_GPIO3->FIOCLR = XFSEL_0;
    } else if (fpga == 1) {
	LPC_GPIO3->FIOCLR = XFSEL_1;
    } else {
	LPC_GPIO4->FIOCLR = XFSEL_2;
    }
    delay(10);

    // Clean up address, add write bit, get data
    uint32_t data = 0;
    addr = addr & ~3;

    if (dir == FPGA_WRITE) {
	addr += 1;
	data = *buf;
    }

    // Wait until TxFIFO empties & flush RxFIFO
    while ((LPC_SSP1->SR & 1) == 0) {
	continue;
    }
    while ((LPC_SSP1->SR & 4) != 0) {
	(void) LPC_SSP1->DR;
    }

    // Send address word
    for (uint32_t i = 0; i < 4; i++) {
	LPC_SSP1->DR = addr >> 24;
	addr <<= 8;
    }

    // and data word (or zero)
    for (uint32_t i = 0; i < 4; i++) {
	LPC_SSP1->DR = data >> 24;
	data <<= 8;
    }

    // Wait until TxFIFO empties
    while ((LPC_SSP1->SR & 1) == 0) {
	continue;
    }

    // Throw away returned address
    for (uint32_t i = 0; i < 4; i++) {
	while ((LPC_SSP1->SR & 4) == 0)	{	// Wait for NotEmpty
	    continue;
	}
	(void) LPC_SSP1->DR;
    }

    // Read returned data
    for (uint32_t i = 0; i < 4; i++) {
	while ((LPC_SSP1->SR & 4) == 0) {	// Wait for NotEmpty
	    continue;
	}
	data <<= 8;
	data |= LPC_SSP1->DR;
    }

    if (dir == FPGA_READ) {
	*buf = data;
    }
    delay(30);

    LPC_GPIO3->FIOSET = XFSEL_0 + XFSEL_1;
    LPC_GPIO4->FIOSET = XFSEL_2;
}

//------------------------------------------------------------------------------

static void ssp0_send(uint32_t d)
{
    while ((LPC_SSP0->SR & 2) == 0) {	// Loop while full
	continue;
    }
    LPC_SSP0->DR = d;
}

void ssp0_write(uint32_t cmd, uint32_t addr, uint32_t len, uint8_t *buf)
{
    LPC_GPIO0->FIOCLR = SF_NCS;		// Assert NCS

    ssp0_send(0x06);			// Sent write enable

    while ((LPC_SSP0->SR & 1) == 0) {	// Wait until TxFIFO empties
	continue;
    }

    delay(10);
    LPC_GPIO0->FIOSET = SF_NCS;		// Deassert NCS

    delay(10);				// Ensure NCS high for a while
    LPC_GPIO0->FIOCLR = SF_NCS;		// Assert NCS

    ssp0_send(cmd);

    if (addr != 0xffffffff) {
	ssp0_send(addr >> 16);
	ssp0_send(addr >> 8);
	ssp0_send(addr);
    }

    while (len--) {
	while ((LPC_SSP0->SR & 2) == 0)	{ // Loop while full
	    continue;
        }
	LPC_SSP0->DR = *buf++;
    }

    while ((LPC_SSP0->SR & 1) == 0) {	// Wait until TxFIFO empties
	continue;
    }
    delay(10);
    LPC_GPIO0->FIOSET = SF_NCS;		// Deassert NCS
}

void ssp0_read(uint32_t cmd, uint32_t addr, uint32_t len, uint8_t *buf)
{
    LPC_GPIO0->FIOCLR = SF_NCS;		// Assert NCS

    ssp0_send(cmd);

    if (addr != 0xffffffff) {
	ssp0_send(addr >> 16);
	ssp0_send(addr >> 8);
	ssp0_send(addr);
    }

    while ((LPC_SSP0->SR & 1) == 0) {	// Wait until TxFIFO empties
	continue;
    }
    while ((LPC_SSP0->SR & 4) != 0) {	// Flush RxFIFO
	(void) LPC_SSP0->DR;
    }
    while (len--) {
	LPC_SSP0->DR = 0;		// Trigger read

	while ((LPC_SSP0->SR & 4) == 0) { // Loop while empty
	    continue;
        }
	*buf++ = LPC_SSP0->DR;
    }

    while ((LPC_SSP0->SR & 1) == 0) {	// Wait until TxFIFO empties
	continue;
    }
    delay(10);
    LPC_GPIO0->FIOSET = SF_NCS;		// Deassert NCS
}

//------------------------------------------------------------------------------

// Copy from Serial Flash on SSP0 to FPGA(s) on SSP1

void ssp0_copy(uint32_t addr, uint32_t len)
{
    LPC_GPIO0->FIOCLR = SF_NCS;		// Assert NCS

    ssp0_send(0x03);

    ssp0_send(addr >> 16);
    ssp0_send(addr >> 8);
    ssp0_send(addr);

    while ((LPC_SSP0->SR & 1) == 0) {	// Wait until TxFIFO empties
	continue;
    }
    while ((LPC_SSP0->SR & 4) != 0) {	// Flush RxFIFO
	(void) LPC_SSP0->DR;
    }
    while (len--) {
	LPC_SSP0->DR = 0;		// Trigger read

	while ((LPC_SSP0->SR & 4) == 0) { // Loop while empty
	    continue;
        }
	LPC_SSP1->DR = LPC_SSP0->DR;	// Copy to SSP1
    }

    while ((LPC_SSP0->SR & 1) == 0) {	// Wait until TxFIFO empties
	continue;
    }
    delay(10);
    LPC_GPIO0->FIOSET = SF_NCS;		// Deassert NCS
}

//------------------------------------------------------------------------------

void sf_write(uint32_t addr, uint32_t len, uint8_t *buf)
{
    uint8_t status[4];

    while (len != 0) {
	if ((addr & 0xfff) == 0) {		// On 4K boundary?
	    ssp0_write(0x39, addr, 0, NULL);	// Unprotect sector
	    ssp0_write(0x20, addr, 0, NULL);	// Erase 4K sector

	    do {
		ssp0_read(0x05, 0xffffffff, 4, status);
	    } while (status[0] & 1);
	}

	uint32_t bytes = (len > 256) ? 256 : len;
	ssp0_write(0x02, addr, bytes, buf);

	do {
	    ssp0_read(0x05, 0xffffffff, 4, status);
	} while (status[0] & 1);
	len -= bytes;
	addr += bytes;
	buf += bytes;
    }
}

void sf_read(uint32_t addr, uint32_t len, uint8_t *buf)
{
    ssp0_read(0x03, addr, len, buf);
}

uint32_t sf_crc32(uint32_t addr, uint32_t len)
{
    uint8_t *buf = (uint8_t *) flash_buf;
    uint32_t crc = 0xffffffff;

    while (len != 0) {
	uint32_t size = (len > FLASH_BYTES) ? FLASH_BYTES : len;
	ssp0_read(0x03, addr, size, buf);
	crc = crc32(buf, size, crc);
	len -= size;
	addr += size;
    }

    return ~crc;
}

//------------------------------------------------------------------------------

#define SSP0_CLK   25000000	// 25 MHz
#define SSP1_CLK_H 25000000	// 25 MHz
#define SSP1_CLK_L  5000000	//  5 MHz

void ssp1_slow(void)
{
    LPC_SSP1->CR1 &= ~SSP_CR1_SSP_EN;

    LPC_SSP1->CPSR = (CCLK / SSP1_CLK_L) & 0xfe;

    LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;
}


void ssp1_fast(void)
{
    LPC_SSP1->CR1 &= ~SSP_CR1_SSP_EN;

    LPC_SSP1->CPSR = (CCLK / SSP1_CLK_H) & 0xfe;

    LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;
}

//------------------------------------------------------------------------------

void configure_ssp(void)
{
    // SSP0 - Serial Flash

    clock_div(CLKPWR_PCLKSEL_SSP0, CLKPWR_PCLKSEL_CCLK_DIV_1);

    LPC_SSP0->CR0 = SSP_CPHA_SECOND | SSP_CPOL_LO | SSP_FRAME_SPI | SSP_DATABIT_8;
    LPC_SSP0->CR1 = SSP_MASTER_MODE;
    LPC_SSP0->CPSR = (CCLK / SSP0_CLK) & 0xfe;
    LPC_SSP0->CR1 |= SSP_CR1_SSP_EN;

    // SSP1 - FPGAs

    clock_div(CLKPWR_PCLKSEL_SSP1, CLKPWR_PCLKSEL_CCLK_DIV_1);

    LPC_SSP1->CR0 = SSP_CPHA_FIRST | SSP_CPOL_HI | SSP_FRAME_SPI | SSP_DATABIT_8;
    LPC_SSP1->CR1 = SSP_MASTER_MODE;
    LPC_SSP1->CPSR = (CCLK / SSP1_CLK_H) & 0xfe;
    LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;
}

//------------------------------------------------------------------------------
