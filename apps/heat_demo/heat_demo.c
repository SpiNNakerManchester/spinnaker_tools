/****a* heat_demo.c/heat_demo_summary
*
* SUMMARY
*  a very, very simple 2D Heat equation SpiNNaker application
*  one core does one point!
*
* AUTHOR
*  Luis Plana - luis.plana@manchester.ac.uk
*
* DETAILS
*  Created on       : 27 Jul 2011
*  Version          : $Revision: 2531 $
*  Last modified on : $Date: 2013-08-20 12:34:21 +0100 (Tue, 20 Aug 2013) $
*  Last modified by : $Author: plana $
*  $Id: heat_demo.c 2531 2013-08-20 11:34:21Z plana $
*  $HeadURL: https://solem.cs.man.ac.uk/svn/demos/heat_demo/heat_demo.c $
*
* COPYRIGHT
*  Copyright (c) The University of Manchester, 2011. All rights reserved.
*  SpiNNaker Project
*  Advanced Processor Technologies Group
*  School of Computer Science
*
*******/

// SpiNNaker API
#include "spin1_api.h"


// ------------------------------------------------------------------------
// DEBUG parameters
// ------------------------------------------------------------------------
//#define DEBUG              TRUE
#define DEBUG_KEYS         500

#define VERBOSE            TRUE

// the visualiser has a bug with negative temperatures!
#define POSITIVE_TEMP      TRUE

// ------------------------------------------------------------------------
// core map choice
// ------------------------------------------------------------------------
#define MAP_2x2_on_4       TRUE		// Spin3 board
//#define MAP_5x4_on_48      TRUE
//#define MAP_8x8_on_48      TRUE	// Spin5 board
//#define MAP_12x12_on_144   TRUE
//#define MAP_24x12_on_288   TRUE
//#define MAP_24x24_on_576   TRUE
//#define MAP_48x24_on_1152  TRUE

// ------------------------------------------------------------------------
// simulation parameters
// ------------------------------------------------------------------------
//#define TIMER_TICK_PERIOD  1000
#define TIMER_TICK_PERIOD  2500
//#define TIMER_TICK_PERIOD  25000

#define PARAM_CX           0.03125
#define PARAM_CY           0.03125

#define NORTH_INIT         (40 << 16)
#define EAST_INIT          (10 << 16)
#define SOUTH_INIT         (10 << 16)
#define WEST_INIT          (40 << 16)

// ------------------------------------------------------------------------
// Routing table constants and macro definitions
// ------------------------------------------------------------------------
#define DONT_ROUTE_KEY     0xffff
#define ROUTING_KEY(chip, core)    ((chip << 5) | core)
#define ROUTE_TO_CORE(core)        (1 << (core + 6))

#define NORTH              3
#define SOUTH              2
#define EAST               1
#define WEST               0

#define NORTH_ARRIVED      (1 << NORTH)
#define SOUTH_ARRIVED      (1 << SOUTH)
#define EAST_ARRIVED       (1 << EAST)
#define WEST_ARRIVED       (1 << WEST)
#define NONE_ARRIVED       0
#define NS_ARRIVED         (NORTH_ARRIVED | SOUTH_ARRIVED)
#define EW_ARRIVED         (EAST_ARRIVED | WEST_ARRIVED)
#define ALL_ARRIVED        (NS_ARRIVED | EW_ARRIVED)

#define CORE_TO_NORTH(core)        (core + 1)
#define CORE_TO_SOUTH(core)        (core - 1)
#define CORE_TO_EAST(core)         (core + 4)
#define CORE_TO_WEST(core)         (core - 4)

#define IS_NORTHERNMOST_CORE(core) (((core - 1) & 0x3) == 0x3)
#define IS_SOUTHERNMOST_CORE(core) (((core - 1) & 0x3) == 0x0)
#define IS_EASTERNMOST_CORE(core)  (((core - 1) & 0xc) == 0xc)
#define IS_WESTERNMOST_CORE(core)  (((core - 1) & 0xc) == 0x0)

#define NORTHERNMOST_CORE(core)    (((core - 1) | 0x3) + 1)
#define SOUTHERNMOST_CORE(core)    (((core - 1) & 0xc) + 1)
#define EASTERNMOST_CORE(core)     (((core - 1) | 0xc) + 1)
#define WESTERNMOST_CORE(core)     (((core - 1) & 0x3) + 1)

#define ROUTE_TO_LINK(link)        (1 << link)
#define EAST_LINK                  0
#define NORTH_EAST_LINK            1
#define NORTH_LINK                 2
#define WEST_LINK                  3
#define SOUTH_WEST_LINK            4
#define SOUTH_LINK                 5

#define CHIP_TO_NORTH(chip)     (chip + 1)
#define CHIP_TO_SOUTH(chip)     (chip - 1)
#define CHIP_TO_EAST(chip)      (chip + (1 << 8))
#define CHIP_TO_WEST(chip)      (chip - (1 << 8))

#define IS_NORTHERNMOST_CHIP(x, y) ((chip_map[x][y] & (1 << NORTH)) != 0)
#define IS_SOUTHERNMOST_CHIP(x, y) ((chip_map[x][y] & (1 << SOUTH)) != 0)
#define IS_EASTERNMOST_CHIP(x, y)  ((chip_map[x][y] & (1 << EAST)) != 0)
#define IS_WESTERNMOST_CHIP(x, y)  ((chip_map[x][y] & (1 << WEST)) != 0)

// use core 17 keys to distribute commands and data
#define STOP_KEY       ROUTING_KEY(0, 17)
#define PAUSE_KEY      ROUTING_KEY(1, 17)
#define RESUME_KEY     ROUTING_KEY(2, 17)
#define TEMP_NORTH_KEY ROUTING_KEY(16, 17)
#define TEMP_EAST_KEY  ROUTING_KEY(17, 17)
#define TEMP_SOUTH_KEY ROUTING_KEY(18, 17)
#define TEMP_WEST_KEY  ROUTING_KEY(19, 17)
#define CMD_MASK       0xfffffe1f

#define NCh   (1 << NORTH)
#define SCh   (1 << SOUTH)
#define ECh   (1 << EAST)
#define WCh   (1 << WEST)
#define NECh ((1 << NORTH) | (1 << EAST))
#define NWCh ((1 << NORTH) | (1 << WEST))
#define SECh ((1 << SOUTH) | (1 << EAST))
#define SWCh ((1 << SOUTH) | (1 << WEST))

#define CHIP_ADDR(x, y)      ((x << 8) | y)

// ------------------------------------------------------------------------
// variables
// ------------------------------------------------------------------------
uint coreID;
uint chipID;
uint my_chip, my_x, my_y;
uint board_loc;

#ifdef MAP_2x2_on_4
  #define NUMBER_OF_XCHIPS 2
  #define NUMBER_OF_YCHIPS 2
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, NWCh},
    {SECh, NECh}
  };
#endif

#ifdef MAP_5x4_on_48
  #define NUMBER_OF_XCHIPS 5
  #define NUMBER_OF_YCHIPS 4
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, WCh,  WCh,  NWCh},
    {SCh,  0,    0,    NCh},
    {SCh,  0,    0,    NCh},
    {SCh,  0,    0,    NCh},
    {SECh, ECh,  ECh,  NECh}
  };
#endif

#ifdef MAP_8x8_on_48
  #define NUMBER_OF_XCHIPS 8
  #define NUMBER_OF_YCHIPS 8
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0,       0,       0,       0},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0,       0,       0},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0,       0},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0,       0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0,       0,       0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0,       0,       0,       0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, WCh,  WCh,  NWCh, 0,    0,    0,    0},
    {SCh,  0,    0,    0,    NWCh, 0,    0,    0},
    {SCh,  0,    0,    0,    0,    NWCh, 0,    0},
    {SCh,  0,    0,    0,    0,    0,    NWCh, 0},
    {SECh, 0,    0,    0,    0,    0,    0,    NWCh},
    {0,    SECh, 0,    0,    0,    0,    0,    NCh},
    {0,    0,    SECh, 0,    0,    0,    0,    NCh},
    {0,    0,    0,    SECh, ECh,  ECh,  ECh,  NECh},
  };
#endif

#ifdef MAP_12x12_on_144
  #define NUMBER_OF_XCHIPS 12
  #define NUMBER_OF_YCHIPS 12
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  NWCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SECh, ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  NECh}
  };
#endif

#ifdef MAP_24x12_on_288
  #define NUMBER_OF_XCHIPS 24
  #define NUMBER_OF_YCHIPS 12
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  NWCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SECh, ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  NECh}
  };
#endif

#ifdef MAP_24x24_on_576
  #define NUMBER_OF_XCHIPS 24
  #define NUMBER_OF_YCHIPS 24
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  NWCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SECh, ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  NECh}
  };
#endif

#ifdef MAP_48x24_on_1152
  #define NUMBER_OF_XCHIPS 48
  #define NUMBER_OF_YCHIPS 24
  uint core_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe},
    {0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe, 0x1fffe}
  };
  uint chip_map[NUMBER_OF_XCHIPS][NUMBER_OF_YCHIPS] =
  {
    {SWCh, WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  WCh,  NWCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SCh,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    NCh},
    {SECh, ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  ECh,  NECh}
  };
#endif

/* multicast routing keys to communicate with neighbours */
uint my_key;
uint north_key;
uint south_key;
uint east_key;
uint west_key;
uint my_route = 0;  // where I send my temperature

uint route_from_north = FALSE;
uint route_from_south = FALSE;
uint route_from_east  = FALSE;
uint route_from_west  = FALSE;

uint rtr_conf;

/* temperature values */
int my_temp = 0;  // any initial value will do!
int old_temp = 0;  // any initial value will do!

// get temperatures from 4 neighbours
// make sure to have room for two values from each neighbour
// given that the communication is asynchronous
volatile int neighbours_temp[2][4];

/* coeficients to compute new temperature value */
/* adjust for 16.16 fixed-point representation  */
int cx_adj = (int) (PARAM_CX * (1 << 16));
int cy_adj = (int) (PARAM_CY * (1 << 16));

/* keep track of which neighbours have sent data */
/* cores in the boder need special values! */
volatile uint arrived[2];
uint init_arrived;
volatile uint now  = 0;
volatile uint next = 1;

volatile uchar updating = TRUE;

sdp_msg_t my_msg;

/* report results in shared memory */
#ifndef OLD_SARK
static volatile int *core_temp;
#else
#ifdef USE_SDRAM
  static volatile int * const core_temp =
                     (int *) (SPINN_SDRAM_BASE + 0x1000 + 16 * sizeof(uint));
#else  //SYSRAM
  static volatile int * const core_temp =
                     (int *) (SPINN_SYSRAM_BASE + (SPINN_SYSRAM_SIZE / 2));
#endif
#endif

#ifdef DEBUG
  uint   dbg_packs_recv = 0;
  uint * dbg_keys_recv;
  uint   dbg_timeouts = 0;
  uint * dbg_stime;
#endif

/****f* heat_demo.c/routing_table_init
*
* SUMMARY
*  This function is called at application start-up.
*  It's used to say setup multicast routing table entries.
*  and initialise routing-related variables.
*
* SYNOPSIS
*  void routing_table_init ()
*
* SOURCE
*/
void routing_table_init ()
{
  /* ------------------------------------------------------------------- */
  /* compute routing keys - mine and my neighbours'                      */
  /* cores on the borders don't receive data from all neighbours         */
  /*     instead use fixed border temperatures                           */
  /* ------------------------------------------------------------------- */
  my_key = ROUTING_KEY(chipID, coreID);
  init_arrived = NONE_ARRIVED;

  if (IS_NORTHERNMOST_CORE(coreID))
  {
    if (IS_NORTHERNMOST_CHIP(my_x, my_y))
    {
      // don't send packets north
      // don't expect packets from north
      north_key = DONT_ROUTE_KEY;
      /* adjust for 16.16 fixed-point representation  */
      neighbours_temp[now][NORTH]  = NORTH_INIT;
      neighbours_temp[next][NORTH] = NORTH_INIT;
      init_arrived |= NORTH_ARRIVED;
    }
    else
    {
      // send packets to chip to the north
      my_route |= ROUTE_TO_LINK(NORTH_LINK);
      // expect packets from chip to the north (southernmost core)
      route_from_north = TRUE;
      north_key = ROUTING_KEY(CHIP_TO_NORTH(chipID), SOUTHERNMOST_CORE(coreID));
    }
  }
  else
  {
    // expect packets from north
    north_key = ROUTING_KEY(chipID, CORE_TO_NORTH(coreID));
    // send to north core
    my_route |= ROUTE_TO_CORE(CORE_TO_NORTH(coreID));
  }

  if (IS_SOUTHERNMOST_CORE(coreID))
  {
    if (IS_SOUTHERNMOST_CHIP(my_x, my_y))
    {
      // don't send packets south
      // don't expect packets from south
      south_key = DONT_ROUTE_KEY;
      /* adjust for 16.16 fixed-point representation  */
      neighbours_temp[now][SOUTH]  = SOUTH_INIT;
      neighbours_temp[next][SOUTH] = SOUTH_INIT;
      init_arrived |= SOUTH_ARRIVED;
    }
    else
    {
      // send packets to chip to the south
      my_route |= ROUTE_TO_LINK(SOUTH_LINK);
      // expect packets from chip to the south (northernmost core)
      route_from_south = TRUE;
      south_key = ROUTING_KEY(CHIP_TO_SOUTH(chipID), NORTHERNMOST_CORE(coreID));
    }
  }
  else
  {
    // expect packets from south
    south_key = ROUTING_KEY(chipID, CORE_TO_SOUTH(coreID));
    // send to south core
    my_route |= ROUTE_TO_CORE(CORE_TO_SOUTH(coreID));
  }

  if (IS_EASTERNMOST_CORE(coreID))
  {
    if (IS_EASTERNMOST_CHIP(my_x, my_y))
    {
      // don't send packets east
      // don't expect packets from east
      east_key = DONT_ROUTE_KEY;
      /* adjust for 16.16 fixed-point representation  */
      neighbours_temp[now][EAST]  = EAST_INIT;
      neighbours_temp[next][EAST] = EAST_INIT;
      init_arrived |= EAST_ARRIVED;
    }
    else
    {
      // send packets to chip to the east
      my_route |= ROUTE_TO_LINK(EAST_LINK);
      // expect packets from chip to the east (westernmost core)
      route_from_east = TRUE;
      east_key = ROUTING_KEY(CHIP_TO_EAST(chipID), WESTERNMOST_CORE(coreID));
    }
  }
  else
  {
    // send to east core
    my_route |= ROUTE_TO_CORE(CORE_TO_EAST(coreID));
    // expect packets from east
    east_key = ROUTING_KEY(chipID, CORE_TO_EAST(coreID));
  }

  if (IS_WESTERNMOST_CORE(coreID))
  {
    if (IS_WESTERNMOST_CHIP(my_x, my_y))
    {
      // don't send packets west
      // don't expect packets from west
      west_key = DONT_ROUTE_KEY;
      /* adjust for 16.16 fixed-point representation  */
      neighbours_temp[now][WEST]  = WEST_INIT;
      neighbours_temp[next][WEST] = WEST_INIT;
      init_arrived |= WEST_ARRIVED;
    }
    else
    {
      // send packets to chip to the west
      my_route |= ROUTE_TO_LINK(WEST_LINK);
      // expect packets from chip to the west (easternmost core)
      route_from_west = TRUE;
      west_key = ROUTING_KEY(CHIP_TO_WEST(chipID), EASTERNMOST_CORE(coreID));
    }
  }
  else
  {
    // expect packets from west
    west_key = ROUTING_KEY(chipID, CORE_TO_WEST(coreID));
    // send to west core
    my_route |= ROUTE_TO_CORE(CORE_TO_WEST(coreID));
  }

  /* prepare for first update */
  arrived[now]  = init_arrived;
  arrived[next] = init_arrived;
  /* ------------------------------------------------------------------- */

  /* ------------------------------------------------------------------- */
  /* initialise routing entries                                          */
  /* ------------------------------------------------------------------- */
  /* set a MC routing table entry to send my packets to my neighbours */
  /* entries depend on coreID to avoid inter-core interference        */

  uint e = rtr_alloc (5); // Get 5 router entries

  if (e == 0)
    rt_error (RTE_ABORT);

  rtr_mc_set (e, 			// entry
	      my_key,             	// key
	      0xffffffff,         	// mask
	      my_route            	// route
	      );

  /* set MC routing table entries to get packets from neighbour chips */
  /* north */
  if (route_from_north)
  {
    rtr_mc_set (e + 1,     		// entry
		north_key,            	// key
		0xffffffff,           	// mask
		ROUTE_TO_CORE(coreID) 	// route
		);
  }

  /* south */
  if (route_from_south)
  {
    rtr_mc_set (e + 2,     		// entry
		south_key,            	// key
		0xffffffff,           	// mask
		ROUTE_TO_CORE(coreID) 	// route
		);
  }

  /* east */
  if (route_from_east)
  {
    rtr_mc_set (e + 3,     		// entry
		east_key,             	// key
		0xffffffff,           	// mask
		ROUTE_TO_CORE(coreID) 	// route
		);
  }

  /* west */
  if (route_from_west)
  {
    rtr_mc_set (e + 4,     		// entry
		west_key,             	// key
		0xffffffff,           	// mask
		ROUTE_TO_CORE(coreID) 	// route
		);
  }

  /* ------------------------------------------------------------------- */
  /* initialise routing entries for host data and commands               */
  /* ------------------------------------------------------------------- */
  /* avoid duplication                                                   */
  if (leadAp)
  {
    // setup routing entries for synchronization barrier
    // setup the local (on-chip) routes
    uint loc_route = core_map[my_x][my_y] & 0xfffffffe;

    // TODO: needs fixing -- fault-tolerant tree
    // setup off-chip routes -- check for borders!
    uint off_route = 0;

    // north
    if ((my_x == 0) && (my_y < (NUMBER_OF_YCHIPS - 1)) &&
        ((core_map[my_x][my_y + 1] & 0xfffffffe) != 0))
    {
      off_route |= ROUTE_TO_LINK(NORTH_LINK);
    }

    // east
    if ((my_x < (NUMBER_OF_XCHIPS - 1)) && (my_y == 0) &&
        ((core_map[my_x + 1][my_y] & 0xfffffffe) != 0))
    {
      off_route |= ROUTE_TO_LINK(EAST_LINK);
    }

    // north-east
    if ((my_y < (NUMBER_OF_YCHIPS - 1)) && (my_x < (NUMBER_OF_XCHIPS - 1)) &&
        ((core_map[my_x + 1][my_y + 1] & 0xfffffffe) != 0))
    {
      off_route |= ROUTE_TO_LINK(NORTH_EAST_LINK);
    }

    uint e = rtr_alloc (5);
    if (e == 0)
      rt_error (RTE_ABORT);

    // command entry (matches any command -- not just STOP!)
    rtr_mc_set (e,                           	// entry
		STOP_KEY,                       // key
		CMD_MASK,                       // mask
		((loc_route << 6) | off_route)  // route
		);

    // northern border temperature
    if (IS_NORTHERNMOST_CHIP(my_x, my_y))
    {
      // send only to local northern-most cores
      rtr_mc_set (e+1,                         	// entry
		  TEMP_NORTH_KEY,              	// key
		  0xffffffff,                  	// mask
		  ((loc_route & 0x11110) << 6 | off_route) // route
		  );
    }
    else
    {
      // send only to other chips
      rtr_mc_set (e+1,                         	// entry
		  TEMP_NORTH_KEY,              	// key
		  0xffffffff,                   // mask
		  off_route                     // route
		  );
    }

    // eastern border temperature
    if (IS_EASTERNMOST_CHIP(my_x, my_y))
    {
      // send only to local eastern-most cores
      rtr_mc_set (e+2,                         	// entry
		  TEMP_EAST_KEY,                // key
		  0xffffffff,                   // mask
		  ((loc_route & 0x1e000) << 6 | off_route) // route
		  );
    }
    else
    {
      // send only to other chips
      rtr_mc_set (e+2,                         	// entry
		  TEMP_EAST_KEY,                // key
		  0xffffffff,                   // mask
		  off_route                     // route
		  );
    }

    // southern border temperature
    // NOTE: currently temperatures never travel south!
    if (IS_SOUTHERNMOST_CHIP(my_x, my_y))
    {
      // send only to local southern-most cores
      rtr_mc_set (e+3,                         	// entry
		  TEMP_SOUTH_KEY,               // key
		  0xffffffff,                   // mask
		  ((loc_route & 0x02222) << 6 | off_route) // route
		  );
    }
    else
    {
      // send only to other chips
      rtr_mc_set (e+3,                         	// entry
		  TEMP_SOUTH_KEY,               // key
		  0xffffffff,                   // mask
		  off_route                     // route
		  );
    }

    // western border temperature
    // NOTE: currently temperatures never travel west!
    if (IS_WESTERNMOST_CHIP(my_x, my_y))
    {
      // send only to local western-most cores
      rtr_mc_set (e+4,                          // entry
		  TEMP_WEST_KEY,                // key
		  0xffffffff,                   // mask
		  ((loc_route & 0x0001e) << 6 | off_route) // route
		  );
    }
    else
    {
      // send only to other chips
      rtr_mc_set (e+4,                         	// entry
		  TEMP_WEST_KEY,                // key
		  0xffffffff,                   // mask
		  off_route                     // route
		  );
    }

    // configure router to use emergency routing
    rtr_conf = rtr[RTR_CONTROL];  // save for later
    rtr[RTR_CONTROL] = (rtr_conf & 0x0000ffff) | 0x31310000;
  }
}
/*
*******/


/****f* heat_demo.c/send_temps_to_host
*
* SUMMARY
*  This function is called at application exit.
*  It's used to report the final temperatures to the host
*
* SYNOPSIS
*  void send_temps_to_host ()
*
* SOURCE
*/
void send_temps_to_host ()
{
  // copy temperatures into msg buffer and set message length
  uint len = 16 * sizeof(uint);
  spin1_memcpy (my_msg.data, (void *) core_temp, len);
  my_msg.length = sizeof (sdp_hdr_t) + sizeof (cmd_hdr_t) + len;

  // and send SDP message!
  (void) spin1_send_sdp_msg (&my_msg, 100); // 100ms timeout
}
/*
*******/


/****f* heat_demo.c/sdp_init
*
* SUMMARY
*  This function is used to initialise SDP message buffer
*
* SYNOPSIS
*  void sdp_init ()
*
* SOURCE
*/
void sdp_init ()
{
  // fill in SDP destination fields
  my_msg.tag = 1;                    // IPTag 1
  my_msg.dest_port = PORT_ETH;       // Ethernet
  my_msg.dest_addr = sv->eth_addr;   // Nearest Ethernet chip

  // fill in SDP source & flag fields
  my_msg.flags = 0x07;
  my_msg.srce_port = coreID;
  my_msg.srce_addr = sv->p2p_addr;
}
/*
*******/


/****f* heat_demo.c/report_temp
*
* SUMMARY
*  This function is used to report current temp
*
* SYNOPSIS
*  void report_temp (uint ticks)
*
* SOURCE
*/
void report_temp (uint ticks)
{
  // report temperature in shared memory
  core_temp[coreID - 1] = my_temp;

  // send results to host
  // only the lead application core does this
  if (leadAp)
  {
    // spread out the reporting to avoid SDP packet drop
    //##    if ((ticks % (NUMBER_OF_XCHIPS * NUMBER_OF_YCHIPS)) == my_chip)
    if ((ticks % 64) == board_loc)
    {
      send_temps_to_host();
    }
  }
}
/*
*******/


/****f* heat_demo.c/report_results
*
* SUMMARY
*  This function is called at application exit.
*  It's used to report some statistics and say goodbye.
*
* SYNOPSIS
*  void report_results ()
*
* SOURCE
*/
void report_results ()
{
  /* report temperature in shared memory */
  core_temp[coreID - 1] = my_temp;

  /* report final temperature */
//  /* skew io_printfs to avoid congesting tubotron */
//  spin1_delay_us (200 * ((chipID << 5) + coreID));

  io_printf (IO_BUF, "T = %7.3f\n", my_temp);
}
/*
*******/


/****f* heat_demo.c/receive_data
*
* SUMMARY
*  This function is used as a callback for packet received events.
* receives data from 4 (NSEW) neighbours and updates the checklist
*
* SYNOPSIS
*  void receive_data (uint key, uint payload)
*
* INPUTS
*   uint key: packet routing key - provided by the RTS
*   uint payload: packet payload - provided by the RTS
*
* SOURCE
*/
void receive_data (uint key, uint payload)
{
  sark.vcpu->user1++;

  #ifdef DEBUG
    dbg_keys_recv[dbg_packs_recv++] = key;
    if (dbg_packs_recv == DEBUG_KEYS)
    {
      dbg_packs_recv = 0;
    }
  #endif

  if (key == north_key)
  {
    if (arrived[now] & NORTH_ARRIVED)
    {
      neighbours_temp[next][NORTH] = payload;
      arrived[next] |= NORTH_ARRIVED;
    }
    else
    {
      neighbours_temp[now][NORTH] = payload;
      arrived[now] |= NORTH_ARRIVED;
    }
  }
  else if (key == south_key)
  {
    if (arrived[now] & SOUTH_ARRIVED)
    {
      neighbours_temp[next][SOUTH] = payload;
      arrived[next] |= SOUTH_ARRIVED;
    }
    else
    {
      neighbours_temp[now][SOUTH] = payload;
      arrived[now] |= SOUTH_ARRIVED;
    }
  }
  else if (key == east_key)
  {
    if (arrived[now] & EAST_ARRIVED)
    {
      neighbours_temp[next][EAST] = payload;
      arrived[next] |= EAST_ARRIVED;
    }
    else
    {
      neighbours_temp[now][EAST] = payload;
      arrived[now] |= EAST_ARRIVED;
    }
  }
  else if (key == west_key)
  {
    if (arrived[now] & WEST_ARRIVED)
    {
      neighbours_temp[next][WEST] = payload;
      arrived[next] |= WEST_ARRIVED;
    }
    else
    {
      neighbours_temp[now][WEST] = payload;
      arrived[now] |= WEST_ARRIVED;
    }
  }
  else if (key == TEMP_NORTH_KEY)
  {
    if ((IS_NORTHERNMOST_CHIP(my_x, my_y)) &&
        (IS_NORTHERNMOST_CORE(coreID))
       )
    {
      neighbours_temp[now][NORTH]  = payload;
      neighbours_temp[next][NORTH] = payload;
    }
  }
  else if (key == TEMP_EAST_KEY)
  {
    if ((IS_EASTERNMOST_CHIP(my_x, my_y)) &&
        (IS_EASTERNMOST_CORE(coreID))
       )
    {
      neighbours_temp[now][EAST]  = payload;
      neighbours_temp[next][EAST] = payload;
    }
  }
  else if (key == TEMP_SOUTH_KEY)
  {
    if ((IS_SOUTHERNMOST_CHIP(my_x, my_y)) &&
        (IS_SOUTHERNMOST_CORE(coreID))
       )
    {
      neighbours_temp[now][SOUTH]  = payload;
      neighbours_temp[next][SOUTH] = payload;
    }
  }
  else if (key == TEMP_WEST_KEY)
  {
    if ((IS_WESTERNMOST_CHIP(my_x, my_y)) &&
        (IS_WESTERNMOST_CORE(coreID))
       )
    {
      neighbours_temp[now][WEST]  = payload;
      neighbours_temp[next][WEST] = payload;
    }
  }
  else if (key == STOP_KEY)
  {
    spin1_exit (0);
  }
  else if (key == PAUSE_KEY)
  {
    updating = FALSE;
  }
  else if (key == RESUME_KEY)
  {
    updating = TRUE;
  }
  else
  {
    // unexpected packet!
    #ifdef DEBUG
      io_printf (IO_STD, "!\n");
    #endif
  }
}
/*
*******/


/****f* heat_demo.c/send_first_value
*
* SUMMARY
*
* SYNOPSIS
*  void send_first_value (uint a, uint b)
*
* SOURCE
*/
void send_first_value (uint a, uint b)
{
    /* send data to neighbours */
    spin1_send_mc_packet(my_key, my_temp, WITH_PAYLOAD);
}
/*
*******/


/****f* heat_demo.c/update
*
* SUMMARY
*
* SYNOPSIS
*  void update (uint ticks, uint b)
*
* SOURCE
*/
void update (uint ticks, uint b)
{
  sark.vcpu->user0++;

  if (updating)
  {
    /* report if not all neighbours' data arrived */
    #ifdef DEBUG
      if (arrived[now] != ALL_ARRIVED)
      {
        io_printf (IO_STD, "@\n");
        dbg_timeouts++;
      }
    #endif

    // if a core does not receive temperature from a neighbour
    // it uses it's own as an estimate for the nieghbour's.
    if (arrived[now] != ALL_ARRIVED)
    {
      if (!(arrived[now] & NORTH_ARRIVED))
      {
        neighbours_temp[now][NORTH] = my_temp;
      }

      if (!(arrived[now] & SOUTH_ARRIVED))
      {
        neighbours_temp[now][SOUTH] = my_temp;
      }

      if (!(arrived[now] & EAST_ARRIVED))
      {
        neighbours_temp[now][EAST] = my_temp;
      }

      if (!(arrived[now] & WEST_ARRIVED))
      {
        neighbours_temp[now][WEST] = my_temp;
      }
    }

    /* compute new temperature */
    /* adjust for 16.16 fixed-point representation  */
    int tmp1 = neighbours_temp[now][EAST] + neighbours_temp[now][WEST]
               - 2 * my_temp;
    int tmp2 = neighbours_temp[now][NORTH] + neighbours_temp[now][SOUTH]
               - 2 * my_temp;
    /* adjust for 16.16 fixed-point representation  */
    int tmp3 = (int) (((long long) cx_adj * (long long) tmp1) >> 16);
    int tmp4 = (int) (((long long) cy_adj * (long long) tmp2) >> 16);
    my_temp = my_temp + tmp3 + tmp4;

    #ifdef POSITIVE_TEMP
      // avoids a problem with negative temps in the visualiser!
      my_temp = (my_temp > 0) ? my_temp : 0;
    #endif

    /* send new data to neighbours */
    spin1_send_mc_packet(my_key, my_temp, WITH_PAYLOAD);

    /* prepare for next iteration */
    arrived[now] = init_arrived;
    now  = 1 - now;
    next = 1 - next;

    /* report current temp */
    report_temp(ticks);
  }
}
/*
*******/


/****f* heat_demo.c/host_data
*
* SUMMARY
*
* SYNOPSIS
*  void host_data (uint *mailbox, uint port)
*
* INPUTS
*   uint mailbox: mailbox where the message is stored
*   uint port: destination port of the SDP message
*
* SOURCE
*/
void host_data (uint mailbox, uint port)
{
  sdp_msg_t *msg = (sdp_msg_t *) mailbox;
  uint *data = (uint *) msg->data;

  #ifdef DEBUG
    io_printf (IO_STD, "cmd: %d\n", msg->cmd_rc);
    if (msg->cmd_rc == 1)
    {
      io_printf (IO_STD, "N: %7.3f\n", data[0]);
      io_printf (IO_STD, "E: %7.3f\n", data[1]);
      io_printf (IO_STD, "S: %7.3f\n", data[2]);
      io_printf (IO_STD, "W: %7.3f\n", data[3]);
    }
  #endif

  switch (msg->cmd_rc)
  {
    case 0: // stop
      spin1_send_mc_packet(STOP_KEY, 0, NO_PAYLOAD);
      break;

    case 1: // new border temperatures
      spin1_send_mc_packet(TEMP_NORTH_KEY, data[0], WITH_PAYLOAD);
      spin1_send_mc_packet(TEMP_EAST_KEY,  data[1], WITH_PAYLOAD);
      spin1_send_mc_packet(TEMP_SOUTH_KEY, data[2], WITH_PAYLOAD);
      spin1_send_mc_packet(TEMP_WEST_KEY,  data[3], WITH_PAYLOAD);
      break;

    case 2: // pause
      spin1_send_mc_packet(PAUSE_KEY, 0, NO_PAYLOAD);
      break;

    case 3: // resume
      spin1_send_mc_packet(RESUME_KEY, 0, NO_PAYLOAD);
      break;

    default:
      // unexpected packet!
      #ifdef DEBUG
        io_printf (IO_STD, "!SDP\n");
      #endif
      break;
  }

  spin1_msg_free (msg);
}
/*
*******/


/****f* heat_demo.c/c_main
*
* SUMMARY
*  This function is called at application start-up.
*  It is used to register event callbacks and begin the simulation.
*
* SYNOPSIS
*  int c_main()
*
* SOURCE
*/
void c_main()
{
  #ifdef VERBOSE
    // say hello
    io_printf (IO_BUF, "starting heat_demo\n");
  #endif

  // get this core's ID
  coreID = spin1_get_core_id();
  chipID = spin1_get_chip_id();

  // get this chip's coordinates for core map
  my_x = chipID >> 8;
  my_y = chipID & 0xff;
  my_chip = (my_x * NUMBER_OF_YCHIPS) + my_y;

  board_loc = ((sv->board_addr >> 5) | sv->board_addr) & 63;

  // operate only if in core map!
  if ((my_x < NUMBER_OF_XCHIPS) && (my_y < NUMBER_OF_YCHIPS)
       && ((core_map[my_x][my_y] & (1 << coreID)) != 0)
     )
  {
    // set the core map for the simulation
    //##    spin1_application_core_map(NUMBER_OF_XCHIPS, NUMBER_OF_YCHIPS, core_map);

    // set timer tick value to 1ms (in microseconds)
    // slow down simulation to alow users to appreciate changes
    spin1_set_timer_tick (TIMER_TICK_PERIOD);

    // register callbacks
    spin1_callback_on (MCPL_PACKET_RECEIVED, receive_data, 0);
    spin1_callback_on (TIMER_TICK, update, 0);
    spin1_callback_on (SDP_PACKET_RX, host_data, 0);

    // initialise routing tables
    routing_table_init ();

    // initialise SDP message buffer
    sdp_init ();

    // initialise temperatures (for absent cores!)

    core_temp = (volatile int *) sv->sysram_base; //##

    if (leadAp)
    {
      for (uint i = 1; i <= 16; i++)
      {
        core_temp[i - 1] = 0;
      }
    }

    #ifdef DEBUG
      // initialise variables
      dbg_keys_recv = spin1_malloc(DEBUG_KEYS * 4 * sizeof(uint));
      // record start time somewhere in SDRAM
      dbg_stime = (uint *) (SPINN_SDRAM_BASE + 4 * coreID);
      *dbg_stime = sv->clock_ms;
    #endif

    // kick-start the update process
    spin1_schedule_callback(send_first_value, 0, 0, 3);

    // go
    spin1_start (SYNC_WAIT);	//##

    // restore router configuration
    rtr[RTR_CONTROL] = rtr_conf;

    #ifdef VERBOSE
      // report results
      report_results();
    #endif
  }

  #ifdef VERBOSE
    // say goodbye
    io_printf (IO_BUF, "stopping heat_demo\n");
  #endif
}
/*
*******/
