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
 * Copyright (c) 2009 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

//! \brief Initialise the ethernet subsystem
//! \param[in] mac: Our MAC address
void eth_init(uchar *mac);

//! \brief Compute an IP checksum
//! \param[in] d: The data to checksum
//! \param[in] len: Number of bytes in data
//! \param[in] sum: Initial value; allows chaining of summing
//! \return the checksum
uint ipsum(uchar *d, uint len, uint sum);

//! \brief Copy MAC address
//! \param[in] f: Where to copy from. Must be at least half-word aligned
//! \param[out] t: Where to copy to. Must be at least half-word aligned
void copy_mac(const uchar *f, uchar *t);

//! \brief Copy IP address
//! \param[in] f: Where to copy from. Must be at least half-word aligned
//! \param[out] t: Where to copy to. Must be at least half-word aligned
void copy_ip(const uchar *f, uchar *t);

//! \brief Compare IP addresses
//! \param[in] a: First address to compare
//! \param[in] b: Second address to compare
//! \return True if the addresses are equal, false otherwise
uint cmp_ip(const uchar *a, const uchar *b);

//! \brief Initialise IP header
//! \param[in] dest: Destination address
//! \param[in] prot: Protocol to use
//! \param[in,out] ip: Packet with header to initialise, copying \p dest into.
//!     The payload must have already been placed in the packet.
//! \param[in] len: The length of the payload in the packet.
void copy_ip_hdr(uchar *dest, uint prot, ip_hdr_t *ip, uint len);

//! \brief Transmit ethernet packet
//! \param[in,out] buf: The buffer containing the packet to send.
//!     Will have the parts that are the ethernet header modified.
//! \param[in] len: Length of buffer (headers and payload)
//! \param[in] type: The type of ethernet packet to send
//! \param[in] dest: The destination MAC address
void eth_transmit(uchar *buf, uint len, uint type, const uchar *dest);

//! \brief Transmit ethernet packet
//! \param[in] hdr: The ethernet header, _already prepared._
//! \param[in] buf: The buffer containing the payload to send.
//! \param[in] hdr_len: Length of header, \p hdr
//! \param[in] buf_len: Length of payload, \p buf
void eth_transmit2(uchar *hdr, uchar *buf, uint hdr_len, uint buf_len);

//! \brief Send an ARP packet
//! \param[in,out] buf: Buffer containing ARP packet.
//!     Payload must have been set.
//! \param[in] dest: Destination MAC address
//! \param[in] tha: Target hadrware address
//! \param[in] tpa: Target protocol address
//! \param[in] type: Opcode (e.g., #ARP_REQ)
void send_arp_pkt(uchar *buf, const uchar *dest,
                  const uchar *tha, const uchar *tpa, uint type);

//! \brief Initiate ARP lookup if required. Lookup might be immediate (out of
//!     cache) or require network traffic.
//! \param[in,out] iptag: The IPTag that needs the lookup done. Will be
//!     updated with results when available.
void arp_lookup(iptag_t *iptag);

//! \brief ARP packet received handler
//! \param[in] rx_pkt: The received packet
//! \param[in] rx_len: The length of the received packet
//! \param[in] tag_table_size: How large is our tag table?
void arp_pkt(uchar *rx_pkt, uint rx_len, uint tag_table_size);

//! \brief Add a MAC/IP address binding to our ARP cache
//! \param[in] mac: The MAC address
//! \param[in] ip: The IP address
void arp_add(uchar *mac, uchar *ip);

//! \brief ICMP packet received handler
//! \param[in] rx_pkt: The received packet
//! \param[in] rx_len: The length of the received packet
void icmp_pkt(uchar *rx_pkt, uint rx_len);

//! \brief Initialise UDP packet header
//! \param[in,out] buf: The UDP packet, header and payload. Payload must have
//!     been initialised.
//! \param[in] len: Length of payload.
//! \param[in] dest: Destination port.
//! \param[in] srce: Source port.
void copy_udp(uchar *buf, uint len, uint dest, uint srce);

#endif
