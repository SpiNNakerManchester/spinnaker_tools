/*! \file
*
* \brief
*  SpiNNaker API internal parameters and function prototypes
*
* \author
*  Luis Plana   - lap@cs.man.ac.uk
*  Thomas Sharp - thomas.sharp@cs.man.ac.uk
*
* \date 03 May 2011
*
* \copyright
*  &copy; The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
* \internal
* DETAILS
*  Created on       : 03 May 2011
*  Version          : $Revision: 2023 $
*  Last modified on : $Date: 2012-10-26 10:37:47 +0100 (Fri, 26 Oct 2012) $
*  Last modified by : $Author: plana $
*  $Id: spin1_api_params.h 2023 2012-10-26 09:37:47Z plana $
*  $HeadURL: https://solem.cs.man.ac.uk/svn/spin1_api/trunk/src/spin1_api_params.h $
*
*******/

/*
 * Copyright (c) 2011-2019 The University of Manchester
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

#ifndef __SPINN_API_PARAM_H__
#define __SPINN_API_PARAM_H__


// ------------------------------------------------------------------------
// internal constants and parameters -- not visible in the API
// ------------------------------------------------------------------------
// ---------------------
/* simulation control */
// ---------------------
#if 0 // DKF: are these unused?
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
#endif

//! link orientation codes
enum spin1_api_link_orientations {
    EAST =                0,
    NORTH_EAST =          1,
    NORTH =               2,
    WEST =                3,
    SOUTH_WEST =          4,
    SOUTH =               5
};

//! print warnings at end of simulation
#define API_WARN              TRUE
//! print debug messages
#define API_DEBUG             TRUE
//! make diagnostics available to the application
#define API_DIAGNOSTICS       TRUE
//! \deprecated No definition available
#define NON_ROOT              FALSE
//! suggested delay (in &mu;s) between calls to io_printf()
#define API_PRINT_DLY         200

//! internal error/warning return codes
enum spin1_api_error_codes {
    NO_ERROR =            0,
    TASK_QUEUE_FULL =     1,
    DMA_QUEUE_FULL =      2,
    PACKET_QUEUE_FULL =   4,
    WRITE_BUFFER_ERROR =  8,
    SYNCHRO_ERROR =      16
};
// --------------------------------

// ----------------
/* data transfer */
// ----------------
//! DMA transfer parameters: <b>16</b>-doubleword bursts
//TODO: may need adjustment for SpiNNaker
#define DMA_BURST_SIZE        4
//! DMA transfer parameters: 16-<b>doubleword</b> bursts
#define DMA_WIDTH             1
//! internal DMA queue size
#define DMA_QUEUE_SIZE        16
//! select write buffer use
#define USE_WRITE_BUFFER      FALSE
// ---------------------------

// -----------------
// communications */
// -----------------
//! Size of outbound packet queue
#define TX_PACKET_QUEUE_SIZE  16
// TX control register programming
//! Default transmit control register value for multicast messages
#define TX_TCR_MCDEFAULT      0x00000000
//! Transmit hardware full mask
#define TX_FULL_MASK          0x40000000
//! Transmit hardware empty mask
#define TX_EMPTY_MASK         0x80000000
//! Message received mask
#define RX_RECEIVED_MASK      0x80000000
// -----------------

// --------------
/* user events */
// --------------
//! internal user event queue size
#define USER_EVENT_QUEUE_SIZE 4

// --------------------
/* memory allocation */
// --------------------
//! memory space reserved for RTS stacks
#define RTS_STACKS            4096
// --------------------

// -----------------------
/* scheduler/dispatcher */
// -----------------------
//! callback queue parameters
enum spin1_api_callback_queue_params {
    N_TASK_QUEUES = 4,    //!< Number of priorities - 1 because priority 0 is
                          //!< not queued
    NUM_PRIORITIES =  5,  //!< Number of priorities
    TASK_QUEUE_SIZE = 16  //!< Size of task queue
};
// -----------------------

// -----------------------
//! Allocation of entries in MC table
// -----------------------

enum spin1_api_multicast_entries {
    SYS_MC_ENTRIES = 1,
    APP_MC_ENTRIES = (MC_TABLE_SIZE - SYS_MC_ENTRIES)
};

// -----------------------
//! VIC priorities
// -----------------------
enum spin1_api_vic_priorties {
    SARK_PRIORITY =        0,   //!< Communication with SARK/SCAMP
    TIMER2_PRIORITY =      1,   //!< Timer 2 interrupt (events)
    TIMER1_PRIORITY =      2,   //!< Timer interrupt
    DMA_DONE_PRIORITY =    3,   //!< DMA complete
    RX_READY_PRIORITY =    4,   //!< Multicast message ready to receive
    FR_READY_PRIORITY =    5,   //!< Fixed route message ready to receive
    CC_TMT_PRIORITY =      6,   //!< Comms controller timeout
    SOFT_INT_PRIORITY =    7,   //!< Software-driven interrupt
#if USE_WRITE_BUFFER == TRUE
    DMA_ERR_PRIORITY =     8    //!< DMA error
#endif
};

// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// helpful macros
// ------------------------------------------------------------------------
//! \brief The address of a chip
//! \param[in] x: X coordinate of the chip
//! \param[in] y: Y coordinate of the chip
//! \return The packed chip coordinates
#define CHIP_ADDR(x, y)      ((x << 8) | y)
//! \brief A peer-to-peer route
//! \param[in] addr: The address of the chip
//! \return The route bit to that chip
#define P2P_ROUTE(addr)      (1 << p2p_get(addr))
//! \brief The route to a core on the current chip
//! \param[in] core: The core to route to
//! \return The route bit to that core
#define CORE_ROUTE(core)     (1 << (core + NUM_LINKS))
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal pre-defined types -- not visible in the API
// ------------------------------------------------------------------------
// ----------------
/* data transfer */
// ----------------
//! Describes a DMA transfer
typedef struct {
    uint id;                    //!< ID
    uint tag;                   //!< User label
    uint* system_address;       //!< Address in SDRAM (or other shared memory)
    uint* tcm_address;          //!< Address in local TCM
    uint description;           //!< Control descriptor
} copy_t;

//! \brief The queue of DMA transfers.
//! \details Implemented as a circular buffer
typedef struct {
    uint start;                 //!< Index of first transfer
    uint end;                   //!< Index of last transfer
    //! Array holding transfer descriptors
    copy_t queue[DMA_QUEUE_SIZE];
} dma_queue_t;
// ----------------

// -----------------
/* communications */
// -----------------
//! A multicast packet
typedef struct {
    uint key;                   //!< The MC packet key
    uint data;                  //!< The MC packet payload (if defined)
    uint TCR;                   //!< The MC packet control word
} packet_t;

//! \brief The queue of multicast packets to be sent.
//! \details Implemented as a circular buffer
typedef struct {
    uint start;                 //!< Index of first packet
    uint end;                   //!< Index of last packet
    //! Array holding packet descriptors
    packet_t queue[TX_PACKET_QUEUE_SIZE];
} tx_packet_queue_t;
// -----------------

// --------------
/* user events */
// --------------
//! Describes the parameters to pass to a user event.
typedef struct {
    uint arg0;                  //!< The first arbitrary parameter
    uint arg1;                  //!< The second arbitrary parameter
} user_event_t;

//! \brief The type of the fixed-capacity queue of user events.
//! \details Implemented as a circular buffer.
typedef struct {
    uint start;                 //!< Index of first event
    uint end;                   //!< Index of last event
    //! Array holding event descriptors
    user_event_t queue[USER_EVENT_QUEUE_SIZE];
} user_event_queue_t;

// -----------------------
/* scheduler/dispatcher */
// -----------------------
//! An external interrupt handler
typedef struct {
    callback_t cback;           //!< Pointer to the function to call
    int priority;               //!< The interrupt priority
} cback_t;

//! An internal interrupt/callback handler
typedef struct {
    callback_t cback;           //!< Pointer to the function to call
    uint arg0;                  //!< The first arbitrary parameter
    uint arg1;                  //!< The second arbitrary parameter
} task_t;

//! \brief The queue of callbacks to do.
//! \details Implemented as a circular buffer
typedef struct {
    uint start;                 //!< Index of first task
    uint end;                   //!< Index of last task
    //! Array holding task descriptors
    task_t queue[TASK_QUEUE_SIZE];
} task_queue_t;

//! The external interrupt handlers
extern cback_t callback[NUM_EVENTS];

//! interrupt service routine
#ifdef __GNUC__
typedef void (*isr_t) (void);
#else
typedef __irq void (*isr_t) (void);
#endif
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// internal variables -- not visible in the API
// ------------------------------------------------------------------------
//! VIC vector table
static volatile isr_t * const vic_vectors  = (isr_t *) (VIC_BASE + 0x100);
//! VIC control table
static volatile uint * const vic_controls = (uint *) (VIC_BASE + 0x200);

// ------------------------------------------------------------------------

#endif /* __SPINN_API_PARAM_H__ */
