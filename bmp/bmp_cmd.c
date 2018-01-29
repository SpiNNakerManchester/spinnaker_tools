//------------------------------------------------------------------------------
//
// bmp_cmd.c	    Command handling for BC&MP
//
// Copyright (C)    The University of Manchester - 2012-2016
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

#include "lpc17xx.h"
#include <string.h>
#include "bmp.h"

//------------------------------------------------------------------------------

extern uint32_t tag_tto;
extern uint32_t reset_vec;

//------------------------------------------------------------------------------


static void fpga_boot(uint32_t base, uint32_t len, uint32_t mask)
{
    ssp0_pins(1);
    (void) fpga_init(mask);
    ssp0_copy(base, len);
    delay_ms(10);
    ssp0_pins(0);
}


//------------------------------------------------------------------------------


static void fpga_xreg(uint32_t count, uint8_t *data)
{
    uint32_t *words = (uint32_t *) data;

    for (uint32_t i = 0; i < count; i++) {
	uint32_t addr = words[2 * i];
	uint32_t *data = words + 2 * i + 1;
	uint32_t fpga = addr & 3;

	fpga = (fpga == 3) ? 7 : 1 << fpga;

	for (uint32_t f = 0; f < 3; f++) {
	    if (fpga & (1 << f)) {
		fpga_word(addr, f, data, FPGA_WRITE);
	    }
	}
    }
}


//------------------------------------------------------------------------------


static void sf_scan(void)
{
    if (! data_ok) {
	return;
    }
    ssp1_fast();

    for (uint32_t i = 0; i < FL_DIR_SIZE; i++) {
	fl_dir_t *e = fl_dir + i;

	if (e->type == FL_FPGA && e->length != 0 && (e->flags & 0x8000) != 0) {
	    fpga_boot(e->base, e->length, e->flags);
	}
    }

    // Reset line high, wait a while then process "xreg" blocks
    fpga_reset(1);
    ssp1_slow();
    delay_ms(5);

    for (uint32_t i = 0; i < FL_DIR_SIZE; i++) {
	fl_dir_t *e = fl_dir + i;

	if (e->type == FL_XREG && e->size > 0) {
	    fpga_xreg(e->size, e->data);
	}
    }
}


//------------------------------------------------------------------------------


void flash_buf_init(void)
{
    for (uint32_t i = 0; i < FLASH_WORDS; i++) {
	flash_buf[i] = 0xffffffff;
    }
}


// arg1 - start address
// arg2 - end address

static uint32_t cmd_flash_erase (sdp_msg_t *msg)
{
    msg->arg1 = flash_erase(msg->arg1, msg->arg2 - 1);
    flash_buf_init();

    if (msg->arg1 == 0) {
	msg->arg1 = (uint32_t) flash_buf;
    } else {
	msg->cmd_rc = RC_ARG;
    }
    return 4;
}


// arg1 - Flash address
// arg2 - Size in bytes
// arg3 - Erase before write

static uint32_t cmd_flash_write(sdp_msg_t *msg)
{
    uint32_t start = msg->arg1;
    uint32_t length = msg->arg2;
    uint32_t erase = msg->arg3;

    if (erase) {
	msg->arg1 = flash_erase(start, start + length - 1);
	if (msg->arg1 != 0) {
	    msg->cmd_rc = RC_ARG;
	    return 4;
	}
    }

    msg->arg1 = flash_write(start, length, flash_buf);
    flash_buf_init();

    if (msg->arg1 == 0) {
	return 0;
    }
    msg->cmd_rc = RC_ARG;
    return 4;
}


//------------------------------------------------------------------------------

// arg1 - opcode
// arg2 - op=0 -> length
//        op=1 -> mask
//        op=2 -> reset code

static uint32_t cmd_xilinx(sdp_msg_t *msg)
{
    uint32_t op = msg->arg1;

    if (op == 0) {		// Send config bitfile
	ssp1_fast();
	ssp1_copy(msg->arg2, msg->data);
    } else if (op == 1) {	// Initialise FPGA
	if (! fpga_init(msg->arg2)) {
	    msg->cmd_rc = RC_TIMEOUT;
	}
    } else if (op == 2) {	// Reset FPGA
	if (msg->arg2 <= 2) {
	    fpga_reset(msg->arg2);
	} else {
	    msg->cmd_rc = RC_ARG;
	}
    } else {
	msg->cmd_rc = RC_ARG;
    }

    return 0;
}


//------------------------------------------------------------------------------

// arg1=addr
// arg2=len
// arg3=fpga_num

uint32_t cmd_fpga_read(sdp_msg_t *msg)
{
    uint32_t len = msg->arg2;
    uint32_t link = msg->arg3;

    if (len > SDP_BUF_SIZE || link > NUM_FPGAS - 1) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint32_t addr = msg->arg1;
    uint32_t *buf = (uint32_t *) &msg->arg1;

    ssp1_slow();

    for (uint32_t i = 0; i < len / 4; i++) {
	fpga_word(addr, link, buf, FPGA_READ);
	addr += 4;
	buf += 1;
    }

    return len;
}

//------------------------------------------------------------------------------

// arg1=addr
// arg2=len
// arg3=link_num

uint32_t cmd_fpga_write(sdp_msg_t *msg)
{
    uint32_t len = msg->arg2;
    uint32_t link = msg->arg3;

    if (len > SDP_BUF_SIZE || link > NUM_FPGAS - 1) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint32_t addr = msg->arg1;
    uint32_t *buf = (uint32_t *) msg->data;

    ssp1_slow();

    for (uint32_t i = 0; i < len / 4; i++) {
	fpga_word(addr, link, buf, FPGA_WRITE);
	addr += 4;
	buf += 1;
    }

    return 0;
}

//------------------------------------------------------------------------------


static uint32_t cmd_sf(sdp_msg_t *msg)
{
    uint32_t addr = msg->arg1;
    uint32_t len = msg->arg2;
    uint32_t op = msg->arg3;

    if ((op < 2 && len > SDP_BUF_SIZE) || op > 2) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint32_t r;

    ssp0_pins(1);
    if (op == 0) {
	sf_read(addr, len, (uint8_t *) &msg->arg1);
	r = len;
    } else if (op == 1) {
	sf_write(addr, len, msg->data);
	r = 0;
    } else {
	msg->arg1 = sf_crc32(addr, len);
	r = 4;
    }
    ssp0_pins(0);

    return r;
}


//------------------------------------------------------------------------------


static uint32_t cmd_ee(sdp_msg_t *msg)
{
    uint32_t addr = msg->arg1;
    int32_t len = msg->arg2;
    uint32_t op = msg->arg3;

    if (len > SDP_BUF_SIZE || op > 1 || ! bp_ctrl) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    if (op == 0) {
	if (read_ee(addr, len, &msg->arg1)) {
	    return len;
	}
	msg->cmd_rc = RC_TIMEOUT;
    } else {
	uint8_t *buf = msg->data;

	if (!read_ee(addr, 0, NULL)) {
	    msg->cmd_rc = RC_TIMEOUT;
	    return 0;
	}

	while (len > 0) {
	    uint32_t count = (len > 16) ? 16 : len;

	    write_ee(addr, count, buf);

	    addr += 16;
	    buf += 16;
	    len -= 16;
	}
    }

    return 0;
}


//------------------------------------------------------------------------------


void proc_led(uint32_t arg, uint32_t mask)
{
    if (mask & (1 << board_ID)) {
	led_set(arg);
    }
}


static uint32_t cmd_led(sdp_msg_t *msg)
{
    uint32_t mask = msg->arg2 & ~(1 << board_ID);

    if (mask) {
	can_proc_cmd(31, PROC_LED, msg->arg1, mask);
    }
    proc_led(msg->arg1, msg->arg2);

    return 0;
}


//------------------------------------------------------------------------------

void proc_reset(uint32_t arg, uint32_t mask)
{
    uint32_t reset = arg & 255;
    uint32_t delay = (arg >> 16) * board_ID;

    if (mask & (1 << board_ID)) {
	if (delay) {
	    delay_ms(delay);
	}
	reset_spin(reset);
    }
}


static uint32_t cmd_reset(sdp_msg_t *msg)
{
    if ((msg->arg1 & 0xfb) > 2) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint32_t mask = msg->arg2 & ~(1 << board_ID);
    if (mask) {
	can_proc_cmd(31, PROC_RESET, msg->arg1, mask);
    }
    proc_reset(msg->arg1, msg->arg2);

    return 0;
}


//------------------------------------------------------------------------------


// Spin4 (Murata) DC/DC converters turn on in ~10ms (measured)
// Spin5 (Texas Inst.) converters turn on in ~5ms (measured)

void proc_power(uint32_t arg, uint32_t mask)
{
    uint32_t on = arg & 255;
    uint32_t delay = (arg >> 16) * board_ID;

    if (mask & (1 << board_ID)) {
	if (delay) {
	    delay_ms(delay);
	}
	if (on) {
	    set_power(POWER_ON);	// Sets POR
	    delay_ms(25);
	    sf_scan();			// Boot FPGAs
	    reset_spin(0);		// Clear POR
	} else {
	    set_power(POWER_OFF);
	}
    }
}


static uint32_t cmd_power(sdp_msg_t *msg)
{
    if ((msg->arg1 & 255) > 1) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint32_t mask = msg->arg2 & ~(1 << board_ID);

    if (mask) {
	can_proc_cmd(31, PROC_POWER, msg->arg1, mask);
    }
    proc_power(msg->arg1, msg->arg2);

    return 0;
}


//------------------------------------------------------------------------------


static uint32_t cmd_ver(sdp_msg_t *msg)
{
    uint32_t cv = (uint32_t) cortex_vec / 0x10000;

    msg->arg1 = (cv << 24) + (ee_data.frame_ID << 16) +
	    (can_ID << 8) + board_ID;
    msg->arg2 = 0xffff0000 + SDP_BUF_SIZE;
    msg->arg3 = cortex_vec->build_date;

    uint32_t len = 1 + strlen (BMP_ID_STR);

    strcpy((char *) msg->data, BMP_ID_STR);
    strcpy((char *) msg->data + len, BMP_VER_STR);

    return 12 + len + strlen (BMP_VER_STR) + 1;
}


//------------------------------------------------------------------------------


// Return various useful bits of information about the BMP

// arg1 = 0 returns
//	    0   - hw_ver
//	    1-4 - LPC1768 serial number
//	    5   - flash buffer address
//          6   - board_stat address (this board)
//          7   - Cortex boot vector address
// arg1 = 1 returns EE data buffer
// arg1 = 2 returns CAN status buffer
// arg1 = 3 returns Board status buffer
// arg1 = 4 returns BMP & Spin IP address data
// arg1 = 5 returns Uninitialised vectors


static uint32_t cmd_bmp_info(sdp_msg_t *msg)
{
    uint32_t *buf = &msg->arg1;

    switch (msg->arg1) {
    case 0:
	buf[0] = 58;
	lpc_iap(buf, buf);
	buf[0] = hw_ver;
	buf[5] = (uint32_t) flash_buf;
	buf[6] = (uint32_t) &board_stat[can_ID];
	buf[7] = (uint32_t) cortex_vec;
	return 32;

    case 1:
	memcpy(buf, &ee_data, sizeof(ee_data_t));
	return sizeof(ee_data_t);

    case 2:
	memcpy(buf, can_status, sizeof(can_status));
	return sizeof(can_status);

    case 3:
	memcpy(buf, &board_stat[can_ID], sizeof(board_stat_t));
	return sizeof(board_stat_t);

    case 4:
	memcpy(buf, &bmp_ip, sizeof(ip_data_t));
	buf += sizeof(ip_data_t) / sizeof(uint32_t);
	memcpy(buf, &spin_ip, sizeof(ip_data_t));
	return 2 * sizeof(ip_data_t);

    case 5:
	memcpy(buf, uni_vec, 32);
	return 32;

    default:
	msg->cmd_rc = RC_ARG;
	return 0;
    }
}


//------------------------------------------------------------------------------


static uint32_t cmd_fill(sdp_msg_t *msg)
{
    uint32_t *to = (uint32_t *) msg->arg1;
    uint32_t data = msg->arg2;
    int32_t n = msg->arg3;

    while (n > 0) {
	*to++ = data;
	n -= 4;
    }

    return 0;
}


//------------------------------------------------------------------------------


static uint32_t cmd_read(sdp_msg_t *msg)
{
    uint32_t addr = msg->arg1;	// Address
    uint32_t len = msg->arg2;	// Length
    uint32_t type = msg->arg3;	// Type

    if (len > SDP_BUF_SIZE || type > TYPE_WORD) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint8_t *buffer = (uint8_t *) &msg->arg1;

    if (type == TYPE_BYTE) {
	uint8_t *mem = (uint8_t *) addr;
	uint8_t *buf = (uint8_t *) buffer;

	for (uint32_t i = 0; i < len; i++) {
	    buf[i] = mem[i];
	}
    } else if (type == TYPE_HALF) {
	uint16_t *mem = (uint16_t *) addr;
	uint16_t *buf = (uint16_t *) buffer;

	for (uint32_t i = 0; i < len / 2; i++) {
	    buf[i] = mem[i];
	}
    } else {
	uint32_t *mem = (uint32_t *) addr;
	uint32_t *buf = (uint32_t *) buffer;

	for (uint32_t i = 0; i < len / 4; i++) {
	    buf[i] = mem[i];
	}
    }

    return len;
}

//------------------------------------------------------------------------------


static uint32_t cmd_write(sdp_msg_t *msg)
{
    uint32_t addr = msg->arg1;	// Address
    uint32_t len = msg->arg2;	// Length
    uint32_t type = msg->arg3;	// Type

    if (len > SDP_BUF_SIZE || type > TYPE_WORD) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint8_t *buffer = msg->data;

    if (type == TYPE_BYTE) {
	uint8_t *mem = (uint8_t *) addr;
	uint8_t *buf = (uint8_t *) buffer;

	for (uint32_t i = 0; i < len; i++) {
	    mem[i] = buf[i];
	}
    } else if (type == TYPE_HALF) {
	uint16_t *mem = (uint16_t *) addr;
	uint16_t *buf = (uint16_t *) buffer;

	for (uint32_t i = 0; i < len / 2; i++) {
	    mem[i] = buf[i];
	}
    } else {
	uint32_t *mem = (uint32_t *) addr;
	uint32_t *buf = (uint32_t *) buffer;

	for (uint32_t i = 0; i < len / 4; i++) {
	    mem[i] = buf[i];
	}
    }

    return 0;
}


//------------------------------------------------------------------------------

// arg1 = 0 : command : 0 : tag
// arg2 = timeout : port
// arg3 = IP

static uint32_t cmd_iptag(sdp_msg_t *msg)
{
    uint32_t op = msg->arg1 >> 16;
    uint32_t tag = msg->arg1 & 255;

    if (op > IPTAG_MAX || tag >= TAG_TABLE_SIZE) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    if (op == IPTAG_NEW || op == IPTAG_SET) {
	if (op == IPTAG_NEW) {
	    tag = iptag_new();
	}
	if (tag != TAG_NONE) {
	    iptag_t *tt = tag_table + tag;
	    uint32_t timeout = (msg->arg2 >> 16) & 15;

	    if (timeout != 0) {
		timeout = 1 << (timeout - 1);
	    }
	    tt->timeout = timeout;

	    tt->port = msg->arg2 & 0xffff;

	    uint8_t *ip_addr = (uint8_t *) &msg->arg3;
	    copy_ip(ip_addr, tt->ip);

	    tt->flags = IPTAG_ARP | timeout; // waiting for ARP
	    arp_lookup(tt);
	}

	msg->arg1 = tag;
	return 4;
    } else if (op == IPTAG_GET) {
	iptag_t *tt = tag_table + tag;
	uint32_t size = msg->arg2 * sizeof(iptag_t);

	if (size > SDP_BUF_SIZE) {
	    msg->cmd_rc = RC_ARG;
	    return 0;
	}

	memcpy(&msg->arg1, tt, size);
	return size;
    } else if (op == IPTAG_TTO) {
	msg->arg1 = (TAG_FIXED_SIZE << 24) + (TAG_POOL_SIZE << 16) + tag_tto;

	if (msg->arg2 < 16) {
	    tag_tto = msg->arg2;
	}
	return 4;
    } else {				// IPTAG_CLR
	tag_table[tag].flags = 0;
    }

    return 0;
}


//------------------------------------------------------------------------------


// arg1 = I2C ctrl byte
// arg2 = addr (not used in receive)
// arg3 = length

static uint32_t cmd_i2c(sdp_msg_t *msg)
{
    uint32_t count = msg->arg3;

    if (count > SDP_BUF_SIZE || !bp_ctrl) {
	msg->cmd_rc = RC_ARG;
	return 0;
    }

    uint32_t ctrl = msg->arg1;
    uint32_t addr = msg->arg2;

    if (ctrl > 256) {		// Poll
	msg->arg1 = i2c_poll (LPC_I2C0, ctrl & 255);
	return 4;
    } else if (ctrl & 1) {	// Read
	i2c_receive(LPC_I2C0, ctrl, addr, count, (void *) &msg->arg1);
	return count;
    } else {			// Write
	i2c_send(LPC_I2C0, ctrl, addr, count, msg->data);
	return 0;
    }
}


//------------------------------------------------------------------------------

extern void configure_pwm(uint32_t period, uint32_t width); //##

uint32_t debug(sdp_msg_t *msg)
{
    uint32_t len = msg->length;

    if (len < 24) {
	msg->cmd_rc = RC_LEN;
	return 0;
    }

    uint32_t t = msg->cmd_rc;
    msg->cmd_rc = RC_OK;

//    io_printf(IO_DBG, "-- BMP debug %u %x %x %x (%u)\n",
//	    t, msg->arg1, msg->arg2, msg->arg3, msg->length);

    switch (t) {
    case CMD_VER:
	return cmd_ver(msg);

    case CMD_READ:
	return cmd_read(msg);

    case CMD_WRITE:
	return cmd_write(msg);

    case CMD_FILL:
	return cmd_fill(msg);

    case CMD_LED:
	return cmd_led(msg);

    case CMD_FLASH_ERASE:
	return cmd_flash_erase(msg);

    case CMD_FLASH_WRITE:
	return cmd_flash_write(msg);

    case CMD_FLASH_COPY:
	boot_vec->flash_copy(msg->arg1, msg->arg2, msg->arg3, 0);
	return 0;

    case CMD_FPGA_READ:
	return cmd_fpga_read(msg);

    case CMD_FPGA_WRITE:
	return cmd_fpga_write(msg);

    case CMD_XILINX:
	return cmd_xilinx(msg);

    case CMD_RESET:
	return cmd_reset(msg);

    case CMD_POWER:
	return cmd_power(msg);

    case CMD_IPTAG:
	return cmd_iptag(msg);

    case CMD_BMP_SF:
	return cmd_sf(msg);

    case CMD_BMP_EE:
	return cmd_ee(msg);

    case CMD_BMP_INFO:
	return cmd_bmp_info(msg);

    case 61:
	return cmd_i2c(msg);

    case 62:
	configure_pwm(msg->arg1, msg->arg2);
	return 0;

    default:
	msg->cmd_rc = RC_CMD;
	return 0;
    }
}


//------------------------------------------------------------------------------
