
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
// Codes < 8 have propagation limited by the ID field in the packet.
// Codes >= 8 have various other ways of handling propagation.
// Codes 0-3, 8-11 have explicit FwdRty in the packet.
// Codes 4-7, 12-15 use stored FwdRty parameters

#define NN_CMD_SIG0		0	// Misc (GTPC, Set FwdRty, LED, etc)
#define NN_CMD_RTRC 		1	// Router Control Reg
#define NN_CMD_LTPC		2	// Local Time Phase Ctrl (ID=0, Fwd=0)
#define NN_CMD_SP_3		3

#define NN_CMD_SIG1     	4	// Misc (MEM, etc)
#define NN_CMD_P2PC 		5	// P2P Address setup
#define NN_CMD_FFS   		6	// Flood fill start
#define NN_CMD_SP_7		7

#define NN_CMD_P2PB		8	// Hop count limited
#define NN_CMD_MSST		9	// Self-limiting
#define NN_CMD_SP_10 		10	// Spare
#define NN_CMD_SP_11 		11	// Spare

#define NN_CMD_FBS 		12	// Filtered in FF code
#define NN_CMD_FBD 		13
#define NN_CMD_FBE 		14
#define NN_CMD_FFE 		15

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

enum alloc_cmd_e
{
  ALLOC_SDRAM,		 	//!< Allocate SDRAM
  FREE_SDRAM,		 	//!< Free SDRAM
  FREE_SDRAM_ID,		//!< Free DRAM by ID
  ALLOC_RTR,		 	//!< Allocate Router
  FREE_RTR,	 		//!< Free Router
  FREE_RTR_ID,		 	//!< Free Router by ID
  ALLOC_MAX=FREE_RTR_ID	 	//!< Maximum command
};

//------------------------------------------------------------------------------

typedef struct 			// IPTAG entry (32 bytes)
{
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

typedef struct pkt_queue_t	// Queue of packets
{
  uchar insert;
  uchar remove;
  volatile uchar count;
  uchar max;
  pkt_t queue[PKT_QUEUE_SIZE];
} pkt_queue_t;


typedef struct pkt_buf_t	// Holds a NN packet awaiting transmission
{
  struct pkt_buf_t *next;
  volatile uchar flags;
  uchar fwd;
  uchar delay;
  uchar link;
  pkt_t pkt;
} pkt_buf_t;


typedef struct		// 64 bytes
{
  uint   level_addr;	// 0
  ushort sent;		// 4
  ushort rcvd;		// 6
  ushort parent;	// 8
  ushort __PAD1;	// 10
  uint result;		// 12
  ushort addr[16];	// 16
  uchar  valid[16];	// 48
} level_t;

//------------------------------------------------------------------------------

extern uint pkt_tx (uint tcr, uint data, uint key);
extern void proc_byte_set (uint a1, uint a2);
extern void msg_queue_insert (sdp_msg_t *msg, uint srce_ip);

// scamp-nn.c

extern void compute_level (uint p2p_addr);
extern void ff_nn_send (uint key, uint data, uint fwd_rty, uint log);
extern void nn_mark (uint key);
extern uint probe_links (uint mask, uint timeout);
extern uint link_read_word (uint addr, uint link, uint *buf, uint timeout);
extern uint link_write_word (uint addr, uint link, uint *buf, uint timeout);
extern void proc_start_app (uint aplx_addr, uint id_mask);
extern uint next_id (void);
extern void nn_init (void);
extern void proc_ffe (uint aplx_addr, uint cpu_mask);

// scamp-3.c

extern uint iptag_new (void);
extern void assign_virt_cpu (uint phys_cpu);

// spinn_srom.c

extern uint cmd_srom (sdp_msg_t *msg);

// scamp-isr.c

extern void vic_setup (void);

// scamp-app.c

extern void signal_app (uint data);
extern void boot_ap (void);
extern void clock_ap (uint virt_mask, uint enable);
extern void reset_ap (uint virt_mask);

// scamp-p2p.c

extern uint p2p_send_msg (uint addr, sdp_msg_t *msg);

// scamp-nn.c

// scamp-cmd.c

extern uint scamp_debug (sdp_msg_t *msg, uint srce_ip);
extern void send_p2pc (uint arg2, uint arg3);

// scamp-boot.c

extern void boot_nn (uint hw_ver);

//------------------------------------------------------------------------------

extern uint p2p_addr;
extern uint p2p_dims;
extern uint p2p_up;
extern uint max_hops;
extern uint link_up;
extern uint num_cpus;
extern uchar v2p_map[MAX_CPUS];

extern uint* hop_table;

extern iptag_t tag_table[];
extern uint tag_tto;

extern uchar v2p_map[MAX_CPUS];
extern uchar core_app[MAX_CPUS];
extern uint app_mask[256];

//------------------------------------------------------------------------------

extern void putz (uint v);

#endif
