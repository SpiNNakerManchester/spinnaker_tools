//------------------------------------------------------------------------------
//
//! \file bmp_net.c
//! \brief          Networking code for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012 The University of Manchester
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

#include <string.h>

#include "lpc17xx.h"
#include "bmp.h"

//------------------------------------------------------------------------------

// Internet constants and structs

//! Maximum size of ethernet message. Larger messages are ignored.
#define ETHERNET_SIZE_MAX       384

//! Byteswap: network to host short
#define ntohs(t)        ((((t) & 0x00ff) << 8) | (((t) & 0xff00) >> 8))
//! Byteswap: host to network short
#define htons(t)        ((((t) & 0x00ff) << 8) | (((t) & 0xff00) >> 8))

//! Size of Ethernet header (::mac_hdr_t)
#define MAC_HDR_SIZE            14
//! Size of IP header (::ip_hdr_t)
#define IP_HDR_SIZE             20
//! Size of UDP header (::udp_hdr_t)
#define UDP_HDR_SIZE            8
//! Size of SDP header (::sdp_msg_t)
#define SDP_PAD_SIZE            2

//! Offset to IP header in Ethernet packet
#define IP_HDR_OFFSET           MAC_HDR_SIZE
//! Offset to IP body in Ethernet packet
#define IP_DATA_OFFSET          (IP_HDR_OFFSET + IP_HDR_SIZE)

//! Offset to UDP header in Ethernet packet
#define UDP_HDR_OFFSET          IP_DATA_OFFSET
//! Offset to UDP body in Ethernet packet
#define UDP_DATA_OFFSET         (UDP_HDR_OFFSET + UDP_HDR_SIZE)

//! Types of Ethernet packets we understand
enum ethernet_packet_types {
    ETYPE_IP = 0x0800,          //!< Message is an IP message
    ETYPE_ARP = 0x0806,         //!< Message is an ARP message
};

//! Types of IP packets we understand
enum internet_protocol_packet_types {
    PROT_ICMP = 1,              //!< Message is an ICMP message
    PROT_UDP = 17,              //!< Message is a UDP message
};

//! ARP operations we know about
enum arp_opcodes {
    ARP_REQ = 1,                //!< Message is an ARP request
    ARP_REPLY = 2,              //!< Message is an ARP reply
};

//! ICMP operations we know about
enum icmp_commands {
    ICMP_ECHO_REPLY = 0,        //!< Message is an ECHO reply
    ICMP_ECHO_REQ = 8,          //!< Message is an ECHO request
};

//! ARP packet definition
typedef struct {
    uint16_t htype;     //!< Hardware address type code
    uint16_t ptype;     //!< Protocol address type code
    uint8_t hlen;       //!< Hardware address length
    uint8_t plen;       //!< Protocol address length
    uint16_t op;        //!< Operation code (see ::arp_opcodes)
    uint8_t sha[6];     //!< Source hardware address
    uint8_t spa[4];     //!< Source protocol address
    uint8_t tha[6];     //!< Target hardware address
    uint8_t tpa[4];     //!< Target protocol address
} arp_pkt_t;

//! UDP header
typedef struct {
    uint16_t srce;      //!< Source port
    uint16_t dest;      //!< Destination port
    uint16_t length;    //!< Length of UDP header and payload
    uint16_t checksum;  //!< Checksum of header and data
} udp_hdr_t;

//! ICMP header; only ECHO request and reply are supported
typedef struct {
    uint8_t type;       //!< ICMP message type (::icmp_commands)
    uint8_t code;       //!< ICMP message subtype
    uint16_t checksum;  //!< Checksum of ICMP message
    uint16_t ident;     //!< Identifier (for ECHO)
    uint16_t seq;       //!< Sequence number (for ECHO)
} icmp_hdr_t;

//! \brief Ethernet packet header; will be followed by ip_hdr_t or arp_pkt_t
//! \note This is *14* bytes long; following data is not aligned!
typedef struct {
    uint8_t dest[6];    //!< Destination MAC address
    uint8_t srce[6];    //!< Source MAC address
    uint16_t type;      //!< Packet type (::ethernet_packet_types)
} mac_hdr_t;

//! IP header; will be followed by udp_hdr_t or icmp_hdr_t
typedef struct {
    uint8_t ver_len;    //!< Version and header length
    uint8_t DS;         //!< Differentiated service flags
    uint16_t length;    //!< Packet length
    uint16_t ident;     //!< Packet identifier
    uint16_t flg_off;   //!< Flags and fragment offset
    uint8_t TTL;        //!< Time-to-live of packet
    uint8_t protocol;   //!< Sub-protocol code (::internet_protocol_packet_types)
    uint16_t checksum;  //!< Header checksum
    uint8_t srce[4];    //!< Source IP address
    uint8_t dest[4];    //!< Destination IP address
} ip_hdr_t;

//------------------------------------------------------------------------------

//! The number of messages we have allocated
#define NUM_MSGS                8
//! The maximum message number, for allocation purposes
#define MAX_MSG                 (NUM_MSGS - 1)
//! Size of the message queue
#define MSG_QUEUE_SIZE          NUM_MSGS

//! The type of the message queue. This is a circular buffer
typedef struct msg_queue_t {
    uint8_t insert;             //!< Where the next message is to be inserted
    uint8_t remove;             //!< Where the next message is to be removed
    volatile uint8_t count;     //!< Number of messages in the queue
    uint8_t max;                //!< Maximum number of messages in the queue
    //! The storage for the message queue
    sdp_msg_t *queue[MSG_QUEUE_SIZE];
} msg_queue_t;

//------------------------------------------------------------------------------

//! Buffer used to mediate with ethernet hardware
static uint8_t eth_buf[ETHERNET_SIZE_MAX];

//! The table of IPTags
iptag_t tag_table[TAG_TABLE_SIZE];

//! The allocatable message buffers
static sdp_msg_t msg_bufs[NUM_MSGS];
//! Pointer to the next message to allocate
static sdp_msg_t *msg_root;
//! The number of allocated messages
static uint32_t msg_count;
//! The maximum value of ::msg_count
static uint32_t msg_max;

//! The message queue
static msg_queue_t msg_queue;

//! Broadcast MAC address
static const uint8_t bc_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
//! Network MAC address
static const uint8_t zero_mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//! IP address of the board we manage
ip_data_t spin_ip;
//! Our own IP address
ip_data_t bmp_ip;

uint32_t tag_tto = 9;   //!< Tag timeout: 2.56s = 10ms * (1 << (9-1))

//------------------------------------------------------------------------------

//! Initialise the message queue
void msg_init(void)
{
    sdp_msg_t *msg = msg_root = msg_bufs;

    for (uint32_t i = 0; i < NUM_MSGS; i++) {
        msg->next = (i != MAX_MSG) ? msg + 1 : NULL;
        msg++;
    }

    msg_queue.insert = 1;
}

//! \brief "Allocate" a message from the free message pool
//! \return The message, or `NULL` if the pool is empty
sdp_msg_t *msg_get(void)
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

//! \brief "Free" a message, returning it to the free message pool
//! \param[in] msg: The message to free. Must not be used after this call.
static void msg_free(sdp_msg_t *msg)
{
    uint32_t cpsr = cpu_int_off();

    msg->next = msg_root;
    msg_root = msg;

    msg_count--;

    cpu_int_restore(cpsr);
}

//! \brief Insert a message in the message queue
//! \param[in] msg: The message to enqueue
//! \return True if the message was enqueued, false if the queue was full
uint32_t msg_queue_insert(sdp_msg_t *msg)
{
    if (msg_queue.count >= MSG_QUEUE_SIZE) {    // !! Fatal error? - free msg?
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

//! \brief Get a message from the head of the message queue and remove it
//!     from the queue.
//! \return the message, or `NULL` if the queue is empty
sdp_msg_t *msg_queue_remove(void)
{
    uint32_t cpsr = cpu_int_off();

    msg_queue.remove = (msg_queue.remove + 1) % MSG_QUEUE_SIZE;
    msg_queue.count--;

    cpu_int_restore(cpsr);

    return msg_queue.queue[msg_queue.remove];
}

//! \brief How big is the message queue?
//! \return The number of messages in the queue
uint32_t msg_queue_size(void)
{
    return msg_queue.count;
}

//------------------------------------------------------------------------------

//! \brief Compute a checksum using IP rules
//! \param[in] d: The buffer to compute the checksum of
//! \param[in] len: The length of buffer in \p d
//! \param[in] sum: Existing checksum value to accumulate to;
//!     use 0 for first step
//! \return the checksum
static uint32_t ipsum( // Use shorts for speed??
	const uint8_t *d, uint32_t len, uint32_t sum)
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

//! \brief Copy MAC address
//! \param[in] f: Where to copy from
//! \param[out] t: Where to copy to
static void copy_mac(const uint8_t *restrict f, uint8_t *restrict t)
{
    uint16_t *ts = (uint16_t*) t;
    uint16_t *fs = (uint16_t*) f;

    ts[0] = fs[0];
    ts[1] = fs[1];
    ts[2] = fs[2];
}

//! \brief Copy IP address
//! \param[in] f: Where to copy from
//! \param[out] t: Where to copy to
void copy_ip(const uint8_t *restrict f, uint8_t *restrict t)
{
    uint16_t *ts = (uint16_t*) t;
    uint16_t *fs = (uint16_t*) f;

    ts[0] = fs[0];
    ts[1] = fs[1];
}

//! \brief Compare two IP addresses
//! \param[in] a: IP address 1
//! \param[in] b: IP address 2
//! \return true if they are equal, false if they differ
uint32_t cmp_ip(const uint8_t *a, const uint8_t *b)
{
    uint16_t *as = (uint16_t*) a;
    uint16_t *bs = (uint16_t*) b;

    return (as[0] == bs[0]) && (as[1] == bs[1]);
}

//------------------------------------------------------------------------------

//! \brief IPTag timeout tick. Called from proc_100hz() every 10ms.
void iptag_timer(void)
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

//! \brief Allocate a transient IPTag
//! \return The IPTag ID, or #TAG_NONE if allocation failed
uint32_t iptag_new(void)
{
    for (uint32_t i = FIRST_POOL_TAG; i <= LAST_POOL_TAG; i++) {
        if (tag_table[i].flags == 0) {
          return i;
        }
    }

    return TAG_NONE;
}

//! \brief Allocate and initialise a transient IPTag
//! \param ip: Source IP address
//! \param mac: Sender MAC address (probably gateway)
//! \param port: Sender UDP port
//! \param timeout: Timeout associated with IPTag
//!     (number of 10ms ticks the tag is to live for)
//! \return The IPTag ID, or #TAG_NONE if allocation failed
uint32_t transient_tag(
	uint8_t *ip, uint8_t *mac, uint32_t port, uint32_t timeout)
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

//! \brief Initialise IP header information
//! \param[in] dest: Destination IP address
//! \param[in] prot: IP sub-protocol
//! \param[out] ip: Buffer holding IP header
//! \param[in] len: Length of whole message
void copy_ip_hdr(uint8_t *dest, uint32_t prot, ip_hdr_t *ip, uint32_t len)
{
    ip->ver_len = 0x45;
    ip->DS = 0;
    ip->length = htons(len);
    ip->ident = htons(0);
    ip->flg_off = htons(2 << 13);       // Flags = 010, offset = 0
    ip->TTL = 64;
    ip->protocol = prot;                // Insert protocol
    ip->checksum = htons(0);
    copy_ip(dest, ip->dest);            // Source IP address
    copy_ip(bmp_ip.ip_addr, ip->srce);  // My IP address

    uint32_t sum = ipsum((uint8_t *) ip, IP_HDR_SIZE, 0);// Update checksum
    ip->checksum = htons(~sum);
}

//! \brief Initialise UDP header information
//! \param[in,out] buf: Buffer holding whole Ethernet message
//! \param[in] len: Length of message _payload_
//! \param[in] dest: Destination UDP port
//! \param[in] srce: Source UDP port
void copy_udp(uint8_t *buf, uint32_t len, uint32_t dest, uint32_t srce)
{
    udp_hdr_t *udp_hdr = (udp_hdr_t *) (buf+UDP_HDR_OFFSET);

    udp_hdr->srce = htons(srce);        // Source port
    udp_hdr->dest = htons(dest);        // Dest port
    udp_hdr->length = htons(len);       // Insert length
    udp_hdr->checksum = 0;              // Zero checksum

    uint32_t t;
    t = ipsum(buf+IP_HDR_OFFSET+12, 8, 0); // Sum IP hdr addresses
    t += len;                           // add in UDP data length
    t += PROT_UDP;                      // and UDP protocol number
    t = ipsum((uint8_t *) udp_hdr, len, t); // Final UDP checksum

    udp_hdr->checksum = htons(~t);
}

//------------------------------------------------------------------------------

//! \brief Send an _ethernet_ packet.
//! \details Writes to the packet header; body must be supplied.
//! \param[in] buf: The buffer holding the ethernet packet
//! \param[in] len: Length of whole message in \p buf
//! \param[in] type: The message type
//! \param[in] dest: The destination MAC address
static void eth_transmit(uint8_t *buf, uint32_t len, uint32_t type,
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

//! \brief Send an ARP packet.
//! \details Writes to the packet header; body must be supplied.
//! \param[in] buf: The buffer holding the ARP packet, including space for
//!     all headers
//! \param[in] dest: The destination MAC address
//! \param[in] tha: Target hardware address (MAC)
//! \param[in] tpa: Target protocol address (IP)
//! \param[in] type: ARP operation
static void send_arp_pkt(uint8_t *buf, const uint8_t *dest,
	const uint8_t *tha, const uint8_t *tpa, uint32_t type)
{
    arp_pkt_t *arp = (arp_pkt_t *) (buf + MAC_HDR_SIZE);

    arp->htype = htons(0x0001);         // Ethernet
    arp->ptype = htons(0x0800);         // IPv4
    arp->hlen = 6;
    arp->plen = 4;
    arp->op = htons(type);

    copy_mac(tha, arp->tha);            // Copy to THA
    copy_ip(tpa, arp->tpa);             // Copy to TPA

    copy_mac(bmp_ip.mac_addr, arp->sha);// Copy my MAC addr
    copy_ip(bmp_ip.ip_addr, arp->spa);  // Copy my IP addr

    eth_transmit(buf, 42, ETYPE_ARP, dest);
}

//! \brief Handle a received ARP packet.
//! \param[in] buf: The received message, including ethernet headers
//! \param[in] rx_len: The received length
static void arp_pkt(uint8_t *buf, uint32_t rx_len)
{
    arp_pkt_t *arp = (arp_pkt_t *) (buf + MAC_HDR_SIZE);

    if (!cmp_ip(arp->tpa, bmp_ip.ip_addr)) { // Ignore unless TPA matches
        return;
    }

    uint32_t op = ntohs(arp->op);

    if (op == ARP_REQ) {
        send_arp_pkt(buf, buf+6, arp->sha, arp->spa, ARP_REPLY);
    } else if (op == ARP_REPLY) {       // Reply & TPA matches
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

//! \brief Handle a received ICMP packet.
//! \param[in] buf: The received message, including ethernet headers
//! \param[in] rx_len: The received length
static void icmp_pkt(uint8_t *buf, uint32_t rx_len)
{
    ip_hdr_t *ip_hdr = (ip_hdr_t *) (buf + IP_HDR_OFFSET);

    if (!cmp_ip (ip_hdr->dest, bmp_ip.ip_addr)) {
        return;
    }

    uint32_t ip_hdr_len = (ip_hdr->ver_len & 15) * 4;

    icmp_hdr_t *icmp = (icmp_hdr_t *) (buf + IP_HDR_OFFSET + ip_hdr_len);

    if (icmp->type == ICMP_ECHO_REQ) {
        uint32_t icmp_len = ntohs(ip_hdr->length) - ip_hdr_len; // Size of ICMP hdr+data

        copy_ip_hdr(ip_hdr->srce, PROT_ICMP, ip_hdr, icmp_len + IP_HDR_SIZE);

        if (ip_hdr_len > IP_HDR_SIZE) {         // Copy down ICMP header & data
            icmp = (icmp_hdr_t *) (buf + IP_DATA_OFFSET); // 'new' ICMP hdr
            memcpy((uint8_t *) icmp, (uint8_t *) icmp + ip_hdr_len, icmp_len);
        }

        icmp->type = ICMP_ECHO_REPLY;           // ICMP reply
        icmp->code = 0;                         // ICMP code = 0
        icmp->checksum = 0;

        uint32_t sum = ipsum((uint8_t *) icmp, icmp_len, 0);
        icmp->checksum = htons(~sum);

        eth_transmit(buf, IP_DATA_OFFSET + icmp_len, ETYPE_IP, buf+6);
    }
}

//! \brief Handle a received UDP packet.
//! \param[in] rx_pkt: The received message, including ethernet headers
//! \param[in] rx_len: The received length
static void udp_pkt(uint8_t *rx_pkt, uint32_t rx_len)
{
    ip_hdr_t *ip_hdr = (ip_hdr_t *) (rx_pkt + IP_HDR_OFFSET);
    uint32_t ip_len = (ip_hdr->ver_len & 15) * 4;

    udp_hdr_t *udp_hdr = (udp_hdr_t *) (rx_pkt + IP_HDR_OFFSET + ip_len);
    uint32_t udp_dest = ntohs(udp_hdr->dest);

    if ((udp_dest == bmp_ip.udp_port) && cmp_ip(ip_hdr->dest, bmp_ip.ip_addr)
            && rx_len > 52) {                   //const NB 52
        uint32_t udp_srce = ntohs(udp_hdr->srce);
        int32_t len = ntohs(udp_hdr->length) - 10; //const UDP_HDR + UDP_PAD

        if (len > (24 + 256)) {                 // SDP=8, CMD=16
            return;
        }

        sdp_msg_t *msg = msg_get();
        if (msg == NULL) {                      // !! fix this - reply somehow?
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

//! \brief Receive a packet off the ethernet hardware.
//! \details Delegates to:
//! * udp_pkt()
//! * icmp_pkt()
//! * arp_pkt()
void eth_receive(void)
{
    uint8_t *rx_pkt = eth_buf;
    int32_t len = eth_rx_size() - 3;    // Removes CRC ??

    if (len > ETHERNET_SIZE_MAX) {
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

//! \brief Send an _ethernet_ message.
//! \param[in] hdr: The ethernet + UDP headers
//! \param[in] buf: The buffer holding the message payload
//! \param[in] len: Length of payload in \p buf
//! \param[in] dest: The destination MAC address
static void eth_transmit2(
	uint8_t *hdr, uint8_t *buf, uint32_t len, uint8_t *dest)
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

//! \brief Send an _SDP_ message over the ethernet port.
//! \param[in] tag: The index of the IPTag that describes the destination
//! \param[in] msg: The SDP message to send
static void eth_send_msg(uint32_t tag, sdp_msg_t *msg)
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
    udp_hdr->checksum = 0;                      // Zero checksum

    uint32_t t;
    t = ipsum(hdr+IP_HDR_OFFSET+12, 8, 0);      // Sum IP hdr addresses
    t += len + UDP_HDR_SIZE + SDP_PAD_SIZE;     // add in UDP data length
    t += PROT_UDP;                              // and UDP protocol number
    t = ipsum((uint8_t *) udp_hdr, 10, t);      // and UDP header and pad
    t = ipsum(&msg->flags, len, t);             // and finally the data

    udp_hdr->checksum = htons(~t);

    eth_transmit2(hdr, &msg->flags, len, iptag->mac);

    if (iptag->flags & IPTAG_TRANS) {           //transient tag
        iptag->flags = 0;
    } else {
        iptag->count++;
    }
}

//! \brief Swap source and destination in an SDP message header
//! \details This reflects the message so it can become its own reply
//! \param[in] msg: The message to be swapped
static void swap_sdp_hdr(sdp_msg_t *msg)
{
    uint32_t dest_port = msg->dest_port;
    uint32_t dest_addr = msg->dest_addr;

    msg->dest_port = msg->srce_port;
    msg->srce_port = dest_port;

    msg->dest_addr = msg->srce_addr;
    msg->srce_addr = dest_addr;
}

//! \brief Returns a message to sender, if sender asked for a response
//! \param[in] msg: The message to return.
//!     _Ownership of the message is taken!_
//!     Caller must not use the message after this.
//! \param[in] rc: Response code. Non-zero indicates an error (when only
//!     header information is used). Zero is an OK response, in which case
//!     the payload and length of the message should be set correctly.
static void return_msg(sdp_msg_t *msg, uint32_t rc)
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

//! \brief Route a message to its destination.
//! \param[in] msg: The message to route.
//!     _Ownership of the message is taken!_
//!     Caller must not use the message after this.
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

//! \brief Request an ARP lookup
//! \param[in] iptag: The IPTag to do the lookup for
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

//! Accumulates chars to write to host
static uint8_t tube_buf[ETHERNET_SIZE_MAX];
static uint32_t tube_ptr;       //!< Point in ::tube_buf to write

//! \brief Add character to ethernet-directed output stream
//! \details If the buffer is full or at end of line, push the buffer to the
//!     configured host machine.
//! \param[in] c: The character to write
//! \internal
//! DKF: How is this different from std_putc()?
void eth_putc(uint32_t c)
{
    uint8_t *buf = (uint8_t *) tube_buf + 56;   // Point at start of msg buffer

    buf[tube_ptr++] = c;                        // Insert char at end

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

//! Copy IP address data from Flash (::bmp_flash_ip and ::spin_flash_ip)
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
