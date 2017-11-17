//------------------------------------------------------------------------------
//
// bmp_hw.c	    Low-level hardware interface code for BMP LPC1768
//
// Copyright (C)    The University of Manchester - 2012-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

#include <string.h>

#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"

#include "bmp.h"

//------------------------------------------------------------------------------

// Port 0

#define FAN_ED2		(1 << 6)

#define IO_EN1      	(1 << 23)

#define I2C_SDA		(1 << 27)
#define I2C_SCL		(1 << 28)

#define P0_EN       	(SF_NCS + IO_EN1 + LED_MASK)

#define P0_INIT     	(SF_NCS + IO_EN1 + LED_0)


//------------------------------------------------------------------------------
// Port 1

#define XIL_RST		(1 << 14)

#define EN_V12	    	(1 << 18)
#define EN_V18	    	(1 << 19)
#define EN_VFPGA      	(1 << 20)

#define XPROGB_0    	(1 << 21)
#define XPROGB_1    	(1 << 23)
#define XPROGB_2    	(1 << 24)

#define XDONE       	(1 << 31)

#define P1_EN       	(EN_V12 + EN_V18 + EN_VFPGA + XPROGB_0 + XPROGB_1 + \
			 XPROGB_2 + XIL_RST)

#define P1_INIT     	(EN_V12 + EN_V18 + XPROGB_0 + XPROGB_1 + XPROGB_2)

//------------------------------------------------------------------------------
// Port 2

#define BOOT_1      	(1 << 0)	// GPIO[1]
#define IO_6      	(1 << 1)	// GPIO[6]
#define IO_7        	(1 << 2)	// GPIO[7]
#define IO_8       	(1 << 3)
#define IO_9       	(1 << 4)
#define FAN_PWM		(1 << 5)
#define POR         	(1 << 6)
#define RST         	(1 << 7)

#define FAN_ED0		(1 << 8)
#define FAN_ED1		(1 << 9)
#define IO_EN2      	(1 << 10)
#define XINITB_0     	(1 << 11)
#define XINITB_1     	(1 << 12)
#define XINITB_2     	(1 << 13)

#define XINIT_B		(XINITB_0 + XINITB_1 + XINITB_2)

#define P2_EN       	(BOOT_1 + POR + RST + IO_EN2 + IO_6 + \
                     	IO_7 + IO_8 + IO_9)

#define P2_INIT     	(IO_6 + IO_EN2)	// BootROM + serial + POST

//------------------------------------------------------------------------------
// Port 3


#define P3_EN 	    	(XFSEL_0 + XFSEL_1)
#define P3_INIT     	(XFSEL_0 + XFSEL_1)

//------------------------------------------------------------------------------
// Port 4


#define P4_EN 	    	(XFSEL_2)
#define P4_INIT     	(XFSEL_2)

//------------------------------------------------------------------------------

#define XTAL_CLK 25000000	// Xtal clock (25 MHz)

/*
  Crystal on main osc is 25 MHz and we want 100 MHz CCLK. Use a 300 MHz
  output from PLL0 set by M = 12 and N = 2 in PLL config register then
  divide PLL0 by 3 in CCLKCFG.

  300 = (2 * XCLK * M) / N
  CCLK = 300 / 3
*/

#define PDIV4 0
#define PDIV1 1
#define PDIV2 2
#define PDIV8 3

#define	PCLKSEL_WDT  		0
#define	PCLKSEL_TIMER0  	2
#define	PCLKSEL_TIMER1  	4
#define	PCLKSEL_UART0  		6
#define	PCLKSEL_UART1  		8
#define	PCLKSEL_PWM1  		12
#define	PCLKSEL_I2C0  		14
#define	PCLKSEL_SPI  		16
#define	PCLKSEL_SSP1  		20
#define	PCLKSEL_DAC  		22
#define	PCLKSEL_ADC  		24
#define	PCLKSEL_CAN1 		26
#define	PCLKSEL_CAN2 		28
#define	PCLKSEL_ACF  		30

#define	PCLKSEL_QEI  		0
#define	PCLKSEL_PCB  		4
#define	PCLKSEL_I2C1  		6
#define	PCLKSEL_SSP0  		10
#define	PCLKSEL_TIMER2  	12
#define	PCLKSEL_TIMER3  	14
#define	PCLKSEL_UART2  		16
#define	PCLKSEL_UART3  		18
#define	PCLKSEL_I2C2  		20
#define	PCLKSEL_I2S  		22
#define	PCLKSEL_RIT  		26
#define	PCLKSEL_SYSCON  	28
#define	PCLKSEL_MC  		30

#define	PCONP_PCTIM0	        (1<<1)
#define	PCONP_PCTIM1	        (1<<2)
#define	PCONP_PCUART0  	        (1<<3)
#define	PCONP_PCUART1  	        (1<<4)
#define	PCONP_PCPWM1	        (1<<6)
#define	PCONP_PCI2C0	        (1<<7)
#define	PCONP_PCSPI  	        (1<<8)
#define	PCONP_PCRTC  	        (1<<9)
#define	PCONP_PCSSP1	        (1<<10)
#define	PCONP_PCAD  	        (1<<12)
#define	PCONP_PCAN1  	        (1<<13)
#define	PCONP_PCAN2 	        (1<<14)
#define	PCONP_PCGPIO 	        (1<<15)
#define	PCONP_PCRIT 	        (1<<16)
#define PCONP_PCMC 	        (1<<17)
#define PCONP_PCQEI	        (1<<18)
#define	PCONP_PCI2C1  	        (1<<19)
#define	PCONP_PCSSP0	        (1<<21)
#define	PCONP_PCTIM2	        (1<<22)
#define	PCONP_PCTIM3	        (1<<23)
#define	PCONP_PCUART2  	        (1<<24)
#define	PCONP_PCUART3  	        (1<<25)
#define	PCONP_PCI2C2	        (1<<26)
#define	PCONP_PCI2S  	        (1<<27)
#define	PCONP_PCGPDMA  	        (1<<29)
#define	PCONP_PCENET	        (1<<30)
#define	PCONP_PCUSB  	        (1<<31)

// Timer 0 - 100MHz
// Timer 1 - 100MHz
// SSP1 - 100 MHz
// CAN - 50 MHz

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


board_stat_t board_stat[CAN_SIZE];

static uint32_t adc_chan;

uint8_t can_ID;		// CAN ID (from backplane)
uint8_t board_ID;	// Board ID (devived from can_ID)
bool bp_ctrl;		// Backplane controller
bool fan_sense;		// Compute fan speed

uint8_t power_state;	// Power supply state

ee_data_t ee_data;	// Copy of EEPROM data

extern uint32_t config1, config2;

const uint8_t *can2board;
const uint8_t *board2can;

const uint32_t hw_ver = 5;


//------------------------------------------------------------------------------

// Enable/disable interrupts

__asm uint32_t cpu_int_off(void)
{
    mrs     r0, primask
    cpsid   i
    bx      lr
}

__asm void cpu_int_restore(uint32_t cpsr)
{
    msr     primask, r0
    bx      lr
}


//------------------------------------------------------------------------------

void clock_div (uint32_t bit_pos, uint32_t value)
{
    if (bit_pos < 32) {
	LPC_SC->PCLKSEL0 &= (~(CLKPWR_PCLKSEL_BITMASK (bit_pos)));
	LPC_SC->PCLKSEL0 |= (CLKPWR_PCLKSEL_SET (bit_pos, value));
    } else {
	bit_pos -= 32;
	LPC_SC->PCLKSEL1 &= ~(CLKPWR_PCLKSEL_BITMASK (bit_pos));
	LPC_SC->PCLKSEL1 |= (CLKPWR_PCLKSEL_SET (bit_pos, value));
    }
}

//------------------------------------------------------------------------------


void set_power(uint32_t state)
{
    if (state == POWER_ON) {
	LPC_GPIO0->FIOSET = LED_1;
	LPC_GPIO2->FIOSET = POR;	// Set POR

	LPC_GPIO0->FIOCLR = IO_EN1;
	LPC_GPIO1->FIOCLR = EN_V12 + EN_V18;
	LPC_GPIO1->FIOSET = EN_VFPGA;

	board_stat_t *stat = &board_stat[can_ID];
	stat->shutdown = 0;
    } else {
	LPC_GPIO0->FIOCLR = LED_1;

	LPC_GPIO0->FIOSET = IO_EN1;
	LPC_GPIO1->FIOSET = EN_V12 + EN_V18;
	LPC_GPIO1->FIOCLR = EN_VFPGA;
    }

    power_state = state;
}

//------------------------------------------------------------------------------


static const uint32_t led_bit[] = {
    LED_0, LED_1, LED_2, LED_3,
    LED_4, LED_5, LED_6, LED_7
};


void led_set(uint32_t leds)
{
    for (uint32_t i = 0; i < 8; i++) {
	uint32_t led = led_bit[i];
	uint32_t cmd = leds & 3;

	leds = leds >> 2;

	switch (cmd) {
	case 1:
	    LPC_GPIO0->FIOPIN ^= led;
	    break;
	case 2:
	    LPC_GPIO0->FIOCLR = led;
	    break;
	case 3:
	    LPC_GPIO0->FIOSET = led;
	    break;
	}
    }
}


//------------------------------------------------------------------------------


void fpga_reset(uint32_t code)
{
    if (code == 0) {
	LPC_GPIO1->FIOCLR = XIL_RST;
    } else if (code == 1) {
	LPC_GPIO1->FIOSET = XIL_RST;
    } else {
	LPC_GPIO1->FIOSET = XIL_RST;
	delay_us(5);
	LPC_GPIO1->FIOCLR = XIL_RST;
	delay_us(5);
	LPC_GPIO1->FIOSET = XIL_RST;
    }
}


uint32_t fpga_init(uint32_t mask)
{
    uint32_t xprogb = 0;
    uint32_t xinitb = 0;

    if (mask & 1) {
	xprogb |= XPROGB_0;
	xinitb |= XINITB_0;
    }
    if (mask & 2) {
	xprogb |= XPROGB_1;
	xinitb |= XINITB_1;
    }
    if (mask & 4) {
	xprogb |= XPROGB_2;
	xinitb |= XINITB_2;
    }

    LPC_GPIO1->FIOCLR = XIL_RST;	// Reset line (HSWAPEN) low

    uint32_t led = LPC_GPIO0->FIOPIN & (LED_5 + LED_6);	// Save state of LEDs
    LPC_GPIO0->FIOSET = LED_5 + LED_6;	// Set M0, M1 high

    LPC_GPIO1->FIOSET = xprogb;		// Set PROGB high
    delay_us(5);
    LPC_GPIO1->FIOCLR = xprogb;		// then low
    delay_us(5);
    LPC_GPIO1->FIOSET = xprogb;		// then high again

    // M0, M1 have to be held for some time after PROGB rises otherwise
    // the FPGAs start up in the wrong configuration mode and drive CCLK
    // themselves (not good!)

    delay_us(2000);		// Determined by guesswork! Should be checked

    LPC_GPIO0->FIOCLR = LED_5 + LED_6;	// Clear LEDs
    LPC_GPIO0->FIOSET = led;		// and restore

    uint32_t t = 1024;

    // Spin4 has all InitB signals commoned - Spin5 has 3 separate lines

    while ((LPC_GPIO2->FIOPIN & xinitb) != xinitb) { // Wait for INITB high
	delay_us(10);
	if (--t == 0) {
	    return 0;
        }
    }

    return 1;
}

//------------------------------------------------------------------------------


#define ADC_PCLK	25000000	// 25 MHz
#define ADC_CLK		13000000	// 13 MHz
#define ADC_PDN		(1 << 21)

#define ADC_VALID	0xde		// Channels 0, 5 not used on Spin5

// Channels 0-7 - V12d, V12c, V12b, V12a, V18, V25, V33, Vpwr

static void configure_adc(void)
{
    uint32_t div = (ADC_PCLK * 2 + ADC_CLK) / ( 2 * ADC_CLK);

    LPC_ADC->ADCR = ADC_PDN + ((div - 1) << 8);

    for (uint32_t i = 0; i < 8; i++) {
	board_stat[can_ID].adc[i] = UINT16_MAX;
    }
}


void read_adc(void)
{
    uint32_t *adc = (uint32_t *) &LPC_ADC->ADDR0;

    if ((1 << adc_chan) & ADC_VALID) {
	board_stat[can_ID].adc[adc_chan] = (adc[adc_chan] >> 4) & 0xfff;
    }

    if (++adc_chan == 8) {		// Move to next channel
	adc_chan = 0;
    }
    LPC_ADC->ADCR &= ~ 0x070000ff;	// Clear channels & start
    LPC_ADC->ADCR |= 1 << adc_chan;	// Select channel
    LPC_ADC->ADCR |= 0x01000000;	// Start conversion
}


//------------------------------------------------------------------------------

// WDT/reset routines

static void configure_wdt(uint32_t period)
{
    LPC_WDT->WDTC = period;
    LPC_WDT->WDCLKSEL = 1;	// Use APB clock (25MHz)
    LPC_WDT->WDMOD = 3;		// Enable WDT and reset
    LPC_WDT->WDFEED = 0xaa;
    LPC_WDT->WDFEED = 0x55;
}


void refresh_wdt(void)
{
    LPC_WDT->WDFEED = 0xaa;
    LPC_WDT->WDFEED = 0x55;
}


// uni_vec word allocation
//
// 0 - copy of last RSID register
// 1 - count of WDT timeouts
// 2 - up time (seconds)
// 3 - time of last WDT (copy of up time)


static void process_reset(void)
{
    uint32_t rsid = LPC_SC->RSID;

    // Clear uninitialised vector if POR or EXTR
    if ((rsid & 3) != 0) {
	for (uint32_t i = 0; i < 8; i++) {
	    uni_vec[i] = 0;
	}
	LPC_SC->RSID = 3;
    }

    // Process watchdog reset
    if (rsid & 4) {
	LPC_GPIO0->FIOSET = LED_7;

	uni_vec[1]++;			// Bump WDT count
	uni_vec[3] = uni_vec[2];	// Copy uptime;
	uni_vec[2] += 10;		// Try to keep uptime correct

	LPC_SC->RSID = 4;
    }

    // Save RSID register
    uni_vec[0] = rsid;
}


//------------------------------------------------------------------------------


void configure_pwm(uint32_t period, uint32_t width)
{
    LPC_PWM1->TCR = 0;			// Disable...

    LPC_PWM1->PCR = 1 << 14;		// Enable PWM6 output
    LPC_PWM1->MR0 = period;		// Period
    LPC_PWM1->MR6 = width;		// Pulse width
    LPC_PWM1->MCR = 2;			// Reset TC on MR0
    LPC_PWM1->LER = (1 << 0) + (1 << 6); // Enable match copy

    LPC_PWM1->TCR = 9;			// Enable everything...
}


//------------------------------------------------------------------------------


static void timer_init(LPC_TIM_TypeDef *timer, uint32_t channel,
	uint32_t value)
{
    volatile uint32_t *match = &timer->MR0;

    match[channel] = value - 1;		// Set match value

    timer->MCR = 1 << (channel * 3 + 1); // Reset on match
    timer->EMR = 3 << (channel * 2 + 4); // Toggle on match

    timer->TCR = 1;			// Enable timer
}


static void configure_timers(void)
{
    clock_div(CLKPWR_PCLKSEL_TIMER0, CLKPWR_PCLKSEL_CCLK_DIV_1);
    clock_div(CLKPWR_PCLKSEL_TIMER1, CLKPWR_PCLKSEL_CCLK_DIV_1);
    clock_div(CLKPWR_PCLKSEL_TIMER2, CLKPWR_PCLKSEL_CCLK_DIV_1);
    clock_div(CLKPWR_PCLKSEL_TIMER3, CLKPWR_PCLKSEL_CCLK_DIV_1);

    // System tick - period 1ms
    SysTick_Config(CCLK / 1000);

    // Timer0 - generates 10MHz on P1.28
    timer_init(LPC_TIM0, 0, 5);

    // Timer1 - generates 100kHz on P1.25
    timer_init(LPC_TIM1, 1, 500);

    // Timer2 - generates 25MHz on P4.28
    timer_init(LPC_TIM2, 0, 2);

    // Timer3 - used for events - initialised in bmp_event code

    // CLKOUT produces 50 MHz on P1.27
    LPC_SC->CLKOUTCFG = CLKPWR_CLKOUTCFG_CLKOUTSEL_CPU +
	    CLKPWR_CLKOUTCFG_CLKOUTDIV(1) +
	    CLKPWR_CLKOUTCFG_CLKOUT_EN;
}


//------------------------------------------------------------------------------


static void set_pin_mf(uint32_t port, uint32_t pin, uint32_t mode,
	uint32_t function)
{
    uint32_t *func_reg = (uint32_t *) &LPC_PINCON->PINSEL0 + 2 * port;
    uint32_t *mode_reg = (uint32_t *) &LPC_PINCON->PINMODE0 + 2 * port;

    pin *= 2;
    if (pin >= 32) {
	pin -= 32;
	func_reg++;
	mode_reg++;
    }

    *func_reg &= ~(3 << pin);
    *func_reg |= function << pin;

    *mode_reg &= ~(3 << pin);
    *mode_reg |= mode << pin;
}


static void set_pin_od(uint32_t port, uint32_t pin, uint32_t mode)
{
    uint32_t *od_reg = (uint32_t *) &LPC_PINCON->PINMODE_OD0 + port;

    if (mode == PINSEL_PINMODE_OPENDRAIN) {
	*od_reg |= 1 << pin;
    } else {
	*od_reg &= ~(1 << pin);
    }
}


static void config_pin(uint32_t port, uint32_t pin, uint32_t func)
{
    set_pin_mf(port, pin, PINSEL_PINMODE_PULLUP, func);
    set_pin_od(port, pin, PINSEL_PINMODE_NORMAL);
}


//  PORT_0, pin 16 is Flash NCS

void ssp0_pins(uint32_t on)
{
    if (on) {
	LPC_GPIO2->FIOSET = IO_EN2;	// Disable Spin from Flash

	LPC_GPIO0->FIODIR |= SF_NCS;	// Enable output (high)

	config_pin(PINSEL_PORT_0, 15, PINSEL_FUNC_2); // Flash SCK
	config_pin(PINSEL_PORT_0, 17, PINSEL_FUNC_2); // Flash SO
	config_pin(PINSEL_PORT_0, 18, PINSEL_FUNC_2); // Flash SI
    } else {
	LPC_GPIO0->FIODIR &= ~SF_NCS;	// Disable output

	config_pin(PINSEL_PORT_0, 15, PINSEL_FUNC_0); // Flash SCK
	config_pin(PINSEL_PORT_0, 17, PINSEL_FUNC_0); // Flash SO
	config_pin(PINSEL_PORT_0, 18, PINSEL_FUNC_0); // Flash SI

	LPC_GPIO2->FIOCLR = IO_EN2;	// Enable Spin onto Flash
    }

    delay_ms(1);
}


static uint32_t configure_pins(void)
{
    uint32_t id = LPC_GPIO2->FIOPIN & 31;

    // Spin4 PCB fault - bits 0, 1 of ID swapped
    bp_ctrl = ((id & 7) == 0);

    //-----------------
    // Configure Port 0

    LPC_GPIO0->FIOSET = P0_INIT;	// Initialise outputs
    LPC_GPIO0->FIODIR = P0_EN;		// Enable outputs

    config_pin(PINSEL_PORT_0,  0, PINSEL_FUNC_1); // CAN RX
    config_pin(PINSEL_PORT_0,  1, PINSEL_FUNC_1); // CAN TX
    config_pin(PINSEL_PORT_0,  2, PINSEL_FUNC_2); // A/D ch 7 (VPWR)
    config_pin(PINSEL_PORT_0,  3, PINSEL_FUNC_2); // A/D ch 6 (V33)
    // 4-5   - LED outputs
    // 6     - spare (Spin4), GSIG (Spin5)
    config_pin(PINSEL_PORT_0,  7, PINSEL_FUNC_2); // Xilinx CCLK
    config_pin(PINSEL_PORT_0,  8, PINSEL_FUNC_2); // Xilinx DOUT
    config_pin(PINSEL_PORT_0,  9, PINSEL_FUNC_2); // Xilinx DIN

    config_pin(PINSEL_PORT_0, 10, PINSEL_FUNC_2); // SDA2
    config_pin(PINSEL_PORT_0, 11, PINSEL_FUNC_2); // SCL2

    // Configure 15-18 (SSP0 - Serial Flash)

    ssp0_pins(1);

    // 19-22 - LED outputs
    config_pin(PINSEL_PORT_0, 24, PINSEL_FUNC_1); // A/D ch 1 (V12C)
    config_pin(PINSEL_PORT_0, 25, PINSEL_FUNC_1); // A/D ch 2 (V12B)
    config_pin(PINSEL_PORT_0, 26, PINSEL_FUNC_1); // A/D ch 3 (V12A)

    config_pin(PINSEL_PORT_0, 27, PINSEL_FUNC_1); // I2C SDA
    config_pin(PINSEL_PORT_0, 28, PINSEL_FUNC_1); // I2C SCL
    // 29-30 - LED outputs

    //-----------------
    // Configure Port 1

    LPC_GPIO1->FIOSET = P1_INIT;	// Initialise outputs
    LPC_GPIO1->FIODIR = P1_EN;		// Enable outputs

    config_pin(PINSEL_PORT_1,  0, PINSEL_FUNC_1); // MII TXD0
    config_pin(PINSEL_PORT_1,  1, PINSEL_FUNC_1); // MII TXD1
    config_pin(PINSEL_PORT_1,  4, PINSEL_FUNC_1); // MII TX_EN
    config_pin(PINSEL_PORT_1,  8, PINSEL_FUNC_1); // MII CRS
    config_pin(PINSEL_PORT_1,  9, PINSEL_FUNC_1); // MII RXD0
    config_pin(PINSEL_PORT_1, 10, PINSEL_FUNC_1); // MII RXD1
    // XIL_RST output
    config_pin(PINSEL_PORT_1, 15, PINSEL_FUNC_1); // MII REF_CLK
    config_pin(PINSEL_PORT_1, 16, PINSEL_FUNC_1); // MII MDC
    config_pin(PINSEL_PORT_1, 17, PINSEL_FUNC_1); // MII MDIO

    // 18-20 - VEN outputs
    // 21    - spare (Spin4), XIL_PROGB[0] (Spin5)
    config_pin(PINSEL_PORT_1, 22, PINSEL_FUNC_1); // MC0B0
    // 23-24 - Xilinx PROGB outputs
    config_pin(PINSEL_PORT_1, 25, PINSEL_FUNC_3); // Timer1/M1
    config_pin(PINSEL_PORT_1, 26, PINSEL_FUNC_1); // MC0B1
    config_pin(PINSEL_PORT_1, 27, PINSEL_FUNC_1); // CLKOUT (50MHz)
    config_pin(PINSEL_PORT_1, 28, PINSEL_FUNC_3); // Timer0/M0
    config_pin(PINSEL_PORT_1, 29, PINSEL_FUNC_1); //MC0B2
    config_pin(PINSEL_PORT_1, 30, PINSEL_FUNC_3); // A/D ch 4 (V18)

    //-----------------
    // Configure Port 2

    LPC_GPIO2->FIOSET = P2_INIT;	// Initialise outputs
    LPC_GPIO2->FIODIR = P2_EN;		// Enable outputs

    config_pin(PINSEL_PORT_2,  5, PINSEL_FUNC_1); // PWM 1.6 (Fan)

    //-----------------
    // Configure Port 3

    LPC_GPIO3->FIOSET = P3_INIT;	// Initialise outputs
    LPC_GPIO3->FIODIR = P3_EN;		// Enable outputs

    // 25    - output XIL_FSEL[0]
    // 26    - output XIL_FSEL[1]

    //-----------------
    // Configure Port 4

    LPC_GPIO4->FIOSET = P4_INIT;	// Initialise outputs
    LPC_GPIO4->FIODIR = P4_EN;		// Enable outputs

    config_pin(PINSEL_PORT_4, 28, PINSEL_FUNC_2); // Timer2/M0
    // 29    - output XIL_FSEL[2]

    return id;
}


//------------------------------------------------------------------------------

// "reset_spin" resets SpiNNaker by raising/lowering POR and switching the
// Serial Flash multiplexer as needed when POR falls.

// code = 0 - lower POR (reset off)
// code = 1 - raise POR (reset on)
// code = 2 - pulse POR

// If Bit 2 of code is set then the FPGAs are also reset

// The Serial Flash is dual ported on Spin5 and the SpiNNaker root chip reads
// its IP address from it just after it is reset. The read takes about 12ms
// for the 'standard' IP address block of 32 bytes. If the Serial Flash is
// set up to contain more code, the delay of 20ms will need to be increased.

void reset_spin(uint32_t code)
{
    uint32_t fpga = code & 4;

    code &= 3;
    if (code != 0) {
	LPC_GPIO2->FIOSET = POR;
	if (fpga) {
	    LPC_GPIO1->FIOCLR = XIL_RST;
	}
    }

    if (code != 1) {
	delay_ms(1);			// Wait a while
	ssp0_pins(0);			// Enable Spin->Flash

	LPC_GPIO2->FIOCLR = POR;	// Clear POR

	if (fpga) {			// Set XRST
	    LPC_GPIO1->FIOSET = XIL_RST;
	}
    }
}


//------------------------------------------------------------------------------

// Set up a 300kHz 120 degree duty-cycle waveform on three outputs
// MC0B0, MC0B1, MC0B2 (NB outputs not enabled on Spin4)

static void configure_mcpwm()
{
    clock_div(CLKPWR_PCLKSEL_MC, CLKPWR_PCLKSEL_CCLK_DIV_1);

    LPC_MCPWM->MCTIM0 = 0;
    LPC_MCPWM->MCTIM1 = 111;
    LPC_MCPWM->MCTIM2 = 222;

    LPC_MCPWM->MCPER0 = 333;
    LPC_MCPWM->MCPER1 = 333;
    LPC_MCPWM->MCPER2 = 333;

    LPC_MCPWM->MCPW0 = 111;
    LPC_MCPWM->MCPW1 = 111;
    LPC_MCPWM->MCPW2 = 111;

    LPC_MCPWM->MCCON_SET = 0x00010101;
}


//------------------------------------------------------------------------------


// Count transitions on the fan sensor inputs. This routine is called
// once per millisec and it's assumed that the fan speed is low enough
// for there to be no more than one transition per ms.

static uint32_t fan_last[4];
static uint16_t fan_count[4];

void read_fans(void)
{
    uint32_t fan0 = LPC_GPIO2->FIOPIN;
    uint32_t fan1 = fan0;
    uint32_t fan2 = LPC_GPIO0->FIOPIN;

    if (((fan_last[0] ^ fan0) & FAN_ED0) != 0) {
	fan_last[0] = fan0;
	fan_count[0]++;
    }

    if (((fan_last[1] ^ fan1) & FAN_ED1) != 0) {
	fan_last[1] = fan1;
	fan_count[1]++;
    }

    if (((fan_last[2] ^ fan2) & FAN_ED2) != 0) {
	fan_last[2] = fan2;
	fan_count[2]++;
    }
}


// Read I2C temperature sensors and also compute the fan speed. This
// routine is called once per second.

void read_temp(void)
{
    board_stat_t *stat = &board_stat[can_ID];

    for (uint32_t i = 0; i < 4; i++) {
	stat->t_int[i] = INT16_MIN;		// 0x8000
	stat->t_ext[i] = INT16_MIN;
	stat->fan[i] = UINT16_MAX;
    }

    stat->t_int[0] = read_ts(LPC_I2C2, 0x90);	// First on-board sensor
    stat->t_int[1] = read_ts(LPC_I2C2, 0x98);	// Second on-board sensor

    if (bp_ctrl) {				// Backplane stuff
	stat->t_ext[0] = read_ts(LPC_I2C0, 0x9c); // First off-board sensor
	stat->t_ext[1] = read_ts(LPC_I2C0, 0x9e); // Second off-board sensor
    }

    if (fan_sense) {
	for (uint32_t i = 0; i < 4; i++) {	// Fans
	    stat->fan[i] = fan_count[i] * 15;
	    fan_count[i] = 0;
	}
    }
}


//------------------------------------------------------------------------------


// Curl up and die (probably because of hardware failure or misconfiguration.
// The top LED is turned on (Red on Spin5) and the code is put on the next
// 4 LEDs (Green on Spin5). The bottom 3 LEDs are turned off.

void die(uint32_t code)
{
    __disable_irq();

    uint32_t bits = LED_7;

    for (uint32_t i = 0; i < 4; i++) {
	if (code & (1 << i)) {
	    bits |= led_bit[i+3];
	}
    }

    LPC_GPIO0->FIOCLR = LED_MASK;
    LPC_GPIO0->FIOSET = bits;

    while (1) {
	refresh_wdt();
    }
}


void delay_us(uint32_t n)
{
    n *= 34;
    while (n--) {
	continue;
    }
}


void delay_ms(uint32_t n)
{
    while (n--) {
	delay_us(1000);
    }
}


//------------------------------------------------------------------------------

// !! Comment needs updating ...

// Copy the SpiNNaker IP address from buffer in memory (spin_ip) to
// the bottom of the serial Flash. The 32 bytes of data is padded with
// the necessary control codes to allow SpiNNaker to load the address
// block. The first 44 bytes of the serial Flash are used for
// this. The data is only updated if it is different from the buffer.

// Word 12 is used to hold a count of the number of times the update has
// occurred. If more load blocks are needed this word can form the
// single entry in a dummy load block.


#define SYSRAM_IP_DATA		0xf5007fe0


static void configure_spin(void)
{
    uint32_t buf[128];	// 512 bytes
    uint32_t *ip = (uint32_t *) &spin_ip;

    // Clear out temporary buffer used to assemble data

    for (uint32_t i = 0; i < 128; i++) {
	buf[i] = 0xffffffff;
    }

    // First thing in buffer is IP address info (11 words)
    // at offset 0. This needs to be byte reversed!

    buf[0] = __rev(0x553a0008);
    buf[1] = __rev(SYSRAM_IP_DATA);

    for (uint32_t i = 0; i < 8; i++) {
	buf[i+2] = __rev(ip[i]);
    }
    buf[10] = __rev(0xaaaaaaaa);

    // Second thing is board_info which starts at offset 256
    // and is <= 256 bytes

    uint32_t *info = (uint32_t *) 0x1e00;
    uint32_t count = info[0];

    if (count > 63) {	// !! Un-init. flash?
	count = 0;
    }
    buf[64] = count;

    for (uint32_t i = 1; i < count + 1; i++) {
	buf[64 + i] = info[i];
    }
    sf_read(0, 512, (uint8_t *) flash_buf);

    if (memcmp(flash_buf, buf, 512) != 0) {
	memcpy(flash_buf, buf, 512);
	sf_write(0, 512, (uint8_t *) flash_buf);
    }
}


//------------------------------------------------------------------------------


static const uint8_t can2board_0[] = {
    0,  1,  2,  3,  4,  5,   0,  0,
    6,  7,  8,  9,  10, 11,  0,  0,
    12, 13, 14, 15, 16, 17,  0,  0,
    18, 19, 20, 21, 22, 23,  0,  0
};

static const uint8_t board2can_0[] = {
    0,  1,  2,  3,  4,  5,
    8,  9, 10, 11, 12, 13,
   16, 17, 18, 19, 20, 21,
   24, 25, 26, 27, 28, 29
};

static const uint8_t null_map[] = {
    0,  1,  2,  3,  4,  5,  6,  7,
    8,  9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23,
   24, 25, 26, 27, 28, 29, 30, 31
};


void proc_setup(uint32_t d1, uint32_t d2)
{
    uint32_t hw_ver = d1 & 7;
    uint32_t mask = (d1 >> 3) & 31;
    uint32_t frame_ID = (d1 >> 8) & 255;

    if (hw_ver == 0) {
	can2board = can2board_0;
	board2can = board2can_0;
    } else if (hw_ver == 1) {
	can2board = null_map;
	board2can = null_map;
    } else {
	die(9);
    }

    board_ID = can2board[can_ID];

    ee_data.frame_ID = frame_ID;
    mask = 0xffffffff << mask;

    if ((bmp_ip.flags & 0x4000) == 0) {
	bmp_ip.ip_addr[0] = d2 >> 24;
	bmp_ip.ip_addr[1] = d2 >> 16;
	bmp_ip.ip_addr[2] = frame_ID;
	bmp_ip.ip_addr[3] = 8 * board_ID;

	bmp_ip.gw_addr[0] = d2 >> 24;
	bmp_ip.gw_addr[1] = d2 >> 16;
	bmp_ip.gw_addr[2] = d2 >> 8;
	bmp_ip.gw_addr[3] = d2 >> 0;

	bmp_ip.net_mask[0] = mask >> 24;
	bmp_ip.net_mask[1] = mask >> 16;
	bmp_ip.net_mask[2] = mask >> 8;
	bmp_ip.net_mask[3] = mask >> 0;

	bmp_ip.mac_addr[0] = 0;
	bmp_ip.mac_addr[1] = 0;
	bmp_ip.mac_addr[2] = 0xa4;
	bmp_ip.mac_addr[3] = frame_ID;
	bmp_ip.mac_addr[4] = d1 >> 16;
	bmp_ip.mac_addr[5] = 8 * board_ID;
    }

    if ((spin_ip.flags & 0x4000) == 0) {
	spin_ip.ip_addr[0] = d2 >> 24;
	spin_ip.ip_addr[1] = d2 >> 16;
	spin_ip.ip_addr[2] = frame_ID;
	spin_ip.ip_addr[3] = 8 * board_ID + 1;

	spin_ip.gw_addr[0] = d2 >> 24;
	spin_ip.gw_addr[1] = d2 >> 16;
	spin_ip.gw_addr[2] = d2 >> 8;
	spin_ip.gw_addr[3] = d2 >> 0;

	spin_ip.net_mask[0] = mask >> 24;
	spin_ip.net_mask[1] = mask >> 16;
	spin_ip.net_mask[2] = mask >> 8;
	spin_ip.net_mask[3] = mask >> 0;

	spin_ip.mac_addr[0] = 0;
	spin_ip.mac_addr[1] = 0;
	spin_ip.mac_addr[2] = 0xa4;
	spin_ip.mac_addr[3] = frame_ID;
	spin_ip.mac_addr[4] = d1 >> 16;
	spin_ip.mac_addr[5] = 8 * board_ID + 1;
    }

    configure_eth(bmp_ip.mac_addr);
    configure_spin();

    uint32_t flags = d1 >> 24;

    if ((flags & 2) == 0) {
	proc_power(((10 * board_ID) << 16) + 1, 1 << board_ID);
    }
}


//------------------------------------------------------------------------------


static const ee_data_t ee_default = {
    0x96,			// Marker
    0,				// SW version
    0,				// HW version
    240,			// Frame ID
    0,				// Mod date
    {192, 168, 240, 254},	// GW address
    0,				// Flags
    16,				// IP mask bits
    0,				// MAC byte 1
    5,				// LCD time
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Pad
    {65, 65, 0, 0},		// T_int warn
    {80, 80, 0, 0},		// T_int shut
    {50, 50, 0, 0},		// T_ext warn
    {65, 65, 0, 0},		// T_ext shut
    {50, 50, 50, 0}, 		// Fan warn
    {0, 0, 0, 0},		// Fan shut
    {114, 114, 114, 114, 166, 184, 203, 184},	// V-under warn
    {102, 102, 102, 102, 147, 164, 0,   0},	// V-under shut
    {140, 140, 140, 140, 203, 225, 248, 225},	// V-over warn
    {152, 152, 152, 152, 221, 246, 0, 0}	// V-over shut
};


void configure_hw(void)
{
    process_reset();

    can_ID = configure_pins();
    configure_timers();
    configure_ssp();
    configure_i2c();
    configure_mcpwm();

    copy_ip_data();
    memcpy(&ee_data, &ee_default, sizeof(ee_data));

    if (bp_ctrl) {
	configure_lcd();

	if (!read_ee(0, sizeof(ee_data), &ee_data) ||
		is_blank(&ee_data, sizeof(ee_data)) ||
		crc32_chk(&ee_data, sizeof(ee_data)) != 0) {
	    LPC_GPIO0->FIOSET = LED_7;
	    memcpy(&ee_data, &ee_default, sizeof(ee_data));
	}
    }

    config2 = (ee_data.gw_addr[0] << 24) +
	    (ee_data.gw_addr[1] << 16) +
	    (ee_data.gw_addr[2] << 8) +
	    (ee_data.gw_addr[3] << 0);

    config1 = (ee_data.flags << 24) +
	    (ee_data.mac_byte << 16) +
	    (ee_data.frame_ID << 8) +
	    (ee_data.mask_bits << 3) +
	    ee_data.hw_ver;

    if (can_ID == 0 || can_ID >= CAN_SIZE) { // >= probably not in a backplane
	proc_setup(config1, config2);
    }
    if (can_ID < CAN_SIZE) {
	configure_can(can_ID);
    }
    if (can_ID == 31) {		// No backplane
	can_ID = board_ID = 0;
	can_status[0] = 1;
	fan_sense = 1;
    } else {
	fan_sense = bp_ctrl;
    }
    if (bp_ctrl && (ee_data.flags & 1)) {
	configure_pwm(1024, 256);
    }
    configure_adc();		// NB uses can_ID

    configure_wdt(62500000);	// 10 secs
}


//------------------------------------------------------------------------------
