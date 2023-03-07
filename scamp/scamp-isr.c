//------------------------------------------------------------------------------
//! \file
//! \brief     SC&MP interrupt routines
//!
//! \copyright &copy; The University of Manchester - 2009, 2010
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "spinnaker.h"
#include "sark.h"
#include "scamp.h"
#include <scamp_spin1_sync.h>

//! Multicast packet handler uses the FIQ VIC slot
#define MC_SLOT SLOT_FIQ


//------------------------------------------------------------------------------

extern pkt_queue_t tx_pkt_queue;

extern void p2p_region(uint data, uint key);
extern void p2p_rcv_data(uint data, uint key);
extern void p2p_rcv_ctrl(uint data, uint key);
extern void nn_rcv_pkt(uint link, uint data, uint key);
extern void peek_ack_pkt(uint link, uint data, uint key);
extern void poke_ack_pkt(uint link, uint data, uint key);

extern void proc_1khz(uint a1, uint a2);
extern void proc_100hz(uint a1, uint a2);
extern void proc_1hz(uint a1, uint a2);

extern void msg_queue_insert(sdp_msg_t *msg, uint srce_ip);

//------------------------------------------------------------------------------

extern uchar v2p_map[MAX_CPUS];
extern uint num_cpus;
extern volatile uint do_sync;

static uint centi_ms;   //!< Counts 0 to 9 in ms

//------------------------------------------------------------------------------

//! Packet transmit ready handler
INT_HANDLER pkt_tx_int(void) // SPIN2 - optimise for register order??
{
    pkt_queue_t *txq = &tx_pkt_queue;
    txq->remove = (txq->remove + 1) % PKT_QUEUE_SIZE;

    pkt_t *pkt = txq->queue + txq->remove;

    cc[CC_TCR] = pkt->ctrl;
    if (pkt->ctrl & PKT_PL) {
        cc[CC_TXDATA] = pkt->data;
    }
    cc[CC_TXKEY] = pkt->key;

    txq->count--;
    if (txq->count == 0) {
        vic[VIC_DISABLE] = 1 << CC_TMT_INT;
    }

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Ethernet packet received handler. Delegates to eth_receive()
#ifdef __GNUC__
void eth_rx_int(void)
{
    asm volatile (
    "   .arm \n\
        .global eth_receive \n\
        .equ    MODE_SYS, 0x1f \n\
        .equ    MODE_IRQ, 0x12 \n\
        .equ    IMASK_IRQ, 0x80 \n\
        .equ    VIC_BASE, 0x1f000000 \n\
        .equ    VIC_VADDR, 0x30 \n\
        sub     lr, lr, #4 \n\
        stmfd   sp!, {r0, lr} \n\
        mrs     lr, spsr \n\
        stmfd   sp!, {r12, lr} \n\
        msr     cpsr_c, #MODE_SYS \n\
        stmfd   sp!, {r1-r3, lr} \n\
        bl      eth_receive \n\
        ldmfd   sp!, {r1-r3, lr} \n\
        msr     cpsr_c, #MODE_IRQ+IMASK_IRQ \n\
        mov     r12, #VIC_BASE \n\
        str     r12, [r12, #VIC_VADDR] \n\
        ldmfd   sp!, {r12, lr} \n\
        msr     spsr_cxsf, lr \n\
        ldmfd   sp!, {r0, pc}^ \n\
    " :::);
}
#else
__asm void eth_rx_int(void)
{
        code32
        import  eth_receive
        preserve8

        sub     lr, lr, #4              ;; Adjust LR_irq and save
        stmfd   sp!, {r0, lr}           ;; with r0

        mrs     lr, spsr                ;; Get SPSR_irq to LR
        stmfd   sp!, {r12, lr}          ;; Save SPSR & r12

        msr     cpsr_c, #MODE_SYS       ;; Go to SYS mode, interrupts enabled

        stmfd   sp!, {r1-r3, lr}        ;; Save working regs and LR_sys

        bl      eth_receive

        ldmfd   sp!, {r1-r3, lr}        ;; Restore working regs & LR_sys

        msr     cpsr_c, #MODE_IRQ+IMASK_IRQ ;; Back to IRQ mode, IRQ disabled

        mov     r12, #VIC_BASE          ;; Tell VIC we are done
        str     r12, [r12, #VIC_VADDR * 4]

        ldmfd   sp!, {r12, lr}          ;; Restore r12 & SPSR_irq
        msr     spsr_cxsf, lr

        ldmfd   sp!, {r0, pc}^          ;; and return restoring r0
}
#endif


//------------------------------------------------------------------------------
//! \brief Maximum difference between timers over 2 seconds in clock ticks.
//! \details Experiments have shown maximum difference is about 1ms over 160
//! 	seconds which is 6.25us over 1 second, which is 2500 clock ticks at
//! 	200Mhz.  This is multiplied by 2 as drift could be in either
//! 	direction.  Further experiments show that slightly higher values are
//! 	encountered, so 10000 is used.
#define MAX_DIFF 10000
//! Number of samples to keep to get an average
#define N_ITEMS 16
//! The mask of the n_beacons part of the message
#define N_BEACONS_MASK 0xFFF
//! The shift of the n_beacons part of the message
#define N_BEACONS_SHIFT 20
//! \brief The mask of the source tc[T1_COUNT] part of the message.
//! \details The timer is set to 200,000 which fits in the bottom 20 bits.
#define SOURCE_T1_COUNT_MASK 0xFFFFF
//! Samples used when synchronising time
static int samples[N_ITEMS];
//! Sum to make moving average easy
static int sum = 0;
//! Ticks recorded last time
static int last_ticks = 0;
//! Number of samples recorded
static int n_samples = 0;
//! Position of next sample
static uint sample_pos = 0;
//! Beacon id recorded last time to detect missed packets
static int last_beacon = 0;

//! Multicast packet received handler. Delegates to signal_app()
INT_HANDLER pkt_mc_int(void)
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];

    // Checksum ??

    if (key == SCAMP_MC_SIGNAL_KEY) {
        signal_app(data);
    } else if (key == SCAMP_MC_TIME_SYNC_KEY && netinit_phase == NETINIT_PHASE_DONE) {
        // Timer synchronisation - only do once netinit phase is complete to
        // avoid clashing with other network traffic.
        int ticks = tc[T1_COUNT];
        uint beacon = (data >> N_BEACONS_SHIFT) & N_BEACONS_MASK;
        uint source_ticks = data & SOURCE_T1_COUNT_MASK;
        // Where are our ticks in relation to the source?
        ticks = source_ticks - ticks;

        if (n_samples == 0) {
            // If there are no samples, just record
            last_ticks = ticks;
            last_beacon = beacon;
            n_samples++;
        } else {
            // Note maximum difference over 2 seconds is quite big but
            // once divided into per-us value, it will be small again.
            int diff = ticks - last_ticks;
            uint n_beacons = beacon - last_beacon;
            last_ticks = ticks;
            last_beacon = beacon;
            // Only look if the last beacon was received.  A noisy network where
            // beacons go missing is likely to give false results anyway.
            if ((diff <= MAX_DIFF) && (diff >= -MAX_DIFF) && (n_beacons == 1)) {
                // Enough samples now, so do the difference
                int scaled_diff = ((diff * DRIFT_FP_FACTOR) + DRIFT_HALF) /
                        TIME_BETWEEN_SYNC_INT;
                sum = (sum - samples[sample_pos]) + scaled_diff;
                samples[sample_pos] = scaled_diff;
                sample_pos = (sample_pos + 1) % N_ITEMS;
                n_samples++;
                // Just use the actual value until there are enough to average;
                // using the average early on tends to lead to odd values, so
                // this seems to work better in experiments
                if (n_samples > N_ITEMS) {
                    sv->clock_drift = sum / N_ITEMS;
                } else {
                    sv->clock_drift = scaled_diff;
                }
            }
        }
    }

#if MC_SLOT != SLOT_FIQ
    vic[VIC_VADDR] = (uint) vic;
#endif
}

//! \brief Nearest-neighbour packet received handler
//!
//! Delegates to one of:
//! * nn_rcv_pkt()
//! * peek_ack_pkt()
//! * poke_ack_pkt()
INT_HANDLER pkt_nn_int(void)
{
    uint ctrl = cc[CC_RSR];
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];
    uint link = (ctrl >> 24) & 7;

    if (ctrl & PKT_PL) {                        // Has payload
        if (key & 1) {                          // Reply to peek
            peek_ack_pkt(link, data, key);
        } else {                                // SNP protocol
            nn_rcv_pkt(link, data, key);
        }
    } else {                                    // No payload
        if (key & 1) {                          // Reply to poke
            poke_ack_pkt(link, data, key);
        } else {
            // not used
        }
    }

    vic[VIC_VADDR] = (uint) vic;
}

//! \brief Peer-to-peer packet received handler
//!
//! Delegates to one of:
//! * p2p_region()
//! * p2p_rcv_data()
//! * p2p_rcv_ctrl()
INT_HANDLER pkt_p2p_int(void)
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];
    uint p2p_type = (key >> 16) & 3;
    key &= 0xffff;

    if (p2p_type == P2P_LEVEL) {
        p2p_region(data, key);
    } else if (p2p_type == P2P_DATA) {
        p2p_rcv_data(data, key);
    } else if (p2p_type == P2P_CTRL) {
        p2p_rcv_ctrl(data, key);
    }

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Time beacon counter
uint n_beacons_sent = 0;
//! Inter-synchronisation time, in microseconds
static uint time_to_next_sync = TIME_BETWEEN_SYNC_US;

//! \brief Millisecond timer interrupt handler
//!
//! Delegates (with appropriate frequency) to:
//! * proc_1khz()
//! * proc_100hz()
//! * proc_1hz()
INT_HANDLER ms_timer_int(void)
{
    tc[T1_INT_CLR] = (uint) tc;         // Clear interrupt

    // Send the sync signal if appropriate
    if ((sv->p2p_root == sv->p2p_addr)
            && (netinit_phase == NETINIT_PHASE_DONE) && do_sync) {
        if (time_to_next_sync == 0) {
            uint data = (n_beacons_sent & N_BEACONS_MASK) << N_BEACONS_SHIFT;
            data |= tc[T1_COUNT] & SOURCE_T1_COUNT_MASK;
            pkt_tx(PKT_MC_PL, data, SCAMP_MC_TIME_SYNC_KEY);
            n_beacons_sent++;
            time_to_next_sync = TIME_BETWEEN_SYNC_US;
        }
        // Take of 1000us per timer tick, assuming the timer is in ms
        time_to_next_sync -= 1000;
    }

    sv->clock_ms++;
    uint ms = sv->time_ms + 1;
    uint unix_time = sv->unix_time;
    if (ms == 1000) {
        ms = 0;
        unix_time++;
        sv->unix_time = unix_time;

        if (!event_queue_proc(proc_1hz, 0, 0, PRIO_2)) { // !!const
            sw_error(SW_OPT);
        }
    }

    sv->time_ms = ms;
    uint cs = centi_ms + 1;
    if (cs == 10) {
        cs = 0;

        if (!event_queue_proc(proc_100hz, 0, 0, PRIO_1)) { // !!const
            sw_error(SW_OPT);
        }
    }

    if (!event_queue_proc(proc_1khz, 0, 0, PRIO_1)) {// !!const
        sw_error(SW_OPT);
    }
    centi_ms = cs;

    if (sv->tp_scale != 0) {
        uint timer = ++sv->tp_timer;
        uint tp = (timer >> sv->tp_scale) & 3;

        rtr[RTR_CONTROL] = (rtr[RTR_CONTROL] & ~0xc0) | (tp << 6);
    }

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------

//! Round robin counter for ap_int()
static uint next_box;

//! Application (local core) message available handler
INT_HANDLER ap_int(void)
{
    do {
        next_box++;
        if (next_box >= num_cpus) {
            next_box = 0;
        }
    } while ((sv->mbox_flags & (1 << next_box)) == 0);

    vcpu_t *vcpu = sv_vcpu + next_box;
    sdp_msg_t *shm_msg = vcpu->mbox_mp_msg;
    uint cmd = vcpu->mbox_mp_cmd;

    uint cpsr = sark_lock_get(LOCK_MBOX);
    sv->mbox_flags &= ~(1 << next_box);
    if (sv->mbox_flags == 0) {
        sc[SC_CLR_IRQ] = SC_CODE + (1 << sark.phys_cpu);
    }
    sark_lock_free(cpsr, LOCK_MBOX);

    if (cmd == SHM_MSG) {

        sdp_msg_t *msg = sark_msg_get();

        if (msg != NULL) {
            sark_msg_cpy(msg, shm_msg);
            sark_shmsg_free(shm_msg);
            vcpu->mbox_mp_cmd = SHM_IDLE;
            msg_queue_insert(msg, 0);
        } else {
            // If we didn't get a message, we are just going to have to throw
            // this one away...
            sark_shmsg_free(shm_msg);
            vcpu->mbox_mp_cmd = SHM_IDLE;
            sw_error(SW_OPT);
        }

    } else {    //## Hook for other commands...
        vcpu->mbox_mp_cmd = SHM_IDLE;
        sw_error(SW_OPT);
    }

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


extern INT_HANDLER timer2_int_han(void);

//! Initialise the VIC
void vic_setup(void)
{
    tc[T2_CONTROL] = 0; // Disable timer2

    sark_vic_set(MC_SLOT, CC_MC_INT, 1, pkt_mc_int);

    sark_vic_set(SLOT_1, CC_NN_INT, 1, pkt_nn_int);
    sark_vic_set(SLOT_2, CC_P2P_INT, 1, pkt_p2p_int);
    sark_vic_set(SLOT_3, CC_TMT_INT, 0, pkt_tx_int);
    sark_vic_set(SLOT_4, TIMER2_INT, 1, timer2_int_han);
    sark_vic_set(SLOT_5, ETH_RX_INT, 1, (int_handler) eth_rx_int);
    sark_vic_set(SLOT_6, TIMER1_INT, 1, ms_timer_int);
    sark_vic_set(SLOT_7, CPU_INT,    1, ap_int);
}


//------------------------------------------------------------------------------
