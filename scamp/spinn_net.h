//------------------------------------------------------------------------------
//
// spinn_net.h	    Ethernet/IP support routines for Spinnaker
//
// Copyright (C)    The University of Manchester - 2009, 2010
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#ifndef SPINN_NET_H
#define SPINN_NET_H

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
    ushort htype;
    ushort ptype;
    uchar hlen;
    uchar plen;
    ushort op;
    uchar sha[6];
    uchar spa[4];
    uchar tha[6];
    uchar tpa[4];
} arp_pkt_t;


typedef struct {
    ushort srce;
    ushort dest;
    ushort length;
    ushort checksum;
} udp_hdr_t;


typedef struct {
    uchar type;
    uchar code;
    ushort checksum;
    ushort ident;
    ushort seq;
} icmp_hdr_t;


typedef struct {
    uchar dest[6];
    uchar srce[6];
    ushort type;
} mac_hdr_t;


typedef struct {
    uchar ver_len;
    uchar DS;
    ushort length;
    ushort ident;
    ushort flg_off;
    uchar TTL;
    uchar protocol;
    ushort checksum;
    uchar srce[4];
    uchar dest[4];
} ip_hdr_t;


extern const uchar bc_mac[];
extern const uchar zero_mac[];

void eth_init(uchar *mac);

uint ipsum(uchar *d, uint len, uint sum);

void copy_mac(const uchar *f, uchar *t);
void copy_ip(const uchar *f, uchar *t);
uint cmp_ip(uchar *a, uchar *b);
void copy_ip_hdr(uchar *dest, uint prot, ip_hdr_t *ip, uint len);

void eth_transmit(uchar *buf, uint len, uint type, const uchar *dest);
void eth_transmit2(uchar *hdr, uchar *buf, uint hdr_len, uint buf_len);

void send_arp_pkt(uchar *buf, const uchar *dest,
		  const uchar *tha, const uchar *tpa, uint type);
void arp_lookup(iptag_t *iptag);
void arp_pkt(uchar *rx_pkt, uint rx_len, uint tag_table_size);

void icmp_pkt(uchar *rx_pkt, uint rx_len);
void copy_udp(uchar *buf, uint len, uint dest, uint srce);

#endif
