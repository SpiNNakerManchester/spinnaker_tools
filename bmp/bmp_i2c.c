//------------------------------------------------------------------------------
//
//! \file bmp_i2c.c
//! \brief          I<sup>2</sup>C peripheral handling for BC&MP
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012-2023 The University of Manchester
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


#include "lpc17xx.h"
#include "lpc17xx_clkpwr.h"

#include "bmp.h"

//------------------------------------------------------------------------------

#define I2C_EE          0xa0    //!< I<sup>2</sup>C address of 24LC08 (EEPROM)
#define I2C_LCD         0x78    //!< I<sup>2</sup>C address of ST7036 (LCD)
#define I2C_TS          0x90    //!< I<sup>2</sup>C base address of LM75B (temp sensors)

#define LCD_SIZE        32      //!< Size of LCD character buffer

//! Buffer for gathering a line to write to the LCD
static uint8_t lcd_buf[LCD_SIZE];
//! Write position within ::lcd_buf
static uint32_t lcd_ptr;

//! Whether the LCD is active; if not, nothing should be sent to the LCD
bool lcd_active;

//------------------------------------------------------------------------------

//! \brief Set up I2C0 and I2C2 - 25MHz PCLK
//! \internal
//! Use I2SCLH = I2SCLL = 125 for 100 kHz
//! Use I2SCLH = I2SCLL = 62  for 200 kHz
//! Use I2SCLH = I2SCLL = 31  for 400 kHz

void configure_i2c(void)
{
    clock_div(CLKPWR_PCLKSEL_I2C0, CLKPWR_PCLKSEL_CCLK_DIV_4);
    clock_div(CLKPWR_PCLKSEL_I2C2, CLKPWR_PCLKSEL_CCLK_DIV_4);

    LPC_I2C0->I2SCLH = LPC_I2C0->I2SCLL = 62;   // 200 kHz

    LPC_I2C2->I2SCLH = LPC_I2C2->I2SCLL = 31;   // 400 kHz

    LPC_I2C0->I2CONSET = 0x40;
    LPC_I2C2->I2CONSET = 0x40;
}

//------------------------------------------------------------------------------

//! \brief Wait for I<sup>2</sup>C to be ready
//! \param[in] i2c: Which I<sup>2</sup>C port to wait for
static void wait_i2c(LPC_I2C_TypeDef *restrict i2c)
{
    while ((i2c->I2CONSET & 0x08) == 0) {
        continue;
    }
}

//! \brief Poll an I<sup>2</sup>C port
//! \param[in] i2c: Which I<sup>2</sup>C port to poll
//! \param[in] ctrl: Control byte
//! \return True if ACK received
uint32_t i2c_poll(LPC_I2C_TypeDef *restrict i2c, uint32_t ctrl)
{
    i2c->I2CONSET = 0x20;               // Send START
    wait_i2c(i2c);

    i2c->I2DAT = ctrl;                  // Send control byte
    i2c->I2CONCLR = 0x28;               // Clear START, SI
    wait_i2c(i2c);

    uint32_t status = i2c->I2STAT;

    i2c->I2CONSET = 0x10;               // Set STOP
    i2c->I2CONCLR = 0x08;               // Clear SI

    while (i2c->I2CONSET & 0x10) {      // Wait for STO to clear
        continue;
    }
    return status == 0x18;              // Return 1 if ACK received
}

//! \brief Send a buffer over I<sup>2</sup>C
//! \param[in] i2c: Which I<sup>2</sup>C port to send via
//! \param[in] ctrl: Control word
//! \param[in] addr: Destination address
//! \param[in] length: Number of bytes to write
//! \param[in] buf: Buffer of data to write
//! \return true if data was written
uint32_t i2c_send(LPC_I2C_TypeDef *restrict i2c, uint32_t ctrl, uint32_t addr,
        uint32_t length, const void *buf)
{
    const uint8_t *data = (const uint8_t *) buf;

    i2c->I2CONSET = 0x20;               // Send START
    wait_i2c(i2c);

    i2c->I2DAT = ctrl;                  // Send control byte
    i2c->I2CONCLR = 0x28;               // Clear START, SI
    wait_i2c(i2c);

    uint32_t status = i2c->I2STAT;

    if (status != 0x18) {
        i2c->I2CONSET = 0x10;           // Set STOP
        i2c->I2CONCLR = 0x08;           // Clear SI

        while (i2c->I2CONSET & 0x10) {  // Wait for STO to clear
            continue;
        }
        return 0;
    }

    i2c->I2DAT = addr;                  // Send address byte
    i2c->I2CONCLR = 0x08;               // Clear SI
    wait_i2c(i2c);

    for (uint32_t i = 0; i < length; i++) {     // Send data...
        i2c->I2DAT = data[i];
        i2c->I2CONCLR = 0x08;   // Clear SI
        (void) wait_i2c(i2c);
    }

    i2c->I2CONSET = 0x10;               // Set STOP
    i2c->I2CONCLR = 0x08;               // Clear SI

    while (i2c->I2CONSET & 0x10) {      // Wait for STO to clear
        continue;
    }
    return 1;
}

//! \brief Receive a buffer over I<sup>2</sup>C
//! \param[in] i2c: Which I<sup>2</sup>C port to receive via
//! \param[in] ctrl: Control word
//! \param[in] addr: Source address
//! \param[in] length: Number of bytes to receive
//! \param[in] buf: Buffer of data to receive into
//! \return true if data was received
uint32_t i2c_receive(LPC_I2C_TypeDef *restrict i2c, uint32_t ctrl,
	uint32_t addr, uint32_t length, void *buf)
{
    uint8_t *data = (uint8_t *) buf;

    i2c->I2CONSET = 0x20;               // Send START
    wait_i2c(i2c);

    i2c->I2DAT = ctrl;                  // Send control byte
    i2c->I2CONCLR = 0x28;               // Clear START, SI
    wait_i2c(i2c);

    uint32_t status = i2c->I2STAT;

    if (status != 0x40) {
        i2c->I2CONSET = 0x10;           // Set STOP
        i2c->I2CONCLR = 0x08;           // Clear SI

        while (i2c->I2CONSET & 0x10) {  // Wait for STO to clear
            continue;
        }
        return 0;
    }

    if (length > 1) {
        i2c->I2CONSET = 0x04;           // Set AA
    }

    for (uint32_t i = 0; i < length; i++) {
        if (i == length - 1) {
            i2c->I2CONCLR = 0x04;       // Clear AA
        }
        i2c->I2CONCLR = 0x08;           // Clear SI
        wait_i2c(i2c);

        data[i] = i2c->I2DAT;
    }

    i2c->I2CONSET = 0x10;               // Set STO
    i2c->I2CONCLR = 0x0c;               // Clear SI, AA

    while (i2c->I2CONSET & 0x10) {      // Wait for STO to clear
        continue;
    }
    return 1;
}

//------------------------------------------------------------------------------

//! \brief Read temperature sensor
//! \param[in] i2c: Which I<sup>2</sup>C port to use
//! \param[in] addr: What address to read. (Controls which sensor to read)
//! \return The temperature sensor value
int16_t read_ts(LPC_I2C_TypeDef *restrict i2c, uint32_t addr)
{
    uint8_t buf[4];

    if (! i2c_send(i2c, addr, 0, 0, NULL)) {    // Send ctrl & ptr=0
        return INT16_MIN;
    }
    i2c_receive(i2c, addr | 1, 0, 2, &buf);     // then read data

    return (buf[0] << 8) + buf[1];
}

//! \brief Read from EEPROM
//! \param[in] addr: Address to read from
//! \param[in] count: Number of bytes to read
//! \param[in] buf: Buffer to read into
//! \return True if the read succeeded
uint32_t read_ee(uint32_t addr, uint32_t count, void *buf)
{
    uint32_t ctrl = I2C_EE + ((addr >> 7) & 6);         // 24lc08 code

    if (!i2c_send(LPC_I2C0, ctrl, addr & 255, 0, NULL)) { // Send address
        return 0;
    }
    if (count != 0) {
        i2c_receive(LPC_I2C0, ctrl + 1, 0, count, buf); // then read data
    }
    return 1;
}

//! \brief Write to EEPROM
//! \param[in] addr: Address to write to
//! \param[in] count: Number of bytes to write
//! \param[in] buf: Buffer to write from
//! \return True if the write succeeded
uint32_t write_ee(uint32_t addr, uint32_t count, const void *buf)
{
    uint32_t ctrl = I2C_EE + ((addr >> 7) & 6);         // 24lc08 code

    if (!i2c_send(LPC_I2C0, ctrl, addr & 255, count, buf)) { // Send addr & data
        return 0;
    }
    while (!i2c_poll(LPC_I2C0, ctrl)) {                 // Wait until done
        continue;
    }
    return 1;
}

//------------------------------------------------------------------------------

//! LCD initialisation descriptor
static const uint8_t lcd_init[] = {
    0x38,       // 8 bit, 2 lines, not dbl ht, IS=00
    0x00,       // Data follows (RS=0)
    0x39,       // 8 bit, 2 lines, not dbl ht, IS=01
    0x14,       // Bias = 1/4, not 3-line
    0x70,       // Contrast[3:0] = 0000
    0x6c,       // Follower on, Rab[2:0] = 100
    0x56,       // Boost on, Contrast[5:4] = 10
    0x0c,       // Display on, no cursor
    0x01        // Clear and home
};

//! Configure the LCD
void configure_lcd(void)
{
    lcd_active = i2c_send(LPC_I2C0, I2C_LCD, 0x80,
            sizeof(lcd_init), (void *) lcd_init);
    delay_us(1000);
}

//! Flush any pending data (i.e., a line of text) to the LCD
static void lcd_flush(void)
{
    if (lcd_ptr) {
        i2c_send(LPC_I2C0, I2C_LCD, 0x40, lcd_ptr, lcd_buf);
        lcd_ptr = 0;
    }
}

//! \brief Write a control byte to the LCD
//! \param[in] c: Control byte to write
void lcd_ctrl(uint32_t c)
{
    uint8_t buf[4];

    buf[0] = c;
    lcd_flush();
    i2c_send(LPC_I2C0, I2C_LCD, 0x00, 1, buf);
}

//! \brief Write a character to the LCD. _Buffers._
//! \param[in] c: The character to write
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
