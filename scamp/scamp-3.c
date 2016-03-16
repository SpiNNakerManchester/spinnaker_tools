
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


typedef struct sw_timer
{
  ushort counter;	// Tick counter
  ushort increment;	// Loaded into counter when counter == 0
  uchar multiplier;	// Increases multiplier mult_rep times
  uchar repeat;		// Number of repeats (0 = infinite, 255 = stop)
  uchar mult_rep;	// Number of times multiplier applied to increment
} sw_timer_t;


//------------------------------------------------------------------------------


// VARS

uint num_cpus;
uint ping_cpu = 1;

uchar v2p_map[MAX_CPUS];

uint p2p_addr;
uint p2p_dims;
uint p2p_up;
uchar p2pb_id;		// Must be 8 bits

sw_timer_t led_timer;
sw_timer_t probe_timer;
sw_timer_t p2pb_timer;
sw_timer_t p2pc_timer;
sw_timer_t biff_timer;

uint ltpc_timer;

uint max_hops;		// Maximum packet hop count
uint link_up;		// Bitmap of working links
uint link_en = 0x3f;	// Bitmap of enabled links
uint ticks;		// Counts 10ms ticks

srom_data_t srom;	// Copy of SROM struct

pkt_queue_t tx_pkt_queue;

uchar watchdog[20]; // !! const - needs to be multiple of 4?

iptag_t tag_table[TAG_TABLE_SIZE];

uint tag_tto = 9;	// 2.56s = 10ms * (1 << (9-1))


//------------------------------------------------------------------------------


static void init_timer (sw_timer_t *timer, uint data)
{
  timer->counter = data >> 24;
  timer->increment = (data >> 16) & 255;
  timer->repeat = (data >> 8) & 255;
  timer->mult_rep = (data >> 4) & 15;
  timer->multiplier = data & 15;
}


static uint run_timer (sw_timer_t *timer)
{
  if (timer->repeat == 255)	// Stopped
    return 0;

  if (--timer->counter != 0)	// Decrementing counter
    return 0;

  // counter gone to zero...

  if (timer->repeat != 0 && --timer->repeat == 0) // Last repetition
    {
      timer->repeat = 255;
      return 1;
    }

  timer->counter = timer->increment; // Reload counter

  // Multiplier reverts to 1 after repetitions

  if (timer->mult_rep != 0 && --timer->mult_rep == 0)
    timer->multiplier = 1;

  timer->increment *= timer->multiplier; // Scale by multiplier

  return 1;
}


//------------------------------------------------------------------------------


void iptag_timer ()
{
  iptag_t *tag = tag_table;

  for (uint i = 0; i < TAG_TABLE_SIZE; i++)
    {
      if ((tag->flags & IPFLAG_VALID) && (tag->timeout > 0))
	{
	  tag->timeout--;
	  if (tag->timeout == 0)
	    tag->flags = 0;
	}
      tag++;
    }
}


uint iptag_new ()
{
  for (uint i = FIRST_POOL_TAG; i <= LAST_POOL_TAG; i++)
    if (tag_table[i].flags == 0)
      return i;

  return TAG_NONE;
}



uint transient_tag (uchar *ip, uchar *mac, uint port, uint timeout)
{
  uint tag = iptag_new ();

  if (tag != TAG_NONE)
    {
      iptag_t *tt = tag_table+tag;

      tt->flags = IPFLAG_VALID + IPFLAG_TRANS + timeout;
      tt->tx_port = port;
      tt->rx_port = 0;
      copy_ip (ip, tt->ip);
      copy_mac (mac, tt->mac);
      if (timeout != 0)
	timeout = 1 << (timeout - 1);
      tt->timeout = timeout;
    }

  return tag;
}

//------------------------------------------------------------------------------



void queue_init ()
{
  tx_pkt_queue.insert = 1;
}


void proc_byte_set (uint a1, uint a2)
{
  * (uchar *) a1 = a2;
}


void proc_route_msg (uint arg1, uint arg2);


void msg_queue_insert (sdp_msg_t *msg, uint srce_ip)
{
  event_queue_proc (proc_route_msg, (uint) msg, srce_ip, PRIO_0);
}


//------------------------------------------------------------------------------


#pragma push
#pragma arm

uint pkt_tx (uint tcr, uint data, uint key)
{
  pkt_queue_t *txq = &tx_pkt_queue;

  pkt_t pkt = {tcr, data, key};

  uint cpsr = cpu_irq_disable ();

  if (txq->count >= PKT_QUEUE_SIZE)
    {
      cpu_int_restore (cpsr);
      return 0;
    }

  if (txq->count == 0)
    vic[VIC_ENABLE] = 1 << CC_TMT_INT;

  txq->count++;

  if (txq->count > txq->max)
    txq->max = txq->count;

  txq->queue[txq->insert] = pkt;
  txq->insert = (txq->insert + 1) % PKT_QUEUE_SIZE;

  cpu_int_restore (cpsr);

  return 1;
}

#pragma pop


//------------------------------------------------------------------------------



static void timer1_init (uint count)
{
  tc[T1_CONTROL] = 0x000000e2; 		// En, Per, IntEn, Pre=1, 32bit, Wrap
  tc[T1_LOAD] = count;    	 	// Reload value
}


//------------------------------------------------------------------------------


__inline void eth_discard ()
{
  volatile uint *er = (uint *) ETH_REGS;
  er[ETH_RX_CMD] = (uint) er;
}


void udp_pkt (uchar *rx_pkt, uint rx_len)
{
   ip_hdr_t *ip_hdr = (ip_hdr_t *) (rx_pkt + IP_HDR_OFFSET);

   if (!cmp_ip (ip_hdr->dest, srom.ip_addr))
     {
       eth_discard ();
       return;
     }

   uint ip_len = (ip_hdr->ver_len & 15) * 4;
   udp_hdr_t *udp_hdr = (udp_hdr_t *) (rx_pkt + IP_HDR_OFFSET + ip_len);
   uint udp_dest = ntohs (udp_hdr->dest);
   uint udp_srce = ntohs (udp_hdr->srce);
   int len = ntohs (udp_hdr->length);

   if (udp_dest == srom.udp_port)
     {
       len -= 10; //const UDP_HDR + UDP_PAD

       if (len > 24 + SDP_BUF_SIZE) // SDP=8, CMD=16
	 {
	   eth_discard ();
	   return;
	 }

       sdp_msg_t *msg = sark_msg_get ();

       if (msg == NULL) // !! fix this - reply somehow?
	 {
	   sw_error (SW_OPT);
	   eth_discard ();
	   return;
	 }

       sark_word_cpy (&msg->flags, (uchar *) udp_hdr+10, len); //const

       msg->length = len;
       msg->srce_addr = p2p_addr;

       uint flags = msg->flags;
       uint tag = msg->tag;
       uint srce_ip;

       copy_ip (ip_hdr->srce, (uchar *) &srce_ip);

       if ((tag == TAG_NONE) && (flags & SDPF_REPLY)) // transient tag & reply req
	 tag = msg->tag = transient_tag (ip_hdr->srce, rx_pkt+6, udp_srce, tag_tto);

       eth_discard ();

       if (((flags & SDPF_REPLY) == 0) ||
	   (tag < TAG_TABLE_SIZE && tag_table[tag].flags != 0))
	 msg_queue_insert (msg, srce_ip);
       else
	 sark_msg_free (msg);
     }
   else // Reverse IPTag...
     {
       len -= 8;  //const UDP_HDR

       if (len > (16 + SDP_BUF_SIZE)) // CMD=16
	 {
	   eth_discard ();
	   return;
	 }

       uint i = 0;

       while (i < TAG_FIXED_SIZE)
	 {
	   if (tag_table[i].flags != 0 && tag_table[i].rx_port == udp_dest)
	     break;
	   i++;
	 }

       if (i == TAG_FIXED_SIZE)
	 {
	   eth_discard ();
	   return;
	 }

       sdp_msg_t *msg = sark_msg_get ();

       if (msg == NULL)
	 {
	   sw_error (SW_OPT);
	   eth_discard ();
	   return;
	 }

       // Set up reply path

       tag_table[i].tx_port = udp_srce;
       copy_ip (ip_hdr->srce, tag_table[i].ip);
       copy_mac (rx_pkt+6, tag_table[i].mac);

       memcpy (&msg->cmd_rc, (uchar *) udp_hdr+8, len); //const

       eth_discard ();

       msg->flags = 0x07;
       msg->tag = i;
       msg->srce_addr = p2p_addr;
       msg->srce_port = PORT_ETH;
       msg->length = len + 8;
       msg->dest_addr = tag_table[i].dest_addr;
       msg->dest_port = tag_table[i].dest_port;

       msg_queue_insert (msg, 0);
     }
}


void eth_receive ()
{
  while (1)
    {
      uint count = (er[ETH_STATUS] >> 1) & 63;

      if (count == 0)
	{
	  er[ETH_INT_CLR] = ETH_RX_CLR;
	  return;
	}

      uint rx_desc = er[ETH_RX_DESC_RP] & 0x3f;   // Descriptor table index
      uint rx_len = eth_rx_desc[rx_desc] & 0x7ff;  // Frame size
      uchar *rx_pkt = eth_rx_ram + (er[ETH_RX_BUF_RP] & 0xfff);

      uint etype = (rx_pkt[12] << 8) + rx_pkt[13];

      if (etype == ETYPE_IP)
	{
	  uint ip_prot = rx_pkt[IP_HDR_OFFSET + 9];

	  if (ip_prot == PROT_UDP)
	    udp_pkt (rx_pkt, rx_len);

	  else if (ip_prot == PROT_ICMP)
	    icmp_pkt (rx_pkt, rx_len);

	  else
	    eth_discard ();
	}
      else if (etype == ETYPE_ARP)
	arp_pkt (rx_pkt, rx_len, TAG_TABLE_SIZE);

      else
	eth_discard ();
    }
}


//------------------------------------------------------------------------------


void eth_send_msg (uint tag, sdp_msg_t *msg)
{
  iptag_t *iptag = tag_table + tag;

  if ((iptag->flags & IPFLAG_VALID) == 0)
    return;

  uchar hdr[44];
  ip_hdr_t *ip_hdr = (ip_hdr_t *) (hdr + IP_HDR_OFFSET);
  udp_hdr_t *udp_hdr = (udp_hdr_t *) (hdr + UDP_HDR_OFFSET);

  uint len = msg->length;

  uchar *buf;
  uint pad;

  if (iptag->flags & IPFLAG_STRIP)
    {
      buf = (uchar *) &msg->cmd_rc;
      len -= 8;
      pad = 0;
    }
  else
    {
      buf = (uchar *) &msg->flags;
      pad = 2;
    }

  if (iptag->flags & IPFLAG_REV)
    udp_hdr->srce = htons (iptag->rx_port);
  else
    udp_hdr->srce = htons (srom.udp_port);
    
  copy_ip_hdr (iptag->ip, PROT_UDP, ip_hdr,
	       len + pad + IP_HDR_SIZE + UDP_HDR_SIZE);

  udp_hdr->length = htons (len + pad + UDP_HDR_SIZE);
  udp_hdr->dest = htons (iptag->tx_port);
  udp_hdr->checksum = 0;		// Zero checksum

  uint t;
  t = ipsum (hdr + IP_HDR_OFFSET + 12, 8, 0); // Sum IP hdr addresses
  t += len + pad + UDP_HDR_SIZE;              // add in UDP data length
  t += PROT_UDP;			  // and UDP protocol number
  t = ipsum ((uchar *) udp_hdr, 8 + pad, t);   // and UDP header and pad
  t = ipsum (buf, len, t); 	  // and finally the data

  udp_hdr->checksum = htons (~t);

  mac_hdr_t *mac_hdr = (mac_hdr_t *) hdr;

  copy_mac (iptag->mac, mac_hdr->dest);
  copy_mac (srom.mac_addr, mac_hdr->srce);
  mac_hdr->type = htons (ETYPE_IP);

  eth_transmit2 (hdr, buf, 42 + pad, len);

  sark_delay_us (5); //## !! Trouble with back-to-back packets??

  if (iptag->flags & IPFLAG_TRANS)	  //transient tag
    iptag->flags = 0;
  else
    iptag->count++;
}


//------------------------------------------------------------------------------


uint shm_ping (uint dest)
{
  vcpu_t *vcpu = sv_vcpu + dest;

  vcpu->mbox_ap_cmd = SHM_NOP;

  sc[SC_SET_IRQ] = SC_CODE + (1 << v2p_map[dest]);

  volatile uchar flag = 0;

  event_t *e = event_new (proc_byte_set, (uint) &flag, 2);

  if (e == NULL)
    {
      sw_error (SW_OPT);
      return 1;
    }

  uint id = e->ID;

  timer_schedule (e, 1000); // !! const??

  while (vcpu->mbox_ap_cmd != SHM_IDLE && flag == 0)
    continue;

  if (flag != 0)
    return 0;

  timer_cancel (e, id);

  vcpu->lr = (uint) vcpu->mbox_ap_msg;

  return 1;
}


uint shm_send_msg (uint dest, sdp_msg_t *msg) // Send msg AP
{
  vcpu_t *vcpu = sv_vcpu + dest;

  sdp_msg_t *shm_msg = sark_shmsg_get ();

  if (shm_msg == NULL)
    return RC_BUF;

  sark_msg_cpy (shm_msg, msg);

  vcpu->mbox_ap_msg = shm_msg;
  vcpu->mbox_ap_cmd = SHM_MSG;

  sc[SC_SET_IRQ] = SC_CODE + (1 << v2p_map[dest]);

  volatile uchar flag = 0;

  event_t *e = event_new (proc_byte_set, (uint) &flag, 2);

  if (e == NULL)
    {
      sw_error (SW_OPT);
      sark_shmsg_free (shm_msg);
      return RC_BUF; // !! not the right RC
    }

  uint id = e->ID;

  timer_schedule (e, 1000); // !! const??

  while (vcpu->mbox_ap_cmd != SHM_IDLE && flag == 0)
    continue;

  if (flag != 0)
    {
      sark_shmsg_free (shm_msg);
      return RC_TIMEOUT;
    }

  timer_cancel (e, id);

  return RC_OK;
}


void swap_sdp_hdr (sdp_msg_t *msg)
{
  uint dest_port = msg->dest_port;
  uint dest_addr = msg->dest_addr;

  msg->dest_port = msg->srce_port;
  msg->srce_port = dest_port;

  msg->dest_addr = msg->srce_addr;
  msg->srce_addr = dest_addr;
}


void return_msg (sdp_msg_t *msg, uint rc) // Zero "rc" skips updating cmd_hdr
{
  uint f = msg->flags;

  if (f & SDPF_REPLY)
    {
      msg->flags = f & ~SDPF_REPLY;
      swap_sdp_hdr (msg);

      if (rc != 0)
	{
	  msg->cmd_rc = rc;
	  msg->length = 12;
	}

      msg_queue_insert (msg, 0);
    }
  else
    sark_msg_free (msg);
}


void proc_route_msg (uint arg1, uint srce_ip)
{
  sdp_msg_t *msg = (sdp_msg_t *) arg1;

  uint flags = msg->flags;

  if (flags & SDPF_SUM)
    {
/*
      uint sum = ipsum ((uchar *) &msg->length, msg->length+4, 0);
      if (sum != 0xffff) // !! fix this
	{
	  //	  msg_free (msg);
	  //	  return;
	}
*/
      msg->flags = flags &= ~SDPF_SUM;
    }

  uint dest_addr = msg->dest_addr;
  uint srce_addr = msg->srce_addr;

  // Off-chip via P2P

  if (dest_addr != srce_addr && dest_addr != p2p_addr &&
      (flags & SDPF_NR) == 0)
    {
      if (p2p_up == 0 || rtr_p2p_get (dest_addr) == 6)
	{
	  return_msg (msg, RC_ROUTE);
	  return;
	}

      uint rc = p2p_send_msg (dest_addr, msg);

      if (rc == RC_OK)
	sark_msg_free (msg);
      else
	return_msg (msg, rc);

      return;
    }

  if (msg->dest_port == PORT_ETH)
    {
      eth_send_msg (msg->tag, msg);
      sark_msg_free (msg);
      return;
    }

  uint dest_cpu = msg->dest_port & CPU_MASK;

  if (dest_cpu >= num_cpus)
    {
      return_msg (msg, RC_CPU);
      return;
    }

  if (dest_cpu != sark.virt_cpu) // !! virt_cpu always zero
    {
      uint rc = shm_send_msg (dest_cpu, msg);

      if (rc == RC_OK)
	sark_msg_free (msg);
      else
	return_msg (msg, rc);

      return;
    }

  uint dest_port = msg->dest_port >> PORT_SHIFT;

  if (dest_port == 0)
    {
      msg->length = 12 + scamp_debug (msg, srce_ip);
      return_msg (msg, 0);
    }
  else
    return_msg (msg, RC_PORT); // APs should not do this!!
}


//------------------------------------------------------------------------------


// Build virtual/physical CPU maps. Caller is monitor processor and
// passes its physical CPU.

void assign_virt_cpu (uint phys_cpu)
{
  for (uint phys = 0; phys < MAX_CPUS; phys++)
    sv->p2v_map[phys] = 255;

  sv->p2v_map[phys_cpu] = sark.virt_cpu = 0;
  sv->v2p_map[0] = phys_cpu;
  sark.vcpu = sv_vcpu + 0;
 
  uint virt = 1;

  for (uint phys = 0; phys < NUM_CPUS; phys++)
    {
      if (phys == phys_cpu)
	continue;

      if (sc[SC_CPU_OK] & (1 << phys))
	{
	  sv->p2v_map[phys] = virt;
	  sv->v2p_map[virt] = phys;
	  virt++;
	}
    }

  sv->num_cpus = num_cpus = virt;

  while (virt < MAX_CPUS)
    sv->v2p_map[virt++] = 255;

  // Turn off clocks of dead cores. If the monitor (this core) is marked as
  // dead, turn off the LED to make deadness more obvious.
  uint cpsr = cpu_int_disable ();
  if (!(sc[SC_CPU_OK] & (1 << phys_cpu)))
    sark_led_set (LED_OFF(0));
  sc[SC_CPU_DIS] = SC_CODE + (~sc[SC_CPU_OK] & ((1 << NUM_CPUS) - 1));
  cpu_int_restore (cpsr);

  sark_word_cpy (v2p_map, sv->v2p_map, MAX_CPUS);
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
  // (which is cleared above) have their clocks stopped. Thus, if the monitor
  // core (which is executing this code) was flagged as bad, it will also be
  // disabled.
  assign_virt_cpu (sark.phys_cpu);

  boot_ap ();

  sark_word_set (sv_vcpu + num_cpus, 0, sizeof (vcpu_t));
}

//------------------------------------------------------------------------------


uint ram_size (void *mem)
{
  volatile uint *ram = (uint *) mem;
  uint zf = 0xff0055aa;

  uint s = ram[0];
  uint i = 0;

  ram[0] = zf;

  while (1)
    {
      uint a = 1 << i++;
      uint s = ram[a];

      ram[a] = i;

      uint z = ram[0];
      uint v = ram[a];

      ram[a] = s;

      if ((v != i) || (z != zf))
	break;
    }

  ram[0] = s;

  if (i == 1)
    return 0;
      
  return 1 << (i + 1);
}


//------------------------------------------------------------------------------


const uint rst_init[] = {0x45206e49, 0x79726576, 0x65724420, 0x48206d61,
			 0x20656d6f, 0x65482061, 0x61747261, 0x00656863};


void get_board_info (void)
{
  sdp_msg_t msg;

  msg.arg1 = (256 << 16) + 32;	// 256 bytes, send 32 bit command
  msg.arg2 = (3 << 24) + 0x100;	// READ, address 0x100

  (void) cmd_srom (&msg);

  sark_word_cpy (sv_board_info, &msg.arg1, 256);

  if (sv_board_info[0] < 64)
    sv->board_info = sv_board_info;
}
  

void sv_init (void)
{
  sark_word_cpy (sv_vectors, rst_init, SV_VSIZE); 	// Copy Reset vectors
  sark_word_cpy (&srom, sv_srom, sizeof (srom_data_t));	// Copy SROM block

  sark_word_set ((void *) 0xf5007fc0, 0, 64);		// Kludge...

  if (srom.flags & SRF_PRESENT)				// Get board_info ??
    get_board_info ();

  if (sv->hw_ver == 0 && srom.flags & SRF_PRESENT)	// Set hardware version
    sv->hw_ver = (srom.flags >> 4) & 15;

  sv->sw_ver = SLLT_VER_NUM;				// Set software version

  sv->sysram_base = (uint *) SYS_USER_BASE;
  uint *sysram_top = sv->sysram_base + (uint) sv->sysram_heap;
  sv->sysram_heap = sark_heap_init (sysram_top, (uint *) SYS_USER_TOP);

  sark_word_set (sv_vcpu, 0, NUM_CPUS * VCPU_SIZE);	// Set up VCPU blocks
  sv->vcpu_base = sv_vcpu;

  sv_vcpu[0].cpu_state = CPU_STATE_RUN;
  sv_vcpu[0].sw_ver = SLLT_VER_NUM;
  sv_vcpu[0].time = sv->unix_time;
  sv_vcpu[0].phys_cpu = sark.phys_cpu;
  sark_str_cpy (sv_vcpu[0].app_name, build_name);

  // Set up SHM buffers

  sv->shm_buf = sark_xalloc (sv->sysram_heap,
			     sv->num_buf * sizeof (sdp_msg_t),
			     0, 0);

  sv->shm_root.free = (mem_link_t *) sv->shm_buf;
  //sv->shm_root.count = sv->shm_root.max = 0;		//## Not needed now...

  sark_block_init (sv->shm_buf, sv->num_buf, sizeof (sdp_msg_t));

  init_timer (&biff_timer, sv->biff_timer);
  init_timer (&p2pc_timer, sv->p2pc_timer);
  init_timer (&p2pb_timer, sv->p2pb_timer);
  init_timer (&probe_timer, sv->probe_timer);

  p2pb_timer.counter += sark_rand () & 7;

  //  p2pb_timer.increment = 50;	// 500ms
  //  p2pb_timer.multiplier = 1;
  //  p2pb_timer.counter = 1 + (sark_rand () & 7);

  //  probe_timer.counter = 1;
  //  probe_timer.increment = 10;
  //  probe_timer.multiplier = 1;
  //  probe_timer.mult_rep = 0;

  //  p2pc_timer.counter = 2;
  //  p2pc_timer.increment = 2;
  //  p2pc_timer.repeat = 10;
  //  p2pc_timer.mult_rep = 5;
  //  p2pc_timer.multiplier = 2;

  //  led_timer.increment = sv->led_period;
  //  led_timer.counter = led_timer.multiplier = 1;

  if (sv->boot_delay == 0 && sv->rom_cpus == 0)
    sv->root_chip = 1;
}


void sdram_init (void)
{
  // Initialise PL340

  pl340_init (sv->mem_clk);

  // Compute sizes

  uint sdram_size = ram_size (sdram);		// SDRAM size (bytes)
  uint sys_size = (uint) sv->sdram_bufs;	// System size (bytes)

  // Fill in sv->sdram... vars

  // System buffers at base of SDRAM
  sv->sdram_bufs = (uint *) SDRAM_BASE;

  // User SDRAM above them...
  sv->sdram_base = sv->sdram_bufs + NUM_CPUS * sv->sysbuf_size;
  uint *sdram_top = sv->sdram_base + (uint) sv->sdram_heap;

  // SDRAM heap
  uint *system = (uint *) (SDRAM_BASE + sdram_size - sys_size);

  sv->sdram_heap = sark_heap_init (sdram_top, system);

  // System block - 256K bytes
  sv->sdram_sys = system;
  system += 65536;

  // Make system heap in SDRAM (Allowing space for system bufs)

  sv->sys_heap = sark_heap_init (system, system + sys_size / 4 - 65536);

  // Allocate system heap
  // Hop table - 64k entries of a word

  sv->hop_table = sark_xalloc (sv->sys_heap, HOP_TABLE_SIZE * 4, 0, 0);

  sark_word_set (sv->hop_table, 0x8000ffff, HOP_TABLE_SIZE * 4);

  // Router MC table copy (NB 1 extra entry in copy table)

  sv->rtr_copy = sark_xalloc (sv->sys_heap,
			      (MC_TABLE_SIZE + 1) * sizeof (rtr_entry_t),
			      0, 0);

  // Alloc ID table

  sv->alloc_tag = sark_xalloc (sv->sys_heap, 65536 * 4, 0, 0);

  sark_word_set (sv->alloc_tag, 0, 65536 * 4);

  // AppID table

  sv->app_data = sark_xalloc (sv->sys_heap, 256 * sizeof (app_data_t), 0, 0);

  sark_word_set (sv->app_data, 0, 256 * sizeof (app_data_t));
}


void random_init (void)
{
  /* Now done from SDRAM...
  if (sv->random == 0)
    {
      uint rand = 0xa5;					// Set up random seed

      for (uint i = 0; i < SV_RSIZE / 4; i++)
	rand ^= (i & 1) ? sv_random[i] : ~sv_random[i];

      sv->random = rand;
    }
  */
  // Make random seed

  uint *rnd = sark_xalloc (sv->sys_heap, 256 * 4, 0, 0);

  if (sv->random == 0)
    {
      uint rand = 0xa5;					// Set up random seed

      for (uint i = 0; i < 256; i++)
	rand ^= (i & 1) ? rnd[i] : ~rnd[i];

      sv->random = rand;
    }

  sark_srand (sv->random);
}

//------------------------------------------------------------------------------

// "proc_1hz" is put on the event queue every second

void proc_1hz (uint a1, uint a2)
{
  if (srom.flags & SRF_ETH)
    {
      uint s = phy_read (PHY_STATUS);
      sv->eth_up = (s & 4) >> 2;
    }

}


void soft_wdog (uint max)
{
  vcpu_t *vcpu = sv_vcpu + ping_cpu;

  if (vcpu->cpu_state >= 4) // !! const
    {
      if (shm_ping (ping_cpu))
	{
	  watchdog[ping_cpu] = 0;
	}
      else if (++watchdog[ping_cpu] == max)
	{
	  clock_ap (1 << v2p_map[ping_cpu], 0);	// Disable clock

	  vcpu_t *vcpu = sv_vcpu + ping_cpu;
	  vcpu->cpu_state = CPU_STATE_WDOG;
	}
    }
  else
    watchdog[ping_cpu] = 0;

  if (++ping_cpu >= num_cpus)
    ping_cpu = 1;
}


//------------------------------------------------------------------------------

uint booted;

// Produce a spanning tree of the system using the P2P routes constructed.
void compute_st (void)
{
  // Definately route here
  uint route = MC_CORE_ROUTE (0);

  // Compile the set of neighbours which route to (0, 0) via this chip and add
  // them to the route, forming a spanning tree.
  uint timeout = sv->peek_time;
  for (uint link = 0; link < NUM_LINKS; link++)
    {
      if (!((1 << link) & link_up & link_en))
        continue;
      
      // Try to read multiple times if required
      uint attempts_remaining = 2;
      uint remote_rtr_p2p_0;
      uint rc;
      do
        rc = link_read_word ((uint)(rtr_p2p), link, &remote_rtr_p2p_0, timeout);
      while (rc != RC_OK && (--attempts_remaining));
      
      // Flag an error if we could not get a p2p entry from a neighbour
      if (rc != RC_OK)
        {
          sw_error (SW_OPT);
          continue;
        }
      
      // Check if (0, 0) route from neighbour points at this chip.
      if ((remote_rtr_p2p_0 & 0x7) == ((link + 3) % 6))
        route |= MC_LINK_ROUTE (link);
    }

  rtr_mc_set (0, 0xffff5555, 0xffffffff, route);
}


// "proc_100hz" is put on the event queue every 10ms

void proc_100hz (uint a1, uint a2)
{
  ticks++;
  
  // Flood-fill all board-info (allowing all dead cores, chips and links to get
  // disabled before P2P tables are configured).
  if (run_timer (&biff_timer))
    {
      if (sv->board_info)
        {
          uint num_info_words = sv->board_info[0];
          uint *info_word = sv->board_info + 1;
          while (num_info_words--)
            {
              // Handle command on this chip
              nn_cmd_biff(0, 0, *(info_word));
              // Also flood to other chips on this board
              biff_nn_send(*(info_word++));
            }
        }
      
      // BIFF is complete when we've sent the BIFF info for the last time
      biff_complete = biff_timer.repeat == 255;
    }

  // Send out board coordinate information after the board-info flood-fill
  // process has been allowed to complete

  if (biff_complete && sv->root_chip && run_timer (&p2pc_timer))
    {
      // For board test only send on links 0-2
      uint p2pc_fwd = (sv->bt_flags & 1) ? 0x0700 : 0x3f00;

      // Send P2PC packet

      send_p2pc ((sv->p2p_dims << 16) + sv->p2p_addr, p2pc_fwd);
    }

  // Process IPTag timeouts

  iptag_timer ();

  // Ping application CPUs to check on status

  if (sv->soft_wdog)
    soft_wdog (sv->soft_wdog);

  // Flip LED0 every now and then

  if (booted && sv->led_period != 0 && ++led_timer.counter >= sv->led_period)
    {
      led_timer.counter = 0;
      sark_led_set (LED_INV(0));	// !! assumes LED_0 always there
    }

  // Probe external links

  if (run_timer (&probe_timer))
    {
      (void) probe_links (0x80 + 0x3f, sv->peek_time);
    }
    
  // Send P2PB packet if P2P is up, on the final iteration, compute the
  // spanning tree and update the level table, sending no more P2PBs

  if (p2p_up && run_timer (&p2pb_timer))
    {
      // On all but the last iteration, broadcast P2P coordinates for P2P table
      // construction
      if (p2pb_timer.repeat != 255)
        {
          hop_table[p2p_addr] = (p2pb_id >> 1) << 24;

          uint flags = 0; //!!

          ff_nn_send ((NN_CMD_P2PB << 24) + (0x3e00 << 8) + p2pb_id,
                      (p2p_addr << 16) + (flags << 12) + 1,
                      0x3f00,
                      0);

          p2pb_id += 2; // Relies on p2pb_id being 8 bits
        }
      else
        {
          compute_st ();
          level_config ();
          booted = 1;
        }
    }

  // Send LTPC packet (untested!)

  if (sv->ltpc_period > 0 && ++ltpc_timer >= sv->ltpc_period)
    {
      ltpc_timer = 0;

      ff_nn_send ((NN_CMD_LTPC << 24) + (0x3e00 << 8),
		  sv->tp_timer,
		  0x3f00,
		  0);
    }
}

//------------------------------------------------------------------------------

// Initialise PLLs

static uint pll_mult (uint freq)
{
  uint f = 0;

  if (freq >= 50)
    f = 1;
  if (freq >= 100)
    f = 2;
  if (freq >= 200)
    f = 3;

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
// To run CPUs at 150, SDRAM at 130, system bus and router at 100
//
void pll_init ()
{
  sark.cpu_clk = 10;			// Set for delay_us
  sc[SC_CLKMUX] = 0;			// Switch to 10MHz everywhere

  sark_delay_us (2);				// and wait a while

  uint cpu_freq = sv->cpu_clk * 2;	// Set local values
  uint mem_freq = sv->mem_clk * 2;

  divmod_t r;				// Divide by 10 and set PLLs

  r = sark_div10 (cpu_freq);		// CPUs on PLL1
  sc[SC_PLL1] = 0x00040100 + pll_mult (cpu_freq) + r.div;

  r = sark_div10 (mem_freq);		// SDRAM on PLL2
  sc[SC_PLL2] = 0x00040100 + pll_mult (mem_freq) + r.div;

  sark_delay_us (PLL_LOCK_TIME);	// Wait for PLLs to lock

  uint sys_div = ((sv->clk_div >> 4) - 1) & 3;
  uint rtr_div = (sv->clk_div - 1) & 3;

  sark.cpu_clk = cpu_freq / 2;
  sc[SC_CLKMUX] = 0x801088a5 + (sys_div << 22) + (rtr_div << 17);
}


//------------------------------------------------------------------------------



void eth_setup ()
{
  if (srom.flags & SRF_ETH)		// Ethernet present (possibly)
    {
      eth_init (srom.mac_addr);

      sark_word_cpy (sv->ip_addr, srom.ip_addr, 4);

      if (srom.flags & SRF_PHY_RST)		// Hardware reset PHY
	{
	  phy_reset ();
	}

      if (srom.flags & SRF_PHY_INIT)		// (Re-)initialise PHY
	{
	  phy_write (PHY_AUTO_ADV, 0x01e1);	// Allow 100/10 meg
	  phy_write (PHY_CONTROL, 0x1200);	// Enable & restart auto-neg
	}

      while (srom.flags & SRF_PHY_WAIT)		// Wait (without timeout)
	{
	  uint s = phy_read (PHY_STATUS);	// Read PHY status
	  sv->eth_up = (s & 4) >> 2;		// Bit 2 says link up
	  if (sv->eth_up)
	    break;
	  event_run (1);
	}
    }
}


//------------------------------------------------------------------------------


void jtag_init (void)
{
  sc[GPIO_CLR] = JTAG_NTRST;		// Ensure NTRST is low
  sc[SC_MISC_CTRL] |= JTAG_INT; 	// Drive JTAG internally
  sark_delay_us (1);			// Wait a short while
  sc[SC_MISC_CTRL] &= ~JTAG_INT; 	// Drive JTAG externally
}


void sark_config (void)
{
  sark_vec->num_msgs = 16;		// Allocate 16 SDP messages
  sark_vec->num_events = 64;		// and 64 events
  sark_vec->app_id = 0;			// Just to be sure...
  sark_vec->app_flags &= ~(1 << APP_FLAG_WAIT); // Don't wait in SARK start-up
}


void c_main (void)
{
  sark.cpu_clk = 160;			// BootROM uses 160 MHz

  jtag_init ();				// Reset JTAG internals

  wd[WD_LOCK] = WD_CODE;		// Disable watchdog!
  wd[WD_CTRL] = 0;

  sc[SC_MISC_CTRL] |= 1;  		// Swap RAM/ROM

  assign_virt_cpu (sark.phys_cpu);	// Assign virtual CPUs

  sv_init ();				// Initialise system RAM

  sark_led_init ();			// Initialise LED drivers

  pll_init ();				// Restart PLLs

  sdram_init ();			// Initialise SDRAM

  random_init ();			// Initialise random

  rtr_init (sark.phys_cpu);		// Initialise router

  if (sv->boot_delay)			// If bootROM boot
    boot_nn (sv->hw_ver);		// Flood fill neighbours

  boot_ap ();				// Start local APs

  timer1_init (sark.cpu_clk * 1000);	// Initialise 1ms timer

  queue_init ();			// Initialise various queues

  nn_init ();				// Initialise NN package

  //  link_up = 0x3f; //##

  vic_setup ();				// Set VIC, interrupts on

  if (sv->boot_delay)
    eth_setup ();		        // Set up Ethernet if present

  while (1)				// Run event loop (forever...)
    {
      event_run (1);
      cpu_wfi ();
    }
}


//------------------------------------------------------------------------------
