//------------------------------------------------------------------------------
//
// spinn_srom.c	    Serial ROM interface routines for Spinnaker
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------


#include "spinnaker.h"
#include "sark.h"

// Serial ROM command codes

#define SROM_READ  	(0x03U << 24)
#define SROM_WRITE 	(0x02U << 24)
#define SROM_WREN  	(0x06U << 24)
#define SROM_WRDI  	(0x04U << 24)
#define SROM_RDSR  	(0x05U << 24)
#define SROM_WRSR  	(0x01U << 24)
#define SROM_PE    	(0x42U << 24)
#define SROM_SE    	(0xd8U << 24)
#define SROM_CE    	(0xc7U << 24)
#define SROM_RDID  	(0xabU << 24)
#define SROM_DPD   	(0xb9U << 24)

#define WAIT 0x40
#define WREN 0x80
#define WRITE 0x100


//------------------------------------------------------------------------------


static void ncs_low()
{
    sc[GPIO_CLR] = SERIAL_NCS;
    sark_delay_us(1);
}


static void ncs_high()
{
    sark_delay_us(1);
    sc[GPIO_SET] = SERIAL_NCS;
    sark_delay_us(1);
}


static void clock()
{
    sark_delay_us(1);
    sc[GPIO_SET] = SERIAL_CLK;
    sark_delay_us(1);
    sc[GPIO_CLR] = SERIAL_CLK;
}


static void send(uint v, uint n)
{
    do {
	if (v & 0x80000000) {
	    sc[GPIO_SET] = SERIAL_SI;
	} else {
            sc[GPIO_CLR] = SERIAL_SI;
	}

	clock();

	v = v << 1;
	n--;
    } while (n != 0);

    sc[GPIO_CLR] = SERIAL_SI;
}


static uint read8()
{
    uint r = 0;
    uint n = 8;

    do {
	uint p = sc[GPIO_READ];
	if (p & SERIAL_SO) {
	    r |= 1;
	}

	clock();

	r = r << 1;
	n--;
    } while (n != 0);

    return r >> 1;
}


//------------------------------------------------------------------------------

/*
  General purpose SROM interface routine

    arg1[31:16] = Data length (or zero)
    arg1[8] = Write (1) or Read (0)
    arg1[7] = Send WREN command first
    arg1[6] = Wait for completion
    arg1[5:0] = Number of command bits (8,16,32)

    arg2[31:24] = SROM command
    arg2[23:0] = extra command bits
*/

uint cmd_srom(sdp_msg_t *msg)
{
    uint arg1 = msg->arg1;
    uint port = sc[GPIO_PORT];	// Preserve output state

    ncs_high();

    if (arg1 & WREN) {
	ncs_low();
	send(SROM_WREN, 8);
	ncs_high();
    }

    ncs_low();

    send(msg->arg2, arg1 & 63);

    uint len = arg1 >> 16;
    uchar* wbuf = msg->data;
    uchar* rbuf = (uchar *) &msg->arg1;

    while (len) {
	if (arg1 & WRITE) {
	    send(*wbuf++ << 24, 8);
        } else {
            *rbuf++ = read8();
        }

	len--;
    }

    ncs_high();

    while (arg1 & WAIT) {
	ncs_low();
	send(SROM_RDSR, 8);
	uint sr = read8();
	ncs_high();

	if ((sr & 1) == 0) {
	    break;
	}
    }

    sc[GPIO_PORT] = port;

    if (arg1 & WRITE) {
	return 0;
    }

    return arg1 >> 16;
}
