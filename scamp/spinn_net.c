//------------------------------------------------------------------------------
//
// spinn_net.c	    Ethernet/IP support routines for Spinnaker
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

#include "spinn_net.h"

#include <string.h>


extern srom_data_t srom;
extern iptag_t tag_table[];


const uchar bc_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
const uchar zero_mac[] = {0, 0, 0, 0, 0, 0};


#ifdef LOCK_ETH

static void lock_get(uint lock)
{
    while (sc[SC_TAS0 + lock] & BIT_31) {
	continue;
    }
}

static void lock_free(uint lock)
{
    (void) sc[SC_TAC0 + lock];
}

#endif // LOCK_ETH


void eth_discard(void) __attribute__((always_inline));

void eth_discard(void)
{
    static volatile uint * const er = (uint *) ETH_REGS;
    er[ETH_RX_CMD] = (uint) er;
}


void eth_init(uchar *mac)
{
    er[ETH_CONTROL] = 0;

    er[ETH_MAC_LO] = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | mac[5];
    er[ETH_MAC_HI] = (mac[0] << 8) | mac[1];

    er[ETH_CONTROL] = 0x45b;
}


#pragma push
#pragma arm

uint ipsum(uchar *d, uint len, uint sum) // Use shorts for speed??
{
    if (len & 1) {
	sum += d[--len] << 8;
    }

    for (uint i = 0; i < len; i += 2) {
	sum += (d[i] << 8) + d[i+1];
    }

    while (sum >> 16) {
	sum = (sum >> 16) + (sum & 0xffff);
    }

    return sum;
}

#pragma pop


void copy_mac(const uchar *f, uchar *t)
{
    ushort *ts = (ushort*) t;
    ushort *fs = (ushort*) f;

    ts[0] = fs[0];
    ts[1] = fs[1];
    ts[2] = fs[2];
}

void copy_ip(const uchar *f, uchar *t)
{
    ushort *ts = (ushort*) t;
    ushort *fs = (ushort*) f;

    ts[0] = fs[0];
    ts[1] = fs[1];
}

uint cmp_ip(uchar *a, uchar *b)
{
    ushort *as = (ushort*) a;
    ushort *bs = (ushort*) b;

    return (as[0] == bs[0]) && (as[1] == bs[1]);
}

void copy_ip_hdr(uchar *dest, uint prot, ip_hdr_t *ip, uint len)
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
    copy_ip(srom.ip_addr, ip->srce);	// My IP address

    uint sum = ipsum((uchar *) ip, IP_HDR_SIZE, 0);// Update checksum
    ip->checksum = htons(~sum);
}


void eth_transmit(uchar *buf, uint len, uint type, const uchar *dest)
{
    mac_hdr_t *mac_hdr = (mac_hdr_t *) buf;

    copy_mac(dest, mac_hdr->dest);
    copy_mac(srom.mac_addr, mac_hdr->srce);
    mac_hdr->type = htons(type);

#ifdef LOCK_ETH
    lock_get(LOCK_ETHER);
#endif

    while (er[ETH_STATUS] & 1) {
	continue;
    }

    sark_word_cpy(eth_tx_ram, buf, len);

    if (len < 60) {
	len = 60;
    }

    er[ETH_TX_LEN] = len;
    er[ETH_TX_CMD] = len;

#ifdef LOCK_ETH
    lock_free(LOCK_ETHER);
#endif
}


void eth_transmit2(uchar *hdr, uchar *buf, uint hdr_len, uint buf_len)
{
#ifdef LOCK_ETH
    lock_get(LOCK_ETHER);
#endif

    while (er[ETH_STATUS] & 1) {
	continue;
    }

    memcpy(eth_tx_ram, hdr, hdr_len);
    memcpy(eth_tx_ram + hdr_len, buf, buf_len);

    uint len = buf_len + hdr_len;
    if (len < 60) {
	len = 60;
    }

    er[ETH_TX_LEN] = len;
    er[ETH_TX_CMD] = len;

#ifdef LOCK_ETH
    lock_free(LOCK_ETHER);
#endif
}


void send_arp_pkt(uchar *buf, const uchar *dest, const uchar *tha,
	const uchar *tpa, uint type)
{
    arp_pkt_t *arp = (arp_pkt_t *) (buf + MAC_HDR_SIZE);

    arp->htype = htons(0x0001);		// Ethernet
    arp->ptype = htons(0x0800);		// IPv4
    arp->hlen = 6;
    arp->plen = 4;
    arp->op = htons(type);

    copy_mac(tha, arp->tha);		// Copy to THA
    copy_ip(tpa, arp->tpa);		// Copy to TPA

    copy_mac(srom.mac_addr, arp->sha);	// Copy my MAC addr
    copy_ip(srom.ip_addr, arp->spa);	// Copy my IP addr

    eth_transmit(buf, 42, ETYPE_ARP, dest);
}


void arp_lookup(iptag_t *iptag)
{
    uchar buf[42];

    uchar *ip_addr = iptag->ip;
    uint *my_ip = (uint *) srom.ip_addr;
    uint *ip = (uint *) ip_addr;
    uint *mask = (uint *) srom.net_mask;

    uchar *target_ip = ip_addr;

    if ((*my_ip & *mask) != (*ip & *mask)) {
	target_ip = srom.gw_addr;
    }

    copy_ip(target_ip, iptag->mac); // !! Bodge - target IP in MAC field!

    send_arp_pkt(buf, bc_mac, zero_mac, target_ip, ARP_REQ);
}


void arp_pkt(uchar *rx_pkt, uint rx_len, uint tag_table_size)
{
    uchar buf[42];
    arp_pkt_t *arp = (arp_pkt_t *) (buf + MAC_HDR_SIZE);

    sark_word_cpy(buf, rx_pkt, 42);
    eth_discard();

    if (! cmp_ip(arp->tpa, srom.ip_addr)) { // Ignore unless TPA matches
	return;
    }

    uint op = ntohs(arp->op);
    if (op == ARP_REQ) {
	send_arp_pkt(buf, buf+6, arp->sha, arp->spa, ARP_REPLY);
    } else if (op == ARP_REPLY) {	// Reply & TPA matches
	iptag_t *tt = tag_table;

	for (uint i = 0; i < tag_table_size; i++) {
	    uint f = tt->flags;
	    if ((f & IPFLAG_ARP) && cmp_ip(arp->spa, tt->mac)) { // !! Bodge
		copy_mac(arp->sha, tt->mac);
		f &= ~IPFLAG_ARP;
		tt->flags = f | IPFLAG_VALID;
		break;
	    }
	    tt++;
	}
    }
}


void icmp_pkt(uchar *rx_pkt, uint rx_len)
{
    if (rx_len > 138) {		// MAC 14, IP_HDR 60, ICMP 64
	eth_discard();
	return;
    }

    uchar buf[138];
    sark_word_cpy(buf, rx_pkt, rx_len);
    eth_discard();

    ip_hdr_t *ip_hdr = (ip_hdr_t *) (buf + IP_HDR_OFFSET);
    if (!cmp_ip(ip_hdr->dest, srom.ip_addr)) {
	return;
    }

    uint ip_hdr_len = (ip_hdr->ver_len & 15) * 4;
    icmp_hdr_t *icmp = (icmp_hdr_t *) (buf + IP_HDR_OFFSET + ip_hdr_len);

    if (icmp->type == ICMP_ECHO_REQ) {
	uint icmp_len = ntohs(ip_hdr->length) - ip_hdr_len; // Size of ICMP hdr+data

	copy_ip_hdr(ip_hdr->srce, PROT_ICMP, ip_hdr, icmp_len + IP_HDR_SIZE);

	if (ip_hdr_len > IP_HDR_SIZE) {		// Copy down ICMP header & data
	    icmp = (icmp_hdr_t *) (buf + IP_DATA_OFFSET);	// 'new' ICMP hdr
	    sark_mem_cpy((uchar *) icmp, (uchar *) icmp + ip_hdr_len, icmp_len);
	}

	icmp->type = ICMP_ECHO_REPLY;		// ICMP reply
	icmp->code = 0;				// ICMP code = 0
	icmp->checksum = 0;

	uint sum = ipsum((uchar *) icmp, icmp_len, 0);
	icmp->checksum = htons(~sum);

	eth_transmit(buf, IP_DATA_OFFSET + icmp_len, ETYPE_IP, buf+6);
    }
}


void copy_udp(uchar *buf, uint len, uint dest, uint srce)
{
    udp_hdr_t *udp_hdr = (udp_hdr_t *) (buf+UDP_HDR_OFFSET);

    udp_hdr->srce = htons(srce);		// Source port
    udp_hdr->dest = htons(dest);		// Dest port
    udp_hdr->length = htons(len);		// Insert length
    udp_hdr->checksum = 0;			// Zero checksum

    uint t;
    t = ipsum(buf+IP_HDR_OFFSET+12, 8, 0);	// Sum IP hdr addresses
    t += len;					// add in UDP data length
    t += PROT_UDP;				// and UDP protocol number
    t = ipsum((uchar *) udp_hdr, len, t);	// Final UDP checksum

    udp_hdr->checksum = htons(~t);
}
