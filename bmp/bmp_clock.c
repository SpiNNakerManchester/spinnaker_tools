//------------------------------------------------------------------------------
//
//! \file bmp_clock.c
//! \brief          Clock configuration code for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012-2019 The University of Manchester
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
#include "lpc17xx_clkpwr.h"

//------------------------------------------------------------------------------

#define XTAL_CLK 25000000       // Xtal clock (25 MHz)

/*
  Crystal on main osc is 25 MHz and we want 100 MHz CCLK. Use a 300 MHz
  output from PLL0 set by M = 12 and N = 2 in PLL config register then
  divide PLL0 by 3 in CCLKCFG.

  300 = (2 * XCLK * M) / N
  CCLK = 300 / 3
*/

//! Clock divisors
enum bmp_clock_div {
    PDIV4 = 0,
    PDIV1 = 1,
    PDIV2 = 2,
    PDIV8 = 3,
};

enum bmp_clock_0_select {
    PCLKSEL_WDT = 0,
    PCLKSEL_TIMER0 = 2,
    PCLKSEL_TIMER1 = 4,
    PCLKSEL_UART0 = 6,
    PCLKSEL_UART1 = 8,
    PCLKSEL_PWM1 = 12,
    PCLKSEL_I2C0 = 14,
    PCLKSEL_SPI = 16,
    PCLKSEL_SSP1 = 20,
    PCLKSEL_DAC = 22,
    PCLKSEL_ADC = 24,
    PCLKSEL_CAN1 = 26,
    PCLKSEL_CAN2 = 28,
    PCLKSEL_ACF = 30,
};

enum bmp_clock_1_select {
    PCLKSEL_QEI = 0,
    PCLKSEL_PCB = 4,
    PCLKSEL_I2C1 = 6,
    PCLKSEL_SSP0 = 10,
    PCLKSEL_TIMER2 = 12,
    PCLKSEL_TIMER3 = 14,
    PCLKSEL_UART2 = 16,
    PCLKSEL_UART3 = 18,
    PCLKSEL_I2C2 = 20,
    PCLKSEL_I2S = 22,
    PCLKSEL_RIT = 26,
    PCLKSEL_SYSCON = 28,
    PCLKSEL_MC = 30,
};

enum bmp_clock_power_control {
    PCONP_PCTIM0 = (1<<1),
    PCONP_PCTIM1 = (1<<2),
    PCONP_PCUART0 = (1<<3),
    PCONP_PCUART1 = (1<<4),
    PCONP_PCPWM1 = (1<<6),
    PCONP_PCI2C0 = (1<<7),
    PCONP_PCSPI = (1<<8),
    PCONP_PCRTC = (1<<9),
    PCONP_PCSSP1 = (1<<10),
    PCONP_PCAD = (1<<12),
    PCONP_PCAN1 = (1<<13),
    PCONP_PCAN2 = (1<<14),
    PCONP_PCGPIO = (1<<15),
    PCONP_PCRIT = (1<<16),
    PCONP_PCMC = (1<<17),
    PCONP_PCQEI = (1<<18),
    PCONP_PCI2C1 = (1<<19),
    PCONP_PCSSP0 = (1<<21),
    PCONP_PCTIM2 = (1<<22),
    PCONP_PCTIM3 = (1<<23),
    PCONP_PCUART2 = (1<<24),
    PCONP_PCUART3 = (1<<25),
    PCONP_PCI2C2 = (1<<26),
    PCONP_PCI2S = (1<<27),
    PCONP_PCGPDMA = (1<<29),
    PCONP_PCENET = (1<<30),
    PCONP_PCUSB = (1<<31)
};

// Timer 0 - 100MHz
// Timer 1 - 100MHz
// SSP1 - 100 MHz
// CAN - 50 MHz

//! Peripheral clock speed selection: clock 0
#define PCLKSEL0_V \
	(PDIV4 << PCLKSEL_WDT) + \
	(PDIV1 << PCLKSEL_TIMER0) + \
	(PDIV1 << PCLKSEL_TIMER1) + \
	(PDIV4 << PCLKSEL_UART0) + \
	(PDIV4 << PCLKSEL_UART1) + \
	(PDIV4 << PCLKSEL_PWM1) + \
	(PDIV4 << PCLKSEL_I2C0) + \
	(PDIV4 << PCLKSEL_SPI) + \
	(PDIV1 << PCLKSEL_SSP1) + \
	(PDIV4 << PCLKSEL_DAC) + \
	(PDIV4 << PCLKSEL_ADC) + \
	(PDIV2 << PCLKSEL_CAN1) + \
	(PDIV2 << PCLKSEL_CAN2) + \
	(PDIV2 << PCLKSEL_ACF)

// SSP0 - 100 MHz
// Timer 2 - 100MHz
// Timer 3 - 100MHz
// MC PWM - 100MHz

//! Peripheral clock speed selection: clock 1
#define PCLKSEL1_V \
	(PDIV4 << PCLKSEL_QEI) + \
	(PDIV4 << PCLKSEL_PCB) + \
	(PDIV4 << PCLKSEL_I2C1) + \
	(PDIV1 << PCLKSEL_SSP0) + \
	(PDIV1 << PCLKSEL_TIMER2) + \
	(PDIV1 << PCLKSEL_TIMER3) + \
	(PDIV4 << PCLKSEL_UART2) + \
	(PDIV4 << PCLKSEL_UART3) + \
	(PDIV4 << PCLKSEL_I2C2) + \
	(PDIV4 << PCLKSEL_I2S) + \
	(PDIV4 << PCLKSEL_RIT) + \
	(PDIV4 << PCLKSEL_SYSCON) + \
	(PDIV1 << PCLKSEL_MC)

//------------------------------------------------------------------------------

#define SCS_V           0x00000030      //!< Range 15-25MHz, main osc on
#define CCLKCFG_V       0x00000002      //!< Divide PLL0 by 3 for CCLK
#define CLKSRCSEL_V     0x00000001      //!< Main clock -> PLL0
#define PLL0CFG_V       0x0001000b      //!< N = 2, M = 12
#define FLASHCFG_V      0x0000403a      //!< Flash uses 5 clks / access

#define PLL_UP          ((1 << 25) | (1 << 24)) //!< PLL is up
#define PLL_LOCK        (1 << 26)       //!< PLL has locked
#define OSC_UP          (1 << 6)        //!< Oscillator is up

//! Which peripherals get a clock signal
#define PCONP_V \
	PCONP_PCTIM0 + \
	PCONP_PCTIM1 + \
	PCONP_PCPWM1 + \
	PCONP_PCI2C0 + \
	PCONP_PCI2C2 + \
	PCONP_PCSSP1 + \
	PCONP_PCAD + \
	PCONP_PCAN1 + \
	PCONP_PCGPIO + \
	PCONP_PCSSP0 + \
	PCONP_PCTIM2 + \
	PCONP_PCTIM3 + \
	PCONP_PCGPDMA + \
	PCONP_PCENET + \
	PCONP_PCMC

//------------------------------------------------------------------------------

//! Configure the clocks
void configure_clocks(void)
{
    LPC_SC->SCS = SCS_V;                // Configure main osc.

    // Wait for Osc ready
    while ((LPC_SC->SCS & OSC_UP) == 0) {
        continue;
    }
    LPC_SC->CCLKCFG   = CCLKCFG_V;      // Set PLL0->CCLK divider

    // Peripheral Clock Selection doesn't seem to work here!

    // LPC_SC->PCLKSEL0  = PCLKSEL0_V;
    // LPC_SC->PCLKSEL1  = PCLKSEL1_V;

    LPC_SC->CLKSRCSEL = CLKSRCSEL_V;    // Select Clock Source for PLL0

    LPC_SC->PLL0CFG   = PLL0CFG_V;      // Configure PLL0
    LPC_SC->PLL0FEED  = 0xaa;
    LPC_SC->PLL0FEED  = 0x55;

    LPC_SC->PLL0CON   = 0x01;           // Enable PLL0
    LPC_SC->PLL0FEED  = 0xaa;
    LPC_SC->PLL0FEED  = 0x55;

    // Wait for PLL to lock
    while ((LPC_SC->PLL0STAT & PLL_LOCK) == 0) {
        continue;
    }
    LPC_SC->PLL0CON   = 0x03;           // PLL0 Enable & Connect
    LPC_SC->PLL0FEED  = 0xaa;
    LPC_SC->PLL0FEED  = 0x55;

    // Wait for PLLC0_STAT & PLLE0_STAT
    while ((LPC_SC->PLL0STAT & PLL_UP) != PLL_UP) {
        continue;
    }
    LPC_SC->FLASHCFG  = FLASHCFG_V;     // Configure flash accelerator

    LPC_SC->PCONP     = PCONP_V;        // Power Control for Peripherals

    uint32_t n = 32 * 1024;             // Wait about 1ms
    while (n--) {
        continue;
    }
}

//------------------------------------------------------------------------------
