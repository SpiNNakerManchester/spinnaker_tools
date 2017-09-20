//------------------------------------------------------------------------------
//
// scamp.h	    Header file for SC&MP
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

#ifndef SCAMP_H
#define SCAMP_H

//------------------------------------------------------------------------------

// Misc constants

#define MAX_CPUS 		20			// Legacy const!

#define MONITOR_CPU 		0			// Virtual CPU number

//------------------------------------------------------------------------------

// IPTag table sizes

#define TAG_FIXED_SIZE		8	// At bottom of table
#define TAG_POOL_SIZE		8

#define FIRST_POOL_TAG		TAG_FIXED_SIZE
#define LAST_POOL_TAG		(TAG_FIXED_SIZE + TAG_POOL_SIZE - 1)

#define TAG_TABLE_SIZE 		(TAG_FIXED_SIZE + TAG_POOL_SIZE)

// IPTAG definitions

#define IPTAG_NEW		0
#define IPTAG_SET		1
#define IPTAG_GET		2
#define IPTAG_CLR		3
#define IPTAG_TTO		4

#define IPTAG_MAX		4

#define IPFLAG_VALID		0x8000	// Entry is valid
#define IPFLAG_TRANS		0x4000	// Entry is transient
#define IPFLAG_ARP		0x2000	// Awaiting ARP resolution

#define IPFLAG_REV		0x0200  // Reverse IPTag
#define IPFLAG_STRIP		0x0100  // Strip SDP headers

//------------------------------------------------------------------------------

// Bits in SDP Flags byte (3 used)

#define SDPF_REPLY		0x80	// Reply expected
#define SDPF_SUM		0x40	// Checksum before routing
#define SDPF_NR			0x20	// Don't route via P2P

//------------------------------------------------------------------------------

// Failure codes

// Non-zero causes CPU to sleep
// Bit 6 set causes CPU_OK bit to be cleared
// Bit 5 set causes LED3 to be turned on

// HW errors have bit 7 set & bit 6 set
// SW errors have bit 7 set & bit 6 clr

#define FAIL_RESET		0xc0		// Catch-all - set at reset
#define FAIL_ROMX		0xc1		// Exception in ROM code
#define FAIL_ITCM0		0xc2		// ITCM top 512 failure
#define FAIL_ITCM1		0xc3		// ITCM main test failure
#define FAIL_DTCM		0xc4		// DTCM test failure

#define FAIL_TIMER		0xc5		// Timer reg test failed
#define FAIL_VIC		0xc6		// VIC reg test failed
#define FAIL_CC			0xc7		// Comms ctlr reg test failed
#define FAIL_DMA		0xc8		// DMAC reg test failed

#define FAIL_MP			0xc9		// Previous monitor proc failure
#define FAIL_LATE		0xca		// App CPU failed to set CPU_OK
#define FAIL_MANUF		0xcb		// App CPU in manuf test
#define FAIL_SLEEP		0xcc		// Ordered to sleep in startup

#define FAIL_TLM		0xcf		// Special for TLM

#define FAIL_VEC		0xa0		// Unhandled exception

//------------------------------------------------------------------------------

// NN opcodes
//   Codes < 7 have propagation limited by the ID field in the packet.
//   Codes >= 7 have various other ways of handling propagation.
//   Codes 0-3, 8-11 have explicit FwdRty in the packet.
//   Codes 4-7, 12-15 use stored FwdRty parameters
// Contrary to the above:
//   NN_CMD_P2PC are limited to only forward packets containing new information
//   NN_CMD_BIFF are limited by ID in a separate ID-space to other commands and
//   their propagation is limited to just the current board.

// Format for NN_CMD_P2PC commands
//   key[31:28] 4-bit one's complement checksum of key and data (see chksum_64)
//   key[27:24] NN command (== NN_CMD_P2PC)
//   key[23:4] Unused
//   key[3:2] Type: P2PC_ADDR, P2PC_NEW or P2PC_DIMS
//   key[1] Must be 0, differentiator between peek/poke responses and other
//          NN packets
// Format for NN_CMD_BIFF commands
//   key[31:28] 4-bit one's complement checksum of key and data (see chksum_64)
//   key[27:24] NN command (== NN_CMD_BIFF)
//   key[23:16] Unused
//   key[13:11] X coordinate of packet within board (updated when forwarded)
//   key[10:8] Y coordinate of packet within board (updated when forwarded)
//   key[7:1] 7-bit packet ID, used for ignoring echos (in a different address
//            space to other NN packets).
//   key[1] Must be 0, differentiator between peek/poke responses and other
//          NN packets
// Nearest neighbour packet format all other commands
//   key[31:28] 4-bit one's complement checksum of key and data (see chksum_64)
//   key[27:24] NN command
//   key[23:16] Forward (for packets with command bit 2 set)
//   key[15:8] Retry (for packets with command bit 2 set)
//   key[7:1] 7-bit packet ID, used for ignoring echos (for packets with
//            commands < 7)
//   key[1] Must be 0, differentiator between peek/poke responses and other
//          NN packets

#define NN_CMD_SIG0		0	// Misc (GTPC, Set FwdRty, LED, etc)
#define NN_CMD_RTRC 		1	// Router Control Reg
#define NN_CMD_LTPC		2	// Local Time Phase Ctrl (ID=0, Fwd=0)
#define NN_CMD_SP_3		3	// Spare

#define NN_CMD_SIG1     	4	// Misc (MEM, etc)
#define NN_CMD_P2PC 		5	// P2P Address setup (Handled specially)
#define NN_CMD_FFS   		6	// Flood fill start
#define NN_CMD_FFCS		7       // Flood fill core and region select

#define NN_CMD_P2PB		8	// Hop count limited
#define NN_CMD_SP_9		9	// Spare
#define NN_CMD_SP_10 		10	// Spare
#define NN_CMD_BIFF		11	// Board-info flood-fill (handled specially)

#define NN_CMD_FBS 		12	// Filtered in FF code
#define NN_CMD_FBD 		13
#define NN_CMD_FBE 		14
#define NN_CMD_FFE 		15

// NN_CMD_P2PC sub-command codes
#define P2PC_ADDR 0  // Your P2P address is...
#define P2PC_NEW  1  // (Broadcast) I/somebody just discovered/updated my/their P2P address
#define P2PC_DIMS 2  // (Broadcast) The current best guess of P2P coordinates is...

//------------------------------------------------------------------------------

// Values in P2P type (seq) field

#define P2P_DATA		0
#define P2P_CTRL		1
#define P2P_LEVEL		2

// Distinguish data/control packets in SDP/P2P

#define P2P_OPEN_REQ   		(1 << 24)
#define P2P_OPEN_ACK   		(2 << 24)
#define P2P_DATA_ACK   		(3 << 24)
#define P2P_CLOSE_REQ  		(4 << 24)
#define P2P_CLOSE_ACK  		(5 << 24)

#define P2P_DEF_SQL  		4			// Seq len = 2^4


//------------------------------------------------------------------------------

// Hop table

#define NN_HOP_MASK 		0x3ff		// !! Allows hops <= 1023

#define HOP_TABLE_SIZE		65536

//------------------------------------------------------------------------------

// Number of microseconds to wait between sending P2PB packets on neighbouring
// chips
#define P2PB_OFFSET_USEC 100

//------------------------------------------------------------------------------

// Phases of the network initialisation process, in order
enum netinit_phase_e {
    // Configure P2P addresses for all chips while waiting for all chips to
    // come online.
    NETINIT_PHASE_P2P_ADDR,
    // Determine the dimensions of the system and kill off all chips outside
    // this range (ensuring incorrectly guessed dimensions do not result in
    // address aliasing).
    NETINIT_PHASE_P2P_DIMS,
    // Send Board-info flood-fill messages to disable all known iffy links,
    // cores and chips.
    NETINIT_PHASE_BIFF,
    // Construct the P2P routing tables
    NETINIT_PHASE_P2P_TABLE,
    // Setting the Ethernet address
    NETINIT_PHASE_SET_ETHERNET_ADDR,
    // The boot process is complete and the system is ready for use
    NETINIT_PHASE_DONE = 0xFF,
};

// Phases of the Ethernet initialisation process, in order
enum ethinit_phase_e {
    // FIRST PHASE - wait for Ethernet to come up
    ETHINIT_PHASE_WAIT_1,
    // SECOND_PHASE - wait for Ethernet to come up
    ETHINIT_PHASE_WAIT_2,
    // Ethernet either up or timed out
    ETHINIT_PHASE_DONE = 0xFF,
};

//------------------------------------------------------------------------------

enum alloc_cmd_e {
    ALLOC_SDRAM,		//!< Allocate SDRAM
    FREE_SDRAM,		 	//!< Free SDRAM
    FREE_SDRAM_ID,		//!< Free DRAM by ID
    ALLOC_RTR,		 	//!< Allocate Router
    FREE_RTR,	 		//!< Free Router
    FREE_RTR_ID,		//!< Free Router by ID
    SDRAM_SPACE,		//!< Total free space & largest free block
    HEAP_TAG_PTR,		//!< Heap block from tag & ID
    ALLOC_MAX=HEAP_TAG_PTR 	//!< Maximum command
};

//------------------------------------------------------------------------------

typedef struct {		// IPTAG entry (32 bytes)
    uchar ip[4];
    uchar mac[6];
    ushort tx_port;
    ushort timeout;
    ushort flags;
    uint count;
    ushort rx_port;
    ushort dest_addr;
    uchar dest_port;
    uchar __PAD1[7];
} iptag_t;


#define PKT_QUEUE_SIZE 		32

typedef struct pkt_queue_t {	// Queue of packets
    uchar insert;
    uchar remove;
    volatile uchar count;
    uchar max;
    pkt_t queue[PKT_QUEUE_SIZE];
} pkt_queue_t;


typedef struct pkt_buf_t {	// Holds a NN packet awaiting transmission
    struct pkt_buf_t *next;
    volatile uchar flags;
    uchar fwd;
    uchar delay;
    uchar link;
    pkt_t pkt;
} pkt_buf_t;


typedef struct {	// 64 bytes
    uint   level_addr;	// 0: This chip's region at this level
    ushort sent;	// 4: Number of requests sent out in this region
    ushort rcvd;	// 6: Number of responses received
    ushort parent;	// 8: P2P address of the chip which sent the last request
    ushort __PAD1;	// 10
    uint result;	// 12: Result accumulated within this region
    ushort addr[16];	// 16: A working chip p2p for each subregion, if valid
    uchar  valid[16];	// 48: Is at least one chip in each sub-region known to be alive?
} level_t;

//------------------------------------------------------------------------------

extern uint pkt_tx(uint tcr, uint data, uint key);
extern void proc_byte_set(uint a1, uint a2);
extern void msg_queue_insert(sdp_msg_t *msg, uint srce_ip);

// scamp-nn.c

extern void compute_eth(void);
extern void compute_level(uint p2p_addr);
extern void level_config(void);
extern void ff_nn_send(uint key, uint data, uint fwd_rty, uint log);
extern void biff_nn_send(uint data);
extern void nn_cmd_biff(uint x, uint y, uint data);
extern void nn_mark(uint key);
extern uint link_read_word(uint addr, uint link, uint *buf, uint timeout);
extern uint link_write_word(uint addr, uint link, uint *buf, uint timeout);
extern void proc_start_app(uint aplx_addr, uint id_mask);
extern uint next_id(void);
extern void nn_init(void);
extern void proc_ffe(uint aplx_addr, uint cpu_mask);

// scamp-3.c

extern uint iptag_new(void);
extern void assign_virt_cpu(uint phys_cpu);
extern void remap_phys_cores(uint phys_cores);

// spinn_srom.c

extern uint cmd_srom(sdp_msg_t *msg);

// scamp-isr.c

extern void vic_setup(void);

// scamp-app.c

extern void signal_app(uint data);
extern void boot_ap(void);
extern void clock_ap(uint virt_mask, uint enable);
extern void reset_ap(uint virt_mask);

// scamp-p2p.c

extern uint p2p_send_msg(uint addr, sdp_msg_t *msg);

// scamp-nn.c
extern void p2pc_addr_nn_send(uint arg1, uint arg2);
extern void p2pc_dims_nn_send(uint arg1, uint arg2);
extern void p2pb_nn_send(uint arg1, uint arg2);

// scamp-cmd.c

extern uint scamp_debug(sdp_msg_t *msg, uint srce_ip);
extern void send_p2pc(uint arg2, uint arg3);

// scamp-boot.c

extern void boot_nn(uint hw_ver);

//------------------------------------------------------------------------------

extern uint biff_complete;
extern uint p2p_addr;
extern uint p2p_dims;
extern uint p2p_root;
extern uint p2p_up;
extern uint link_en;
extern uint num_cpus;
extern uchar v2p_map[MAX_CPUS];

extern uint* hop_table;

extern iptag_t tag_table[];
extern uint tag_tto;

extern uchar v2p_map[MAX_CPUS];
extern uchar core_app[MAX_CPUS];
extern uint app_mask[256];

extern volatile enum netinit_phase_e netinit_phase;
extern volatile enum ethinit_phase_e ethinit_phase;
extern volatile uint ticks_since_last_p2pc_new;
extern volatile uint ticks_since_last_p2pc_dims;
extern volatile int p2p_addr_guess_x;
extern volatile int p2p_addr_guess_y;
extern volatile int p2p_min_x;
extern volatile int p2p_max_x;
extern volatile int p2p_min_y;
extern volatile int p2p_max_y;
extern uchar *p2p_addr_table;

// Initial value of p2p_addr_guess_{x,y} when not the root chip
#define NO_IDEA			(-1024)

// Size of p2p_addr_table in bytes.
#define P2P_ADDR_TABLE_BYTES	(512 * 512 / 8)

//------------------------------------------------------------------------------

extern void putz(uint v);

#endif
