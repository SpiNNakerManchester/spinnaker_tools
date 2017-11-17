//------------------------------------------------------------------------------
//
// bmp_net.c	    Networking code for BMP LPC1768
//
// Copyright (C)    The University of Manchester - 2012-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

#include <string.h>

#include "lpc17xx.h"
#include "bmp.h"

//------------------------------------------------------------------------------

// Internet constants and structs

#define ntohs(t)	((((t) & 0x00ff) << 8) | (((t) & 0xff00) >> 8))
#define htons(t)	((((t) & 0x00ff) << 8) | (((t) & 0xff00) >> 8))

#define MAC_HDR_SIZE    	14
#define IP_HDR_SIZE     	20
#define UDP_HDR_SIZE		8
#define SDP_PAD_SIZE		2

#define IP_HDR_OFFSET   	MAC_HDR_SIZE
#define IP_DATA_OFFSET  	(IP_HDR_OFFSET + IP_HDR_SIZE)

#define UDP_HDR_OFFSET		IP_DATA_OFFSET
#define UDP_DATA_OFFSET		(UDP_HDR_OFFSET + UDP_HDR_SIZE)

#define ETYPE_IP  		0x0800
#define ETYPE_ARP 		0x0806

#define PROT_ICMP 		1
#define PROT_UDP  		17

#define ARP_REQ 		1
#define ARP_REPLY 		2

#define ICMP_ECHO_REPLY 	0
#define ICMP_ECHO_REQ 		8


typedef struct {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t op;
    uint8_t sha[6];
    uint8_t spa[4];
    uint8_t tha[6];
    uint8_t tpa[4];
} arp_pkt_t;

typedef struct {
    uint16_t srce;
    uint16_t dest;
    uint16_t length;
    uint16_t checksum;
} udp_hdr_t;

typedef struct {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t ident;
    uint16_t seq;
} icmp_hdr_t;

typedef struct {
    uint8_t dest[6];
    uint8_t srce[6];
    uint16_t type;
} mac_hdr_t;

typedef struct {
    uint8_t ver_len;
    uint8_t DS;
    uint16_t length;
    uint16_t ident;
    uint16_t flg_off;
    uint8_t TTL;
    uint8_t protocol;
    uint16_t checksum;
    uint8_t srce[4];
    uint8_t dest[4];
} ip_hdr_t;

//------------------------------------------------------------------------------

#define NUM_MSGS 	        8
#define MAX_MSG  		(NUM_MSGS - 1)
#define MSG_QUEUE_SIZE 		NUM_MSGS

typedef struct msg_queue_t {
    uint8_t insert;
    uint8_t remove;
    volatile uint8_t count;
    uint8_t max;
    sdp_msg_t *queue[MSG_QUEUE_SIZE];
} msg_queue_t;

//------------------------------------------------------------------------------

static uint8_t eth_buf[384];

iptag_t tag_table[TAG_TABLE_SIZE];

static sdp_msg_t msg_bufs[NUM_MSGS];
static sdp_msg_t *msg_root;
static uint32_t msg_count;
static uint32_t msg_max;

static msg_queue_t msg_queue;

static const uint8_t bc_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static const uint8_t zero_mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

ip_data_t spin_ip;
ip_data_t bmp_ip;

uint32_t tag_tto = 9;	// 2.56s = 10ms * (1 << (9-1))

//------------------------------------------------------------------------------

void msg_init()
{
    sdp_msg_t *msg = msg_root = msg_bufs;

    for (uint32_t i = 0; i < NUM_MSGS; i++) {
	msg->next = (i != MAX_MSG) ? msg + 1 : NULL;
	msg++;
    }

    msg_queue.insert = 1;
}

sdp_msg_t* msg_get()
{
    uint32_t cpsr = cpu_int_off();

    sdp_msg_t *msg = msg_root;

    if (msg != NULL) {
	msg_root = msg->next;
	msg->next = NULL; // !! Needed??

	msg_count++;
	if (msg_count > msg_max) {
	    msg_max = msg_count;
        }
    }

    cpu_int_restore(cpsr);
    return msg;
}

void msg_free(sdp_msg_t *msg)
{
    uint32_t cpsr = cpu_int_off();

    msg->next = msg_root;
    msg_root = msg;

    msg_count--;

    cpu_int_restore(cpsr);
}

uint32_t msg_queue_insert(sdp_msg_t *msg)
{
    if (msg_queue.count >= MSG_QUEUE_SIZE) {	// !! Fatal error? - free msg?
	//##      sw_error (SW_ERR + 0);
	return 0;
    }

    uint32_t cpsr = cpu_int_off();

    msg_queue.queue[msg_queue.insert] = msg;
    msg_queue.count++;

    if (msg_queue.count > msg_queue.max) {
	msg_queue.max = msg_queue.count;
    }

    msg_queue.insert = (msg_queue.insert + 1) % MSG_QUEUE_SIZE;
    cpu_int_restore(cpsr);
    return 1;
}

sdp_msg_t *msg_queue_remove()
{
    uint32_t cpsr = cpu_int_off();

    msg_queue.remove = (msg_queue.remove + 1) % MSG_QUEUE_SIZE;
    msg_queue.count--;

    cpu_int_restore(cpsr);

    return msg_queue.queue[msg_queue.remove];
}

uint32_t msg_queue_size(void)
{
    return msg_queue.count;
}

//------------------------------------------------------------------------------

uint32_t ipsum(uint8_t *d, uint32_t len, uint32_t sum) // Use shorts for speed??
{
    if (len & 1) {
	sum += d[--len] << 8;
    }

    for (uint32_t i = 0; i < len; i += 2) {
	sum += (d[i] << 8) + d[i+1];
    }

    while (sum >> 16) {
	sum = (sum >> 16) + (sum & 0xffff);
    }

    return sum;
}

void copy_mac(const uint8_t *f, uint8_t *t)
{
    uint16_t *ts = (uint16_t*) t;
    uint16_t *fs = (uint16_t*) f;

    ts[0] = fs[0];
    ts[1] = fs[1];
    ts[2] = fs[2];
}

void copy_ip(const uint8_t *f, uint8_t *t)
{
    uint16_t *ts = (uint16_t*) t;
    uint16_t *fs = (uint16_t*) f;

    ts[0] = fs[0];
    ts[1] = fs[1];
}

uint32_t cmp_ip(const uint8_t *a, const uint8_t *b)
{
    uint16_t *as = (uint16_t*) a;
    uint16_t *bs = (uint16_t*) b;

    return (as[0] == bs[0]) && (as[1] == bs[1]);
}

//------------------------------------------------------------------------------

void iptag_timer()
{
    iptag_t *tag = tag_table;

    for (uint32_t i = 0; i < TAG_TABLE_SIZE; i++) {
	if ((tag->flags & IPTAG_VALID) && (tag->timeout > 0)) {
	    tag->timeout--;
	    if (tag->timeout == 0) {
		tag->flags = 0;
	    }
	}
	tag++;
    }
}

uint32_t iptag_new()
{
    for (uint32_t i = FIRST_POOL_TAG; i <= LAST_POOL_TAG; i++) {
	if (tag_table[i].flags == 0) {
	  return i;
        }
    }

    return TAG_NONE;
}

uint32_t transient_tag(uint8_t *ip, uint8_t *mac, uint32_t port, uint32_t timeout)
{
    uint32_t tag = iptag_new();

    if (tag != TAG_NONE) {
	iptag_t *tt = tag_table+tag;

	tt->flags = IPTAG_VALID + IPTAG_TRANS + timeout;
	tt->port = port;
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

void copy_ip_hdr(uint8_t *dest, uint32_t prot, ip_hdr_t *ip, uint32_t len)
{
    ip->ver_len = 0x45;
    ip->DS = 0;
    ip->length = htons(len);
    ip->ident = htons(0);
    ip->flg_off = htons(2 << 13);	// Flags = 010, offset = 0
    ip->TTL = 64;
    ip->protocol = prot;		// Insert protocol
    ip->checksum = htons(0);
    copy_ip(dest, ip->dest);		// Source IP address
    copy_ip(bmp_ip.ip_addr, ip->srce);	// My IP address

    uint32_t sum = ipsum((uint8_t *) ip, IP_HDR_SIZE, 0);// Update checksum
    ip->checksum = htons(~sum);
}

void copy_udp(uint8_t *buf, uint32_t len, uint32_t dest, uint32_t srce)
{
    udp_hdr_t *udp_hdr = (udp_hdr_t *) (buf+UDP_HDR_OFFSET);

    udp_hdr->srce = htons(srce);	// Source port
    udp_hdr->dest = htons(dest);	// Dest port
    udp_hdr->length = htons(len);	// Insert length
    udp_hdr->checksum = 0;		// Zero checksum

    uint32_t t;
    t = ipsum(buf+IP_HDR_OFFSET+12, 8, 0); // Sum IP hdr addresses
    t += len;				// add in UDP data length
    t += PROT_UDP;			// and UDP protocol number
    t = ipsum((uint8_t *) udp_hdr, len, t); // Final UDP checksum

    udp_hdr->checksum = htons(~t);
}

//------------------------------------------------------------------------------

void eth_transmit(uint8_t *buf, uint32_t len, uint32_t type,
	const uint8_t *dest)
{
    mac_hdr_t *mac_hdr = (mac_hdr_t *) buf;

    copy_mac(dest, mac_hdr->dest);
    copy_mac(bmp_ip.mac_addr, mac_hdr->srce);
    mac_hdr->type = htons(type);

    if (len < 60) {
	len = 60;
    }

    while (! eth_tx_rdy()) {
	continue;
    }

    eth_copy_txbuf((uint32_t *) buf, len);
    eth_update_tx();
}

void send_arp_pkt(uint8_t *buf, const uint8_t *dest, const uint8_t *tha,
	const uint8_t *tpa, uint32_t type)
{
    arp_pkt_t *arp = (arp_pkt_t *) (buf + MAC_HDR_SIZE);

    arp->htype = htons(0x0001);		// Ethernet
    arp->ptype = htons(0x0800);		// IPv4
    arp->hlen = 6;
    arp->plen = 4;
    arp->op = htons(type);

    copy_mac(tha, arp->tha);		// Copy to THA
    copy_ip(tpa, arp->tpa);		// Copy to TPA

    copy_mac(bmp_ip.mac_addr, arp->sha);// Copy my MAC addr
    copy_ip(bmp_ip.ip_addr, arp->spa); 	// Copy my IP addr

    eth_transmit(buf, 42, ETYPE_ARP, dest);
}

void arp_pkt(uint8_t *buf, uint32_t rx_len)
{
    arp_pkt_t *arp = (arp_pkt_t *) (buf + MAC_HDR_SIZE);

    if (!cmp_ip(arp->tpa, bmp_ip.ip_addr)) { // Ignore unless TPA matches
	return;
    }

    uint32_t op = ntohs(arp->op);

    if (op == ARP_REQ) {
	send_arp_pkt(buf, buf+6, arp->sha, arp->spa, ARP_REPLY);
    } else if (op == ARP_REPLY) {	// Reply & TPA matches
	iptag_t *tt = tag_table;

	for (uint32_t i = 0; i < TAG_TABLE_SIZE; i++) {
	    uint32_t f = tt->flags;

	    if ((f & IPTAG_ARP) && cmp_ip(arp->spa, tt->mac)) { // !! Bodge
		copy_mac(arp->sha, tt->mac);
		f &= ~IPTAG_ARP;
		tt->flags = f | IPTAG_VALID;
		break;
	    }

	    tt++;
	}
    }
}

void icmp_pkt(uint8_t *buf, uint32_t rx_len)
{
    ip_hdr_t *ip_hdr = (ip_hdr_t *) (buf + IP_HDR_OFFSET);

    if (!cmp_ip (ip_hdr->dest, bmp_ip.ip_addr)) {
	return;
    }

    uint32_t ip_hdr_len = (ip_hdr->ver_len & 15) * 4;

    icmp_hdr_t *icmp = (icmp_hdr_t *) (buf + IP_HDR_OFFSET + ip_hdr_len);

    if (icmp->type == ICMP_ECHO_REQ) {
	uint32_t icmp_len = ntohs(ip_hdr->length) - ip_hdr_len;	// Size of ICMP hdr+data

	copy_ip_hdr(ip_hdr->srce, PROT_ICMP, ip_hdr, icmp_len + IP_HDR_SIZE);

	if (ip_hdr_len > IP_HDR_SIZE) {		// Copy down ICMP header & data
	    icmp = (icmp_hdr_t *) (buf + IP_DATA_OFFSET); // 'new' ICMP hdr
	    memcpy((uint8_t *) icmp, (uint8_t *) icmp + ip_hdr_len, icmp_len);
	}

	icmp->type = ICMP_ECHO_REPLY;		// ICMP reply
	icmp->code = 0;				// ICMP code = 0
	icmp->checksum = 0;

	uint32_t sum = ipsum((uint8_t *) icmp, icmp_len, 0);
	icmp->checksum = htons(~sum);

	eth_transmit(buf, IP_DATA_OFFSET + icmp_len, ETYPE_IP, buf+6);
    }
}

void udp_pkt(uint8_t *rx_pkt, uint32_t rx_len)
{
    ip_hdr_t *ip_hdr = (ip_hdr_t *) (rx_pkt + IP_HDR_OFFSET);
    uint32_t ip_len = (ip_hdr->ver_len & 15) * 4;

    udp_hdr_t *udp_hdr = (udp_hdr_t *) (rx_pkt + IP_HDR_OFFSET + ip_len);
    uint32_t udp_dest = ntohs(udp_hdr->dest);

    if ((udp_dest == bmp_ip.udp_port) && cmp_ip(ip_hdr->dest, bmp_ip.ip_addr)
	    && rx_len > 52) {			//const NB 52
	uint32_t udp_srce = ntohs(udp_hdr->srce);
	int32_t len = ntohs(udp_hdr->length) - 10; //const UDP_HDR + UDP_PAD

	if (len > (24 + 256)) {			// SDP=8, CMD=16
	    return;
	}

	sdp_msg_t *msg = msg_get();
	if (msg == NULL) {			// !! fix this - reply somehow?
	    return;
	}

	memcpy(&msg->flags, (uint8_t *) udp_hdr+10, len); //const

	msg->length = len;
	msg->srce_port = (7 << PORT_SHIFT) + board_ID;
	msg->srce_addr = ee_data.frame_ID;

	uint32_t flags = msg->flags;
	uint32_t tag = msg->tag;

	if ((tag == TAG_NONE) && (flags & SDPF_REPLY)) { // transient tag & reply req
	    tag = msg->tag = transient_tag(ip_hdr->srce, rx_pkt+6, udp_srce, tag_tto);
	}

	if (((flags & SDPF_REPLY) == 0) ||
		(tag < TAG_TABLE_SIZE && tag_table[tag].flags != 0)) {
	    msg_queue_insert(msg);
	} else {
	    msg_free(msg);
	}
    }
}

void eth_receive()
{
    uint8_t *rx_pkt = eth_buf;
    int32_t len = eth_rx_size() - 3;	// Removes CRC ??

    if (len > 384) {
	eth_rx_discard();
	return;
    }

    eth_copy_rxbuf((uint32_t *) eth_buf, len);
    eth_rx_discard();

    uint32_t etype = (rx_pkt[12] << 8) + rx_pkt[13];

    if (etype == ETYPE_IP) {
	uint32_t ip_prot = rx_pkt[IP_HDR_OFFSET + 9];

	if (ip_prot == PROT_UDP) {
	    udp_pkt(rx_pkt, len);
	} else if (ip_prot == PROT_ICMP) {
	    icmp_pkt(rx_pkt, len);
        }
    } else if (etype == ETYPE_ARP) {
	arp_pkt(rx_pkt, len);
    }
}

void eth_transmit2(uint8_t *hdr, uint8_t *buf, uint32_t len, uint8_t *dest)
{
    mac_hdr_t *mac_hdr = (mac_hdr_t *) hdr;

    copy_mac(dest, mac_hdr->dest);
    copy_mac(bmp_ip.mac_addr, mac_hdr->srce);
    mac_hdr->type = htons(ETYPE_IP);

    memcpy(eth_buf, hdr, 44);
    memcpy(eth_buf + 44, buf, len);

    len += 44;

    if (len < 60) {
	len = 60;
    }
    while (! eth_tx_rdy()) {
	continue;
    }
    eth_copy_txbuf((uint32_t *) eth_buf, len);
    eth_update_tx();
}

void eth_send_msg(uint32_t tag, sdp_msg_t *msg)
{
    uint8_t hdr[44];
    iptag_t *iptag = tag_table + tag;

    if ((iptag->flags & IPTAG_VALID) == 0) {
	return;
    }

    uint32_t len = msg->length;

    ip_hdr_t *ip_hdr = (ip_hdr_t *) (hdr + IP_HDR_OFFSET);

    copy_ip_hdr(iptag->ip, PROT_UDP, ip_hdr,
	    len + IP_HDR_SIZE + UDP_HDR_SIZE + SDP_PAD_SIZE);

    udp_hdr_t *udp_hdr = (udp_hdr_t *) (hdr + UDP_HDR_OFFSET);

    udp_hdr->srce = htons(bmp_ip.udp_port);
    udp_hdr->dest = htons(iptag->port);
    udp_hdr->length = htons(len+UDP_HDR_SIZE+SDP_PAD_SIZE);// Insert length
    udp_hdr->checksum = 0;			// Zero checksum

    uint32_t t;
    t = ipsum(hdr+IP_HDR_OFFSET+12, 8, 0);	// Sum IP hdr addresses
    t += len + UDP_HDR_SIZE + SDP_PAD_SIZE;	// add in UDP data length
    t += PROT_UDP;				// and UDP protocol number
    t = ipsum((uint8_t *) udp_hdr, 10, t);	// and UDP header and pad
    t = ipsum(&msg->flags, len, t);		// and finally the data

    udp_hdr->checksum = htons(~t);

    eth_transmit2(hdr, &msg->flags, len, iptag->mac);

    if (iptag->flags & IPTAG_TRANS) {		//transient tag
	iptag->flags = 0;
    } else {
	iptag->count++;
    }
}

void swap_sdp_hdr(sdp_msg_t *msg)
{
    uint32_t dest_port = msg->dest_port;
    uint32_t dest_addr = msg->dest_addr;

    msg->dest_port = msg->srce_port;
    msg->srce_port = dest_port;

    msg->dest_addr = msg->srce_addr;
    msg->srce_addr = dest_addr;
}

void return_msg(sdp_msg_t *msg, uint32_t rc) // Zero "rc" skips updating cmd_hdr
{
    uint32_t f = msg->flags;

    if (f & SDPF_REPLY) {
	msg->flags = f & ~SDPF_REPLY;
	swap_sdp_hdr(msg);

	if (rc != 0) {
	    msg->cmd_rc = rc;
	    msg->length = 12;
	}

	msg_queue_insert(msg);
    } else {
	msg_free(msg);
    }
}

void route_msg(sdp_msg_t *msg)
{
    uint32_t dest = msg->dest_port & BOARD_MASK;

    if (dest != board_ID) {
	uint32_t rc = can_send_msg(dest, msg);

	if (rc == RC_OK) {
	    msg_free(msg);
        } else {
            return_msg(msg, rc);
	}
	return;
    }

    uint32_t port = msg->dest_port >> PORT_SHIFT;

    if (port == 0) {
	msg->length = 12 + debug(msg);
	return_msg(msg, 0);
    } else if (port == 7) {
	eth_send_msg(msg->tag, msg);
	msg_free(msg);
    } else {
	return_msg(msg, RC_PORT);
    }
}

void arp_lookup(iptag_t *iptag)
{
    uint8_t buf[128];

    uint8_t *addr = iptag->ip;
    uint32_t *my_ip = (uint32_t *) bmp_ip.ip_addr;
    uint32_t *ip = (uint32_t *) addr;
    uint32_t *mask = (uint32_t *) bmp_ip.net_mask;

    const uint8_t *target_ip = addr;

    if ((*my_ip & *mask) != (*ip & *mask)) {
	target_ip = bmp_ip.gw_addr;
    }

    copy_ip(target_ip, iptag->mac); // !! Bodge - target IP in MAC field!

    send_arp_pkt(buf, bc_mac, zero_mac, target_ip, ARP_REQ);
}

//------------------------------------------------------------------------------

// ## Low level Tube

static uint8_t tube_buf[384];
static uint32_t tube_ptr;

void eth_putc(uint32_t c)
{
    uint8_t *buf = (uint8_t *) tube_buf + 56; 	// Point at start of msg buffer

    buf[tube_ptr++] = c;			// Insert char at end

    if (c == 0 || c == '\n' || tube_ptr == 256) {
	iptag_t *tag = tag_table;

	if (tag->flags & IPTAG_VALID) {
	    sdp_msg_t *msg = (sdp_msg_t *) (buf - 20);

	    msg->cmd_rc = 64;
	    msg->srce_port = board_ID;
	    msg->srce_addr = ee_data.frame_ID;

	    ip_hdr_t *ip_hdr = (ip_hdr_t *) (tube_buf + IP_HDR_OFFSET);

	    copy_ip_hdr(tag->ip, PROT_UDP, ip_hdr,
		    IP_HDR_SIZE + UDP_HDR_SIZE + 14 + tube_ptr);

	    copy_udp(tube_buf, UDP_HDR_SIZE + 14 + tube_ptr, tag->port, 0);

	    eth_transmit(tube_buf, 56 + tube_ptr, ETYPE_IP, tag->mac);
	}

	tube_ptr = 0;
    }
}

//------------------------------------------------------------------------------

void copy_ip_data(void)
{
    // Copy the IP data from sector 1 of BMP Flash
    memcpy(&bmp_ip, bmp_flash_ip, sizeof(ip_data_t));
    memcpy(&spin_ip, spin_flash_ip, sizeof(ip_data_t));

    // Fill in defaults if not programmed
    if (bmp_ip.flags == 0xffff || !data_ok) {
	bmp_ip.flags = 0x8000;
	bmp_ip.udp_port = 17893;
    }

    if (spin_ip.flags == 0xffff || !data_ok) {
	spin_ip.flags = 0x8059;
	spin_ip.udp_port = 17893;
    }
}

//------------------------------------------------------------------------------
