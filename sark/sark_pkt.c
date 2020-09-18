//------------------------------------------------------------------------------
//! \file
//! \brief     Packet handling routines for SARK
//!
//! \copyright &copy; The University of Manchester - 2009-2013
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

#include <sark.h>

//------------------------------------------------------------------------------


extern INT_HANDLER txpkt_int_han(void);

void event_register_pkt(uint queue_size, vic_slot slot)
{
    if (queue_size <= 256 &&
            (queue_size & (queue_size - 1)) == 0 &&
            slot <= SLOT_MAX &&
            vic[VIC_CNTL + slot] == 0) {
        event.pkt_queue = sark_alloc(queue_size, sizeof(pkt_t));

        if (event.pkt_queue != NULL) {
            event.pkt_insert = 1;
            event.pkt_size = queue_size;

            sark_vic_set(slot, CC_TNF_INT, 0, txpkt_int_han);

            cc[CC_TCR] = PKT_MC;

            return;
        }
    }

    rt_error(RTE_PKT);
}


//------------------------------------------------------------------------------

// Transmit a packet (4 variants)


uint pkt_tx_kdc(uint key, uint data, uint ctrl)
{
    pkt_t pkt = {(ctrl << 16) + 3, data, key};

    uint cpsr = cpu_int_disable();

    if (event.pkt_count >= event.pkt_size) {
        cpu_int_restore(cpsr);
        return 0;
    }

    if (event.pkt_count == 0) {
        vic[VIC_ENABLE] = 1 << CC_TNF_INT;
    }

    event.pkt_count++;

    if (event.pkt_count > event.pkt_max) {
        event.pkt_max = event.pkt_count;
    }

    event.pkt_queue[event.pkt_insert] = pkt;
    event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

    cpu_int_restore(cpsr);

    return 1;
}


uint pkt_tx_kd(uint key, uint data)
{
    pkt_t pkt = {2, data, key};

    uint cpsr = cpu_int_disable();

    if (event.pkt_count >= event.pkt_size) {
        cpu_int_restore(cpsr);
        return 0;
    }

    if (event.pkt_count == 0) {
        vic[VIC_ENABLE] = 1 << CC_TNF_INT;
    }

    event.pkt_count++;

    if (event.pkt_count > event.pkt_max) {
        event.pkt_max = event.pkt_count;
    }

    event.pkt_queue[event.pkt_insert] = pkt;
    event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

    cpu_int_restore(cpsr);

    return 1;
}


uint pkt_tx_k(uint key)
{
    pkt_t pkt = {0, 0, key};

    uint cpsr = cpu_int_disable();

    if (event.pkt_count >= event.pkt_size) {
        cpu_int_restore(cpsr);
        return 0;
    }

    if (event.pkt_count == 0) {
        vic[VIC_ENABLE] = 1 << CC_TNF_INT;
    }

    event.pkt_count++;
    if (event.pkt_count > event.pkt_max) {
        event.pkt_max = event.pkt_count;
    }

    event.pkt_queue[event.pkt_insert] = pkt;
    event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

    cpu_int_restore(cpsr);

    return 1;
}


uint pkt_tx_kc(uint key, uint ctrl)
{
    pkt_t pkt = {(ctrl << 16) + 1, 0, key};

    uint cpsr = cpu_int_disable();

    if (event.pkt_count >= event.pkt_size) {
        cpu_int_restore(cpsr);
        return 0;
    }

    if (event.pkt_count == 0) {
        vic[VIC_ENABLE] = 1 << CC_TNF_INT;
    }

    event.pkt_count++;
    if (event.pkt_count > event.pkt_max) {
        event.pkt_max = event.pkt_count;
    }

    event.pkt_queue[event.pkt_insert] = pkt;
    event.pkt_insert = (event.pkt_insert + 1) & (event.pkt_size - 1);

    cpu_int_restore(cpsr);

    return 1;
}


//------------------------------------------------------------------------------
