//------------------------------------------------------------------------------
//! \file
//! \brief     Ethernet/IP support routines for Spinnaker
//!
//! \copyright &copy; The University of Manchester - 2009-2019
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

#ifndef SPINN_NET_H
#define SPINN_NET_H

//! Size of mac_hdr_t
#define MAC_HDR_SIZE            14
//! Size of ip_hdr_t
#define IP_HDR_SIZE             20
//! Size of udp_hdr_t
#define UDP_HDR_SIZE            8
//! Padding added to start of SDP for alignment efficiency
#define SDP_PAD_SIZE            2

//! Offset from start of packet to start of IP header
#define IP_HDR_OFFSET           MAC_HDR_SIZE
//! Offset from start of packet to start of IP data
#define IP_DATA_OFFSET          (IP_HDR_OFFSET + IP_HDR_SIZE)

//! Offset from start of packet to start of UDP header
#define UDP_HDR_OFFSET          IP_DATA_OFFSET
//! Offset from start of packet to start of UDP data
#define UDP_DATA_OFFSET         (UDP_HDR_OFFSET + UDP_HDR_SIZE)

#define ETYPE_IP                0x0800 //!< Ethernet message type: IP
#define ETYPE_ARP               0x0806 //!< Ethernet message type: ARP

#define PROT_ICMP               1  //!< Protocol: ICMP
#define PROT_UDP                17 //!< Protocol: UDP

#define ARP_REQ                 1  //!< ARP message operation: request
#define ARP_REPLY               2  //!< ARP message operation: reply

#define ICMP_ECHO_REPLY         0  //!< ICMP echo (ping) reply
#define ICMP_ECHO_REQ           8  //!< ICMP echo (ping) request

//! ARP packet format
typedef struct {
    ushort htype;    //!< Hardware type
    ushort ptype;    //!< Protocol type (e.g., #ETYPE_IP)
    uchar hlen;      //!< Hardware address length
    uchar plen;      //!< Protocol address length
    ushort op;       //!< Operation (e.g., #ARP_REQ)
    uchar sha[6];    //!< Sender hardware address
    uchar spa[4];    //!< Sender protocol address
    uchar tha[6];    //!< Target hardware address
    uchar tpa[4];    //!< Target protocol address
} arp_pkt_t;

//! UDP header
typedef struct {
    ushort srce;     //!< Source port
    ushort dest;     //!< Destination port
    ushort length;   //!< Length
    ushort checksum; //!< Checksum
} udp_hdr_t;

//! ICMP header
typedef struct {
    uchar type;      //!< Message type (e.g., #ICMP_ECHO_REQ)
    uchar code;      //!< Error code (zero)
    ushort checksum; //!< Checksum
    ushort ident;    //!< Conversation identifier
    ushort seq;      //!< Sequence number
} icmp_hdr_t;

//! MAC header for ethernet packet
typedef struct mac_hdr_t {
    uchar dest[6]; //!< Destination MAC address
    uchar srce[6]; //!< Source MAC address
    ushort type;   //!< Message type, "EtherType" (e.g., #ETYPE_IP)
} mac_hdr_t;

//! IP packet header
typedef struct {
    uchar ver_len;   //!< Version and header length
    uchar DS;        //!< DSCP/ECN
    ushort length;   //!< Message length
    ushort ident;    //!< Identification (for fragment reassembly)
    ushort flg_off;  //!< Flags/fragment offset; SCAMP doesn't support fragments
    uchar TTL;       //!< Time-to-live
    uchar protocol;  //!< Protocol (e.g., #PROT_UDP)
    ushort checksum; //!< Checksum
    uchar srce[4];   //!< Source IP address
    uchar dest[4];   //!< Destination IP address
} ip_hdr_t;

//! Broadcast MAC address
extern const uchar bc_mac[];
//! Zero MAC address
extern const uchar zero_mac[];

//! Initialise the ethernet subsystem
void eth_init(uchar *mac);

//! Compute an IP checksum
uint ipsum(uchar *d, uint len, uint sum);

//! Copy MAC address
void copy_mac(const uchar *f, uchar *t);
//! Copy IP address
void copy_ip(const uchar *f, uchar *t);
//! Compare IP addresses
uint cmp_ip(uchar *a, uchar *b);
//! Copy IP header
void copy_ip_hdr(uchar *dest, uint prot, ip_hdr_t *ip, uint len);

//! Transmit ethernet packet
void eth_transmit(uchar *buf, uint len, uint type, const uchar *dest);
//! Transmit ethernet packet
void eth_transmit2(uchar *hdr, uchar *buf, uint hdr_len, uint buf_len);

//! Send an ARP packet
void send_arp_pkt(uchar *buf, const uchar *dest,
                  const uchar *tha, const uchar *tpa, uint type);
//! ARP lookup
void arp_lookup(iptag_t *iptag);
//! ARP packet
void arp_pkt(uchar *rx_pkt, uint rx_len, uint tag_table_size);
//! Add a MAC/IP address binding to our ARP cache
void arp_add(uchar *mac, uchar *ip);

//! ICMP packet
void icmp_pkt(uchar *rx_pkt, uint rx_len);
//! Copy UDP header
void copy_udp(uchar *buf, uint len, uint dest, uint srce);

#endif
