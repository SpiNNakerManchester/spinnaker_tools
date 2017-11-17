//------------------------------------------------------------------------------
//
// bmp_i2c.c	    I2C peripheral handling for BC&MP
//
// Copyright (C)    The University of Manchester - 2012-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


#include "lpc17xx.h"
#include "lpc17xx_clkpwr.h"

#include "bmp.h"

//------------------------------------------------------------------------------

#define I2C_EE		0xa0	// 24LC08
#define I2C_LCD		0x78	// ST7036
#define I2C_TS		0x90	// LM75B

#define LCD_SIZE	32	// LCD character buffer

static uint8_t lcd_buf[LCD_SIZE];
static uint32_t lcd_ptr;

bool lcd_active;

//------------------------------------------------------------------------------

// Set up I2C0 and I2C2 - 25MHz PCLK

// Use I2SCLH = I2SCLL = 125 for 100 kHz
// Use I2SCLH = I2SCLL = 62  for 200 kHz
// Use I2SCLH = I2SCLL = 31  for 400 kHz

void configure_i2c()
{
    clock_div(CLKPWR_PCLKSEL_I2C0, CLKPWR_PCLKSEL_CCLK_DIV_4);
    clock_div(CLKPWR_PCLKSEL_I2C2, CLKPWR_PCLKSEL_CCLK_DIV_4);

    LPC_I2C0->I2SCLH = LPC_I2C0->I2SCLL = 62;	// 200 kHz

    LPC_I2C2->I2SCLH = LPC_I2C2->I2SCLL = 31;	// 400 kHz

    LPC_I2C0->I2CONSET = 0x40;
    LPC_I2C2->I2CONSET = 0x40;
}

//------------------------------------------------------------------------------

static void wait_i2c(LPC_I2C_TypeDef *i2c)
{
    while ((i2c->I2CONSET & 0x08) == 0) {
	continue;
    }
}

uint32_t i2c_poll(LPC_I2C_TypeDef *i2c, uint32_t ctrl)
{
    i2c->I2CONSET = 0x20;		// Send START
    wait_i2c(i2c);

    i2c->I2DAT = ctrl;			// Send control byte
    i2c->I2CONCLR = 0x28;		// Clear START, SI
    wait_i2c(i2c);

    uint32_t status = i2c->I2STAT;

    i2c->I2CONSET = 0x10;		// Set STOP
    i2c->I2CONCLR = 0x08;		// Clear SI

    while (i2c->I2CONSET & 0x10) {	// Wait for STO to clear
	continue;
    }
    return status == 0x18;		// Return 1 if ACK received
}

uint32_t i2c_send(LPC_I2C_TypeDef *i2c, uint32_t ctrl, uint32_t addr,
	uint32_t length, void *buf)
{
    uint8_t *data = (uint8_t *) buf;

    i2c->I2CONSET = 0x20;		// Send START
    wait_i2c(i2c);

    i2c->I2DAT = ctrl;			// Send control byte
    i2c->I2CONCLR = 0x28;		// Clear START, SI
    wait_i2c(i2c);

    uint32_t status = i2c->I2STAT;

    if (status != 0x18) {
	i2c->I2CONSET = 0x10;		// Set STOP
	i2c->I2CONCLR = 0x08;		// Clear SI

	while (i2c->I2CONSET & 0x10) {	// Wait for STO to clear
	    continue;
        }
	return 0;
    }

    i2c->I2DAT = addr;			// Send address byte
    i2c->I2CONCLR = 0x08;		// Clear SI
    wait_i2c(i2c);

    for (uint32_t i = 0; i < length; i++) {	// Send data...
	i2c->I2DAT = data[i];
	i2c->I2CONCLR = 0x08;	// Clear SI
	(void) wait_i2c(i2c);
    }

    i2c->I2CONSET = 0x10;		// Set STOP
    i2c->I2CONCLR = 0x08;		// Clear SI

    while (i2c->I2CONSET & 0x10) {	// Wait for STO to clear
	continue;
    }
    return 1;
}

uint32_t i2c_receive(LPC_I2C_TypeDef *i2c, uint32_t ctrl, uint32_t addr,
	uint32_t length, void *buf)
{
    uint8_t *data = (uint8_t *) buf;

    i2c->I2CONSET = 0x20;		// Send START
    wait_i2c(i2c);

    i2c->I2DAT = ctrl;			// Send control byte
    i2c->I2CONCLR = 0x28;		// Clear START, SI
    wait_i2c(i2c);

    uint32_t status = i2c->I2STAT;

    if (status != 0x40) {
	i2c->I2CONSET = 0x10;		// Set STOP
	i2c->I2CONCLR = 0x08;		// Clear SI

	while (i2c->I2CONSET & 0x10) {	// Wait for STO to clear
	    continue;
	}
	return 0;
    }

    if (length > 1) {
	i2c->I2CONSET = 0x04;		// Set AA
    }

    for (uint32_t i = 0; i < length; i++) {
	if (i == length - 1) {
	    i2c->I2CONCLR = 0x04;	// Clear AA
        }
	i2c->I2CONCLR = 0x08;		// Clear SI
	wait_i2c(i2c);

	data[i] = i2c->I2DAT;
    }

    i2c->I2CONSET = 0x10;		// Set STO
    i2c->I2CONCLR = 0x0c;		// Clear SI, AA

    while (i2c->I2CONSET & 0x10) {	// Wait for STO to clear
	continue;
    }
    return 1;
}

//------------------------------------------------------------------------------

int16_t read_ts(LPC_I2C_TypeDef *i2c, uint32_t addr)
{
    uint8_t buf[4];

    if (! i2c_send(i2c, addr, 0, 0, NULL)) {	// Send ctrl & ptr=0
	return INT16_MIN;
    }
    i2c_receive(i2c, addr | 1, 0, 2, &buf);	// then read data

    return (buf[0] << 8) + buf[1];
}

uint32_t read_ee(uint32_t addr, uint32_t count, void *buf)
{
    uint32_t ctrl = I2C_EE + ((addr >> 7) & 6);		// 24lc08 code

    if (!i2c_send(LPC_I2C0, ctrl, addr & 255, 0, NULL)) { // Send address
	return 0;
    }
    if (count != 0) {
	i2c_receive(LPC_I2C0, ctrl + 1, 0, count, buf);	// then read data
    }
    return 1;
}

uint32_t write_ee(uint32_t addr, uint32_t count, void *buf)
{
    uint32_t ctrl = I2C_EE + ((addr >> 7) & 6);		// 24lc08 code

    if (!i2c_send(LPC_I2C0, ctrl, addr & 255, count, buf)) { // Send addr & data
	return 0;
    }
    while (!i2c_poll(LPC_I2C0, ctrl)) {			// Wait until done
	continue;
    }
    return 1;
}

//------------------------------------------------------------------------------

static const uint8_t lcd_init[] = {
    0x38,	// 8 bit, 2 lines, not dbl ht, IS=00
    0x00,	// Data follows (RS=0)
    0x39,	// 8 bit, 2 lines, not dbl ht, IS=01
    0x14,	// Bias = 1/4, not 3-line
    0x70,	// Contrast[3:0] = 0000
    0x6c,	// Follower on, Rab[2:0] = 100
    0x56,	// Boost on, Contrast[5:4] = 10
    0x0c,	// Display on, no cursor
    0x01	// Clear and home
};

void configure_lcd(void)
{
    lcd_active = i2c_send(LPC_I2C0, I2C_LCD, 0x80,
	    sizeof(lcd_init), (void *) lcd_init);
    delay_us(1000);
}

static void lcd_flush(void)
{
    if (lcd_ptr) {
	i2c_send(LPC_I2C0, I2C_LCD, 0x40, lcd_ptr, lcd_buf);
	lcd_ptr = 0;
    }
}

void lcd_ctrl(uint32_t c)
{
    uint8_t buf[4];

    buf[0] = c;
    lcd_flush();
    i2c_send(LPC_I2C0, I2C_LCD, 0x00, 1, buf);
}


void lcd_putc(uint32_t c)
{
    if (c != '\n') {
	lcd_buf[lcd_ptr++] = c;
    }
    if (c == '\n' || lcd_ptr == LCD_SIZE) {
	lcd_flush();
    }
}

//------------------------------------------------------------------------------
