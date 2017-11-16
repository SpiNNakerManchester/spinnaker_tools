//------------------------------------------------------------------------------
//
// bmp_io.c	    Simple character I/O library for BMP
//
// Copyright (C)    The University of Manchester - 2009-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


#include <stdarg.h>
#include <string.h>

#include "lpc17xx.h"
#include "bmp.h"

//------------------------------------------------------------------------------

extern void eth_putc (uint32_t c);

//------------------------------------------------------------------------------

static const char hex[] = "0123456789abcdef";

static uint32_t sp_ptr; 		// for 'sprintf'
static uint32_t tube_ptr;
static uint8_t tube_buf[256];

//------------------------------------------------------------------------------

static void std_putc(uint32_t c)
{
    tube_buf[tube_ptr++] = c;

    if (c != 0 && c != '\n' && tube_ptr != 256) {
	return;
    }

    sdp_msg_t *msg = msg_get();

    if (msg != NULL) {
	memcpy(&msg->arg1, tube_buf, tube_ptr);

	msg->cmd_rc = CMD_TUBE;
	msg->seq = 0;
	msg->flags = 0x07;
	msg->tag = TAG_HOST;
	// msg->dest_port = PORT_ETH;		// Sent via board 0
	msg->dest_port = PORT_ETH + board_ID;	// Sent from this board
	msg->srce_port = board_ID;
	msg->dest_addr = 0;
	msg->srce_addr = ee_data.frame_ID;
	msg->length = 12 + tube_ptr;

	msg_queue_insert(msg);
    }

    tube_ptr = 0;
}

//------------------------------------------------------------------------------

static void io_put_char(char *stream, uint32_t c)
{
    if (stream == IO_STD) {
	std_putc(c);
    } else if (stream == IO_DBG) {
	eth_putc(c);
    } else if (stream == IO_LCD) {
	lcd_putc(c);
    } else if (stream > IO_NULL) {
	stream[sp_ptr++] = c;
	stream[sp_ptr] = 0;
    }
}

static void io_put_str(char *stream, char *s, int d)
{
    char *t = s;
    int n = 0;

    while (*t++) {
	n++;
    }
    while (d-- > n) {
	io_put_char(stream, ' ');
    }
    while (*s) {
	io_put_char(stream, *s++);
    }
}

// pad not used!
static void io_put_int(char *stream, int n, uint32_t d, uint32_t pad)
{
    char s[16];
    int i = 0;
    uint32_t neg = 0;

    if (n < 0) {
	n = -n;
	neg = '-';
    }

    while (1) {
	s[i++] = n % 10 + '0';
	n = n / 10;
	if (n == 0) {
	    break;
	}
    }

    while (i > 0 && s[--i] == '0') {
	continue;
    }
    if (neg) {
	s[++i] = neg;
    }
    while (d-- > i + 1) {
	io_put_char(stream, ' ');
    }
    while (i >= 0) {
	io_put_char(stream, s[i--]);
    }
}

static void io_put_uint(char *stream, uint32_t n, uint32_t d, uint32_t pad)
{
    char s[16];
    int i = 0;

    while (1) {
	s[i++] = n % 10 + '0';
	n = n / 10;
	if (n == 0) {
	    break;
	}
    }

    while (i > 0 && s[--i] == '0') {
	continue;
    }
    while (d-- > i + 1) {
	io_put_char(stream, pad);
    }
    while (i >= 0) {
	io_put_char(stream, s[i--]);
    }
}

static void io_put_zhex(char *stream, uint32_t n, uint32_t d)
{
    for (int i = d - 1; i >= 0; i--) {
	io_put_char(stream, hex[(n >> (4 * i)) & 15]);
    }
}

static void io_put_hex(char *stream, uint32_t n, uint32_t d, uint32_t pad)
{
    char s[16];
    int i = 0;

    while (1) {
	s[i++] = hex[n & 15];
	n = n >> 4;
	if (n == 0) {
	    break;
	}
    }

    while (i > 0 && s[--i] == '0') {
	continue;
    }

    while (d-- > i + 1) {
	io_put_char(stream, pad);
    }

    while (i >= 0) {
	io_put_char(stream, s[i--]);
    }
}


static void io_put_mac(char *stream, uint8_t *s)
{
    for (uint32_t i = 0; i < 6; i++) {
	io_put_zhex(stream, s[i], 2);
	if (i != 5) {
	    io_put_char(stream, ':');
	}
    }
}


static void io_put_ip(char *stream, uint8_t *s)
{
    for (uint32_t i = 0; i < 4; i++) {
	io_put_int(stream, s[i], 0, 0);
	if (i != 3) {
	    io_put_char(stream, '.');
	}
    }
}


void io_printf(char *stream, char *f, ...)
{
    va_list ap;

    if (stream == IO_NULL || (stream == IO_LCD && ! lcd_active)) {
	return;
    }

    if (stream > IO_NULL) {	// Initialise string (sprintf)
	sp_ptr = stream[0] = 0;
    }

    va_start(ap, f);

    while (1) {
	char c = *f++;

	if (c == 0) {
	    break;
	}

	if (c != '%') {
	    io_put_char(stream, c);
	    continue;
	}

	c = *f++;

	if (c == 0) {
	    break;
	}

	char pad = ' ';
	if (c == '0') {
	    pad = c;
	}

	uint32_t size = 0;

	while (c >= '0' && c <= '9') {
	    size = 10 * size + c - '0';
	    c = *f++;
	}

	if (c == 0) {
	    break;
	}

	switch (c) {
	case 'c': // character
	    io_put_char(stream, va_arg(ap, uint32_t));
	    break;

	case 's': // string
	    io_put_str(stream, va_arg(ap, char *), size);
	    break;

	case 'd': //signed integer
	    io_put_int(stream, va_arg(ap, int), size, pad);
	    break;

	case 'u': // unsigned integer
	    io_put_uint(stream, va_arg(ap, uint32_t), size, pad);
	    break;

	case 'x': // hex, digits as needed
	    io_put_hex(stream, va_arg(ap, uint32_t), size, pad);
	    break;

	case 'z': // zero prefixed hex, exactly "size" digits
	    io_put_zhex(stream, va_arg(ap, uint32_t), size);
	    break;

	case 'p': // pointer to IP address
	    io_put_ip(stream, va_arg(ap, uint8_t *));
	    break;

	case 'm': // pointer to MAC address
	    io_put_mac(stream, va_arg(ap, uint8_t *));
	    break;

	case 'q': // LCD control code
	    if (stream == IO_LCD) {
		lcd_ctrl(va_arg(ap, uint32_t));
	    }
	    break;

	default:
	    io_put_char(stream, c);
	}
    }

    va_end(ap);
}
