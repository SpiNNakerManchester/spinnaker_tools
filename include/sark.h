//------------------------------------------------------------------------------
//! \file      sark.h
//! \brief     Spinnaker Application Runtime Kernel API
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

#ifndef __SARK_H__
#define __SARK_H__

#ifdef __cplusplus
#pragma GCC diagnostic ignored "-Wwrite-strings"
extern "C"
{
#endif /* __cplusplus */

#include <spinnaker.h>
#include <version.h>

//------------------------------------------------------------------------------

#define SARK_EVENT      1   //!< Defined to link to "event"

//------------------------------------------------------------------------------

//! \name SARK stack defaults
//! \{
//! The sizes below define individual sizes for three modes of four used by
//! SARK. The sizes of individual stacks and the stack top can be modified at
//! startup by using "sark_config". The size of the SYS mode stack is the
//! difference between the sum of the three stack sizes given here and the
//! overall stack length from the linker script. The default is for a stack
//! length of 2048 and hence a SYS stack of 1152 bytes. All of these values
//! must be a multiple of 4!
//! \note default stack top and length are specified in the linker script.

#define SVC_STACK          384  //!< Size in bytes of SVC stack
#define IRQ_STACK          256  //!< Size in bytes of IRQ stack
#define FIQ_STACK          256  //!< Size in bytes of FIQ stack

//! \}

// SARK configuration defaults - these can be modified at SARK
// start-up using "sark_config"

#define NUM_SDP_MSGS     4  //!< Number of SDP messages for SARK
#define SARK_SLOT       15  //!< VIC slot for SARK interrupt
#define INIT_EVENTS     16  //!< Number of events initially allocated

#define DEAD_WORD       0xdeaddead  //!< Stack fill value

//------------------------------------------------------------------------------

/*!
Defines and typedefs to rationalise ARM/GNU interrupt handlers
*/

#ifdef DOXYGEN
//! The basic type of an interrupt handler. Takes no arguments. Returns nothing.
typedef void (*int_handler) (void);

#else
#ifdef __GNUC__
# define INT_HANDLER \
    void __attribute__ ((interrupt ("IRQ")))
# define SARK_IS_A_MALLOC(size_arg) \
    __attribute__ ((malloc, alloc_size(size_arg), assume_aligned(4)))
# define SARK_IS_A_CALLOC(size_arg1, size_arg2) \
    __attribute__ ((malloc, alloc_size(size_arg1, size_arg2), assume_aligned(4)))
typedef void (*int_handler) (void);

#else
# define INT_HANDLER \
    __irq void
# define SARK_IS_A_MALLOC(size_arg) \
    __attribute__ ((malloc))
# define SARK_IS_A_CALLOC(size_arg1, size_arg2) \
    __attribute__ ((malloc))
typedef __irq void (*int_handler) (void);   //!< Interrupt handler
#endif

#endif

// doxygen has overrides for all of these.
#ifndef NONNULL
#define NONNULL __attribute__((nonnull))
#define NONNULL2 __attribute__((nonnull(2)))
#define NORETURN __attribute__((noreturn))
#endif

//------------------------------------------------------------------------------

// Useful macros...

#ifndef ntohs

//! Macro to convert shorts from network byte order
# define ntohs(t) ((((t) & 0x00ff) << 8) | (((t) >> 8) & 0x00ff))

//! Macro to convert shorts to network byte order
# define htons(t) ((((t) & 0x00ff) << 8) | (((t) >> 8) & 0x00ff))

#endif

//! Make a chip ID from X and Y coordinates
# define CHIP_ID(x, y) (((x) << 8) + (y))

//! Extract chip X coordinate from a chip ID
# define CHIP_X(id)    ((id) >> 8)

//! Extract chip Y coordinate from a chip ID
# define CHIP_Y(id)    ((id) & 255)


//------------------------------------------------------------------------------

// APLX definitions

//! Commands used in the APLX loader.
enum sark_aplx_command {
    APLX_ACOPY = 1,     //!< Absolute copy
    APLX_RCOPY = 2,     //!< Relative copy
    APLX_FILL = 3,      //!< Fill memory
    APLX_EXEC = 4,      //!< Execute

    APLX_END = 0xff     //!< End marker (rarely used)
};

#define APLX_ENTRY_SIZE 16      //!< Bytes per table entry
#define APLX_BUF_SIZE   64      //!< Size of APLX loader code

//! The APLX buffer lives at the top of ITCM, starting at 0x7fc0
#define APLX_BUFFER     (ITCM_TOP - APLX_BUF_SIZE)

//------------------------------------------------------------------------------

// IPTAG definitions

#define TAG_NONE        255     //!< Invalid tag/transient request
#define TAG_HOST        0       //!< Reserved for host

//------------------------------------------------------------------------------

// Definition of range of router MC entries which may be allocated

#define RTR_ALLOC_FIRST     1               //!< Must be > 0
#define RTR_ALLOC_LAST      MC_TABLE_SIZE   //!< Must be <= MC_TABLE_SIZE

//! Largest allowable request (1023 entries)
#define RTR_ALLOC_MAX       (RTR_ALLOC_LAST - RTR_ALLOC_FIRST)

// Flags to sark_xalloc() (etc)

#define ALLOC_LOCK      1       //!< Lock this operation
#define ALLOC_ID        2       //!< Use supplied AppID

//! This allows for a "retry" of the allocation request when an SDRAM tag is
//! specified.  If this flag is specified, and the tag in use, and the size of
//! the allocation requested is the same as the size currently allocated, just
//! return the address of the tag from the previous allocation.  Without this
//! flag, the previous behavior is maintained, meaning that each tag can only
//! be allocated once, and a re-allocation request will result in a NULL being
//! returned.
#define ALLOC_TAG_RETRY 4

//------------------------------------------------------------------------------

/*!
Allocations of SysCtl Test & Set registers (locks)
*/

enum spin_lock_e {
    LOCK_MSG,           //!< Msg buffers in SysRAM
    LOCK_MBOX,          //!< Mailbox flag variable
    LOCK_ETHER,         //!< Ethernet Tx (unused...)
    LOCK_GPIO,          //!< GPIO port (unused...)
    LOCK_API_ROOT,      //!< Spin1 API
    LOCK_SEMA,          //!< Sema access
    LOCK_HEAP,          //!< Heap in System / SDRAM
    LOCK_RTR            //!< Router
};

typedef enum spin_lock_e spin_lock; //!< Typedef for enum spin_lock_e

//! For "sark_alib.s" (maintain sync with enum spin_lock_e)

#define A_LOCK_SEMA     5

//------------------------------------------------------------------------------

//! Flags in app_flags field
enum app_flags_e {
    APP_FLAG_WAIT       //!< Wait for start signal
};


//------------------------------------------------------------------------------

//! Run time error codes, passed as first arg to rt_error()
enum rte_code_e {
    RTE_NONE,           //!< 0 No error
    RTE_RESET,          //!< 1 Branch through zero
    RTE_UNDEF,          //!< 2 Undefined instruction
    RTE_SVC,            //!< 3 Undefined SVC or no handler
    RTE_PABT,           //!< 4 Prefetch abort
    RTE_DABT,           //!< 5 Data abort
    RTE_IRQ,            //!< 6 Unhandled IRQ
    RTE_FIQ,            //!< 7 Unhandled FIQ
    RTE_VIC,            //!< 8 Unconfigured VIC vector

    RTE_ABORT,          //!< Generic user abort
    RTE_MALLOC,         //!< "malloc" failure
    RTE_DIV0,           //!< Divide by zero
    RTE_EVENT,          //!< Event startup failure
    RTE_SWERR,          //!< Fatal SW error
    RTE_IOBUF,          //!< Failed to allocate IO buffer
    RTE_ENABLE,         //!< Bad event enable
    RTE_NULL,           //!< Generic null pointer error
    RTE_PKT,            //!< Pkt startup failure
    RTE_TIMER,          //!< Timer startup failure
    RTE_API,            //!< API startup failure
    RTE_VER             //!< SW version conflict
};

typedef enum rte_code_e rte_code;   //!< Typedef for enum rte_code_e

//! For "sark_alib.s" (maintain sync with enum rte_code_e)
enum sark_alib_rte_codes {
    A_RTE_RESET = 1,    //!< Branch through zero
    A_RTE_UNDEF = 2,    //!< Undefined instruction
    A_RTE_SVC = 3,      //!< Undefined SVC or no handler
    A_RTE_PABT = 4,     //!< Prefetch abort
    A_RTE_DABT = 5,     //!< Data abort
    A_RTE_IRQ = 6,      //!< Unhandled IRQ
    A_RTE_FIQ = 7,      //!< Unhandled FIQ
    A_RTE_VIC = 8,      //!< Unconfigured VIC vector
};

//------------------------------------------------------------------------------

// Various enumerated types, etc

/*!
\brief Runtime state of a core (limited to 16)

Held in vcpu_t::cpu_state byte of vcpu_t struct.
States less than 4 are 'dead' and not expected to respond to anything
*/

enum cpu_state_e {
    CPU_STATE_DEAD,     //!< Dead core
    CPU_STATE_PWRDN,    //!< Powered down
    CPU_STATE_RTE,      //!< Died with Run Time Error
    CPU_STATE_WDOG,     //!< Watchdog expired

    CPU_STATE_INIT,     //!< Initialising (transient)
    CPU_STATE_WAIT,     //!< Ready to execute
    CPU_STATE_SARK,     //!< Entered c_main
    CPU_STATE_RUN,      //!< Running (API/Event)

    CPU_STATE_SYNC0,    //!< Waiting for sync 0
    CPU_STATE_SYNC1,    //!< Waiting for sync 1
    CPU_STATE_PAUSE,    //!< Paused in application
    CPU_STATE_EXIT,     //!< Exited application

    CPU_STATE_12,       //!< Spare
    CPU_STATE_13,       //!< Spare
    CPU_STATE_14,       //!< Spare
    CPU_STATE_IDLE      //!< Idle (SARK stub)
};

typedef enum cpu_state_e cpu_state; //!< Typedef for enum cpu_state_e

//! For "sark_alib.s" (maintain sync with enum cpu_state_e)
#define A_STATE_RTE 2

//! Types of event
enum event_type_e {
    EVENT_TIMER,        //!< Timer elapsed
    EVENT_RXPKT,        //!< MC packet received
    EVENT_SDP,          //!< SDP packet received
    EVENT_USER,         //!< User triggered event
    EVENT_SIG,          //!< Signal from hosts
    EVENT_DMA,          //!< DMA complete
    EVENT_MAX=EVENT_DMA //!< Maximum event number
};

typedef enum event_type_e event_type;   //!< typedef for enum event_type_e

//! For "sark_alib.s" (maintain sync with enum event_type_e)
#define EVENT_COUNT 6

//! State of event execution
enum event_state_e {    // RUN must be first (0)
    EVENT_RUN,          //!< Running
    EVENT_PAUSE,        //!< Paused
    EVENT_STOP          //!< Stopped
};

typedef enum event_state_e event_state; //!< typedef for enum event_state_e

//! Slots in the VIC interrupt controller
enum vic_slot_e {
    SLOT_0,
    SLOT_1,
    SLOT_2,
    SLOT_3,
    SLOT_4,
    SLOT_5,
    SLOT_6,
    SLOT_7,
    SLOT_8,
    SLOT_9,
    SLOT_10,
    SLOT_11,
    SLOT_12,
    SLOT_13,
    SLOT_14,
    SLOT_15,
    SLOT_FIQ,           //!< Special slot for FIQ
    SLOT_MAX = SLOT_15  //!< Index of largest ordinary slot
};

typedef enum vic_slot_e vic_slot;   //!< Typedef for enum vic_slot_e

//! Event queue priorities
enum event_priority_e {
    PRIO_0,
    PRIO_1,
    PRIO_2,
    PRIO_3,
    PRIO_MAX = PRIO_3
};

typedef enum event_priority_e event_priority;   //!< Typedef for enum event_priority_e

//! Mode for sw_error() calls to decide if rt_error() is to be called
enum sw_err_mode_e {
    SW_OPT,             //!< Optional, controlled by sark.sw_rte
    SW_NEVER,           //!< Never call rt_error
    SW_RTE              //!< Always call rt_error
};

typedef enum sw_err_mode_e sw_err_mode; //!< Typedef for enum sw_err_mode_e

//! Startup synchronisation bool
enum sync_bool_e {
    SYNC_NOWAIT,        //!< Don't wait
    SYNC_WAIT           //!< Wait for synchronisation
};

typedef enum sync_bool_e sync_bool; //!< Typedef for enum sync_bool_e

//------------------------------------------------------------------------------

// LED definitions - pass these to "sark_led_set"

#define LED_ON(n)   (3 << (2 * n))  //!< Turn LED on
#define LED_OFF(n)  (2 << (2 * n))  //!< Turn LED off
#define LED_INV(n)  (1 << (2 * n))  //!< Flip state of LED
#define LED_FLIP(n) (1 << (2 * n))  //!< Flip state of LED

//------------------------------------------------------------------------------

//! Mailbox commands passed to APs
enum shm_cmd_e {
    SHM_IDLE,           //!< Idle state of mailbox
    SHM_MSG,            //!< Passing SDP message
    SHM_NOP,            //!< Does nothing...
    SHM_SIGNAL,         //!< Signal application
    SHM_CMD             //!< Command to MP
};

typedef enum shm_cmd_e shm_cmd; //!< Typedef for enum shm_cmd_e

//------------------------------------------------------------------------------

//! Signals passed to applications
enum signal_e {
    SIG_INIT,           //!< Initialise cores
    SIG_PWRDN,          //!< Powerdown cores
    SIG_STOP,           //!< Stop application and clean up
    SIG_START,          //!< Start application if waiting

    SIG_SYNC0,          //!< Synchronisation barrier 0
    SIG_SYNC1,          //!< Synchronisation barrier 1
    SIG_PAUSE,          //!< Pause application
    SIG_CONT,           //!< Continue application

    SIG_EXIT,           //!< Terminate application
    SIG_TIMER,          //!< Trigger timer for application
    SIG_USR0,           //!< Send user signal 0
    SIG_USR1,           //!< Send user signal 1

    SIG_USR2,           //!< Send user signal 2
    SIG_USR3,           //!< Send user signal 3
};

typedef enum signal_e signal;   //!< Typedef for enum signal_e

//------------------------------------------------------------------------------

//! \name SARK I/O stream identifiers
//! \{
//! Used with io_printf() and io_put_char()

# define IO_STD     ((char *) 0)    //!< Direct I/O via SDP
# define IO_DBG     ((char *) 1)    //!< Low-level debug I/O
# define IO_BUF     ((char *) 2)    //!< SDRAM buffer (normal for running apps)
# define IO_NULL    ((char *) 3)    //!< Output > `/dev/null`

//! \}

//------------------------------------------------------------------------------

//! SCP command codes
enum sark_scp_command_codes {
    CMD_VER = 0,                //!< Return version/core info
    CMD_RUN = 1,                //!< Run at PC (Deprecated)
    CMD_READ = 2,               //!< Read memory
    CMD_WRITE = 3,              //!< Write memory
    CMD_APLX = 4,               //!< Run via APLX (Deprecated)
    CMD_FILL = 5,               //!< Fill memory

    // Following for monitors only
    CMD_REMAP = 16,             //!< Remap application core
    CMD_LINK_READ = 17,         //!< Read neighbour memory
    CMD_LINK_WRITE = 18,        //!< Write neighbour memory
    CMD_AR = 19,                //!< Application core reset

    CMD_NNP = 20,               //!< Send broadcast NN packet
    CMD_APP_COPY_RUN = 21,      //!< Copy app from adjacent chip and reset
    CMD_SIG = 22,               //!< Send signal to apps
    CMD_FFD = 23,               //!< Send flood-fill data

    CMD_AS = 24,                //!< Application core APLX start
    CMD_LED = 25,               //!< Control LEDs
    CMD_IPTAG = 26,             //!< Configure IPTags
    CMD_SROM = 27,              //!< Read/write/erase serial ROM

    CMD_ALLOC = 28,             //!< Memory allocation
    CMD_RTR = 29,               //!< Router control
    CMD_RSVD = 30,              //!< Reserved (used elsewhere in software)
    CMD_INFO = 31,              //!< Get chip/core info
    CMD_SYNC = 32,              //!< Control sending of synchronization msgs

    // 48-63 reserved for BMP

    CMD_TUBE = 64               //!< Tube output
};

//! SCP return codes
enum sark_scp_return_codes {
    RC_OK = 0x80,               //!< Command completed OK
    RC_LEN = 0x81,              //!< Bad packet length
    RC_SUM = 0x82,              //!< Bad checksum
    RC_CMD = 0x83,              //!< Bad/invalid command
    RC_ARG = 0x84,              //!< Invalid arguments
    RC_PORT = 0x85,             //!< Bad port number
    RC_TIMEOUT = 0x86,          //!< Timeout
    RC_ROUTE = 0x87,            //!< No P2P route
    RC_CPU = 0x88,              //!< Bad CPU number
    RC_DEAD = 0x89,             //!< SHM dest dead
    RC_BUF = 0x8a,              //!< No free SHM buffers
    RC_P2P_NOREPLY = 0x8b,      //!< No reply to open
    RC_P2P_REJECT = 0x8c,       //!< Open rejected
    RC_P2P_BUSY = 0x8d,         //!< Dest busy
    RC_P2P_TIMEOUT = 0x8e,      //!< Dest died?
    RC_PKT_TX = 0x8f,           //!< Pkt Tx failed
};

//! Memory size types
enum sark_scp_memory_size_types {
    TYPE_BYTE = 0,              //!< Specifies byte access
    TYPE_HALF = 1,              //!< Specifies short (16-bit) access
    TYPE_WORD = 2               //!< Specifies word (32-bit) access
};

//------------------------------------------------------------------------------

// Misc typedefs

//! Returned (div, mod) from divmod()
typedef struct divmod {
    uint div;           //!< Dividend
    uint mod;           //!< Modulus
} divmod_t;

//! Used in the block memory allocator (4 bytes)
typedef struct mem_link {
    struct mem_link *next;      //!< Pointer to next free block
} mem_link_t;

//! Used in the block memory allocator (8 bytes)
typedef struct mem_block {
    mem_link_t *free;   //!< Pointer to first free block
    ushort count;       //!< Count of blocks in use
    ushort max;         //!< Maximum blocks used
} mem_block_t;


//! Contents of SV SROM area (32 bytes)
typedef struct srom_data {
    ushort flags;       //!< 16 bit flags
    uchar  mac_addr[6]; //!< MAC address
    uchar  ip_addr[4];  //!< IP address
    uchar  gw_addr[4];  //!< Gateway address
    uchar  net_mask[4]; //!< Net mask
    ushort udp_port;    //!< UDP port for SDP messages
    ushort __PAD1;      //!< Spare...
    uint   __PAD2;      //!< Spare...
    uint   __PAD3;      //!< Spare...
} srom_data_t;

//! Copy of router entry (16 bytes)
typedef struct rtr_entry {
    ushort next;        //!< Index of next block
    ushort free;        //!< Index of next free block (or app_id)
    uint route;         //!< Route word
    uint key;           //!< Key word
    uint mask;          //!< Mask word
} rtr_entry_t;

//! \brief Stores info relating to AppIDs.
//!
//! The "cores" field is zero if the ID is not in use.
typedef struct app_data {
    uchar cores;        //!< Number of cores using this ID
    uchar clean;        //!< Set if this ID has been cleaned
    uchar sema;         //!< Semaphore
    uchar lead;         //!< Lead core number
    uint mask;          //!< Mask of cores using this ID
} app_data_t;


//------------------------------------------------------------------------------

// SDP messages

#define NUM_SDP_PORTS   8       //!< Number of SDP ports (0-7)
#define PORT_SHIFT      5       //!< Port is in top 3 bits
#define PORT_MASK       0xe0    //!< Port is in top 3 bits
#define CPU_MASK        0x1f    //!< CPU is in bottom 5 bits

#define PORT_ETH        255     //!< Special to indicate Ethernet

#define SDP_BUF_SIZE    256     //!< SDP data buffer capacity

/*!
\brief SDP message definition

\note the length field is the number of bytes _following the checksum._
It will be a minimum of 8 as the SDP header should always be present.

\note make sure to comply with sark_block_init() requirements:
    1. size must be a non-zero multiple of 4 bytes.
    2. first field must be a pointer to same struct type.
*/

typedef struct sdp_msg {        // SDP message - 292 bytes
    struct sdp_msg *next;       //!< Next in free list
    ushort length;              //!< length
    ushort checksum;            //!< checksum (if used)

    // sdp_hdr_t (mandatory)

    uchar flags;                //!< Flag byte
    uchar tag;                  //!< IP tag
    uchar dest_port;            //!< Destination port/CPU
    uchar srce_port;            //!< Source port/CPU
    ushort dest_addr;           //!< Destination address
    ushort srce_addr;           //!< Source address

    // cmd_hdr_t (optional)

    ushort cmd_rc;              //!< Command/Return Code
    ushort seq;                 //!< Sequence number
    uint arg1;                  //!< Arg 1
    uint arg2;                  //!< Arg 2
    uint arg3;                  //!< Arg 3

    // user data (optional)

    uchar data[SDP_BUF_SIZE];   //!< User data (256 bytes)

    uint __PAD1;                //!< Private padding
} sdp_msg_t;


// Legacy definitions...

//! SDP header: legacy
typedef struct sdp_hdr {
    uchar flags;
    uchar tag;
    uchar dest_port;
    uchar srce_port;
    ushort dest_addr;
    ushort srce_addr;
} sdp_hdr_t;

//! Command header: legacy
typedef struct cmd_hdr {
    ushort cmd_rc;
    ushort flags;
    uint arg1;
    uint arg2;
    uint arg3;
} cmd_hdr_t;

//------------------------------------------------------------------------------

// Heap data structures

/*!
\brief Heap data block.

One of these appears at the start of each
block in the heap so each allocation of N bytes in the heap
requires N+8
*/

typedef struct block {
    struct block *next;         //!< Chains all blocks together (in address order)
    struct block *free;         //!< Chains free blocks together (in address order)
} block_t;

/*!
\brief Heap root structure.

One of these appears at the start of the
heap area and maintains two lists, one containing all blocks in
the heap and one containing only free blocks. The heap always
contains a 'last' block, of zero size, which is used to mark the
end of the heap.
*/

typedef struct {
    block_t *free;              //!< Root of free block chain
    block_t *first;             //!< First block
    block_t *last;              //!< Last block (zero size, never used for storage)
    uint free_bytes;            //!< Number of free bytes left
    uchar buffer[];             //!< Buffer for blocks
} heap_t;


//------------------------------------------------------------------------------

// Struct "sark_vec" containing ARM exception vectors, etc

/*!
\brief An event_vec fits into 32 bits and contains a handler address in 16
bits and a priority and slot number, each 8 bits.

The handler is therefore constrained to live in the bottom 64K of the address
space (i.e., in ITCM)
*/

typedef struct event_vec {
    ushort proc;                //!< Handler address (squeezed into 16 bits!)
    uchar slot;                 //!< VIC slot
    uchar priority;             //!< Priority for queued events
} event_vec_t;

/*!
\brief A struct of type sark_vec_t lives at address 0x20, that is, it is
built into the code section.

It contains exception vectors and start-up configuration data which can be
modified at the appropriate point during start-up
*/
typedef struct sark_vec {
    int_handler reset_vec;      //!< 0x20 Reset vector
    int_handler undef_vec;      //!< 0x24 Undefined instruction vector
    int_handler svc_vec;        //!< 0x28 SVC vector
    int_handler pabt_vec;       //!< 0x2c Prefetch abort vector
    int_handler dabt_vec;       //!< 0x30 Data abort vector
    int_handler aplx_proc;      //!< 0x34 Pointer to "proc_aplx"
    int_handler irq_vec;        //!< 0x38 IRQ vector
    int_handler fiq_vec;        //!< 0x3c FIQ vector

    ushort svc_stack;           //!< 0x40 SVC stack size (words)
    ushort irq_stack;           //!< 0x42 IRQ stack size (words)
    ushort fiq_stack;           //!< 0x44 FIQ stack size (words)
    ushort stack_size;          //!< 0x46 Total stack size (bytes)

    uint *code_top;             //!< 0x48 Points to top of code
    uint *heap_base;            //!< 0x4c Points to base of heap
    uint *stack_top;            //!< 0x50 Points to top of stacks

    uint stack_fill;            //!< 0x54 Stack fill word

    uchar num_msgs;             //!< 0x58 Number of SDP msgs buffers
    uchar sark_slot;            //!< 0x59 VIC slot for MP->AP interrupt

    uchar num_events;           //!< 0x5a Number of initial events
    uchar api;                  //!< 0x5b API active
    uchar app_id;               //!< 0x5c App ID
    volatile uchar app_flags;   //!< 0x5d App flags
    ushort __PAD;               //!< 0x5e Pad

    event_vec_t event[EVENT_COUNT]; //!< 0x60 Event vectors...
} sark_vec_t;

//! A pointer to sark_vec at 0x20
static sark_vec_t * const sark_vec = (sark_vec_t *) 0x20;

//------------------------------------------------------------------------------

/*!
\brief Struct containing information about each virtual processor.

An array of NUM_CPU of these is based at SV_VCPU in System RAM and pointed
to by ::sv_vcpu
*/
typedef struct vcpu {           // 128 bytes
    uint r[8];                  //!<  0 - r0-r7
    uint psr;                   //!< 32 - cpsr
    uint sp;                    //!< 36 - sp
    uint lr;                    //!< 40 - lr
    uchar rt_code;              //!< 44 - RT error code
    uchar phys_cpu;             //!< 45 - Physical CPU
    uchar cpu_state;            //!< 46 - CPU state
    uchar app_id;               //!< 47 - Application ID
    void *mbox_ap_msg;          //!< 48 - mbox msg MP->AP
    void *mbox_mp_msg;          //!< 52 - mbox msg AP->MP
    volatile uchar mbox_ap_cmd; //!< 56 - mbox command MP->AP
    volatile uchar mbox_mp_cmd; //!< 57 - mbox command AP->MP
    ushort sw_count;            //!< 58 - SW error count (saturating)
    char *sw_file;              //!< 60 - SW source file name
    uint sw_line;               //!< 64 - SW source line (could be short?)
    uint time;                  //!< 68 - Time of loading
    char app_name[16];          //!< 72 - Application name
    void *iobuf;                //!< 88 - IO buffer in SDRAM (or 0)
    uint sw_ver;                //!< 92 - SW version
    uint __PAD[4];              //!< 96 - (spare)
    uint user0;                 //!< 112 - User word 0
    uint user1;                 //!< 116 - User word 1
    uint user2;                 //!< 120 - User word 2
    uint user3;                 //!< 124 - User word 3
} vcpu_t;


// For "sark_alib.s" (maintain sync with vcpu_t)

#define VCPU_SIZE       128     //!< Size of vcpu_t
#define VCPU_RT_CODE    44      //!< Offset of rt_code
#define VCPU_CPU_STATE  46      //!< Offset of cpu_state
#define VCPU_APP_ID     47      //!< Offset of app_id

//------------------------------------------------------------------------------

// Events

#define SARK_MSG_INT    (INT0_INT)  //!< Used for SARK to APP messaging
#define SARK_SIG_INT    (INT1_INT)  //!< Used for SARK to APP signalling

//! Generic (void) function taking two "uint" args
typedef void (*event_proc) (uint, uint);

/*!
\brief Event structure used by "event_xxx" and "timer_xxx" routines.

When the event is invoked, the proc is called with the two supplied
arguments. Queues of events are maintained either on a timer queue for
events which have to occur at a particular time or on a set of
priority ordered queues for events which are processed by a scheduler.

\note make sure to comply with sark_block_init() requirements:
    1. size must be a non-zero multiple of 4 bytes.
    2. first field must be a pointer to same struct type.
*/

typedef struct event {          // 28 bytes
    struct event *next;         //!< Next in Q or NULL

    event_proc proc;            //!< Proc to be called or NULL
    uint arg1;                  //!< First arg to proc
    uint arg2;                  //!< Second arg to proc
    uint time;                  //!< Time (CPU ticks) until event due
                                //!< (or 0 if at head of Q)
    uint ID;                    //!< Unique ID for active event (0 if inactive)

    uchar reuse;                //!< do not free event after queue processing
    uchar __PAD1;               //!< (spare - included for correct size)
    uchar __PAD2;               //!< (spare - included for correct size)
    uchar __PAD3;               //!< (spare - included for correct size)
} event_t;

//! Struct holding head and tail of a list of "event_t"
typedef struct proc_queue {
    event_t *proc_head;         //!< List of queued "proc" events
    event_t *proc_tail;         //!< and tail of that list
} proc_queue_t;

//! Struct holding a packet
typedef struct pkt {
    uint ctrl;                  //!< TCR in 23:16, flags in 1:0
    uint data;                  //!< Data (payload) field
    uint key;                   //!< Key (non-payload!) field
} pkt_t;

/*! \brief Struct holding data for "sark_event" and "sark_timer".

This holds all the variables needed by these two packages.
*/

typedef struct event_data {
    event_t *free;              //!< List of free events
    event_t *timer_queue;       //!< List of active timer events

    ushort count;               //!< Number of events currently in use
    ushort max;                 //!< Maximum number ever used
    uint id;                    //!< Holds unique ID for active events

    uint failed;                //!< Counts failures of event_new
    uint rc;                    //!< Failure codes

    volatile uchar state;       //!< Stop/pauses event loop
    uchar id_rsvd;              //!< number of reserved IDs
    uchar wait;                 //!< Wait state
    uchar user;                 //!< Non-zero if user event pending

    uint ticks;                 //!< Timer tick counter
    uint arg1;                  //!< Arg1 for user event
    uint arg2;                  //!< Arg2 for user event

    sdp_msg_t *msg;             //!< Passes msg from SARK to app
    uint signal;                //!< Passes signal from SARK to app

    event_proc pause_proc;      //!< Called on pause
    uint pause_arg2;            //!< Arg2 to pause_proc (arg1 is pause)

    uint exit_rc;               //!< Return value from "event_start"

    uchar pkt_insert;           //!< Insert point for packet queue
    uchar pkt_remove;           //!< Remove point for packet queue
    volatile uchar pkt_count;   //!< Count of items in packet queue
    uchar __PAD2;               //!< (Spare)
    ushort pkt_size;            //!< Size of packet queue (<= 256)
    ushort pkt_max;             //!< Max occupancy of packet queue
    pkt_t *pkt_queue;           //!< Pointer to packet queue

    uint vic_select;            //!< Builds VIC IRQ/FIQ select word
    uint vic_enable;            //!< Builds VIC interrupt enable word
    uint old_select;            //!< Keeps old state of vic_select
    uint old_enable;            //!< Keeps old state of vic_enable

    int_handler fiq_vector;     //!< New FIQ vector (or 0)
    int_handler old_vector;     //!< Old FIQ vector

    int_handler vic_addr[EVENT_COUNT]; //!< Record of handler for each event

    /*! Queues of events - one for each priority level */

    proc_queue_t proc_queue[PRIO_MAX + 1];
} event_data_t;

//------------------------------------------------------------------------------

//! A struct holding all of the variables maintained by SARK
typedef struct sark_data {
    uint virt_cpu;              //!< 00 Virtual CPU number
    uint phys_cpu;              //!< 04 Physical CPU number

    uint random[2];             //!< 08 Random number variable

    uint *stack_base;           //!< 10 Bottom of stack area (& heap limit)
    uint *heap_base;            //!< 14 Bottom of heap area
    heap_t *heap;               //!< 18 Heap in DTCM

    vcpu_t *vcpu;               //!< 1c Pointer to VCPU block

    mem_block_t msg_root;       //!< 20 Control block for SDP messages

    uint msg_rcvd;              //!< 28 Numbers of msgs received
    uint msg_sent;              //!< 2c Numbers of msgs sent

    ushort cpu_clk;             //!< 30 CPU clock speed (MHz)
    uchar sw_rte;               //!< 32 Set to 1 for SW error calls rt_error
    uchar __PAD1;               //!< 33
    void *sdram_buf;            //!< 34 Pointer to SDRAM buffer
    uint pkt_count;             //!< 38 Count of thrown packets
} sark_data_t;

//

#define SARK_VIRT_CPU 0  //!< For "sark_alib.s" (maintain sync with sark_data_t))
#define SARK_RANDOM   8  //!< For "sark_alib.s" (maintain sync with sark_data_t))
#define SARK_CPU_CLK  48 //!< For "sark_alib.s" (maintain sync with sark_data_t))

//------------------------------------------------------------------------------

//! \name System RAM definitions
//! \{
//! ```
//! SYSRAM_TOP  +-------------------------------+   f5008000
//!             | 256 bytes                     |
//! SV_SV       +-------------------------------+   f5007f00
//!             | 32 bytes                      |
//! SV_VECTORS  +-------------------------------+   f5007ee0
//!             | 64 bytes                      |
//! SV_RANDOM   +-------------------------------+   f5007ea0
//!             | 1440 bytes                    |
//! SV_SPARE    +-------------------------------+   f5007900
//!             | NUM_CPUS * VCPU_SIZE          |
//! SV_VCPU     +-------------------------------+   f5007000 sv->sysram_top
//!             |                               |
//!             ~ "User" SysRAM                 ~
//!             |                               |
//!             +-------------------------------+   f5000100 sv->sysram_base
//!             | 256 bytes                     |
//! SYSRAM_BASE +-------------------------------+   f5000000
//! ```

#define SV_SSIZE        32          //!< SROM data size
#define SV_USIZE        96          //!< Uninitialised size
#define SV_ISIZE        128         //!< Initialised to 0
#define SV_VSIZE        32          //!< Reset vectors
#define SV_RSIZE        64          //!< Random in SysRAM
#define SV_SIZE         0x1000      //!< Everything fits in this!

#define SV_SROM         (SYSRAM_TOP - SV_SSIZE)          //!< e5007fe0
#define SV_UBASE        (SV_SROM - SV_USIZE)             //!< e5007f80
#define SV_IBASE        (SV_UBASE - SV_ISIZE)            //!< e5007f00
#define SV_SV           SV_IBASE                         //!< e5007f00

#define SYS_BOOT        256                              //!< Boot space size
#define SYS_USER_TOP    (SYSRAM_TOP - SV_SIZE)           //!< e5007000 (sysram_top)
#define SYS_USER_BASE   (SYSRAM_BASE + SYS_BOOT)         //!< e5000100 (sysram_base)

#define SV_VECTORS      (SV_IBASE - SV_VSIZE)            //!< e5007ee0
#define SV_RANDOM       (SV_VECTORS - SV_RSIZE)          //!< e5007ea0

#define SV_VCPU         SYS_USER_TOP                     //!< e5007000
#define SV_SPARE        (SV_VCPU + NUM_CPUS * VCPU_SIZE) //!< e5007900
#define SPARE_SIZE      (SV_RANDOM - SV_SPARE)           //!< spare space size

#define SROM_FLAG_BASE  (SV_SROM)                        //!< e5007fe0
#define STATUS_MAP_BASE (SV_UBASE)                       //!< e5007f80
#define RST_BLOCK_BASE  (SV_VECTORS)                     //!< f5007ee0

// Bits in srom_data->flags

#define SRF_PRESENT     0x8000          //!< SROM present
#define SRF_HW_VER      0x00f0          //!< Hardware version
#define SRF_PHY_INIT    0x0008          //!< Init PHY on startup
#define SRF_PHY_RST     0x0004          //!< Reset PHY on startup
#define SRF_PHY_WAIT    0x0002          //!< Wait for PHY up on startup
#define SRF_ETH         0x0001          //!< Ethernet present
#define SRF_NONE        0x0000          //!< None of the above

/*!
\brief Struct holding the System Variables.

Placed at the top of System RAM at 0xe5007f00 (#SV_SV)
*/

typedef struct sv {
    ushort p2p_addr;            //!< 00 P2P address of this chip
    ushort p2p_dims;            //!< 02 P2P dimensions

    ushort dbg_addr;            //!< 04 P2P address for debug messages
    uchar  p2p_up;              //!< 06 Non-zero if P2P networking active
    uchar  last_id;             //!< 07 Last ID used in NNBC

    ushort eth_addr;            //!< 08 P2P address of nearest Ethernet node
    uchar hw_ver;               //!< 0a Hardware version
    uchar eth_up;               //!< 0b Non-zero if Ethernet active

    uchar p2pb_repeats;         //!< 0c Number of times to send out P2PB packets
    uchar p2p_sql;              //!< 0d P2P sequence length (**)
    uchar clk_div;              //!< 0e Clock divisors for system & router bus
    uchar tp_scale;             //!< 0f Scale for router phase timer

    volatile uint64 clock_ms;   //!< 10 Milliseconds since boot
    volatile ushort time_ms;    //!< 18 Milliseconds in second (0..999)
    ushort ltpc_period;         //!< 1a Local Time Phase Control message interval (*10 ms)

    volatile uint unix_time;    //!< 1c Seconds since 1970
    uint tp_timer;              //!< 20 Router time phase timer

    ushort cpu_clk;             //!< 24 CPU clock in MHz
    ushort mem_clk;             //!< 26 SDRAM clock in MHz

    uchar forward;              //!< 28 NNBC forward parameter
    uchar retry;                //!< 29 NNBC retry parameter
    uchar peek_time;            //!< 2a Timeout for link read/write (us)
    uchar led_period;           //!< 2b LED flash period (* 10 ms)

    uchar netinit_bc_wait;      //!< 2c Minimum time after last BC during netinit (*10 ms)
    uchar netinit_phase;        //!< 2d Phase of boot process
    ushort p2p_root;            //!< 2e The P2P address from which the system was booted

    uint led0;                  //!< 30 LED definition words (for up
    uint led1;                  //!< 34 to 15 LEDs)
    int clock_drift;            //!< 38 drift of clock from boot chip clock in ticks / us
                                //      NOTE: This is a fixed point number!!!
    uint random;                //!< 3c Random number seed

    uchar root_chip;            //!< 40 Set if we are the root chip
    uchar num_buf;              //!< 41 Number of SHM buffers
    uchar boot_delay;           //!< 42 Delay between boot NN pkts (us)
    uchar soft_wdog;            //!< 43 Soft watchdog control

    uint sync_alignment;        //!< 44 delay for sync0/1 alignment (us)

    heap_t *sysram_heap;        //!< 48 Heap in SysRAM
    heap_t *sdram_heap;         //!< 4c Heap in SDRAM

    uint iobuf_size;            //!< 50 Size of IO buffers (bytes)
    uint *sdram_bufs;           //!< 54 SDRAM buffers
    uint sysbuf_size;           //!< 58 Size of system buffers (words)
    uint boot_sig;              //!< 5c Boot signature word

    uint mem_ptr;               //!< 60 Memory pointer for NNBC memory setter

    volatile uchar lock;        //!< 64 Lock variable
    uchar link_en;              //!< 65 Bit map of enabled links
    uchar last_biff_id;         //!< 66 Last ID used in BIFF
    uchar bt_flags;             //!< 67 Board Test flags

    mem_block_t shm_root;       //!< 68 Control block for SHM bufs

    uint utmp0;                 //!< 70 Four temps...
    uint utmp1;                 //!< 74
    uint utmp2;                 //!< 78
    uint utmp3;                 //!< 7c

    uchar status_map[20];       //!< 80 Set during SC&MP ROM boot
    uchar p2v_map[20];          //!< 94 Phys to Virt core map
    uchar v2p_map[20];          //!< a8 Virt to Phys core map

    uchar num_cpus;             //!< bc Number of good cores
    uchar rom_cpus;             //!< bd SC&MP ROM good cores
    ushort board_addr;          //!< be Position of chip on PCB

    uint *sdram_base;           //!< c0 Base of user SDRAM
    uint *sysram_base;          //!< c4 Base of user SysRAM
    uint *sdram_sys;            //!< c8 System SDRAM
    vcpu_t *vcpu_base;          //!< cc Start of VCPU blocks

    heap_t *sys_heap;           //!< d0 System heap in SDRAM
    rtr_entry_t *rtr_copy;      //!< d4 Copy of router MC tables
    uint *hop_table;            //!< d8 P2P hop table
    block_t** alloc_tag;        //!< dc Start of alloc_tag table
    ushort rtr_free;            //!< e0 Start of free router entry list
    ushort p2p_active;          //!< e2 Count of active P2P addresses
    app_data_t *app_data;       //!< e4 Array of app_id structs
    sdp_msg_t *shm_buf;         //!< e8 SHM buffers
    uint mbox_flags;            //!< ec AP->MP communication flags

    uchar ip_addr[4];           //!< f0 IP address (or 0)
    uint fr_copy;               //!< f4 (Virtual) copy of router FR reg
    uint *board_info;           //!< f8 Pointer to board_info area !!
    uint sw_ver;                //!< fc Software version number
} sv_t;


// Pointers to useful bits of system RAM

/*!
"sv" points to SV struct at top of sys RAM
*/

static sv_t*   const sv         = (sv_t *)   SV_SV;

//! "sv_vcpu" points to base of array of "vcpu_t"

static vcpu_t* const sv_vcpu    = (vcpu_t *) SV_VCPU;

//! SROM
static uint*   const sv_srom    = (uint *)   SV_SROM;
//! RANDOM
static uint*   const sv_random  = (uint *)   SV_RANDOM;
//! VECTORS
static uint*   const sv_vectors = (uint *)   SV_VECTORS;

//! Board information
static uint*   const sv_board_info = (uint *)   SV_SPARE;
//! \}

//------------------------------------------------------------------------------

// Various bits of global data

//! Main SARK variables
extern sark_data_t sark;

//! SARK event variables
extern event_data_t event;

//! Date of build
extern uint build_date;

//! Name of build
extern char build_name[];

//------------------------------------------------------------------------------

// Routines exported by SARK - sark_alib.s

/*!
\brief Enable the IRQ interrupt in the core by clearing the appropriate bit
in the CPSR.

Returns the previous CPSR so that the state prior to the
call can be restored.

(Unlikely to be needed by application code)
\return previous state of CPSR
*/

uint cpu_irq_enable (void);

/*!
\brief Disable the IRQ interrupt in the core by setting the appropriate bit
in the CPSR.

Returns the previous CPSR so that the state prior to the
call can be restored.
\return previous state of CPSR
*/

uint cpu_irq_disable (void);

/*!
\brief Enable the FIQ interrupt in the core by clearing the appropriate bit
in the CPSR.

Returns the previous CPSR so that the state prior to the
call can be restored.

(Unlikely to be needed by application code).
\return previous state of CPSR
*/

uint cpu_fiq_enable (void);

/*!
\brief Disable the FIQ interrupt in the core by setting the appropriate bit
in the CPSR.

Returns the previous CPSR so that the state prior to the
call can be restored.
\return previous state of CPSR
*/

uint cpu_fiq_disable (void);

/*!
\brief Enable the FIQ and IRQ interrupts in the core by clearing the
appropriate bits in the CPSR.

Returns the previous CPSR so that the state prior to the call can be restored.

(Unlikely to be needed by application code).

\return previous state of CPSR
*/

uint cpu_int_enable (void);

/*!
\brief Disable the FIQ and IRQ interrupts in the core by setting the
appropriate bits in the CPSR.

Returns the previous CPSR so that the
state prior to the call can be restored.

\return previous state of CPSR
*/

uint cpu_int_disable(void);

/*!
\brief Restore the CPSR to the state provided in the argument.

Generally used after a call to cpu_fiq_disable(), cpu_irq_disable(), or
cpu_int_disable()
\param cpsr: value to be restored to CPSR
*/

void cpu_int_restore (uint cpsr);

/*!
\brief Returns the current value of the core's stack pointer.

(Unlikely to be needed by application code).

\return current SP value
*/

uint *cpu_get_sp (void);

/*!
\brief Returns the current value of the core's CPSR.

(Unlikely to be needed by application code).

\return current CPSR value
*/

uint cpu_get_cpsr (void);

/*!
\brief Set the core's CPSR to the supplied value.

This routine always returns to the caller even if the mode changes as a
result of the new CPSR value.

(Unlikely to be needed by application code)
\param cpsr: new value of the CPSR
*/

void cpu_set_cpsr (uint cpsr);

/*!
\brief Returns the current value of the core's CP15 Control register.

(Unlikely to be needed by application code)
\return current CP15 control register value
*/

uint cpu_get_cp15_cr (void);

/*!
\brief Set the core's CP15 Control Register to the supplied value.

(Unlikely to be needed by application code)
\param[in] value: new value of the CP15 control register
*/

void cpu_set_cp15_cr(uint value);

/*!
\brief Puts the core into sleep mode until an interrupt occurs at which point
the function will return.

Works regardless of whether interrupts are masked in the core or not but
relies on the VIC passing the interrupt to the core so won't work if all
interrupts are masked by the VIC.
*/

void cpu_wfi (void);

/*!
\brief Puts the core into sleep mode until an interrupt occurs at which point
the interrupt will be serviced and the core will go back to sleep.

This means that the core will respond to interrupts but the function
never returns.
*/

NORETURN void
cpu_sleep(void);

/*!
\brief Puts the core into sleep mode and disable all interrupts in the VIC.

This means that the core sleeps and cannot be woken up. Usually used
when a fatal error has occurred. Application code will probably want
to call rt_error() on a fatal error.
*/

NORETURN void
cpu_shutdown(void);

/*!
\brief Called to signal a fatal error.

The error code argument is placed in a known place in the VCPU block for this
core as is a dump of r0-r7, lr, sp and cpsr. Optional arguments will end up
in the register dump as r1, r2, etc.  Calls cpu_shutdown() when all relevant
state has been dumped.

\param[in] code: an error code (usually from enum rte_code_e)
\param ... further (integer) arguments
*/

NORETURN void
rt_error(uint code, ...);

/*!
\brief Copy \p n bytes of memory from \p src to \p dest.

The memory is copied
byte by byte and so will be inefficient for large values of \p n.
Not intended for use where source and destination blocks overlap.

\param[in] dest: destination buffer address
\param[in] src: source buffer address
\param[in] n: number of bytes to copy
*/

NONNULL void
sark_mem_cpy(void *dest, const void *src, uint n);

/*!
\brief Copies a NULL terminated string from \p src to \p dest.

The string is copied byte by byte and so will be inefficient for long
strings. Not intended for use where source and destination strings overlap.

\param[in] dest: destination string address; must be enough space available
\param[in] src: source string address
*/

NONNULL void
sark_str_cpy(char *dest, const char *src);

/*!
\brief Counts the number of characters in a zero terminated string.

The terminator is not included in the count.

\param string: zero terminated string
\return number of characters
*/

NONNULL uint
sark_str_len (char *string);

/*!
\brief Sets the CPU state field in the VCPU block for this core.

\param state: the state to set
*/

void sark_cpu_state (cpu_state state);

/*!
\brief Copies an SDP message from one buffer to another.

Quite efficient for long messages as it uses sark_word_cpy()

\param to: pointer to destination buffer
\param from: pointer to source buffer
*/

NONNULL void sark_msg_cpy(sdp_msg_t *to, sdp_msg_t *from);

/*!
\brief A fast copy for memory buffers.

The byte count "n" will be rounded up
to be a multiple of word size before the copy takes place and a whole
number of words will be transferred.  The inner loop copies 8 words at
a time for efficiency. The "src" and "dest" pointers must be word
aligned. Not intended for use where source and destination blocks
overlap.

\param[out] dest: destination buffer address
\param[in] src: source buffer address
\param[in] n: number of bytes to copy (routine rounds up to multiple of 4)
*/

NONNULL void
sark_word_cpy(void *dest, const void *src, uint n);

/*!
\brief A fast memory setter for a block of memory.

The byte count "n" must be a multiple of the word size and the destination
pointer must be word aligned. The inner loop fills 4 words at a time for
efficiency.

\param[out] dest: destination buffer address
\param[in] data: word to be used for filling
\param[in] n: number of bytes to fill (must be multiple of 4)
*/

NONNULL void
sark_word_set(void *dest, uint data, uint n);

/*!
\brief Acquires one of the 32 built-in hardware locks provided by the chip.

Disables all interrupts and busy-waits for the lock to become free so
must be used carefully. Returns the value of the CPSR at entry so that
the interrupt state can be restored by the matching "sark_lock_free"
routine. Locks already defined for use by SARK are listed in the enum
"sark_lock_e";

\param lock: hardware lock number
\return CPSR at time of call
*/

uint sark_lock_get (spin_lock lock);

/*!
\brief Free a previously acquired hardware lock.

The supplied "cpsr" is used to restore the interrupt state that was in place
when the lock was acquired.

\param cpsr: CPSR to be reinstated after unlocking
\param lock: hardware lock number
*/

void sark_lock_free (uint cpsr, spin_lock lock);

/*!
\brief Raise (increment) an 8-bit semaphore variable which is accessed via a
pointer.

A hardware lock is used to gain exclusive access to the variable.
Returns the post-increment value. No check is made for overflow from 255.

\param sema: pointer to semaphore byte (usually in shared memory)
\return post-increment value of semaphore
*/

NONNULL uint sark_sema_raise(uchar *sema);

/*!
\brief Lower (decrement) an 8-bit semaphore variable which is accessed via a
pointer.

The decrement will only take place on a non-zero variable. A
hardware lock is used to gain exclusive access to the variable.
Returns the pre-decrement value.

\param sema: pointer to semaphore byte (usually in shared memory)
\return pre-decrement value of semaphore
*/

NONNULL uint sark_sema_lower(uchar *sema);

/*!
\brief Raise a semaphore associated with the AppID running on this core.

The semaphore is initialised to zero before the application starts.
Returns the new value of the semaphore and can be used to determine
when all cores running a given application have reached a given point.
Also allows a single core to be differentiated from other cores by
testing for value 1 returned from this function. Uses sark_sema_raise.

\return value of semaphore after raising
*/
uint sark_app_raise(void);

/*!
\brief Lowers a semaphore associated with the AppID running on this core.

If all cores running an application raise the semaphore when they start
and lower it when they complete this can be used to determine when all
cores have started (semaphore equals number of cores) and when all
cores have finished (semaphore equals zero). Uses sark_sema_lower.

\return value of semaphore before lowering.
*/
uint sark_app_lower(void);

/*!
\brief Used to get the value of the semaphore associated with the AppID
running on this core.

\return current value of semaphore.
*/

uint sark_app_sema (void);

/*!
\brief Used to get the number of cores on this chip which are running
the current application.

\return number of cores with same AppID
*/

uint sark_app_cores (void);

/*!
\brief Returns the core ID of the lowest numbered core on this chip
which is running the current application.

Can be used to identify a single core to do shared initialisation.

\return lowest numbered core running this app.
*/

uint sark_app_lead (void);

/*!
\brief Seed the random number generator with the supplied value.

The 33rd bit of the random value is set to zero.

\param seed: 32-bit value to seed generator
*/

void sark_srand (uint seed);

/*!
\brief Return the next 32-bit pseudo-random number in the sequence.

A 33 bit number is maintained internally and the bottom 32 bits returned by
this function.

\return next random number in sequence
*/

uint sark_rand (void);

/*!
\brief Divide the argument by 10 and return dividend and modulus in a
struct of type divmod_t which has fields "div" and "mod".

\param n: number to divide by 10
\result a divmod_t containing dividend and modulus
*/

divmod_t sark_div10 (uint n);

/*!
\brief Count the number of bits set in a word

\param word: whose bits are to be counted
\result number of bits set
*/

uint sark_count_bits(uint word);

/*!
\brief Performs an ARM BX instruction to the address in \p addr.

Can be used to branch to arbitrary pieces of code.

**Dangerous** and unlikely to be useful to most applications!

\param addr: address to branch to (bottom bit to be set for Thumb)
*/

void sark_bx (uint addr);

/*!
\brief Used to unpack an APLX table whose address is provided.

Where this results in the loading and execution of a new application, the
second argument is the AppID.

(Unlikely to be needed by application code)

\param table: pointer to an APLX table
\param app_id: AppID to be passed to new application
*/

void sark_aplx (uint *table, uint app_id);

/*!
\brief Sets up stack for a given CPU mode.

Changes to the given mode, sets
stack pointer then returns to original mode. Also computes base of
stack which will probably be the top of an adjacent stack.

(Unlikely to be needed by application code).

\param stack: top of stack for given mode
\param mode: mode for which stack applies
\param size: size of stack in words
\return base of stack (ie stack - size)
*/

uint *cpu_init_mode (uint *stack, uint mode, uint size);

/*!
Compute 4-bit ones-complement checksum of a 64-bit quantity.
(Unlikely to be needed by application code).

\param a: low 32 bits of value to be summed
\param b: high 32 bits of value to be summed
\result 4-bit checksum in bits 31:28 - other bits zero
*/

uint chksum_64 (uint a, uint b);

/*!
\brief Compute 4-bit ones-complement checksum of a 32-bit quantity.

(Unlikely to be needed by application code).

\param a: value to be summed
\result 4-bit checksum in bits 31:28 - other bits zero
*/

uint chksum_32(uint a);

//------------------------------------------------------------------------------

// Routines exported by SARK - sark_base.c

/*!
\brief Returns the ID of this chip as a 16-bit number.

The upper 8 bits are the X coordinate of the chip and the lower 8 bits are
the Y coordinate. Just a wrapper to return sv->p2p_addr.
\return chip ID ((X << 8) + Y)
*/

uint sark_chip_id (void);

/*!
\brief Returns the virtual core number of this core.

This is in the range 0 to 17 (though 0 will only be returned on the core
which hosts the monitor processor). Just a wrapper to return sark.virt_cpu.
\return core number in the range 0..17
*/

uint sark_core_id(void);

/*!
\brief Returns the ID of the application running on this core.

Just a wrapper to return sark_vec->app_id.

\return application ID in the range 0..255
*/

uint sark_app_id(void);

/*!
\brief Returns a pointer to the name of the application running on this core.

Implemented as an inline.

\return pointer to application name
*/

static inline char *sark_app_name(void)
{
    return build_name;
}

/*!
\brief Get an SDP message buffer from the pool maintained by SARK.

Applications typically have 4 buffers in the pool though this can be changed
by setting sark_vec->num_msgs in sark_config. If there are no free buffers
available, NULL is returned.

\return pointer to SDP buffer (or NULL)
*/

sdp_msg_t *sark_msg_get (void);

/*!
\brief Return an SDP message buffer which was acquired by sark_msg_get() to
the free buffer pool.

\param msg: pointer to SDP message
*/

void sark_msg_free (sdp_msg_t *msg);

/*!
\brief Send an SDP message.

The message is sent to the monitor processor
using a shared memory buffer and then sent to the appropriate place
from there. A timeout in milliseconds must be supplied which applies
to the transfer of the message to the monitor processor. Normally this
will take less than 1ms if the monitor processor is functioning
normally. A timeout of 10ms is probably appropriate in most
circumstances. The call can fail either because of a timeout or if
there are no shared memory SDP buffers available.

\param msg: pointer to an SDP message buffer
\param timeout: time in ms before sending is abandoned
\return 1 if successful, 0 for failure
*/

NONNULL uint
sark_msg_send(sdp_msg_t *msg, uint timeout);

/*!
\brief Perform a busy-wait for the given number of microseconds.

The core will continue to service interrupts while the delay takes place but
this is otherwise a wasteful way to delay for long periods of time.

\param delay: number of microseconds to delay
*/

void sark_delay_us (uint delay);

/*!
\brief Generates a software error, logging the filename and line number at
which the error occurs.

Usually used via the #sw_error macro which
inserts the filename and line number automatically. The mode parameter
determines whether or not rt_error should be called to shut down the
core. mode can be SW_RTE to always call rt_error(), SW_NEVER to always
return or SW_OPT which consults sark.sw_rte to choose what to do.

\param mode: determines if rt_error() is called
\param file: file name
\param line: line number
*/

void sw_error_fl (sw_err_mode mode, char *file, uint line);

/*!
\brief sw_error calls sw_error_fl(), inserting file name and line
number automatically

\param mode: determines if rt_error() is called
*/

# define sw_error(mode) sw_error_fl (mode, __FILE__, __LINE__)

/*!
\brief SCP handler for the Version command.

Copies various pieces of
information into the supplied SDP message buffer. The information
copied is the chip address (16 bits), virtual and physical core
numbers (8 bits each), the version number of SARK (16 bits), the size
of SDP message data buffers (16 bits) and the Unix time at which the
application was built (32 bits). A text string is also returned which
gives the base kernel name (eg SARK) and the system on which it is
running (eg SpiNNaker).

(Unlikely to be called by application code).

\param msg: SDP message buffer
\return size of returned message
*/

uint sark_cmd_ver (sdp_msg_t *msg);

/*!
\brief SCP handler for the Read command which reads memory and returns the
data read to the host.

Various argument errors are trapped but if
successful, data is transferred from memory into the SDP message
starting at msg->arg1. If half or word alignment is specified, the
address and count must be 2 or 4 byte aligned.

(Unlikely to be called by application code).

\param msg: The message:
 * `msg->arg1`: first address to be read (suitably aligned)
 * `msg->arg2`: number of bytes to read (suitably aligned)
 * `msg->arg3`: mode of reading (TYPE_BYTE, TYPE_HALF or TYPE_WORD)

\return size of returned message
*/

uint sark_cmd_read(sdp_msg_t *msg);

/*!
\brief SCP handler for the Write command which writes memory with data
    supplied by the host.

Various argument errors are trapped but if
successful, data is transferred to memory from the SDP message
starting at msg->data. If half or word alignment is specified, the
address and count must be 2 or 4 byte aligned.

(Unlikely to be called by application code).

\param msg: The message:
 * `msg->arg1`: first address to be written (suitably aligned)
 * `msg->arg2`: number of bytes to write (suitably aligned)
 * `msg->arg3`: mode of writing (TYPE_BYTE, TYPE_HALF or TYPE_WORD)
 * `msg->data`: data to be written

\return size of returned message (usually 0)
*/

uint sark_cmd_write (sdp_msg_t *msg);

/*!
\brief SCP handler for the Fill command which fills memory with a data word.

(Unlikely to be called by application code).

\param msg: The message
 * `msg->arg1`: first address to be written (must be word aligned)
 * `msg->arg2`: data word
 * `msg->arg3`: byte count (must be non-zero and a multiple of 4)

\return size of returned message (usually 0)
*/

uint sark_cmd_fill(sdp_msg_t *msg);

/*!
\brief Initialise a memory buffer into a linked list of "count" blocks of
"size" bytes.

\p size needs to be at least 4 and a multiple of 4 and
the memory buffer needs to be at least `size * count" bytes`. The first
word of each block is a pointer to the next block. Returns a pointer
to the last block. The buffer can be used to initialise a "root"
struct of type mem_block_t.

\param buf: base address of memory buffer (word aligned)
\param size: of each block (at least 4 and a multiple of 4)
\param count: number of blocks
\return pointer to last block
*/

void *sark_block_init(void *buf, uint size, uint count);

/*!
\brief Generic call to get a buffer from a pool whose "root" is supplied.

Returns pointer to buffer on success, NULL on failure.

\param root: pointer to a block pool held in a mem_block_t
\return pointer to a free block if available or NULL if not
*/

void *sark_block_get (mem_block_t *root);

/*!
\brief Generic call to free a buffer into a pool whose "root" is supplied.

The block should have been allocated from the same pool previously!

\param root: pointer to a block pool held in a mem_block_t
\param blk: pointer to the block to be freed.
*/

void sark_block_free(mem_block_t *root, void *blk);

/*!
\brief Get a free SDP message from the shared SysRAM pool.

Returns pointer to message on success, NULL on failure. Because several cores
may attempt to access a shared memory message concurrently, a hardware lock
is used to ensure exclusive access. Interrupts are turned off while this
occurs but this should be for a relatively short time (1-5us ??)

\return pointer to a shared memory SDP message or NULL if none available
*/

sdp_msg_t *sark_shmsg_get(void);

/*!
\brief Return a shared memory SDP message to the shared SysRAM pool.

Because several cores may attempt to access a shared memory message
concurrently, a hardware lock is used to ensure exclusive
access. Interrupts are turned off while this occurs but this should be
for a relatively short time (1-5us ??)

\param msg: pointer to the message
*/

NONNULL void
sark_shmsg_free(sdp_msg_t *msg);

/*!
\brief Calls the constructors for any C++ objects created at global scope.

Should generally be called from any user-supplied sark_pre_main() function
*/

void sark_call_cpp_constructors(void);


//------------------------------------------------------------------------------

// Routines exported by SARK - sark_io.c

//! \brief A simple "printf" routine.
//!
//! The first argument is a 'stream' which is
//! either a constant (#IO_BUF, #IO_STD, #IO_DBG, #IO_NULL) or a pointer to a
//! character array which will be filled in as in "sprintf". No checks for
//! buffer overflow are made for this latter case! The range of formats is
//! very limited to minimise code size. The maximum width for integer
//! fields is 16.The maximum width for the 16.16 fixed point format is 25.
//! Don't exceed these maxima!
//!
//! The four constants provide 4 possible output streams for the output
//! string. IO_STD will send the string as an SDP packet back to the
//! host. IO_BUF will place the string in a buffer in SDRAM while IO_NULL
//! will ignore the string. IO_DBG is not currently implemented.
//!
//! Formats are:
//! - \c \%c - character
//! - \c \%s - string - can be eg \c \%8s for right justification
//! - \c \%d - signed integer (also <tt>\%8d</tt>, <tt>\%06d</tt>, etc)
//! - \c \%u - signed integer (also <tt>\%4u</tt>, <tt>\%06u</tt>, etc)
//! - \c \%x - integer in hex (also <tt>\%4x</tt>, <tt>\%08x</tt>, etc)
//! - \c \%z - integer in hex, exactly _N_ digits
//! - \c \%n.mf - 16.16 fixed point with width _n_, precision _m_
//!
//! \param stream: constant (#IO_BUF, #IO_STD, #IO_DBG, #IO_NULL) or pointer
//!     to char array
//! \param format: a format string
//! \param ... arguments to be formatted
NONNULL2 void
io_printf(char *stream, char *format, ...);

//! \brief Routine to put a character to an output stream.
//!
//! Has different behaviour for four possible streams:
//!
//! * #IO_STD: Streamed out to a `tubotron` instance via SDP.
//! * #IO_BUF: Stored in a buffer in memory for later retrieval by host
//! * #IO_NULL: Ignores the write
//! * "sprintf" strings: writes into the provided buffer.
//!
//! For #IO_STD and #IO_BUF the character is placed in a holding buffer which
//! is flushed on buffer-full or newline or NULL.
//!
//! \param stream: constant (IO_BUF, IO_STD, IO_NULL) or pointer
//!     to char array
//! \param c: a character
void io_put_char(char *stream, uint c);

/*! \brief Routine to reset the IOBUF for the core.

Frees additional allocated IOBUF blocks in the system heap.
 */

void sark_io_buf_reset (void);

//------------------------------------------------------------------------------

// Routines exported by SARK - sark_alloc.c

/*!
\brief Allocate a block of memory of \p count &times; \p size bytes from the
DTCM heap.
Returns either a pointer to the block on success, or NULL on failure.

The total amount of memory available for allocation is around 62KB
less all of the static variables used by the application.

\param[in] count: number of sub-blocks to allocate
\param[in] size: size of each sub-block
\return pointer to block or NULL
*/

#ifndef DOXYGEN
SARK_IS_A_CALLOC(1, 2)
#endif
void *
sark_alloc(uint count, uint size);

/*!
\brief Frees a block of memory which was previously allocated by sark_alloc()
(i.e., from the DTCM heap).

\param[in] ptr: pointer to memory block
*/

NONNULL void
sark_free(void *ptr);

/*!
\brief Allocate a memory block from the specified heap.

The block will be
tagged with the supplied "tag" which must be <= 255. Returns NULL on
failure (not enough memory, bad tag or tag in use).

The flag parameter contains two flags in the bottom two bits. If bit 0
is set (#ALLOC_LOCK), the heap manipulation is done behind a lock with
interrupts disabled. If bit 1 is set (#ALLOC_ID), the block is tagged
with the AppID provided in bits 15:8 of the flag, otherwise the AppID
of the current application is used.

The 8-bit \p tag is combined with the AppID and stored in the "free"
field while the block is allocated. If the \p tag is non-zero, the
block is only allocated if the tag is not in use.  The tag (and AppID)
is stored in the "alloc_tag" table while the block is allocated.

If this command is used to allocate for an app-id which isn't the
caller's, the user should set `sv->app_data[app_id].clean` to 0 for the
application concerned.  This ensures that the memory will be cleaned
up by a stop signal.

\param[in] heap: the heap from which the block should be allocated
\param[in] size: the size of the block in bytes
\param[in] tag: 8-bit tag
\param[in] flag: flags to control locking and AppID
\return pointer to allocated block or NULL on failure
*/

SARK_IS_A_MALLOC(2) void *
sark_xalloc(heap_t *heap, uint size, uint tag, uint flag);

/*!
\brief Free a memory block in the specified heap.

If the lock bit in the \p flag parameter is set, the manipulation of the heap
is done behind a lock. This will turn interrupts off for some time while the
heap is searched. If a NULL pointer (\p ptr) is passed in, a runtime error will
occur. If there is an "alloc_tag" entry associated with the block it
is set to NULL.

\param[in] heap: the heap to which the free block should be returned
\param[in] ptr: a pointer to the memory block to be freed
\param[in] flag: bit 0 set if the heap should be locked during free
*/

NONNULL2 void
sark_xfree(heap_t *heap, void *ptr, uint flag);

/*!
\brief Free all allocated blocks in the specified heap which are tagged with
the given "app_id".

The \p flag parameter specifies if locking is needed in
the sark_xfree() call. Returns the number of blocks freed.

\param[in] heap: the heap from which blocks should be freed
\param[in] app_id: the AppID whose block should be freed
\param[in] flag: controls locking of heap transactions
\return number of blocks freed
*/

uint sark_xfree_id(heap_t *heap, uint app_id, uint flag);

/*!
\brief Search the supplied heap and return the size of the largest free
block (in bytes).

The \p flag parameter allows the heap to be locked while the search takes
place.

\param[in] heap: the heap to be searched
\param[in] flag: controls locking of heap transactions
\return size of largest block
*/

uint sark_heap_max(heap_t *heap, uint flag);

/*!
\brief Initialise an area of memory as a heap.

Arguments are (uint) pointers to base and top of the area. Returns a pointer
to the heap (same address as the base). Assumes the area is large enough to
hold a minimal heap (a zero size heap needs 28 bytes).

\param[in] base: pointer to bottom of heap memory
\param[in] top: pointer to top of heap memory
\return pointer to heap
*/

heap_t *sark_heap_init(uint *base, uint *top);

/*!
\brief Allocate a block of entries in the router multicast table (and
    associate the caller's app_id with this block).
\param[in] size: number of entries to allocate
\return index of first entry in block on success, zero on failure.
*/

uint rtr_alloc(uint size);

/*!
\brief Allocate a block of entries in the router multicast table (and
associate the supplied app_id with this block).

If this command is used to allocate for an app-id which isn't the
caller's, the user should set sv->app_data[app_id].clean to 0 for the
application concerned.  This ensures that the memory will be cleaned
up by a stop signal.

\param[in] size: number of entries to allocate
\param[in] app_id: AppID associated with block
\return index of first entry in block on success, zero on failure.
*/

uint rtr_alloc_id(uint size, uint app_id);

/*!
\brief Free a block of MC table entries which starts with the supplied
entry. The relevant router registers can be optionally re-initialised.

\param[in] entry: first entry in the block (as returned by rtr_alloc)
\param[in] clear: non-zero to cause router registers to be re-initialised
*/

void rtr_free(uint entry, uint clear);

/*!
\brief Free all allocated blocks in the router which are tagged with the
given "app_id".

The relevant router registers can be optionally re-initialised.
Returns number of blocks freed.

\param[in] app_id: AppID whose entries are to be freed
\param[in] clear: non-zero to cause router registers to be re-initialised
\return number of blocks freed
*/

uint rtr_free_id(uint app_id, uint clear);

/*!
\brief Return the size of the largest free block in the router multicast
table (or zero if table is full).

\return number of entries in largest block
*/

uint rtr_alloc_max(void);

/*!
\brief Get a pointer to a tagged allocation.
\param[in] tag: The tag of the allocation to get a pointer to
\param[in] app_id: AppID whose tagged allocation to read (or 0 to use the
    current core's app ID)
\return A pointer to the first allocation found with the given tag, or NULL
    if no such allocation exists.
*/

void *sark_tag_ptr(uint tag, uint app_id);

//------------------------------------------------------------------------------

// Routines exported by SARK - sark_hw.c

/*!
\brief Performs a full software initialisation of the VIC.

All interrupts are disabled and all vector entries are initialised to a
disabled state. The default handler address is set to point to a return from
interrupt instruction.
*/

void sark_vic_init (void);

/*!
\brief Initialises one of the VIC slots to set up an interrupt handler.

No check is made that the slot is not already in use. The slot can be
SLOT_FIQ in which case the FIQ interrupt is set up.

\param slot: the slot to be used
\param interrupt: the interrupt number (0..31)
\param enable: non-zero if the interrupt should be enabled
\param handler: pointer to the handler code
*/

NONNULL void
sark_vic_set(vic_slot slot, uint interrupt, uint enable, int_handler handler);

/*!
\brief Initialises the hardware (GPIO port) which drives the LEDs attached to
the SpiNNaker chip.

(Unlikely to be used by application code)
*/

void sark_led_init (void);

/*!
\brief Controls one or more of the LEDs attached to the SpiNNaker chip.

The argument should be a sum of calls to macros LED_ON, LED_OFF, LED_INV
which turn on, turn off or flip a LED. The argument to each macro is the
LED number, an integer starting at zero. All SpiNNaker boards to date
have at least one LED so LED_ON(0) will always work!

To turn on LED 1 and turn off LED 2 the call looks like this...
\code
  sark_led_set (LED_ON(1) + LED_OFF(2));
\endcode

\param leds: encoded value specifying LEDs and operations thereon
*/

void sark_led_set (uint leds);

/*!
\brief Initialises a set of the router's multicast (MC) table registers so
that routing is disabled.

Because some of the MC hardware registers
are not readable, copies of these registers are kept in memory to
allow them to be read back. These copies are also initialised here.
The entries in the router are disabled by setting route and mask
to zero and the key to all ones.

\param start first entry to be cleared
\param count number of entries to be cleared
\return 1 if OK, 0 on failure (last entry out of range)
*/

uint rtr_mc_clear (uint start, uint count);

/*!
\brief Initialises the router's multicast (MC) tables and sets up the data
structures for allocating router entries.

Because some of the MC
hardware registers are not readable, copies of these registers are
kept in memory to allow them to be read back. These copies are also
initialised here. All entries in the router are disabled (route and
mask set to zero and key set to all ones). The start parameter allows
some low entries to be left unchanged.

\param start first entry to clear - usually 0 or 1
*/

void rtr_mc_init (uint start);

/*!
\brief Load router MC table from a table in memory.

Within the table, which
is an array of "rtr_entry_t". the "next" field holds the entry number
to be loaded. If the count parameter is zero, the count is taken from
the "free" field of the first entry entry in the table. The offset
parameter is added to the entry number to address each router table
entry. The "app_id" parameter sets the app_id in the router table
copy if non-zero. Otherwise it will be taken from SARK.

\param[in] e: pointer to first entry in table
\param[in] count: size of table (or 0 to use count from table)
\param[in] offset: added to entry numbers from table
\param[in] app_id: sets app_id in table if non-zero
\return 1 if successful, 0 on failure (count or entry out of range)
*/

NONNULL uint
rtr_mc_load(rtr_entry_t *e, uint count, uint offset, uint app_id);

/*!
\brief Sets a given entry in the router MC table.

The supplied route is based
on virtual core numbers and this is mapped into physical core numbers
before being written to the router. Copies of all three router
registers are also maintained in memory so that they can be read back
even though the router hardware doesn't support reading of key and
mask registers.

\param[in] entry: the router entry number (should be in range 0..1023)
\param[in] key: the key field
\param[in] mask: the mask field
\param[in] route: the route field
\return 1 on success, 0 on failure (entry out of range)
*/

uint rtr_mc_set (uint entry, uint key, uint mask, uint route);

/*!
\brief Gets a given entry in the router MC table.

The parameter \p r is a
pointer to a rtr_entry_t structure. The router register fields in the
struct are filled in from the RAM copy of the router registers which
is maintained by the system.

\param[in] entry: number of entry
\param[out] r: pointer to rtr_entry_t
\return 1 on success, 0 on failure (entry out of range)
*/

NONNULL uint rtr_mc_get(uint entry, rtr_entry_t *r);

/*!
\brief Sets the fixed-route register in the router.

Only the lower 24 bits
are used as the register has bits with another function in its top 8
bits.

\param route: fixed-route mask (bottom 24 bits)
*/

void rtr_fr_set (uint route);

/*!
\brief Gets the fixed-route register from the router and masks off the
top 8 bits.

\return Value of router FR register (bottom 24 bits only)
*/

uint rtr_fr_get(void);

/*!
\brief Initialise the router point-to-point (P2P) table.

All 64K entries are set to 6. (Unlikely to be used by application code).
*/

void rtr_p2p_init (void);

/*!
\brief Sets a P2P table entry.

The value should be in the range 0 to 7.
Values from 0 to 5 cause the packet to be routed to one of the
external links while 7 causes the packet to be sent to the monitor
processor. The value 6 causes the packet to be dropped. (Unlikely to
be used by application code).

\param[in] entry: the entry number in the range 0..65535
\param[in] value: the value to be written in the range 0..7
*/

void rtr_p2p_set(uint entry, uint value);

/*!
\brief Gets a P2P table entry.

\param entry: the entry number in the range 0..65535
\return table entry (range 0 to 7)
*/

uint rtr_p2p_get(uint entry);

/*!
\brief Initialises the 16 diagnostic registers in the router using the
supplied table (which should have 16 entries).

The count registers
are first disabled and cleared, the new register values set and and then
the counters are enabled. (Unlikely to be used by application code).

\param table pointer to table of 16 initialisation words
*/

NONNULL void
rtr_diag_init(const uint *table);

/*!
\brief Performs a full initialisation of the router.

Calls rtr_mc_init(),
rtr_p2p_init() and rtr_diag_init() and also sets the router control
register. The control register Wait1 field is set to 0x40 and the
Wait2 field is set to 0x00. The initialisation of the diagnostic
registers places 'useful' values in 12 of the registers and leaves 4
(numbers 12 to 15) free for applications. (Unlikely to be used by
application code).

\param monitor: physical core ID of monitor processor
*/

void rtr_init (uint monitor);

/*!
\brief Given a bit mask of virtual core IDs, returns a mask of the
corresponding physical core IDs.

(Unlikely to be used by
application code).

\param virt_mask: virtual core bit mask
\return physical core bit mask
*/

uint v2p_mask (uint virt_mask);

/*!
\brief Initialises the PL340 memory controller.

(_Extremely_ unlikely to be used by application code.)

\param[in] mem_clk: memory clock speed in MHz
\return 0 on failure, 1 on success
*/

uint pl340_init (uint mem_clk);

//------------------------------------------------------------------------------

// Routines exported by SARK - sark_event.c

/*!
\brief Create additional free events by calling sark_alloc().

Caller specifies number of events to allocate and they will be added to the
existing free queue. Returns 1 on success, 0 on failure.

\param[in] events: number of new events to allocate
\return 0 on failure, 1 otherwise
*/

uint event_alloc (uint events);

/*!
\brief Free a previously allocated event.

The event _must not_ have been scheduled.

\param[in] e: the event to free
*/

NONNULL
void event_free(event_t *e);

/*!
\brief Allocate a new event from the free queue and intialise "proc", "arg1"
and "arg2" fields.

The "ID", "next" and "time" fields are also set.

\param[in] proc: pointer to an event_proc
\param[in] arg1: argument 1 to the event_proc
\param[in] arg2: argument 2 to the event_proc
\return pointer to event if successful, NULL otherwise
*/

NONNULL event_t *
event_new(event_proc proc, uint arg1, uint arg2);

/*!
\brief Configure a (reusable) event that has already been allocated.

Configure fields "proc", "arg1" and "arg2" from the parameters.
Fields "next" and "time" are set to default values.

\param[in] event: pointer to an event (to be configured)
\param[in] proc: pointer to an event_proc
\param[in] arg1: argument 1 to the event_proc
\param[in] arg2: argument 2 to the event_proc
*/

void event_config(event_t* event, event_proc proc, uint arg1, uint arg2);

/*!
\brief Transmit a packet which contains only a key.

The packet is placed on a
transmit queue and sent when it reaches the head of the queue. The
transmit control register (TCR) which controls the type of packet
which is sent is not modified by this routine so the previous value
will be used. The routine pkt_register must be called (once)
before any of the pkt_tx_XXX routines are used.

\param key: the key field to be placed in the packet
\return zero if the transmit queue is full, 1 otherwise
*/

uint pkt_tx_k (uint key);
//
/*!
\brief Transmit a packet which contains key and data fields.

The packet is placed
on a transmit queue and sent when it reaches the head of the queue. The
transmit control register (TCR) which controls the type of packet which
is sent is not modified by this routine so the previous value will be
used.

\param key; the key field to be placed in the packet
\param data: the data field to be placed in the packet
\return zero if the transmit queue is full, 1 otherwise
*/

uint pkt_tx_kd (uint key, uint data);

/*!
\brief Transmit a packet which contains key and control byte (cb) fields.

The packet is placed on a transmit queue and sent when it reaches the head
of the queue. The "ctrl" field is in the bottom 8 bits of the argument
and is shifted left by 16 bits when it is placed in the hardware TCR
in the chip.

\param key: the key field to be placed in the packet
\param ctrl: the control byte to be placed in the packet
\return zero if the transmit queue is full, 1 otherwise
*/

uint pkt_tx_kc (uint key, uint ctrl);

/*!
\brief Transmit a packet which contains key, data and control byte (cb)
fields.

The packet is placed on a transmit queue and sent when it
reaches the head of the queue. The "ctrl" field is in the bottom 8 bits
of the argument and is shifted left by 16 bits when it is placed in
the hardware TCR in the chip.

\param key: the key field to be placed in the packet
\param data: the key field to be placed in the packet
\param ctrl: the control byte to be placed in the packet
\return zero if the transmit queue is full, 1 otherwise
*/

uint pkt_tx_kdc (uint key, uint data, uint ctrl);

/*!
\brief Start event processing.

An event scheduler runs which takes events
from event queues and executes them according to priority ordering.
In addition, interrupts will be serviced and any events associated
with them will also be executed. This routine will exit if the
application calls event_stop or if a kill signal is received from
the host.

\param period: timer period in microseconds (timer disabled if zero)
\param events: number of new events to be created before processing begins
\param wait: if non-zero causes application to wait from signal from
    host before processing events.

\return return code from event_stop routine or 255 if killed
*/

uint event_start (uint period, uint events, sync_bool wait);

/*!
\brief Used to stop (pause) event processing or resume it again.

If a pause
handler has been registered then this will be called with "pause" as
first argument and the second argument that was supplied at the time
of registration. (Unlikely to be called from application code).

\param pause: non-zero to pause, zero to resume
*/

void event_pause (uint pause);

/*!
\brief Used to stop event processing.

This will cause the routine event_start
to terminate and the user's code after this will then execute.
event_start will return the value provided in the "rc" parameter.
This is an internal routine of the event processing system and
unlikely to be called from an application.

\param rc: return code to return from event_start
*/

void event_stop (uint rc);

/*!
\brief Wait for a signal from the monitor processor (i.e., host) before
proceeding. Can be used to implement barrier waits.

There are two wait states which are used alternately. The first wait after
event_start begins is for the signal WAIT0 and subsequent waits
wait for signals WAIT1, WAIT0, WAIT1, etc.
*/

void event_wait (void);

/*!
\brief Places an event on an event queue for execution at a later time.

There are (currently) 4 event queues numbered PRIO_0 (highest
priority) to PRIO_3 (lowest). Events are taken from queue 0 until it
is empty, then they are taken from queue 1 until it is empty, etc,
etc.

\param e: pointer to the event to add
\param priority: priority of queue to add to
\return zero on failure (invalid priority), 1 otherwise
*/

NONNULL uint
event_queue(event_t *e, event_priority priority);

/*!
\brief Creates a new event and places it on an event queue for execution at a
later time.

Calls event_new() to create and initialise a new event and
then, if that is successful, calls event_queue() to place it on an event
queue.

\param proc: pointer to an event_proc
\param arg1: argument 1 to the event_proc
\param arg2: argument 2 to the event_proc
\param priority: priority of queue to add to
\return zero on failure (invalid priority or failed to allocate a new
    event), 1 otherwise
*/

NONNULL uint
event_queue_proc(event_proc proc, uint arg1, uint arg2, event_priority priority);

/*!
\brief Run the event queue until it is empty.

Events are taken from the
event queues in priority order and executed. The "restart" parameter
controls whether all queues are rescanned for events when processing
of a queue at a particular priority is complete. In general, this will
be necessary if executing an event can cause new events to be queued.

\param restart: non-zero to cause queues to be rescanned
*/

void event_run(uint restart);

/*!
\brief Register an event_proc to be called when a particular event occurs and
associate that with an IRQ or FIQ interrupt.

A slot in the VIC to be
used for the event must be provided. The pseudo-slot SLOT_FIQ is used
to indicate that the FIQ interrupt should be used to ensure the
fastest possible processing. Only one event can be associated with
each slot and a runtime error will occur if a slot is used more than
once.

\param proc: the event_proc to call
\param event: the event to associate with the event_proc
\param slot: the VIC slot to use (or SLOT_FIQ for FIQs)
*/

NONNULL void
event_register_int(event_proc proc, event_type event, vic_slot slot);

/*!
\brief Register an event_proc to be placed on an event queue when a
particular event occurs and associate that with an IRQ interrupt.

A slot in the VIC to be used for the event must be provided. Only one
event can be associated with each slot and a runtime error will occur
if a slot is used more than once or if an invalid queue priority is
used.

\param proc: the event_proc to call
\param event: the event to associate with the event_proc
\param slot: the VIC slot to use
\param priority: the priority of the event queue to use
*/

NONNULL void
event_register_queue(event_proc proc, event_type event, vic_slot slot,
	event_priority priority);

/*!
\brief Register an event_proc to be called when event processing is paused.

The event_proc will receive the pause state (0 = resume, 1 = pause) as
its first argument and the value of "arg2" as its second argument.

\param proc: the event_proc to call
\param arg2: the second argument to be supplied to the event_proc
*/

NONNULL void
event_register_pause(event_proc proc, uint arg2);

/*!
\brief Enable or disable an event.

Enabling restores its interrupt handler to
one which calls the handler which was originally registered with the
event. Disabling changes its interrupt handler to one which receives
and acknowledges the interrupt but which doesn't call the handler which was
originally registered with the event. It is expected that disable will
be called before enable in all cases

\param event_num: the event to enable
\param enable: non-zero to enable, zero to disable
*/

void event_enable (event_type event_num, uint enable);

/*!
\brief Register the use of the second timer on the core so that it can be
used to provide delayed events via timer_schedule().

A runtime error will occur if the VIC slot is already in use.

\param slot: the VIC slot to use for the timer interrupt
*/

void event_register_timer (vic_slot slot);

/*!
\brief Register the use of "pkt" routines.

An interrupt handler needs to be
installed and assigned to a VIC slot. In addition, the size of the
transmit packet queue must be specified. It should be a power of two
no larger than 256 and refers to the number of packets in the queue.
A run time error will occur if the queue size is invalid, the VIC
slot is in use or memory for the packet queue could not be allocated.
This routine also initialises the Transmit Control Register to send
multicast packets.

\param queue_size: number of packets in the transmit queue
\param slot: the VIC slot to use for the packet transmit interrupt
*/

void event_register_pkt(uint queue_size, vic_slot slot);

/*!
\brief Used to trigger a user event if one has been registered.

The two arguments will be provided to the event_proc() when it is called.
Only one user event can be outstanding at any given time.

\param arg1: first argument to event_proc
\param arg2: second argument to event_proc
\return zero if a user event is already pending, 1 otherwise
*/

uint event_user (uint arg1, uint arg2);

//------------------------------------------------------------------------------

// Routines exported by SARK - sark_timer.c

/*!
\brief Schedule an event to occur at some time in the future.

Requires that (hardware) TIMER2 has been set up by a call to timer_register()

\note this procedure assumes the following event conditions on entry:
<pre>
e->next == NULL
e->time == 0
</pre>

\param e: event to execute
\param time: delay in microseconds (non-zero)
*/

NONNULL void
timer_schedule(event_t *e, uint time);

/*!
\brief Allocates an event, initialises it with the supplied parameters and
schedules it to occur at some time in the future.

Requires that the second timer has been set up by a call to timer_register()

\param proc: pointer to an event_proc
\param arg1: argument 1 to the event_proc
\param arg2: argument 2 to the event_proc
\param time: delay in microseconds (non-zero)
\return zero: if allocation of new event failed, one otherwise
*/

NONNULL uint
timer_schedule_proc(event_proc proc, uint arg1, uint arg2, uint time);

/*!
\brief Cancel a timer event that was previously scheduled.

The ID that was allocated when the timer was created must be given in case
the timer has already executed and the event has possibly been recycled.
This means that a timer event which may be cancelled must be allocated by
event_new(), the ID noted and the event then scheduled with
timer_schedule().

It is potentially quite difficult to cancel a timer at the head
of the timer queue so in that case the timer is replaced with a
placeholder with "proc" set to NULL and the placeholder is left
to terminate on the timer interrupt.

\param e: event to cancel
\param ID: ID of event to cancel
*/

NONNULL void timer_cancel(event_t *e, uint ID);

/*!
\brief Initialise a statically allocated event to be used in place
of an event that is cancelled at the head of the timer queue.

It is potentially quite difficult to cancel a timer at the head
of the timer queue so in that case the timer is replaced with a
placeholder with "proc" set to NULL and the placeholder is left
to terminate on the timer interrupt.
*/
void timer_cancel_init(void);


//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __SARK_H__
