//------------------------------------------------------------------------------
//
// scamp.c	    SC&MP - Spinnaker Control & Monitor Program
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// TODO
//
//   FF to allow soft reset of APs
//   Sort out pre/post delays for NN fwd
//   FF to filter by P2P address?
//   Make DBG output interrupt safe & buffer frugal
//   Sleep mode command??
//   Fix P2P checksum
//   Fatal error on no free msgs or inserting into full queue
//   Deal with Ethernet coming & going?
//
// Done
//   20sep10 - Worry about PHY start-up
//   21jun10 - Hop tables to SDRAM (made smaller - now in DTCM)
//   14jun10 - Fix TAG_TABLE_SIZE define (now passed to arp_pkt)
//   14jun10 - Transient tag not needed if reply bit not set in flags
//   14jun10 - Update peripheral tests for SPIN2
//   14jun10 - SPIN2 compatibility
//   10jun10 - Merge int off/on into lock_get/free
//   10jun10 - Implement NN packet to reset ID maps
//   09jun10 - APLX at FF end
//   09jun10 - Return RCs from xxx_send_msg
//   09jun10 - Drive SHM msgs via interrupts (MP uses polling)
//   04jun10 - Save last NN ID used so controller can resume (sv->last_id)
//   03jun10 - Randomise/control P2PB & PROBE ops
//   03jun10 - Timeouts for link read/write & probe (sv->peek_time)
//   03jun10 - Retries & timeouts for SDP/P2P (sv->xxx_retry, sv->xxx_time)
//   21may10 - nn_rcv_pkt - how to deal with delays in forwarding phase?
//   21may10 - NN_ACK packets ?? - not implemented
//   19may10 - CMD bitmaps
//   19may10 - Optimise out xxx_init routines by ZINIT
//   18may10 - Convert SV area to struct
//   18may10 - Split ports 3/5 - Port/CPU
//   18may10 - Timeouts for IPTAGs (transient timeout in UDP pad[0])
//   17may10 - Reenable interrupts in eth_rx_int
//   17may10 - Tidy up PLL/clock config
//   15may10 - Log usage of queues
//   15may10 - Lock for msg_free/get (seems to work!)
//   14may10 - Get source port number correct in eth_send_msg (not necessary?)
//   14may10 - Need extra bit for hop count
//   14may10 - Tidy up RUN cmd
//   13may10 - Specify SEQ_LEN for P2P - how? (Byte in SV space - init 16)
//   11may10 - p2p_addr is broken on APs
//   10may10 - Periodic link probe
//
//------------------------------------------------------------------------------


#include "sark.h"
#include "scamp.h"

#include "spinn_net.h"
#include "spinn_phy.h"
#include "spinn_regs.h"

#include <string.h>


//------------------------------------------------------------------------------


// VARS

uint num_cpus;
uint ping_cpu = 1;

uchar v2p_map[MAX_CPUS];

uint p2p_addr = 0;
uint p2p_dims = 0;
uint p2p_root = 0;
uint p2p_up = 0;

uint ltpc_timer;

uint link_en = 0x3f;	// Bitmap of enabled links

srom_data_t srom;	// Copy of SROM struct

pkt_queue_t tx_pkt_queue;

uchar watchdog[20]; // !! const - needs to be multiple of 4?

iptag_t tag_table[TAG_TABLE_SIZE];

uint tag_tto = 9;	// 2.56s = 10ms * (1 << (9-1))

//------------------------------------------------------------------------------

// Network initialisation state variables

// The network initialisation process phase currently in progress
volatile enum netinit_phase_e netinit_phase;

// The Ethernet initialisation process phase currently in progress
volatile enum ethinit_phase_e ethinit_phase;

// Number of 10ms ticks ellapsed since the last P2PC_NEW arrived
volatile uint ticks_since_last_p2pc_new;
// Number of 10ms ticks ellapsed since the last P2PC_DIMS arrived
volatile uint ticks_since_last_p2pc_dims;

// During NETINIT_PHASE_P2P_ADDR, the current best guess of P2P address. Note
// that this value may be negative and may be much larger than realistic
// machines as the first P2P address assigned may be the result of a packet
// taking a very spirally route.
volatile int p2p_addr_guess_x;
volatile int p2p_addr_guess_y;

// During NETINIT_PHASE_P2P_DIMS, records the current best guess of the systems
// dimensions.
volatile int p2p_min_x;
volatile int p2p_max_x;
volatile int p2p_min_y;
volatile int p2p_max_y;

// A bitmap giving the coordinates of all P2P coordinates which have been seen
// mentioned in a P2PC_NEW message. A 2D array of bits whose *bit* indexes are
// ((bx<<9) | by) where bx and by are x + 256 and y + 256 respectively.
uchar *p2p_addr_table = NULL;


//------------------------------------------------------------------------------

// LED control status variables/definitions

// The current system load, the LED brightness smoothly will track this value.
// 0 = idle, 255 = saturated.
volatile uchar load = 0;

// Interval between flashing the LEDs when displaying load to indicate chip
// liveness in 10ms units
#define LIVENESS_FLASH_INTERVAL 256

// Spacing between flashing adjacent chips when indicating liveness in 10ms
// units
#define LIVENESS_FLASH_SPACING 5

// The number of fractional bits to use for the internal representation of the
// load. The more bits, the longer the displayed load will take to catch up
// with the actual load.
#define LOAD_FRAC_BITS 1

// The number of bits to use for the PWM generator. The larger this is, the
// greater the resolution of the brightness but the more flicker-y it will
// look.
#define PWM_BITS 4

// The actual, displayed load value (fixed point). This value tracks the 'load'
// value above, transitioning smoothly towards it over time.
volatile uint disp_load = 0 << LOAD_FRAC_BITS;

//------------------------------------------------------------------------------


void iptag_timer()
{
    iptag_t *tag = tag_table;

    for (uint i = 0; i < TAG_TABLE_SIZE; i++) {
	if ((tag->flags & IPFLAG_VALID) && (tag->timeout > 0)) {
	    tag->timeout--;
	    if (tag->timeout == 0) {
		tag->flags = 0;
	    }
	}
	tag++;
    }
}


uint iptag_new()
{
    for (uint i = FIRST_POOL_TAG; i <= LAST_POOL_TAG; i++) {
	if (tag_table[i].flags == 0) {
	    return i;
        }
    }

    return TAG_NONE;
}

uint transient_tag(uchar *ip, uchar *mac, uint port, uint timeout)
{
    uint tag = iptag_new();

    if (tag != TAG_NONE) {
	iptag_t *tt = tag_table+tag;

	tt->flags = IPFLAG_VALID + IPFLAG_TRANS + timeout;
	tt->tx_port = port;
	tt->rx_port = 0;
	copy_ip(ip, tt->ip);
	copy_mac(mac, tt->mac);
	if (timeout != 0) {
	    timeout = 1 << (timeout - 1);
	}
	tt->timeout = timeout;
    }

    return tag;
}

//------------------------------------------------------------------------------

void queue_init()
{
    tx_pkt_queue.insert = 1;
}


void proc_byte_set(uint a1, uint a2)
{
    * (uchar *) a1 = a2;
}


void proc_route_msg(uint arg1, uint arg2);


void msg_queue_insert(sdp_msg_t *msg, uint srce_ip)
{
    if (event_queue_proc(proc_route_msg, (uint) msg, srce_ip, PRIO_0) == 0) {
	// if no event is queued free SDP msg buffer
	sark_msg_free(msg);
    }
}


//------------------------------------------------------------------------------


#pragma push
#pragma arm

uint pkt_tx(uint tcr, uint data, uint key)
{
    pkt_queue_t *txq = &tx_pkt_queue;
    pkt_t pkt = {tcr, data, key};
    uint cpsr = cpu_irq_disable();

    if (txq->count >= PKT_QUEUE_SIZE) {
	cpu_int_restore(cpsr);
	return 0;
    }

    if (txq->count == 0) {
	vic[VIC_ENABLE] = 1 << CC_TMT_INT;
    }

    txq->count++;
    if (txq->count > txq->max) {
	txq->max = txq->count;
    }

    txq->queue[txq->insert] = pkt;
    txq->insert = (txq->insert + 1) % PKT_QUEUE_SIZE;

    cpu_int_restore(cpsr);
    return 1;
}

#pragma pop


//------------------------------------------------------------------------------


static void timer1_init(uint count)
{
    tc[T1_CONTROL] = 0x000000e2; 	// En, Per, IntEn, Pre=1, 32bit, Wrap
    tc[T1_LOAD] = count;    	 	// Reload value
}


//------------------------------------------------------------------------------


__inline void eth_discard()
{
    volatile uint *er = (uint *) ETH_REGS;
    er[ETH_RX_CMD] = (uint) er;
}


void udp_pkt(uchar *rx_pkt, uint rx_len)
{
    ip_hdr_t *ip_hdr = (ip_hdr_t *) (rx_pkt + IP_HDR_OFFSET);

    if (!cmp_ip(ip_hdr->dest, srom.ip_addr)) {
	eth_discard();
	return;
    }

    uint ip_len = (ip_hdr->ver_len & 15) * 4;
    udp_hdr_t *udp_hdr = (udp_hdr_t *) (rx_pkt + IP_HDR_OFFSET + ip_len);
    uint udp_dest = ntohs(udp_hdr->dest);
    uint udp_srce = ntohs(udp_hdr->srce);
    int len = ntohs(udp_hdr->length);

    if (udp_dest == srom.udp_port) {
	len -= 10;			//const UDP_HDR + UDP_PAD

	if (len > 24 + SDP_BUF_SIZE) {	// SDP=8, CMD=16
	    eth_discard();
	    return;
	}

	sdp_msg_t *msg = sark_msg_get();

	if (msg == NULL) {		// !! fix this - reply somehow?
	    sw_error(SW_OPT);
	    eth_discard();
	    return;
	}

	sark_word_cpy(&msg->flags, (uchar *) udp_hdr+10, len); //const

	msg->length = len;
	msg->srce_addr = p2p_addr;

	uint flags = msg->flags;
	uint tag = msg->tag;
	uint srce_ip;

	copy_ip(ip_hdr->srce, (uchar *) &srce_ip);

	if ((tag == TAG_NONE) && (flags & SDPF_REPLY)) { // transient tag & reply req
	    tag = msg->tag = transient_tag(ip_hdr->srce, rx_pkt+6, udp_srce, tag_tto);
	}

	eth_discard();

	if ((flags & SDPF_REPLY) == 0 ||
		(tag < TAG_TABLE_SIZE && tag_table[tag].flags != 0)) {
	    arp_add(rx_pkt+6, ip_hdr->srce);
	    msg_queue_insert(msg, srce_ip);
	} else {
	    sark_msg_free(msg);
	}
    } else {				// Reverse IPTag...
	len -= 8;			//const UDP_HDR

	if (len > 16 + SDP_BUF_SIZE) {	// CMD=16
	    eth_discard();
	    return;
	}

	uint i = 0;

	while (i < TAG_FIXED_SIZE) {
	    if (tag_table[i].flags != 0 && tag_table[i].rx_port == udp_dest) {
		break;
	    }
	    i++;
	}

	if (i == TAG_FIXED_SIZE) {
	    eth_discard();
	    return;
	}

	sdp_msg_t *msg = sark_msg_get();
	if (msg == NULL) {
	    sw_error(SW_OPT);
	    eth_discard();
	    return;
	}

	// Set up reply path

	tag_table[i].tx_port = udp_srce;
	copy_ip(ip_hdr->srce, tag_table[i].ip);
	copy_mac(rx_pkt+6, tag_table[i].mac);

	memcpy(&msg->cmd_rc, (uchar *) udp_hdr+8, len); //const

	eth_discard();

	msg->flags = 0x07;
	msg->tag = i;
	msg->srce_addr = p2p_addr;
	msg->srce_port = PORT_ETH;
	msg->length = len + 8;
	msg->dest_addr = tag_table[i].dest_addr;
	msg->dest_port = tag_table[i].dest_port;

	msg_queue_insert(msg, 0);
    }
}


void eth_receive()
{
    while (1) {
	uint count = (er[ETH_STATUS] >> 1) & 63;
	if (count == 0) {
	    er[ETH_INT_CLR] = ETH_RX_CLR;
	    return;
	}

	uint rx_desc = er[ETH_RX_DESC_RP] & 0x3f;	// Descriptor table index
	uint rx_len = eth_rx_desc[rx_desc] & 0x7ff;	// Frame size
	uchar *rx_pkt = eth_rx_ram + (er[ETH_RX_BUF_RP] & 0xfff);

	uint etype = (rx_pkt[12] << 8) + rx_pkt[13];

	if (etype == ETYPE_IP) {
	    uint ip_prot = rx_pkt[IP_HDR_OFFSET + 9];

	    if (ip_prot == PROT_UDP) {
		udp_pkt(rx_pkt, rx_len);
	    } else if (ip_prot == PROT_ICMP) {
		icmp_pkt(rx_pkt, rx_len);
	    } else {
		eth_discard();
	    }
	} else if (etype == ETYPE_ARP) {
	    arp_pkt(rx_pkt, rx_len, TAG_TABLE_SIZE);
        } else {
            eth_discard();
        }
    }
}


//------------------------------------------------------------------------------


void eth_send_msg(uint tag, sdp_msg_t *msg)
{
    iptag_t *iptag = tag_table + tag;

    if ((iptag->flags & IPFLAG_VALID) == 0) {
	return;
    }

    uchar hdr[44];
    ip_hdr_t *ip_hdr = (ip_hdr_t *) (hdr + IP_HDR_OFFSET);
    udp_hdr_t *udp_hdr = (udp_hdr_t *) (hdr + UDP_HDR_OFFSET);

    uint len = msg->length;
    uchar *buf;
    uint pad;

    if (iptag->flags & IPFLAG_STRIP) {
	buf = (uchar *) &msg->cmd_rc;
	len -= 8;
	pad = 0;
    } else {
	buf = (uchar *) &msg->flags;
	pad = 2;
    }

    if (iptag->flags & IPFLAG_REV) {
	udp_hdr->srce = htons(iptag->rx_port);
    } else {
	udp_hdr->srce = htons(srom.udp_port);
    }

    copy_ip_hdr(iptag->ip, PROT_UDP, ip_hdr,
	    len + pad + IP_HDR_SIZE + UDP_HDR_SIZE);

    udp_hdr->length = htons(len + pad + UDP_HDR_SIZE);
    udp_hdr->dest = htons(iptag->tx_port);
    udp_hdr->checksum = 0;		// Zero checksum

    uint t;
    t = ipsum(hdr + IP_HDR_OFFSET + 12, 8, 0);	// Sum IP hdr addresses
    t += len + pad + UDP_HDR_SIZE;		// add in UDP data length
    t += PROT_UDP;				// and UDP protocol number
    t = ipsum((uchar *) udp_hdr, 8 + pad, t);	// and UDP header and pad
    t = ipsum(buf, len, t);			// and finally the data

    udp_hdr->checksum = htons(~t);

    mac_hdr_t *mac_hdr = (mac_hdr_t *) hdr;

    copy_mac(iptag->mac, mac_hdr->dest);
    copy_mac(srom.mac_addr, mac_hdr->srce);
    mac_hdr->type = htons(ETYPE_IP);

    eth_transmit2(hdr, buf, 42 + pad, len);

    sark_delay_us(5);		//## !! Trouble with back-to-back packets??

    if (iptag->flags & IPFLAG_TRANS) {		//transient tag
	iptag->flags = 0;
    } else {
	iptag->count++;
    }
}


//------------------------------------------------------------------------------


uint shm_ping(uint dest)
{
    vcpu_t *vcpu = sv_vcpu + dest;
    vcpu->mbox_ap_cmd = SHM_NOP;
    sc[SC_SET_IRQ] = SC_CODE + (1 << v2p_map[dest]);

    volatile uchar flag = 0;
    event_t *e = event_new(proc_byte_set, (uint) &flag, 2);
    if (e == NULL) {
	sw_error(SW_OPT);
	return 1;
    }

    uint id = e->ID;
    timer_schedule(e, 1000); // !! const??

    while (vcpu->mbox_ap_cmd != SHM_IDLE && flag == 0) {
	continue;
    }

    if (flag != 0) {
	return 0;
    }

    timer_cancel(e, id);
    vcpu->lr = (uint) vcpu->mbox_ap_msg;
    return 1;
}


uint shm_send_msg(uint dest, sdp_msg_t *msg) // Send msg AP
{
    vcpu_t *vcpu = sv_vcpu + dest;

    sdp_msg_t *shm_msg = sark_shmsg_get();
    if (shm_msg == NULL) {
	return RC_BUF;
    }

    sark_msg_cpy(shm_msg, msg);

    vcpu->mbox_ap_msg = shm_msg;
    vcpu->mbox_ap_cmd = SHM_MSG;

    sc[SC_SET_IRQ] = SC_CODE + (1 << v2p_map[dest]);

    volatile uchar flag = 0;
    event_t *e = event_new(proc_byte_set, (uint) &flag, 2);
    if (e == NULL) {
	sw_error(SW_OPT);
	sark_shmsg_free(shm_msg);
	return RC_BUF;		// !! not the right RC
    }

    uint id = e->ID;
    timer_schedule(e, 1000);	// !! const??

    while (vcpu->mbox_ap_cmd != SHM_IDLE && flag == 0) {
	continue;
    }

    if (flag != 0) {
	sark_shmsg_free(shm_msg);
	return RC_TIMEOUT;
    }

    timer_cancel(e, id);
    return RC_OK;
}


void swap_sdp_hdr(sdp_msg_t *msg)
{
    uint dest_port = msg->dest_port;
    uint dest_addr = msg->dest_addr;

    msg->dest_port = msg->srce_port;
    msg->srce_port = dest_port;

    msg->dest_addr = msg->srce_addr;
    msg->srce_addr = dest_addr;
}


void return_msg(sdp_msg_t *msg, uint rc) // Zero "rc" skips updating cmd_hdr
{
    uint f = msg->flags;

    if (f & SDPF_REPLY) {
	msg->flags = f & ~SDPF_REPLY;
	swap_sdp_hdr(msg);

	if (rc != 0) {
	    msg->cmd_rc = rc;
	    msg->length = 12;
	}

	msg_queue_insert(msg, 0);
    } else {
	sark_msg_free(msg);
    }
}


void proc_route_msg(uint arg1, uint srce_ip)
{
    sdp_msg_t *msg = (sdp_msg_t *) arg1;
    uint flags = msg->flags;

    if (flags & SDPF_SUM) {
/*
	uint sum = ipsum((uchar *) &msg->length, msg->length+4, 0);
	if (sum != 0xffff) { // !! fix this
	  //	  msg_free(msg);
	  //	  return;
	}
*/
	msg->flags = flags &= ~SDPF_SUM;
    }

    // Map (255, 255) to the root chip's coordinates
    if (msg->dest_addr == 0xFFFF) {
	msg->dest_addr = p2p_root;
    }
    if (msg->srce_addr == 0xFFFF) {
	msg->srce_addr = p2p_root;
    }

    // Off-chip via P2P

    if (msg->dest_addr != msg->srce_addr && msg->dest_addr != p2p_addr &&
	    (flags & SDPF_NR) == 0) {
	if (p2p_up == 0 || rtr_p2p_get(msg->dest_addr) == 6) {
	    return_msg(msg, RC_ROUTE);
	    return;
	}

	uint rc = p2p_send_msg(msg->dest_addr, msg);
	if (rc == RC_OK) {
	    sark_msg_free(msg);
        } else {
            return_msg(msg, rc);
        }
	return;
    }

    if (msg->dest_port == PORT_ETH) {
	eth_send_msg(msg->tag, msg);
	sark_msg_free(msg);
	return;
    }

    uint dest_cpu = msg->dest_port & CPU_MASK;
    if (dest_cpu >= num_cpus) {
	return_msg(msg, RC_CPU);
	return;
    }

    if (dest_cpu != sark.virt_cpu) {	// !! virt_cpu always zero
	uint rc = shm_send_msg(dest_cpu, msg);
	if (rc == RC_OK) {
	    sark_msg_free(msg);
        } else {
            return_msg(msg, rc);
        }
	return;
    }

    uint dest_port = msg->dest_port >> PORT_SHIFT;
    if (dest_port == 0) {
	msg->length = 12 + scamp_debug(msg, srce_ip);
	return_msg(msg, 0);
    } else {
	return_msg(msg, RC_PORT);	// APs should not do this!!
    }
}


//------------------------------------------------------------------------------


// Build virtual/physical CPU maps. Caller is monitor processor and
// passes its physical CPU.

void assign_virt_cpu(uint phys_cpu)
{
    for (uint phys = 0; phys < MAX_CPUS; phys++) {
	sv->p2v_map[phys] = 255;
    }

    sv->p2v_map[phys_cpu] = sark.virt_cpu = 0;
    sv->v2p_map[0] = phys_cpu;
    sark.vcpu = sv_vcpu + 0;

    uint virt = 1;

    for (uint phys = 0; phys < NUM_CPUS; phys++) {
	if (phys == phys_cpu) {
	    continue;
        }

	if (sc[SC_CPU_OK] & (1 << phys)) {
	    sv->p2v_map[phys] = virt;
	    sv->v2p_map[virt] = phys;
	    virt++;
	}
    }

    sv->num_cpus = num_cpus = virt;

    while (virt < MAX_CPUS) {
	sv->v2p_map[virt++] = 255;
    }

    // Turn off clocks of dead cores. If the monitor (this core) is marked as
    // dead, turn off the LED to make deadness more obvious.
    uint cpsr = cpu_int_disable();
    if (!(sc[SC_CPU_OK] & (1 << phys_cpu))) {
	sark_led_set(LED_OFF(0));
    }
    sc[SC_CPU_DIS] = SC_CODE + (~sc[SC_CPU_OK] & ((1 << NUM_CPUS) - 1));
    cpu_int_restore(cpsr);

    sark_word_cpy(v2p_map, sv->v2p_map, MAX_CPUS);
}

//------------------------------------------------------------------------------

// Disables a specified core and recomputes the virtual core map accordingly.
// This command has a number of dangerous effects:
// * All application cores are rebooted (so that the new virtual core map takes
//   effect)
// * If the core to be disabled includes the monitor then the monitor is
//   disabled without being remapped rendering the chip non-communicative.

void remap_phys_cores(uint phys_cores)
{
    sc[SC_CLR_OK] = phys_cores;

    // At the end of this function all CPUs which are not in the "OK" state
    // (which is cleared above) have their clocks stopped. Thus, if the
    // monitor core (which is executing this code) was flagged as bad, it
    // will also be disabled.
    assign_virt_cpu(sark.phys_cpu);

    boot_ap();

    sark_word_set(sv_vcpu + num_cpus, 0,
		  (NUM_CPUS - num_cpus) * sizeof(vcpu_t));
}

//------------------------------------------------------------------------------


uint ram_size(void *mem)
{
    volatile uint *ram = (uint *) mem;
    uint zf = 0xff0055aa;

    uint s = ram[0];
    uint i = 0;

    ram[0] = zf;

    uint v, z;
    do {
	uint a = 1 << i++;
	uint s = ram[a];

	ram[a] = i;

	z = ram[0];
	v = ram[a];
	ram[a] = s;
    } while ((v == i) && (z == zf));

    ram[0] = s;

    if (i == 1) {
	return 0;
    }

    return 1 << (i + 1);
}


//------------------------------------------------------------------------------


const uint rst_init[] = {0x45206e49, 0x79726576, 0x65724420, 0x48206d61,
			 0x20656d6f, 0x65482061, 0x61747261, 0x00656863};


void get_board_info(void)
{
    sdp_msg_t msg;

    msg.arg1 = (256 << 16) + 32;	// 256 bytes, send 32 bit command
    msg.arg2 = (3 << 24) + 0x100;	// READ, address 0x100

    (void) cmd_srom(&msg);

    sark_word_cpy(sv_board_info, &msg.arg1, 256);

    if (sv_board_info[0] < 64) {
	sv->board_info = sv_board_info;
    } else {
	// NB: this function is now called before this value is set to 0
	sv->board_info = 0;
    }
}


void sv_init(void)
{
    sark_word_cpy(sv_vectors, rst_init, SV_VSIZE); 	// Copy Reset vectors

    sark_word_set((void *) 0xf5007fc0, 0, 64);		// Kludge...

    if (srom.flags & SRF_PRESENT) {			// Get board_info ??
	get_board_info();
    }

    if (sv->hw_ver == 0 && srom.flags & SRF_PRESENT) {	// Set hardware version
	sv->hw_ver = (srom.flags >> 4) & 15;
    }

    sv->sw_ver = SLLT_VER_NUM;				// Set software version

    sv->sysram_base = (uint *) SYS_USER_BASE;
    uint *sysram_top = sv->sysram_base + (uint) sv->sysram_heap;
    sv->sysram_heap = sark_heap_init(sysram_top, (uint *) SYS_USER_TOP);

    sark_word_set(sv_vcpu, 0, NUM_CPUS * VCPU_SIZE);	// Set up VCPU blocks
    sv->vcpu_base = sv_vcpu;

    sv_vcpu[0].cpu_state = CPU_STATE_RUN;
    sv_vcpu[0].sw_ver = SLLT_VER_NUM;
    sv_vcpu[0].time = sv->unix_time;
    sv_vcpu[0].phys_cpu = sark.phys_cpu;
    sark_str_cpy(sv_vcpu[0].app_name, build_name);

    // Set up SHM buffers
    sv->shm_buf = sark_xalloc(sv->sysram_heap,
	    sv->num_buf * sizeof(sdp_msg_t), 0, 0);

    sv->shm_root.free = (mem_link_t *) sv->shm_buf;
    //sv->shm_root.count = sv->shm_root.max = 0;	//## Not needed now...

    sark_block_init(sv->shm_buf, sv->num_buf, sizeof(sdp_msg_t));

    if (sv->boot_delay == 0 && sv->rom_cpus == 0) {
	sv->root_chip = 1;
    }
}


void sdram_init(void)
{
    // Initialise PL340
    pl340_init(sv->mem_clk);

    // Compute sizes

    uint sdram_size = ram_size(sdram);		// SDRAM size (bytes)
    uint sys_size = (uint) sv->sdram_bufs;	// System size (bytes)

    // Fill in sv->sdram... vars

    // System buffers at base of SDRAM
    sv->sdram_bufs = (uint *) SDRAM_BASE;

    // User SDRAM above them...
    sv->sdram_base = sv->sdram_bufs + NUM_CPUS * sv->sysbuf_size;
    uint *sdram_top = sv->sdram_base + (uint) sv->sdram_heap;

    // SDRAM heap
    uint *system = (uint *) (SDRAM_BASE + sdram_size - sys_size);

    sv->sdram_heap = sark_heap_init(sdram_top, system);

    // System block - 256K bytes
    sv->sdram_sys = system;
    system += 65536;

    // Make system heap in SDRAM (Allowing space for system bufs)

    sv->sys_heap = sark_heap_init(system, system + sys_size / 4 - 65536);

    // Allocate system heap
    // Hop table - 64k entries of a word

    sv->hop_table = sark_xalloc(sv->sys_heap, HOP_TABLE_SIZE * 4, 0, 0);
    sark_word_set(sv->hop_table, 0x8000ffff, HOP_TABLE_SIZE * 4);

    // Router MC table copy (NB: 1 extra entry in copy table)

    sv->rtr_copy = sark_xalloc(sv->sys_heap,
	    (MC_TABLE_SIZE + 1) * sizeof(rtr_entry_t), 0, 0);

    // Alloc ID table

    sv->alloc_tag = sark_xalloc(sv->sys_heap, 65536 * 4, 0, 0);
    sark_word_set(sv->alloc_tag, 0, 65536 * 4);

    // AppID table

    sv->app_data = sark_xalloc(sv->sys_heap, 256 * sizeof(app_data_t), 0, 0);
    sark_word_set(sv->app_data, 0, 256 * sizeof(app_data_t));
}


void random_init(void)
{
    /* Now done from SDRAM...
      if (sv->random == 0) {
	  uint rand = 0xa5;				// Set up random seed
	  for (uint i = 0; i < SV_RSIZE / 4; i++) {
	      rand ^= (i & 1) ? sv_random[i] : ~sv_random[i];
	  }
	  sv->random = rand;
      }
     */
    // Make random seed

    uint *rnd = sark_xalloc(sv->sys_heap, 256 * 4, 0, 0);
    if (sv->random == 0) {
	uint rand = 0xa5;				// Set up random seed
	for (uint i = 0; i < 256; i++) {
	    rand ^= (i & 1) ? rnd[i] : ~rnd[i];
        }
	sv->random = rand;
    }

    sark_srand(sv->random);
}

//------------------------------------------------------------------------------

// Update the 'load' variable with an estimate of the system's load
void update_load(uint arg1, uint arg2)
{
    uint num_working = 0;
    uint num_with_apps = 0;
    uint num_awake = 0;

    uint sleeping_cpus = sc[SC_SLEEP];

    for (uint cpu = 1; cpu < NUM_CPUS; cpu++) {
	vcpu_t *vcpu = sv_vcpu + cpu;

	if (vcpu->cpu_state != CPU_STATE_DEAD) {
	    num_working++;
        }

	// NB: Ignores cores not running apps
	if (vcpu->app_id > 0) {
	    num_with_apps++;
	    if (!(sleeping_cpus & (1 << vcpu->phys_cpu))) {
		num_awake++;
            }
        }
    }

    // Load is simply the proportion of application processors which are
    // awake with the load clamped at a minimum of zero if any applications
    // are loaded.
    uint new_load = (num_awake * 255) / (num_working);
    if (num_with_apps > 0 && new_load == 0) {
	new_load = 1;
    }
    load = new_load;
}

//------------------------------------------------------------------------------

// "proc_1hz" is put on the event queue every second

void proc_1hz(uint a1, uint a2)
{
    if (srom.flags & SRF_ETH) {
	uint s = phy_read(PHY_STATUS);
	sv->eth_up = (s & 4) >> 2;

	// If the Ethernet is up now, we are done
	if (sv->eth_up) {
	    ethinit_phase = ETHINIT_PHASE_DONE;
        }
    }

    switch (ethinit_phase) {
    case ETHINIT_PHASE_WAIT_1:
        ethinit_phase = ETHINIT_PHASE_WAIT_2;
        break;
    case ETHINIT_PHASE_WAIT_2:
        ethinit_phase = ETHINIT_PHASE_DONE;
        break;
    default:
        break;
    }
}


void soft_wdog(uint max)
{
    vcpu_t *vcpu = sv_vcpu + ping_cpu;

    if (vcpu->cpu_state >= 4) {				// !! const
	if (shm_ping(ping_cpu)) {
	    watchdog[ping_cpu] = 0;
	} else if (++watchdog[ping_cpu] == max) {
	    clock_ap(1 << v2p_map[ping_cpu], 0);	// Disable clock

	    vcpu_t *vcpu = sv_vcpu + ping_cpu;
	    vcpu->cpu_state = CPU_STATE_WDOG;
	}
    } else {
	watchdog[ping_cpu] = 0;
    }

    if (++ping_cpu >= num_cpus) {
	ping_cpu = 1;
    }
}


//------------------------------------------------------------------------------

// Late-stage boot related functions which initialise higher-level network
// functionality, e.g. P2P tables and addresses


// Start the higher-level network initialisation process. Must be called only
// once, before the nearest neighbour interrupt handler is enabled.
void netinit_start(void)
{
    sv->netinit_phase = netinit_phase = NETINIT_PHASE_P2P_ADDR;
    ethinit_phase = ETHINIT_PHASE_WAIT_1;

    // Initial P2P address guess
    if (sv->root_chip) {
	p2p_addr_guess_x = 0;
	p2p_addr_guess_y = 0;
    } else {
	p2p_addr_guess_x = NO_IDEA;
	p2p_addr_guess_y = NO_IDEA;
    }

    // Initial P2P dimension range, initially empty
    p2p_min_x = 0;
    p2p_max_x = 0;
    p2p_min_y = 0;
    p2p_max_y = 0;

    // Allocate and clear the P2P addr bitmap
    p2p_addr_table = sark_xalloc(sv->sys_heap, P2P_ADDR_TABLE_BYTES, 0, 0);
    sark_word_set(p2p_addr_table, 0, P2P_ADDR_TABLE_BYTES);

    ticks_since_last_p2pc_new = 0;
    ticks_since_last_p2pc_dims = 0;
}

// Sets up a broadcast MC route by constructing a spanning tree of the  P2P
// routes constructed routing back to chip used to boot the machine (p2p_root).
void compute_st(void)
{
    // Work out the position of the p2p_root in the P2P routing table.
    uint word = p2p_root >> P2P_LOG_EPW;
    uint offset = P2P_BPE * (p2p_root & P2P_EMASK);

    // Definately route here
    uint route = MC_CORE_ROUTE(0);

    // Compile the set of neighbours which route to the root via this chip
    // and add them to the route, forming a spanning tree.
    uint timeout = sv->peek_time;
    for (uint link = 0; link < NUM_LINKS; link++) {
	if (!((1 << link) & link_en)) {
	    continue;
        }

	// Try to read multiple times if required
	uint attempts_remaining = 2;
	uint remote_rtr_p2p;
	uint rc;
	do {
	    rc = link_read_word((uint)(rtr_p2p + word), link,
		    &remote_rtr_p2p, timeout);
        } while (rc != RC_OK && (--attempts_remaining));

	// Flag an error if we could not get a p2p entry from a neighbour
	if (rc != RC_OK) {
	    sw_error(SW_OPT);
	    continue;
        }

	// Check if p2p_root route from neighbour points at this chip.
	if (((remote_rtr_p2p >> offset) & 0x7) == ((link + 3) % 6)) {
	    route |= MC_LINK_ROUTE(link);
        }
    }

    rtr_mc_set(0, 0xffff5555, 0xffffffff, route);
}


//------------------------------------------------------------------------------

// "proc_100hz" is put on the event queue every 10ms
void proc_100hz(uint a1, uint a2)
{
    // Counter used to time how long we've been in certain netinit states.
    static uint netinit_biff_tick_counter = 0;
    static uint netinit_p2p_tick_counter = 0;

    // Boot-up related packet sending and boot-phase advancing
    switch (netinit_phase) {
    case NETINIT_PHASE_P2P_ADDR:
	// Periodically re-send the neighbours their P2P address as
	// neighbouring chips may take some time to come online.
	p2pc_addr_nn_send(0, 0);

	// If no new P2P addresses have been broadcast for a while we can
	// assume all chips are have a valid P2P address so it is now time to
	// determine the system's dimensions.
	if (ticks_since_last_p2pc_new++ > (uint)sv->netinit_bc_wait) {
	    netinit_phase = NETINIT_PHASE_P2P_DIMS;

	    p2p_min_x = (p2p_addr_guess_x < 0) ? p2p_addr_guess_x : 0;
	    p2p_min_y = (p2p_addr_guess_y < 0) ? p2p_addr_guess_y : 0;
	    p2p_max_x = (p2p_addr_guess_x > 0) ? p2p_addr_guess_x : 0;
	    p2p_max_y = (p2p_addr_guess_y > 0) ? p2p_addr_guess_y : 0;
        }
	break;

    case NETINIT_PHASE_P2P_DIMS:
	// Periodically re-broadcast the local best guess of system
	// dimensions as a safety net in the event of packet loss.
	p2pc_dims_nn_send(0, 0);

	// If no new guesses have been broadcast for a while we can assume
	// the current guess is accurate so its time to move onto the next
	// phase
	if (ticks_since_last_p2pc_dims++ > (uint)sv->netinit_bc_wait) {
	    // If no coordinate discovered, just shut down this chip
	    if (p2p_addr_guess_x == NO_IDEA || p2p_addr_guess_y == NO_IDEA) {
		remap_phys_cores(0x3ffff);
            }

	    // Record the coordinates/dimensions discovered
	    sv->p2p_addr = p2p_addr = ((p2p_addr_guess_x - p2p_min_x) << 8) |
		    ((p2p_addr_guess_y - p2p_min_y) << 0);
	    sv->p2p_dims = p2p_dims = ((1 + p2p_max_x - p2p_min_x) << 8) |
		    ((1 + p2p_max_y - p2p_min_y) << 0);
	    sv->p2p_root = p2p_root = (-p2p_min_x << 8) | -p2p_min_y;

	    sv->p2p_active += 1;

	    // Reseed uniquely for each chip
	    sark_srand(p2p_addr);

	    // Set our P2P addr in the comms controller
	    cc[CC_SAR] = 0x07000000 + p2p_addr;

	    // Work out the local Ethernet connected chip coordinates
	    compute_eth();

	    netinit_biff_tick_counter = 0;
	    netinit_phase = NETINIT_PHASE_BIFF;
        }
	break;

    case NETINIT_PHASE_BIFF:
	// The board information floodfill is allowed three 100Hz ticks. In
	// the first tick, the board information is actually broadcast. In
	// the second tick, nothing happens and in the third the state
	// advances to the P2P table generation phase.
	//
	// The reason for using more than one tick is that the 10ms ticks
	// around the machine are not aligned. As a result, some chips may be
	// *almost* 10ms ahead of others. Since it is important that
	// blacklisting information is broadcast ahead of P2P generation,
	// leaving an extra "tick" before moving to the next state should
	// deal with the problem. A third tick is left to allow extra leeway
	// accounting for the fact that the timers are not necessarily
	// *perfectly* aligned to within 10ms...

	netinit_biff_tick_counter++;

	if (netinit_biff_tick_counter == 1) {
	    if (sv->board_info) {
		uint num_info_words = sv->board_info[0];
		uint *info_word = sv->board_info + 1;
		while (num_info_words--) {
		    // Handle command on this chip
		    nn_cmd_biff(0, 0, *(info_word));
		    // Also flood to other chips on this board
		    biff_nn_send(*(info_word++));
                }
            }
        } else if (netinit_biff_tick_counter >= 3) {
            netinit_p2p_tick_counter = 0;
            netinit_phase = NETINIT_PHASE_P2P_TABLE;
        }
	break;

    case NETINIT_PHASE_P2P_TABLE: {
	// Broadcast P2P table generation packets, staggered by chip to
	// reduce network load.
	uint p2pb_period = ((p2p_dims >> 8) * (p2p_dims & 0xFF)) * P2PB_OFFSET_USEC;
	if (netinit_p2p_tick_counter == 0) {
	    hop_table[p2p_addr] = 0;
	    rtr_p2p_set(p2p_addr, 7);
	    timer_schedule_proc(p2pb_nn_send, 0, 0,
		    (sark_rand() % p2pb_period) + 1);
	}

	// Once all P2P messages have had ample time to send (and the
	// required number of repeats have occurred), compute the level
	// config and signalling broadcast spanning tree.
	if (netinit_p2p_tick_counter++ >= (p2pb_period / 10000) + 2) {
	    netinit_p2p_tick_counter = 0;

	    if (sv->p2pb_repeats-- == 0) {
	        // check if delegating
	        if (mon_del) {
		    netinit_phase = NETINIT_PHASE_DEL;
		} else {
		    netinit_phase = NETINIT_PHASE_DONE;
		}

		level_config();
		compute_st();
		sv->p2p_up = p2p_up = 1;

		if (srom.flags & SRF_ETH) {
		    uint s = phy_read(PHY_STATUS);
		    sv->eth_up = (s & 4) >> 2;
		    if (sv->eth_up) {
			ethinit_phase = ETHINIT_PHASE_DONE;
		    }
		}
	    }
	}
	break;
    }

    case NETINIT_PHASE_DEL:
        // delegate if boot image DMA completed
        if (dma[DMA_STAT] & (1 << 10)) {
	    // clear DMA transfer complete interrupt,
	    // NB: not really needed - this core will die
	    //dma[DMA_CTRL] = 1 << 3;

	    // take this core out of the application pool
	    sc[SC_CLR_OK] = (1 << sark.phys_cpu);

	    // NB: this function will not return
	    delegate();
	}
	break;

    case NETINIT_PHASE_DONE:
    default:
	// Unrecognised or finished state? Do nothing.
	break;
    }
    sv->netinit_phase = netinit_phase;

    // Light the LED every-so-often to make it clear that this chip is alive
    if (netinit_phase == NETINIT_PHASE_DONE) {
	static uint ticks = 0;

	uint p2p_x = p2p_addr >> 8;
	uint p2p_y = p2p_addr & 0xFF;
	uint p2p_dist = p2p_x + p2p_y;
	uint flash_time = (p2p_dist * LIVENESS_FLASH_SPACING)
		% LIVENESS_FLASH_INTERVAL;
	if (ticks == flash_time) {
	    disp_load = ((load < 128) ? 255 : 0) << LOAD_FRAC_BITS;
        }

	if (++ticks >= LIVENESS_FLASH_INTERVAL) {
	    ticks = 0;
        }
    }

    // Sample core sleep states at a random interval to estimate chip load.
    timer_schedule_proc(update_load, 0, 0, (sark_rand() % 9999) + 1);

    // Process IPTag timeouts
    iptag_timer();

    // Ping application CPUs to check on status
    if (sv->soft_wdog) {
	soft_wdog(sv->soft_wdog);
    }

    // Send LTPC packet (untested!)
    if (sv->ltpc_period > 0 && ++ltpc_timer >= sv->ltpc_period) {
	ltpc_timer = 0;

	ff_nn_send((NN_CMD_LTPC << 24) + (0x3e00 << 8),
		sv->tp_timer, 0x3f00, 0);
    }
}

//------------------------------------------------------------------------------

// "proc_1khz" is put on the event queue every millisecond and is used to PWM
// the LEDs

void proc_1khz(uint a1, uint a2)
{
    // Display status on LED0 except when booting
    if (netinit_phase >= NETINIT_PHASE_DONE) {
	if (sv->led_period == 1) {
	    // sv->led_period == 1: Display current load using PWM

	    // Slowly track the actual load value
	    uint fractional_load = ((uint)load) << LOAD_FRAC_BITS;
	    if (disp_load < fractional_load) {
		disp_load++;
            } else if (disp_load > fractional_load) {
        	disp_load--;
            }

	    // PWM generation
	    static uint period = 0;
	    uint duty = disp_load >> (LOAD_FRAC_BITS + PWM_BITS);

	    // If there is *any* load, keep the LED on a little bit.
	    if (disp_load > 0 && duty == 0) {
		duty = 1;
	    }

	    if (period >= duty) {
		sark_led_set(LED_OFF(0));
	    } else {
		sark_led_set(LED_ON(0));
	    }

	    if (++period >= (1 << PWM_BITS)) {
		period = 0;
	    }
	} else if (sv->led_period >= 2) {
	    // sv->led_period >= 2: Blink at a given frequency
	    static uint last_toggle = 0;
	    if (last_toggle++ > (sv->led_period * 10)) {
		last_toggle = 0;
		sark_led_set(LED_INV(0));	// !! assumes LED_0 always there
	    }
	}
    }
}

//------------------------------------------------------------------------------

// Initialise PLLs

static uint pll_mult(uint freq)
{
    uint f = 0;

    if (freq >= 50) {
	f = 1;
    }
    if (freq >= 100) {
	f = 2;
    }
    if (freq >= 200) {
	f = 3;
    }

    return f << 16;
}


// cpu_freq - CPU clock frequency in MHz
// mem_freq - SDRAM clock frequency in MHz
// sys_div - system bus clock divider (range 1..4)
// rtr_div - router clock divider (range 1..4)
//
// Note that system bus is clocked at (cpu_freq * 2 / sys_div)
// and router at (cpu_freq * 2 / rtr_div)
//
// To run CPUs at 200, SDRAM at 130, system bus and router at 133
//
void pll_init()
{
    sark.cpu_clk = 10;			// Set for delay_us
    sc[SC_CLKMUX] = 0;			// Switch to 10MHz everywhere

    sark_delay_us(2);			// and wait a while

    uint cpu_freq = sv->cpu_clk * 2;	// Set local values
    uint mem_freq = sv->mem_clk * 2;

    divmod_t r;				// Divide by 10 and set PLLs

    r = sark_div10(cpu_freq);		// CPUs on PLL1
    sc[SC_PLL1] = 0x00040100 + pll_mult(cpu_freq) + r.div;

    r = sark_div10(mem_freq);		// SDRAM on PLL2
    sc[SC_PLL2] = 0x00040100 + pll_mult(mem_freq) + r.div;

    sark_delay_us(PLL_LOCK_TIME);	// Wait for PLLs to lock

    uint sys_div = ((sv->clk_div >> 4) - 1) & 3;
    uint rtr_div = (sv->clk_div - 1) & 3;

    sark.cpu_clk = cpu_freq / 2;
    sc[SC_CLKMUX] = 0x801088a5 + (sys_div << 22) + (rtr_div << 17);
}


//------------------------------------------------------------------------------

void eth_setup()
{
    if (srom.flags & SRF_ETH) {			// Ethernet present (possibly)
	eth_init(srom.mac_addr);
	sark_word_cpy(sv->ip_addr, srom.ip_addr, 4);

	if (srom.flags & SRF_PHY_RST) {		// Hardware reset PHY
	    phy_reset();
	}

	if (srom.flags & SRF_PHY_INIT) {	// (Re-)initialise PHY
	    phy_write(PHY_AUTO_ADV, 0x01e1);	// Allow 100/10 meg
	    phy_write(PHY_CONTROL, 0x1200);	// Enable & restart auto-neg
	}

	while (srom.flags & SRF_PHY_WAIT) {	// Wait (without timeout)
	    uint s = phy_read(PHY_STATUS);	// Read PHY status
	    sv->eth_up = (s & 4) >> 2;		// Bit 2 says link up
	    if (sv->eth_up) {
		ethinit_phase = ETHINIT_PHASE_DONE;
		break;
	    }
	    ethinit_phase = ETHINIT_PHASE_WAIT_1;
	    event_run(1);
	}
    }
}


//------------------------------------------------------------------------------


void jtag_init(void)
{
    sc[GPIO_CLR] = JTAG_NTRST;		// Ensure NTRST is low
    sc[SC_MISC_CTRL] |= JTAG_INT; 	// Drive JTAG internally
    sark_delay_us(1);			// Wait a short while
    sc[SC_MISC_CTRL] &= ~JTAG_INT; 	// Drive JTAG externally
}


void sark_config(void)
{
    sark_vec->num_msgs = 32;		// Allocate 32 SDP messages
    sark_vec->num_events = 64;		// and 64 events
    sark_vec->app_id = 0;		// Just to be sure...
    sark_vec->app_flags &= ~(1 << APP_FLAG_WAIT); // Don't wait in SARK start-up
}


void delegate()
{
    // choose delegate
    uint del      = NUM_CPUS - 1;         // potential delegate
    uint del_mask = 1 << (NUM_CPUS - 1);  // delegate mask

    while (~sc[SC_CPU_OK] & del_mask) {
        del--;
	del_mask = del_mask >> 1;
    }

    // copy img_cp_exe to system RAM for delegate,
    sark_word_cpy (sysram, (void *) img_cp_exe, 128);

    // let system controller know of new monitor,
    sc[SC_MON_ID] = SC_CODE + del;

    // mark delegate as POWER DOWN to avoid triggering soft wdog,
    vcpu_t *vcpu = sv_vcpu + sv->p2v_map[del];
    vcpu->cpu_state = 1;

    // start delegate,
    sc[SC_SOFT_RST_L] = SC_CODE + del_mask;
    clock_ap(del_mask, 1);
    sark_delay_us(5);
    sc[SC_SOFT_RST_L] = SC_CODE;

    // disable all interrupts,
    vic[VIC_DISABLE] = 0xffffffff;
    vic[VIC_SELECT] = 0;

    // and wait here for delegate to kill this core
    while (1) {
        cpu_wfi();
    }
}


void chk_bl_del(void)
{
    // get board blacklist,
    get_board_info();

    // and check if need to delegate
    if (sv->board_info) {
        uint   bl_len   = sv->board_info[0];
	uint * bl_dat   = sv->board_info + 1;
	uint   bl_cores = 0;

	// get (0, 0) blacklist entry if it exists,
	while (bl_len--) {
	    uint data  = *(bl_dat++);
	    uint type  = data >> 30;
	    uint coord = (data >> 24) & 0x3f;

	    if ((type == 0) && (coord == 0)) {
	        bl_cores = data & 0x3ffff;
		break;
	    }
	}

	// and delegate if this core is blacklisted
	if (bl_cores & (1 << sark.phys_cpu)) {
	    // start boot image DMA to SDRAM for delegate,
	    dma[DMA_ADRS] = (uint) SDRAM_BASE;
	    dma[DMA_ADRT] = (uint) DTCM_BASE + 0x00008000;
	    dma[DMA_DESC] = 1 << 24 | 4 << 21 | 1 << 19 | 0x7100;

	    // take blacklisted cores out of the application pool
	    sc[SC_CLR_OK] = bl_cores;

	    // copy SROM data to system RAM for delegate,
	    sark_word_cpy (sysram + 128, sv_srom, sizeof(srom_data_t));

	    // wait for boot image DMA to complete,
	    while (!(dma[DMA_STAT] & (1 << 10))) {
	        continue;
	    }

	    // clear DMA transfer complete interrupt,
	    // NB: not really needed - this core will die
	    //dma[DMA_CTRL] = 1 << 3;

	    // NB: this function will not return
	    delegate();
	}
    }
}


void c_main(void)
{
    sark.cpu_clk = 160;			// BootROM uses 160 MHz

    jtag_init();			// Reset JTAG internals

    wd[WD_LOCK] = WD_CODE;		// Disable watchdog!
    wd[WD_CTRL] = 0;

    sc[SC_MISC_CTRL] |= 1;  		// Swap RAM/ROM

    // check if this core is a delegate
    uint mon = (rtr[RTR_CONTROL] >> 8) & 0x1f;
    uint del = (sark.phys_cpu != mon);
    if (del) {
        // get SROM block from copy made by monitor
        sark_word_cpy(&srom, sysram + 128, sizeof(srom_data_t));

	// if board-local (0, 0) stop treating as delegate
	if (srom.flags & SRF_PRESENT) {
  	    del = 0;
	}

        // let router know of new monitor
        rtr[RTR_CONTROL] = 0x00400001 | (sark.phys_cpu << 8);

	// and disable the previous monitor
	clock_ap(1 << mon, 0);
    } else {
        // get original SROM block
        sark_word_cpy(&srom, sv_srom, sizeof(srom_data_t));

	// if board-local (0, 0) check blacklist and delegate if necessary
	if (srom.flags & SRF_PRESENT) {
	    // NB: this function will not return if monitor is blacklisted
	    chk_bl_del();
	}
    }

    if (!del) {  // normal boot
        assign_virt_cpu(sark.phys_cpu);	// Assign virtual CPUs

        sv_init();			// Initialise system RAM
	sark_led_init();		// Initialise LED drivers
	pll_init();			// Restart PLLs
	sdram_init();			// Initialise SDRAM
	random_init();			// Initialise random
	rtr_init(sark.phys_cpu);	// Initialise router

	if (sv->boot_delay) {		// If bootROM boot
	    boot_nn(sv->hw_ver);	// Flood fill neighbours
	}
	boot_ap();			// Start local APs

	timer1_init(sark.cpu_clk * 1000);	// Initialise 1ms timer

	queue_init();			// Initialise various queues
	nn_init();			// Initialise NN package
	netinit_start();		// Initialise late-stage boot process datastructures
	vic_setup();			// Set VIC, interrupts on

	if (sv->boot_delay) {
	    eth_setup();	        // Set up Ethernet if present
	}
    } else {   // do not attempt to boot again
	// fix VCPU block
 	sv_vcpu[0].phys_cpu = sark.phys_cpu;

	remap_phys_cores(~sc[SC_CPU_OK]);  // re-assign application cores

	sark.cpu_clk = sv->cpu_clk;	// Set local values

	// replace old monitor with new in MC route for signal distribution
	uint sig_rt = rtr_ram[0] & ~MC_CORE_ROUTE(mon);
	rtr_ram[0]  = sig_rt | MC_CORE_ROUTE(sark.phys_cpu);

	timer1_init(sark.cpu_clk * 1000);  // Initialise 1ms timer

	queue_init();			// Initialise various queues
	nn_init();			// Initialise NN package
	
	// recover the coordinates/dimensions discovered by previous monitor
	p2p_addr = sv->p2p_addr;
	p2p_dims = sv->p2p_dims;
	p2p_root = sv->p2p_root;
	p2p_up   = sv->p2p_up;

	// (re-)construct the level/region information (not in shared memory)
	level_config();

	// Reseed uniquely for each chip
	sark_srand(p2p_addr);

	// Set our P2P addr in the comms controller
	cc[CC_SAR] = 0x07000000 + p2p_addr;

        // Initialise, as DONE, late-stage boot process variables
	sv->netinit_phase = netinit_phase = NETINIT_PHASE_DONE;
	ethinit_phase = ETHINIT_PHASE_DONE;

        // Set VIC, interrupts on
	vic_setup();
    }

    // Run event loop (forever...)
    while (1) {
	event_run(0);

	// interrupts must be disabled to avoid queue-access hazard,
	uint cpsr = cpu_int_disable();

	// check if queue is empty,
	if (event.proc_queue->proc_head == NULL) {
	    // NB: interrupts will wake up the core even if disabled
	    cpu_wfi();
	}

	// and re-enable interrupts to service any pending ones
	cpu_int_restore(cpsr);
    }
}


//------------------------------------------------------------------------------
