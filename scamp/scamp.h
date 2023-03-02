//------------------------------------------------------------------------------
//! \file
//! \brief     Header file for SC&MP
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

#ifndef SCAMP_H
#define SCAMP_H

//------------------------------------------------------------------------------

//! \name Misc Constants
//! \{
#define MAX_CPUS                20                      //!< Legacy const!

#define MONITOR_CPU             0                       //!< Virtual CPU number
//! \}
//------------------------------------------------------------------------------

//! \name SCAMP Multicast constants
//! \{

//! Key used in the routing table
#define SCAMP_MC_ROUTING_KEY    0xffff5554

//! Mask used in the routing table
#define SCAMP_MC_ROUTING_MASK   0xfffffffe

// The meaning of the individual keys - an enum might be better but doesn't
// work because the 'enumeration value is out of "signed int" range'

//! The key used for time synchronisation packets
#define SCAMP_MC_TIME_SYNC_KEY 0xffff5554

//! The key used for signal packets
#define SCAMP_MC_SIGNAL_KEY 0xffff5555
//! \}
//------------------------------------------------------------------------------

//! \name SCAMP boot image constants
//! \{

// These settings handle a boot image of size BLOCK_COUNT * BYTE_COUNT bytes
// Image sizes < 32kB are possible.
// BLOCK_COUNT * BYTE_COUNT must be < 32kB

//! The number of blocks in the image to boot
#define BLOCK_COUNT     31      // From 1-256
//! The number of words in a block (1kB)
#define WORD_COUNT      256     // From 1-256
//! The number of bytes in a block
#define BYTE_COUNT      (WORD_COUNT * sizeof(uint))
//! The maximum boot image size
#define BOOT_IMAGE_SIZE (BLOCK_COUNT * BYTE_COUNT)
//! \}
//------------------------------------------------------------------------------

//! \name IPTags
//! \{
// IPTag table sizes

//! Number of IPTags available for application use
#define TAG_FIXED_SIZE          8       // At bottom of table
//! Number of (temporary) IPTags available for managing SDP replies
#define TAG_POOL_SIZE           8

//! Index of first temporary IPTag
#define FIRST_POOL_TAG          TAG_FIXED_SIZE
//! Index of last temporary IPTag
#define LAST_POOL_TAG           (TAG_FIXED_SIZE + TAG_POOL_SIZE - 1)

//! Total number of IPTags available
#define TAG_TABLE_SIZE          (TAG_FIXED_SIZE + TAG_POOL_SIZE)

// IPTAG definitions
//! Commands applicable to IPTags
enum scamp_iptag_commands {
    IPTAG_NEW = 0,              //!< Create IPTag
    IPTAG_SET = 1,              //!< Configure IPTag
    IPTAG_GET = 2,              //!< Retrieve IPTag
    IPTAG_CLR = 3,              //!< Clear IPTag
    IPTAG_TTO = 4,              //!< Configure IPTag Timeout

    IPTAG_MAX = 4               //!< Maximum command value
};

//! IPTag flag bits
enum scamp_iptag_flags {
    IPFLAG_VALID =      0x8000, //!< Entry is valid
    IPFLAG_TRANS =      0x4000, //!< Entry is transient
    IPFLAG_ARP =        0x2000, //!< Awaiting ARP resolution

    IPFLAG_USE_SENDER = 0x0400, //!< Use sender address and port
    IPFLAG_REV =        0x0200, //!< Reverse IPTag
    IPFLAG_STRIP =      0x0100  //!< Strip SDP headers
};
//! \}

//------------------------------------------------------------------------------

//! \name SDP Flags
//! \{

//! Bits in SDP Flags byte (3 used)
enum scamp_sdp_flags {
    SDPF_REPLY =        0x80,   //!< Reply expected
    SDPF_SUM =          0x40,   //!< Checksum before routing
    SDPF_NR =           0x20    //!< Don't route via P2P
};
//! \}

//------------------------------------------------------------------------------

//! \name Failure Codes
//! \{

//! \brief Failure codes
//!
//! Non-zero causes CPU to sleep.
//! Bit 6 set causes CPU_OK bit to be cleared.
//! Bit 5 set causes LED3 to be turned on.
//!
//! HW errors have bit 7 set & bit 6 set.
//! SW errors have bit 7 set & bit 6 clr.
enum scamp_failure_codes {
    FAIL_RESET =        0xc0,   //!< Catch-all - set at reset
    FAIL_ROMX =         0xc1,   //!< Exception in ROM code
    FAIL_ITCM0 =        0xc2,   //!< ITCM top 512 failure
    FAIL_ITCM1 =        0xc3,   //!< ITCM main test failure
    FAIL_DTCM =         0xc4,   //!< DTCM test failure

    FAIL_TIMER =        0xc5,   //!< Timer reg test failed
    FAIL_VIC =          0xc6,   //!< VIC reg test failed
    FAIL_CC =           0xc7,   //!< Comms ctlr reg test failed
    FAIL_DMA =          0xc8,   //!< DMAC reg test failed

    FAIL_MP =           0xc9,   //!< Previous monitor proc failure
    FAIL_LATE =         0xca,   //!< App CPU failed to set CPU_OK
    FAIL_MANUF =        0xcb,   //!< App CPU in manuf test
    FAIL_SLEEP =        0xcc,   //!< Ordered to sleep in startup

    FAIL_TLM =          0xcf,   //!< Special for TLM

    FAIL_VEC =          0xa0,   //!< Unhandled exception
};
//! \}

//------------------------------------------------------------------------------

//! \name Nearest-neighbour Discovery Protocol
//! \{

//! \brief NN opcodes
//!
//! * Codes < 7 have propagation limited by the ID field in the packet.
//! * Codes >= 7 have various other ways of handling propagation.
//! * Codes 0-3, 8-11 have explicit FwdRty in the packet.
//! * Codes 4-7, 12-15 use stored FwdRty parameters
//!
//! Contrary to the above:
//!
//!   NN_CMD_P2PC are limited to only forward packets containing new information,
//!   NN_CMD_BIFF are limited by ID in a separate ID-space to other commands and
//!   their propagation is limited to just the current board.
//!
//! Format for NN_CMD_P2PC commands:
//! ```
//!   key[31:28] 4-bit one's complement checksum of key and data (see chksum_64())
//!   key[27:24] NN command (== NN_CMD_P2PC)
//!   key[23:4] Unused
//!   key[3:2] Type: P2PC_ADDR, P2PC_NEW or P2PC_DIMS
//!   key[1] Must be 0, differentiator between peek/poke responses and other
//!          NN packets
//! ```
//!
//! Format for NN_CMD_BIFF commands
//! ```
//!   key[31:28] 4-bit one's complement checksum of key and data (see chksum_64())
//!   key[27:24] NN command (== NN_CMD_BIFF)
//!   key[23:16] Unused
//!   key[13:11] X coordinate of packet within board (updated when forwarded)
//!   key[10:8] Y coordinate of packet within board (updated when forwarded)
//!   key[7:1] 7-bit packet ID, used for ignoring echos (in a different address
//!            space to other NN packets).
//!   key[1] Must be 0, differentiator between peek/poke responses and other
//!          NN packets
//! ```
//!
//! Nearest neighbour packet format all other commands
//! ```
//!   key[31:28] 4-bit one's complement checksum of key and data (see chksum_64())
//!   key[27:24] NN command
//!   key[23:16] Forward (for packets with command bit 2 set)
//!   key[15:8] Retry (for packets with command bit 2 set)
//!   key[7:1] 7-bit packet ID, used for ignoring echos (for packets with
//!            commands < 7)
//!   key[1] Must be 0, differentiator between peek/poke responses and other
//!          NN packets
//! ```
enum scamp_nn_opcodes {
    NN_CMD_SIG0 =  0,  //!< Misc (GTPC, Set FwdRty, LED, etc)
    NN_CMD_RTRC =  1,  //!< Router Control Reg
    NN_CMD_LTPC =  2,  //!< Local Time Phase Control (ID=0, Fwd=0)
    NN_CMD_SP_3 =  3,  //!< Spare

    NN_CMD_SIG1 =  4,  //!< Misc (MEM, etc)
    NN_CMD_P2PC =  5,  //!< P2P Address setup (Handled specially)
    NN_CMD_FFS =   6,  //!< Flood fill start
    NN_CMD_FFCS =  7,  //!< Flood fill core and region select

    NN_CMD_P2PB =  8,  //!< Hop count limited
    NN_CMD_SP_9 =  9,  //!< Spare
    NN_CMD_SP_10 = 10, //!< Spare
    NN_CMD_BIFF =  11, //!< Board-info flood-fill (handled specially)

    NN_CMD_FBS =   12, //!< Filtered in FF code
    NN_CMD_FBD =   13,
    NN_CMD_FBE =   14,
    NN_CMD_FFE =   15
};
//! \brief NN_CMD_P2PC sub-command codes
enum scamp_nn_p2p_config_subcommands {
    P2PC_ADDR = 0, //!< Your P2P address is...
    P2PC_NEW = 1,  //!< (Broadcast) I/somebody just discovered/updated my/their P2P address
    P2PC_DIMS = 2  //!< (Broadcast) The current best guess of P2P coordinates is...
};
//! \}

//------------------------------------------------------------------------------

//! \name Peer-to-peer messaging
//! \{

//! Values in P2P type (seq) field
enum scamp_p2p_type_codes {
    P2P_DATA =  0,      //!< Data message
    P2P_CTRL =  1,      //!< Control message
    P2P_LEVEL = 2       //!< Used for CountState requests
};

//! Distinguish data/control packets in SDP/P2P
enum scamp_p2p_control_message_codes {
    P2P_OPEN_REQ =      (1 << 24),      //!< Open channel request
    P2P_OPEN_ACK =      (2 << 24),      //!< Open channel acknowledge
    P2P_DATA_ACK =      (3 << 24),      //!< Data acknowledge
    P2P_CLOSE_REQ =     (4 << 24),      //!< Close channel request
    P2P_CLOSE_ACK =     (5 << 24),      //!< Close channel acknowledge

    // These should only be sent when the table is empty, as they can then
    // send and receive to a fake (0, 0)
    P2P_PING_REQ =      (6 << 24),      //!< Ping request
    P2P_PING_ACK =      (7 << 24)       //!< Ping response
};

#define P2P_DEF_SQL     4               //!< Seq len = 2<sup>4</sup>

//------------------------------------------------------------------------------
//! Initial value of p2p_addr_guess_{x,y} when not the root chip
#define NO_IDEA                 (-1024)

//! Size of p2p_addr_table in bytes.
#define P2P_ADDR_TABLE_BYTES    (512 * 512 / 8)

//! \brief Number of microseconds to wait between sending P2PB packets on
//! neighbouring chips
#define P2PB_OFFSET_USEC 100

//! \}

//------------------------------------------------------------------------------

//! \name sync0/sync1 arrival alignment
//! \{

#define NODE_DLY_NS             500     //!< per-node delay (ns)
#define BRD_DLY_NS              900     //!< board-to-board delay (ns)
#define TOP_DLY_US              155     //!< largest delay in 256x256 nodes (us)

//! \}

//------------------------------------------------------------------------------

//! \name Hop table
//! \{

#define NN_HOP_MASK     0x3ff           //!< !! Allows hops <= 1023

#define HOP_TABLE_SIZE  65536           //!< Size of the hop table
//! \}

//------------------------------------------------------------------------------

//! \name Network initialisation
//! \{

//! Phases of the network initialisation process, in order
enum netinit_phase_e {
    //! Configure P2P addresses for all chips while waiting for all chips to
    //! come online.
    NETINIT_PHASE_P2P_ADDR,
    //! Determine the dimensions of the system and kill off all chips outside
    //! this range (ensuring incorrectly guessed dimensions do not result in
    //! address aliasing).
    NETINIT_PHASE_P2P_DIMS,
    //! Send Board-info flood-fill messages to disable all known iffy links,
    //! cores and chips.
    NETINIT_PHASE_BIFF,
    //! Construct the P2P routing tables
    NETINIT_PHASE_P2P_TABLE,
    //! Setting the Ethernet address
    NETINIT_PHASE_SET_ETHERNET_ADDR,
    //! Blacklisted monitor delegates its functions to another core
    NETINIT_PHASE_DEL,
    //! The boot process is complete and the system is ready for use
    NETINIT_PHASE_DONE = 0xFF,
};

//! Phases of the Ethernet initialisation process, in order
enum ethinit_phase_e {
    //! FIRST PHASE - wait for Ethernet to come up
    ETHINIT_PHASE_WAIT_1,
    //! SECOND_PHASE - wait for Ethernet to come up
    ETHINIT_PHASE_WAIT_2,
    //! Ethernet either up or timed out
    ETHINIT_PHASE_DONE = 0xFF,
};
//! \}

//------------------------------------------------------------------------------

//! Allocation commands
enum alloc_cmd_e {
    ALLOC_SDRAM,                //!< Allocate SDRAM
    FREE_SDRAM,                 //!< Free SDRAM
    FREE_SDRAM_ID,              //!< Free DRAM by ID
    ALLOC_RTR,                  //!< Allocate Router
    FREE_RTR,                   //!< Free Router
    FREE_RTR_ID,                //!< Free Router by ID
    SDRAM_SPACE,                //!< Total free space & largest free block
    HEAP_TAG_PTR,               //!< Heap block from tag & ID
    ALLOC_MAX=HEAP_TAG_PTR      //!< Maximum command
};

//------------------------------------------------------------------------------

//! \name Structures
//! \{

//! IPTAG entry (32 bytes)
typedef struct {
    uchar ip[4];                //!< IP address
    uchar mac[6];               //!< MAC address
    ushort tx_port;             //!< Transmit port
    ushort timeout;             //!< Timeout when waiting to transmit
    ushort flags;               //!< See scamp_iptag_flags
    uint count;                 //!< Count of packets routed via this IPTag
    ushort rx_port;             //!< Receive port
    ushort dest_addr;           //!< Destination SDP address
    uchar dest_port;            //!< Destination SDP port
    uchar __PAD1[7];
} iptag_t;

//! Max number of entries in packet queue
#define PKT_QUEUE_SIZE          32

//! Queue of packets (really a circular buffer)
typedef struct pkt_queue_t {
    uchar insert;               //!< Insertion index
    uchar remove;               //!< Removal index
    volatile uchar count;
    uchar max;
    pkt_t queue[PKT_QUEUE_SIZE];
} pkt_queue_t;

//! Holds a NN packet awaiting transmission
typedef struct pkt_buf_t {
    struct pkt_buf_t *next;     //!< Next packet to transmit
    volatile uchar flags;       //!< General flags
    uchar fwd;                  //!< Is this a forwarded packet
    uchar delay;                //!< How long to wait before sending
    uchar link;                 //!< Which ways to send along
    pkt_t pkt;                  //!< What to send
} pkt_buf_t;

//! Used for tracking signal dispatch
typedef struct {        // 64 bytes
    uint   level_addr;  //!< 0: This chip's region at this level
    ushort sent;        //!< 4: Number of requests sent out in this region
    ushort rcvd;        //!< 6: Number of responses received
    ushort parent;      //!< 8: P2P address of the chip which sent the last request
    ushort __PAD1;
    uint result;        //!< 12: Result accumulated within this region
    ushort addr[16];    //!< 16: A working chip p2p for each subregion, if valid
    uchar  valid[16];   //!< 48: Is at least one chip in each sub-region known to be alive?
} level_t;
//! \}

//------------------------------------------------------------------------------

//! \name SCAMP core
//! \brief See scamp-3.c
//! \{

extern uint pkt_tx(uint tcr, uint data, uint key);
extern void proc_byte_set(uint a1, uint a2);
extern void msg_queue_insert(sdp_msg_t *msg, uint srce_ip);
extern uint iptag_new(void);
extern void assign_virt_cpu(uint phys_cpu);
extern void remap_phys_cores(uint phys_cores);
extern void delegate(void);
//! \}

//! \name SCAMP nearest-neighbour discovery protocol
//! \brief See scamp-nn.c
//! \{

extern void compute_eth(void);
extern void compute_level(uint p2p_addr);
extern void level_config(void);
extern void ff_nn_send(uint key, uint data, uint fwd_rty, uint log);
extern void biff_nn_send(uint data);
extern void nn_cmd_biff(uint x, uint y, uint data);
extern void nn_mark(uint key);
extern uint link_read_word(uint addr, uint link, uint *buf, uint timeout);
extern uint link_write_word(uint addr, uint link, uint *buf, uint timeout);
extern uint next_id(void);
extern void nn_init(void);
//! \deprecated No definition available; not used
//! \param aplx_addr: Address of APLX?
//! \param cpu_mask: Core select mask?
extern void proc_ffe(uint aplx_addr, uint cpu_mask);
//! \}

//! \name SCAMP SROM interface
//! \brief See spinn_srom.c
//! \{

//! \brief General purpose SROM interface routine
//!
//! ```
//! msg->arg1[31:16] = Data length (or zero)
//! msg->arg1[8] = Write (1) or Read (0)
//! msg->arg1[7] = Send WREN command first
//! msg->arg1[6] = Wait for completion
//! msg->arg1[5:0] = Number of command bits (8,16,32)
//!
//! msg->arg2[31:24] = SROM command
//! msg->arg2[23:0] = extra command bits
//! ```
//!
//! \param[in,out] msg: SCP message describing what to do.
//!     Updated with response payload if relevant.
//! \return the length of the result message
extern uint cmd_srom(sdp_msg_t *msg);
//! \}

//! \name SCAMP interrupt handlers
//! \brief See scamp-isr.c
//! \{

extern void vic_setup(void);
//! \}

//! \name SCAMP delegation
//! \brief See scamp-del.c
//! \{

extern void img_cp_exe(void);
//! \}

//! \name SCAMP application interface
//! \brief See scamp-app.c
//! \{

extern void proc_start_app(uint aplx_addr, uint id_op_mask);
extern void signal_app(uint data);
extern void boot_ap(void);
extern void clock_ap(uint virt_mask, uint enable);
extern void reset_ap(uint virt_mask);
//! \}

//! \name SCAMP peer-to-peer networking
//! \brief See scamp-p2p.c
//! \{

extern uint p2p_send_msg(uint addr, sdp_msg_t *msg);
extern void desc_init(void);
extern uint p2p_ping(uint addr, uint link, uint timeout);
//! \}

//! \name SCAMP nearest-neighbour discovery protocol
//! \brief See scamp-nn.c
//! \{

extern void p2pc_addr_nn_send(uint arg1, uint arg2);
extern void p2pc_dims_nn_send(uint arg1, uint arg2);
extern void p2pb_nn_send(uint arg1, uint arg2);
//! \}

//! \name SCAMP command interface
//! \brief See scamp-cmd.c
//! \{

extern uint scamp_debug(sdp_msg_t *msg, uint srce_ip);
//! \}

//! \name SCAMP bootROM support
//! \brief See scamp-boot.c
//! \{

extern void boot_nn(uint hw_ver);

//! \name SCAMP disable links function.  See scamp-3.c
extern void disable_unidirectional_links(void);
//! \}

//------------------------------------------------------------------------------

//! \name SCAMP core
//! \{
extern srom_data_t srom;

extern uint link_en;
extern uint num_cpus;

extern iptag_t tag_table[];
extern uint tag_tto;

extern uchar v2p_map[MAX_CPUS];

extern volatile enum netinit_phase_e netinit_phase;
extern volatile enum ethinit_phase_e ethinit_phase;
//! \}

//! \name SCAMP nearest-neighbour discovery protocol
//! \{
extern uint mon_del;

extern uint* hop_table;

extern uchar core_app[MAX_CPUS];
extern uint app_mask[256];
//! \}

//! \name SCAMP peer-to-peer networking
//! \{
extern uint p2p_addr;
extern uint p2p_dims;
extern uint p2p_root;
extern uint p2p_up;

extern volatile uint ticks_since_last_p2pc_new;
extern volatile uint ticks_since_last_p2pc_dims;
extern volatile int p2p_addr_guess_x;
extern volatile int p2p_addr_guess_y;
extern volatile int p2p_min_x;
extern volatile int p2p_max_x;
extern volatile int p2p_min_y;
extern volatile int p2p_max_y;
extern uchar *p2p_addr_table;
//! \}

//------------------------------------------------------------------------------

#endif
