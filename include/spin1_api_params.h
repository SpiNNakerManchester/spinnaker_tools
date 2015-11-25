/****a* spinn_api_params.h/spinn_api_params_header
*
* SUMMARY
*  SpiNNaker API internal parameters and function prototypes
*
* AUTHOR
*  Luis Plana   - lap@cs.man.ac.uk
*  Thomas Sharp - thomas.sharp@cs.man.ac.uk
*
* DETAILS
*  Created on       : 03 May 2011
*  Version          : $Revision: 2023 $
*  Last modified on : $Date: 2012-10-26 10:37:47 +0100 (Fri, 26 Oct 2012) $
*  Last modified by : $Author: plana $
*  $Id: spin1_api_params.h 2023 2012-10-26 09:37:47Z plana $
*  $HeadURL: https://solem.cs.man.ac.uk/svn/spin1_api/trunk/src/spin1_api_params.h $
*
* COPYRIGHT
*  Copyright (c) The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
*******/

#ifndef __SPINN_API_PARAM_H__
#define __SPINN_API_PARAM_H__


// ------------------------------------------------------------------------
// internal constants and parameters -- not visible in the API
// ------------------------------------------------------------------------
// ---------------------
/* simulation control */
// ---------------------
/* synchronization barrier key and mask codes */
#define GO_KEY                (uint) ((0x1ffff << 11) | 0)
#define RDYGO_KEY             (uint) ((0x1ffff << 11) | 1)
#define RDY1_KEY              (uint) ((0x1ffff << 11) | 2)
#define RDY2_KEY              (uint) ((0x1ffff << 11) | 3)
#define BARRIER_MASK          (uint) (0xffffffff)

#define BARRIER_GO_WAIT       50000
#define BARRIER_RDY2_WAIT     25000
#define BARRIER_RDYGO_WAIT    25000
#define BARRIER_LOCK_WAIT     50000
#define BARRIER_RESEND_WAIT   100

#define ROUTER_INIT_WAIT      5000

/* lock usage */
#define CLEAR_LCK             0
#define RTR_INIT_LCK          0xad
#define RDYGO_LCK             0xbe

/* link orientation codes */
#define EAST                  0
#define NORTH_EAST            1
#define NORTH                 2
#define WEST                  3
#define SOUTH_WEST            4
#define SOUTH                 5

/* print warnings at end of simulation */
#define API_WARN              TRUE
/* print debug messages */
#define API_DEBUG             TRUE
/* make diagnostics available to the application */
#define API_DIAGNOSTICS       TRUE
#define NON_ROOT              FALSE
#define API_PRINT_DLY         200

/* internal error/warning return codes */
#define NO_ERROR              0
#define TASK_QUEUE_FULL       1
#define DMA_QUEUE_FULL        2
#define PACKET_QUEUE_FULL     4
#define WRITE_BUFFER_ERROR    8
#define SYNCHRO_ERROR         16
// --------------------------------

// ----------------
/* data transfer */
// ----------------
// DMA transfer parameters: 4-doubleword bursts
//TODO: may need adjustment for SpiNNaker
#define DMA_BURST_SIZE        4
#define DMA_WIDTH             1
// internal DMA queue size
#define DMA_QUEUE_SIZE        16
// select write buffer use
#define USE_WRITE_BUFFER      FALSE
// ---------------------------

// -----------------
// communications */
// -----------------
#define TX_PACKET_QUEUE_SIZE  16
// TX control register programming
#define TX_TCR_MCDEFAULT      0x00000000
#define TX_FULL_MASK          0x40000000
#define TX_EMPTY_MASK         0x80000000
#define RX_RECEIVED_MASK      0x80000000
// -----------------

// --------------------
/* memory allocation */
// --------------------
// memory space reserved for RTS stacks
#define RTS_STACKS            4096
// --------------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
// callback queue parameters
#define NUM_PRIORITIES    5
#define TASK_QUEUE_SIZE   16
// -----------------------

// -----------------------
// Allocation of entries in MC table
// -----------------------

//#define SYS_MC_ENTRIES 24
#define SYS_MC_ENTRIES 1
#define APP_MC_ENTRIES (MC_TABLE_SIZE - SYS_MC_ENTRIES)


// -----------------------
// VIC priorities
// -----------------------
#define SARK_PRIORITY          0
#define TIMER1_PRIORITY        1
#define DMA_DONE_PRIORITY      2
#define RX_READY_PRIORITY      3
#define FR_READY_PRIORITY      4
#define CC_TMT_PRIORITY        5
#define SOFT_INT_PRIORITY      6
#if USE_WRITE_BUFFER == TRUE
  #define DMA_ERR_PRIORITY     7
#endif

// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// helpful macros
// ------------------------------------------------------------------------
#define CHIP_ADDR(x, y)      ((x << 8) | y)
#define P2P_ROUTE(addr)      (1 << p2p_get(addr))
#define CORE_ROUTE(core)     (1 << (core + NUM_LINKS))
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal pre-defined types -- not visible in the API
// ------------------------------------------------------------------------
// ----------------
/* data transfer */
// ----------------
typedef struct
{
  uint id;
  uint tag;
  uint* system_address;
  uint* tcm_address;
  uint description;
} copy_t;


typedef struct
{
  uint start;
  uint end;
  copy_t queue[DMA_QUEUE_SIZE];
} dma_queue_t;
// ----------------

// -----------------
/* communications */
// -----------------
typedef struct
{
  uint key;
  uint data;
  uint TCR;
} packet_t;

typedef struct
{
  uint start;
  uint end;
  packet_t queue[TX_PACKET_QUEUE_SIZE];
} tx_packet_queue_t;
// -----------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
typedef struct
{
  callback_t cback;
  int priority;
} cback_t;

typedef struct
{
  callback_t cback;
  uint arg0;
  uint arg1;
} task_t;

typedef struct
{
  uint start;
  uint end;
  task_t queue[TASK_QUEUE_SIZE];
} task_queue_t;

extern cback_t callback[NUM_EVENTS];

// interrupt service routine
#ifdef __GNUC__
typedef void (*isr_t) (void);
#else
typedef __irq void (*isr_t) (void);
#endif
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal variables -- not visible in the API
// ------------------------------------------------------------------------
/* VIC vector tables */
static volatile isr_t * const vic_vectors  = (isr_t *) (VIC_BASE + 0x100);
static volatile uint * const vic_controls = (uint *) (VIC_BASE + 0x200);

// ------------------------------------------------------------------------

#endif /* __SPINN_API_PARAM_H__ */
