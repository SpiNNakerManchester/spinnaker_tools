/****a* spin1_api.h/spin1_api_header
*
* SUMMARY
*  SpiNNaker API main header file
*
* AUTHOR
*  Luis Plana - lap@cs.man.ac.uk
*
* DETAILS
*  Created on       : 03 May 2011
*  Version          : $Revision: 2011 $
*  Last modified on : $Date: 2012-10-24 14:50:54 +0100 (Wed, 24 Oct 2012) $
*  Last modified by : $Author: plana $
*  $Id: spin1_api.h 2011 2012-10-24 13:50:54Z plana $
*  $HeadURL: https://solem.cs.man.ac.uk/svn/spin1_api/trunk/src/spin1_api.h $
*
* COPYRIGHT
*  Copyright (c) The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
*******/

#ifndef __SPIN1_API_H__
#define __SPIN1_API_H__

#include "sark.h"

// ------------------------------------------------------------------------
// Useful SpiNNaker parameters
// ------------------------------------------------------------------------
/* shared memory */
/* system RAM address and size */
#define SPINN_SYSRAM_BASE     SYSRAM_BASE
#define SPINN_SYSRAM_SIZE     SYSRAM_SIZE
/* SDRAM address and size */
#define SPINN_SDRAM_BASE      SDRAM_BASE
#define SPINN_SDRAM_SIZE      SDRAM_SIZE

// ------------------------------------------------------------------------
// general parameters and definitions
// ------------------------------------------------------------------------
/* boolean constants */
#define TRUE               (0 == 0)
#define FALSE              (0 != 0)
/* function results   */
#define SUCCESS            (uint) 1
#define FAILURE            (uint) 0
/* Null pointer value */
#define NULL               0

// ------------------------------------------------------------------------
// event definitions
// ------------------------------------------------------------------------
// event-related parameters
#define NUM_EVENTS            6
#define MC_PACKET_RECEIVED    0
#define DMA_TRANSFER_DONE     1
#define TIMER_TICK            2
#define SDP_PACKET_RX         3
#define USER_EVENT            4
#define MCPL_PACKET_RECEIVED  5

// ------------------------------------------------------------------------
// DMA transfer parameters
// ------------------------------------------------------------------------
// DMA transfer direction (from core point of view)
#define DMA_READ           0
#define DMA_WRITE          1


// ------------------------------------------------------------------------
// packet parameters
// ------------------------------------------------------------------------
// payload presence
#define NO_PAYLOAD         0
#define WITH_PAYLOAD       1


// ------------------------------------------------------------------------
// type definitions
// ------------------------------------------------------------------------

typedef void (*callback_t) (uint, uint);  // callbacks
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// simulation control functions
// ------------------------------------------------------------------------
uint spin1_start (sync_bool sync);
void spin1_exit (uint error);
void spin1_set_timer_tick(uint time);
uint spin1_get_simulation_time(void);
void spin1_delay_us (uint n);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// callback and task functions
// ------------------------------------------------------------------------
void spin1_callback_on(uint event_id, callback_t cback, int priority);
void spin1_callback_off(uint event_id);
uint spin1_schedule_callback(callback_t cback, uint arg0, uint arg1, uint priority);
uint spin1_trigger_user_event(uint arg0, uint arg1);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  data transfer functions
// ------------------------------------------------------------------------
uint spin1_dma_transfer(uint tag, void *system_address, void *tcm_address, uint direction, uint length);
void spin1_memcpy(void *dst, void const *src, uint len);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  communications functions
// ------------------------------------------------------------------------
uint spin1_send_mc_packet(uint key, uint data, uint load);
void spin1_flush_rx_packet_queue(void);
void spin1_flush_tx_packet_queue(void);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
// SDP related functions
// ------------------------------------------------------------------------
void spin1_msg_free (sdp_msg_t *msg);
sdp_msg_t* spin1_msg_get (void);
uint spin1_send_sdp_msg (sdp_msg_t *msg, uint timeout);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  interrupt control functions
// ------------------------------------------------------------------------
uint spin1_irq_disable(void);
uint spin1_fiq_disable(void);
uint spin1_int_disable(void);
void spin1_mode_restore(uint sr);
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//  system resources access funtions
// ------------------------------------------------------------------------
uint  spin1_get_id(void);
uint  spin1_get_core_id(void);
uint  spin1_get_chip_id(void);
void  spin1_led_control (uint p);
uint  spin1_set_mc_table_entry(uint entry, uint key, uint mask, uint route);
void* spin1_malloc(uint bytes);
// ------------------------------------------------------------------------


// ----------------------------------
// pseudo-random number generation
// ----------------------------------
void  spin1_srand (uint seed);
uint  spin1_rand  (void);


// ------------------------------------------------------------------------
//  diagnostic data available to the application
// ------------------------------------------------------------------------
typedef struct
{
  uint exit_code;               // simulation exit code
  uint warnings;                // warnings type bit map
  uint total_mc_packets;        // total routed MC packets during simulation
  uint dumped_mc_packets;       // total dumped MC packets by the router
  uint discarded_mc_packets;    // total discarded MC packets by API
  uint dma_transfers;           // total DMA transfers requested
  uint dma_bursts;              // total DMA bursts completed
  uint dma_queue_full;          // dma queue full count
  uint task_queue_full;         // task queue full count
  uint tx_packet_queue_full;    // transmitter packet queue full count
  uint writeBack_errors;        // write-back buffer errror count
} diagnostics_t;

extern diagnostics_t diagnostics;
extern uchar         leadAp;   // lead appl. core has special functions
// ------------------------------------------------------------------------


#endif /* __SPIN1_API_H__ */
