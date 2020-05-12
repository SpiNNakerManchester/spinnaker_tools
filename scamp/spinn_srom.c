//------------------------------------------------------------------------------
//! \file
//! \brief     Serial ROM interface routines for Spinnaker
//!
//! \copyright &copy; The University of Manchester - 2009, 2010
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009-2019 The University of Manchester
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


#include "spinnaker.h"
#include "sark.h"

//! Serial ROM command codes
enum spinn_srom_commands {
    SROM_READ =     (0x03U << 24),
    SROM_WRITE =    (0x02U << 24),
    SROM_WREN =     (0x06U << 24),
    SROM_WRDI =     (0x04U << 24),
    SROM_RDSR =     (0x05U << 24),
    SROM_WRSR =     (0x01U << 24),
    SROM_PE =       (0x42U << 24),
    SROM_SE =       (0xd8U << 24),
    SROM_CE =       (0xc7U << 24),
    SROM_RDID =     (0xabU << 24),
    SROM_DPD =      (0xb9U << 24)
};

//! Flag bits within the SCP message
enum spinn_srom_scp_flag_bits {
    WAIT = 0x40,
    WREN = 0x80,
    WRITE = 0x100
};

//------------------------------------------------------------------------------

//! Set the NCS line low
static void ncs_low(void)
{
    sc[GPIO_CLR] = SERIAL_NCS;
    sark_delay_us(1);
}

//! Set the NCS line high
static void ncs_high(void)
{
    sark_delay_us(1);
    sc[GPIO_SET] = SERIAL_NCS;
    sark_delay_us(1);
}

//! Cycle the clock line on the SROM
static void clock(void)
{
    sark_delay_us(1);
    sc[GPIO_SET] = SERIAL_CLK;
    sark_delay_us(1);
    sc[GPIO_CLR] = SERIAL_CLK;
}

//! \brief Send data to the SROM
//! \param[in] v: Value to send, in top \p n bits
//! \param[in] n: Number of bits to send
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

//! \brief Read a byte from the SROM (must have been requested)
//! \return The byte that was read.
static uint read8(void)
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
    uint port = sc[GPIO_PORT];  // Preserve output state

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
