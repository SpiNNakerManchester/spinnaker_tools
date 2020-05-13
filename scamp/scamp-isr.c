//------------------------------------------------------------------------------
//
// scamp-isr.c      SC&MP interrupt routines
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
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
#include "scamp.h"


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

static uint centi_ms;   // Counts 0 to 9 in ms

//------------------------------------------------------------------------------


INT_HANDLER pkt_tx_int() // SPIN2 - optimise for register order??
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


#ifdef __GNUC__
void eth_rx_int(void)
{
  asm volatile (
  "     .arm \n\
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

        msr     cpsr_c, #MODE_IRQ+IMASK_IRQ ; Back to IRQ mode, IRQ disabled

        mov     r12, #VIC_BASE          ;; Tell VIC we are done
        str     r12, [r12, #VIC_VADDR * 4]

        ldmfd   sp!, {r12, lr}          ;; Restore r12 & SPSR_irq
        msr     spsr_cxsf, lr

        ldmfd   sp!, {r0, pc}^          ;; and return restoring r0
}
#endif


//------------------------------------------------------------------------------
// Maximum difference between timers over 2 seconds in clock ticks; experiments
// have shown maximum difference is about 1ms over 160 seconds which is
// 6.25us over 1 second, which is 2500 clock ticks at 200Mhz.  This is
// multiplied by 2 as drift could be in either direction.  Further experiments
// show that slightly higher values are encountered, so 10000 is used.
#define MAX_DIFF 10000

// Number of samples to keep to get an average
#define N_ITEMS 16
static int samples[N_ITEMS];
// Sum to make moving average easy
static int sum = 0;
// Ticks recorded last time
static int last_ticks = 0;
// Number of samples recorded
static uint n_samples = 0;
// Position of next sample
static uint sample_pos = 0;
// Beacon id recorded last time to detect missed packets
static int last_beacon = 0;

INT_HANDLER pkt_mc_int()
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];

    // Checksum ??

    if (key == 0xffff5555) {
        signal_app(data);
    } else if (key == 0xffff5554 && netinit_phase == NETINIT_PHASE_DONE) {
        // Timer synchronisation - only do once netinit phase is complete to
        // avoid clashing with other network traffic.
        int ticks = tc[T1_COUNT];
        if (n_samples == 0) {
            // If there are no samples, take one now, but don't do anything else
            last_ticks = ticks;
            last_beacon = data;
            n_samples += 1;
        } else {
            // Note maximum difference over 2 seconds is quite big but
            // once divided into per-us value, it will be small again.
            // We need a 64-bit int to represent the value before division
            // but this will fit in a 32-bit value once divided.
            int diff = last_ticks - ticks;
            int n_beacons = data - last_beacon;
            last_ticks = ticks;
            last_beacon = data;
            // Note, we store ticks even if out of range; this should help when
            // things are moving but going to converge again on a different
            // value.  This might mean that we ignore more than one value if
            // there is a lot of jitter, but this is OK.
            if ((diff <= MAX_DIFF) && (diff >= -MAX_DIFF) && (n_beacons > 0)) {
                // Enough samples now, so do the difference
                int scaled_diff = (diff << DRIFT_FP_BITS)
                        / (n_beacons * TIME_BETWEEN_SYNC_US);
                sum = (sum - samples[sample_pos]) + scaled_diff;
                samples[sample_pos] = scaled_diff;
                sample_pos = (sample_pos + 1) % N_ITEMS;
                n_samples += 1;
                // Just use the actual value until there are enough to average;
                // using the average early on tends to lead to odd values, so
                // this seems to work better in experiments
                if (n_samples >= N_ITEMS) {
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


INT_HANDLER pkt_nn_int()
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


INT_HANDLER pkt_p2p_int()
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

static uint n_beacons_sent = 0;
static uint time_to_next_sync = TIME_BETWEEN_SYNC_US;

INT_HANDLER ms_timer_int()
{
    tc[T1_INT_CLR] = (uint) tc;         // Clear interrupt
    
    // Send the sync signal if appropriate
    if ((sv->p2p_root == sv->p2p_addr)
            && (netinit_phase == NETINIT_PHASE_DONE)) {
        if (time_to_next_sync == 0) {
            pkt_tx(PKT_MC_PL, n_beacons_sent, 0xffff5554);
            n_beacons_sent += 1;
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
        unix_time += 1;
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


uint next_box;

INT_HANDLER ap_int()
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
