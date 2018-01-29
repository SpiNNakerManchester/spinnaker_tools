//------------------------------------------------------------------------------
//
// scamp-isr.c	    SC&MP interrupt routines
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

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

static uint centi_ms;	// Counts 0 to 9 in ms

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
  "	.arm \n\
	.global	eth_receive \n\
	.equ    MODE_SYS, 0x1f \n\
	.equ    MODE_IRQ, 0x12 \n\
	.equ	IMASK_IRQ, 0x80 \n\
	.equ	VIC_BASE, 0x1f000000 \n\
	.equ	VIC_VADDR, 0x30 \n\
    	sub  	lr, lr, #4 \n\
    	stmfd  	sp!, {r0, lr} \n\
    	mrs    	lr, spsr \n\
    	stmfd  	sp!, {r12, lr} \n\
    	msr    	cpsr_c, #MODE_SYS \n\
    	stmfd  	sp!, {r1-r3, lr} \n\
    	bl     	eth_receive \n\
    	ldmfd  	sp!, {r1-r3, lr} \n\
    	msr    	cpsr_c, #MODE_IRQ+IMASK_IRQ \n\
	mov    	r12, #VIC_BASE \n\
	str    	r12, [r12, #VIC_VADDR] \n\
    	ldmfd  	sp!, {r12, lr} \n\
    	msr    	spsr_cxsf, lr \n\
    	ldmfd  	sp!, {r0, pc}^ \n\
  " :::);
}
#else
__asm void eth_rx_int(void)
{
	code32
	import	eth_receive
	preserve8

    	sub  	lr, lr, #4		;; Adjust LR_irq and save
    	stmfd  	sp!, {r0, lr}		;; with r0

    	mrs    	lr, spsr		;; Get SPSR_irq to LR
    	stmfd  	sp!, {r12, lr}  	;; Save SPSR & r12

    	msr    	cpsr_c, #MODE_SYS    	;; Go to SYS mode, interrupts enabled

    	stmfd  	sp!, {r1-r3, lr} 	;; Save working regs and LR_sys

    	bl     	eth_receive

    	ldmfd  	sp!, {r1-r3, lr} 	;; Restore working regs & LR_sys

    	msr    	cpsr_c, #MODE_IRQ+IMASK_IRQ ; Back to IRQ mode, IRQ disabled

	mov    	r12, #VIC_BASE		;; Tell VIC we are done
	str    	r12, [r12, #VIC_VADDR * 4]

    	ldmfd  	sp!, {r12, lr} 	      	;; Restore r12 & SPSR_irq
    	msr    	spsr_cxsf, lr

    	ldmfd  	sp!, {r0, pc}^       	;; and return restoring r0
}
#endif


//------------------------------------------------------------------------------


INT_HANDLER pkt_mc_int()
{
    uint data = cc[CC_RXDATA];
    uint key = cc[CC_RXKEY];

    // Checksum ??

    if (key == 0xffff5555) {
	signal_app(data);
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

    if (ctrl & PKT_PL) {			// Has payload
	if (key & 1) { 				// Reply to peek
	    peek_ack_pkt(link, data, key);
	} else {				// SNP protocol
	    nn_rcv_pkt(link, data, key);
	}
    } else {					// No payload
	if (key & 1) { 				// Reply to poke
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


INT_HANDLER ms_timer_int()
{
    tc[T1_INT_CLR] = (uint) tc;    	// Clear interrupt

    sv->clock_ms++;
    uint ms = sv->time_ms + 1;
    if (ms == 1000) {
	ms = 0;
	sv->unix_time++;

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
	vcpu->mbox_mp_cmd = SHM_IDLE;

	sdp_msg_t *msg = sark_msg_get();

	if (msg != NULL) {
	    sark_msg_cpy(msg, shm_msg);
	    msg_queue_insert(msg, 0);
	} else {
	    sw_error(SW_OPT);
        }

	sark_shmsg_free(shm_msg);
    } else {	//## Hook for other commands...
	vcpu->mbox_mp_cmd = SHM_IDLE;
	sw_error(SW_OPT);
    }

    vic[VIC_VADDR] = (uint) vic;
}


//------------------------------------------------------------------------------


extern INT_HANDLER timer2_int_han(void);


void vic_setup(void)
{
    tc[T2_CONTROL] = 0;	// Disable timer2

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
