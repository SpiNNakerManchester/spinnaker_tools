//------------------------------------------------------------------------------
//
// bmp_main.c	    Control program for Spin4/Spin5 BMP
//
// Copyright (C)    The University of Manchester - 2012-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------
//
// History
//
// 1.30 - Apr  14 - update for version 1.3 of SpiNNaker tools
// 1.31 - 03jul14 - added support for separate bitfiles for three FPGAs
// 1.32 - 30jul14 - separated boot function from application
// 1.33 - 05aug14 - added support for WDT
//                - enable XIL_RST pin at startup (was undriven)
// 1.34 - 18aug14 - add reset code to "cmd_xilinx"
// 1.35 - 19aug14 - add code to control LEDs
// 1.36 - 09sep14 - add SPI code to talk to FPGAs
//		  - make can_ID=31 look like can_ID=0
//
//------------------------------------------------------------------------------

#include <string.h>

#include "lpc17xx.h"
#include "bmp.h"

//------------------------------------------------------------------------------

static uint32_t ms_tens;	// counts 1ms ticks 0..9
static uint32_t ticks;		// counts 10ms ticks 0..99

bool data_ok;			// Data sector CRC OK
uint32_t boot_sec;		// Boot block number
cortex_vec_t *cortex_vec;	// Cortex boot vector

//------------------------------------------------------------------------------


static uint32_t led2_period;
static uint32_t led2_timer;
static uint32_t shut_count;
static uint32_t eth_timeout;

static char text[12];
static uint32_t scan_next;


static void set_text(const char *s, uint32_t mask)
{
    strcpy(text, s);

    if ((mask & 0x000ff) == 0) {	// Volts
	text[5] = ' ';
    }
    if ((mask & 0x00f00) == 0) {	// T_int
	text[6] = ' ';
    }
    if ((mask & 0x0f000) == 0) {	// T_ext
	text[7] = ' ';
    }
    if ((mask & 0xf0000) == 0) {	// Fan
	text[8] = ' ';
    }
}


static void update_lcd(void)
{
    // Print frame number and up-time on top line

    uint32_t up_time = uni_vec[2];
    uint32_t days = up_time / 86400;
    uint32_t hours = up_time / 3600;
    uint32_t mins = up_time / 60;

    io_printf(IO_LCD, "%qFrame %u%qUp", LCD_POS(0, 0), ee_data.frame_ID,
	    LCD_POS(10, 0));

    if (hours < 10) {
	io_printf(IO_LCD, "%2u:%02u:%02u\n", hours, mins % 60, up_time % 60);
    } else if (hours < 100) {
	io_printf(IO_LCD, "%4uh%02um\n", hours, mins % 60);
    } else {
	io_printf(IO_LCD, "%4ud%02uh\n", days, hours % 24);
    }

    // Print status on second line

    if (up_time < ee_data.LCD_time) {
	io_printf(IO_LCD, "%q%p%q%5u\n",
		LCD_POS(0, 1), &bmp_ip.ip_addr,
		LCD_POS(15, 1), board_ID);
	return;
    }

    while (1) {
	scan_next++;

	if (scan_next == CAN_SIZE) {
	    uint32_t count = 0;

	    for (uint32_t i = 0; i < CAN_SIZE; i++) {
		if (can_status[i]) {
		    count++;
	        }
	    }

	    uint32_t v = cortex_vec->sw_ver;
	    if (v < 65535) {
		io_printf(IO_LCD, "%qBMP %d.%02d  %4u slots\n",
			LCD_POS(0, 1), v / 100, v % 100, count);
	    } else {
		io_printf(IO_LCD, "%qBMP %d.%d.%d %q%4u slots\n",
			LCD_POS(0, 1), (v >> 16) & 255,
			(v >> 8) & 255, v & 255, LCD_POS(10, 1), count);
	    }
	    break;
	}

	if (scan_next == CAN_SIZE + 1) {
	    scan_next = 0;
	}

	if (can_status[scan_next]) {
	    board_stat_t *stat = &board_stat[scan_next];

	    if (stat->shutdown) {
		set_text("Shut VIEF ", stat->shutdown);
	    } else if (stat->warning) {
		set_text("Warn VIEF ", stat->warning);
	    } else {
		continue;
	    }

	    io_printf(IO_LCD, "%qSlot %u  %q%s\n",
		    LCD_POS(0, 1), can2board[scan_next],
		    LCD_POS(10, 1), text);
	    break;
	}
    }
}

static uint8_t fan = 0;

static void fan_control(void)
{
    if (!bp_ctrl) {
	return;
    }

    uint32_t size = (ee_data.hw_ver == 0) ? 6 : 8;
    int32_t max = INT32_MIN;

    for (uint32_t i = 0; i < size; i++) {
	board_stat_t *stat = &board_stat[can_ID + i];

	for (uint32_t t = 0; t < 4; t++) {
	    if (stat->t_int[t] > max) {
		max = stat->t_int[t];
	    }
        }
    }

    max /= 256;

    if (max > 70 && fan == 2) {
	fan = 3;
    } else if (max > 55 && fan == 1) {
	fan = 2;
    } else if (max > 40 && fan == 0) {
	fan = 1;
    }

    if (max <= 35 && fan == 1) {
	fan = 0;
    } else if (max <= 50 && fan == 2) {
	fan = 1;
    } else if (max <= 65 && fan == 3) {
	fan = 2;
    }

    // io_printf(IO_STD, "Fan max %d fan %d\n", max, fan);

    LPC_PWM1->MR6 = 255 + 256 * fan;
    LPC_PWM1->LER = (1 << 6);
}


static void check_status(void)
{
    board_stat_t *stat = &board_stat[can_ID];

    uint32_t shutdown = 0;
    uint32_t warning = 0;

    // Bits 0-7 are voltage readings

    for (uint32_t i = 0; i < 8; i++) {
	uint32_t volts = stat->adc[i];

	if (volts == UINT16_MAX) {	// Ignore unused channels
	    continue;
        }

	// Ignore all except V33, VPWR if power off
	if (i < 6 && power_state == POWER_OFF) {
	    continue;
        }

	uint32_t wv = ee_data.warn_vhigh[i] << 4;
	uint32_t sv = ee_data.shut_vhigh[i] << 4;

	warning |= (wv != 0 && volts > wv) << i;
	shutdown |= (sv != 0 && volts > sv) << i;

	wv = ee_data.warn_vlow[i] << 4;
	sv = ee_data.shut_vlow[i] << 4;

	warning |= (wv != 0 && volts < wv) << i;
	shutdown |= (sv != 0 && volts < sv) << i;
    }

    // Bits 8-11 are on-board temperature sensors

    for (uint32_t i = 0; i < 4; i++) {
	int32_t temp = stat->t_int[i];
	int32_t wt = ee_data.warn_int[i] * 256;
	int32_t st = ee_data.shut_int[i] * 256;

	warning |= (temp != INT16_MIN && wt != 0 && temp > wt) << (i + 8);
	shutdown |= (temp != INT16_MIN && st != 0 && temp > st) << (i + 8);
    }

    if (bp_ctrl) {
	// Bits 12-15 are off-board temperature sensors

	for (uint32_t i = 0; i < 4; i++) {
	    int32_t temp = stat->t_ext[i];
	    int32_t wt = ee_data.warn_ext[i] * 256;
	    int32_t st = ee_data.shut_ext[i] * 256;

	    warning |= (wt != 0 && temp > wt) << (i + 12);
	    shutdown |= (temp != INT16_MIN && st != 0 && temp > st) << (i + 12);
	}
    }

    if (fan_sense) {
	// Bits 16-19 are fan speeds

	for (uint32_t i = 0; i < 4; i++) {
	    uint32_t speed = stat->fan[i];
	    uint32_t wf = ee_data.warn_fan[i] << 4;
	    uint32_t sf = ee_data.shut_fan[i] << 4;

	    warning |= (wf != 0 && speed < wf) << (i + 16);
	    shutdown |= (speed != UINT16_MAX && sf != 0 && speed < sf) << (i + 16);
	}
    }

    stat->warning = warning;

    if (shutdown && stat->shutdown == 0) {
	if (shut_count++ == 10) {
	    stat->shutdown = shutdown;
	    set_power(POWER_SHUT);
	}
    } else {
	shut_count = 0;
    }

    led2_period = (stat->shutdown) ? 10 : (warning) ? 25 : 0;

    if (lcd_active) {
	update_lcd();
    }
}


static void proc_100hz(uint32_t arg1, uint32_t arg2)
{
    refresh_wdt();

    if (can_ID < CAN_SIZE) {
	can_timer();
    }

    iptag_timer();

    if (ticks == 0) {
	LPC_GPIO0->FIOCLR = LED_0 + LED_2;
	led2_timer = led2_period;
	read_temp();
    } else if (ticks == 49) {
	LPC_GPIO0->FIOSET = LED_0;
	check_status();
    } else if (ticks == 75) {
	fan_control();
    }

    if (ticks % 8 == 0) {	// Every 80ms
	read_adc();
    }

    if (--led2_timer == 0) {
	led2_timer = led2_period;
	LPC_GPIO0->FIOPIN ^= LED_2;
    }

    if (eth_timeout && --eth_timeout == 0) {
	LPC_GPIO0->FIOCLR = LED_6;
    }

    if (ticks++ == 99) {
	ticks = 0;
	uni_vec[2]++;
    }
}

void SysTick_Handler(void)
{
    if (fan_sense) {
	read_fans();
    }

    if (ms_tens++ == 9) {
	ms_tens = 0;

	event_t *e = event_new(proc_100hz, 0, 0);

	if (e != NULL) {
	    proc_queue_add(e);
        }
    }
}

void c_main(cortex_vec_t *cortex_vec_tmp,
	bool data_ok_tmp,
	uint32_t arg3,
	uint32_t arg4)
{
    cortex_vec = cortex_vec_tmp;
    data_ok = data_ok_tmp;

    event_init(2);
    msg_init();
    configure_hw();

    while (1) {
	if (msg_queue_size () != 0) {
	    route_msg(msg_queue_remove());
        }

	if (eth_rx_rdy()) {
	    LPC_GPIO0->FIOSET = LED_6;
	    eth_timeout = 25;
	    eth_receive();
	}

	proc_queue_run();
    }
}
