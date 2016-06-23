
//------------------------------------------------------------------------------
//
// sark_hw.c	    Spinnaker hardware/peripheral support routines
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#include <sark.h>

//------------------------------------------------------------------------------

extern void null_int_han (void);
extern void vic_error (void);


void sark_vic_init (void)
{
  vic[VIC_DISABLE] = 0xffffffff; 		// Disable all
  vic[VIC_SELECT] = 0; 				// All use IRQ
  vic[VIC_VADDR] = 0;				// Ack just in case...

  for (uint i = 0; i < 16; i++)  		// Clear all entries
    {
      vic[VIC_CNTL + i] = 0;
      vic[VIC_ADDR + i] = (uint) vic_error;
    }

  // Default handler (fixes VIC 'feature'!)

  vic[VIC_DEFADDR] = (uint) null_int_han;
}


void sark_vic_set (vic_slot slot, uint interrupt, uint enable,
		   int_handler handler)
{
  if (slot == SLOT_FIQ)
    {
      sark_vec->fiq_vec = handler;
      vic[VIC_SELECT] = 1 << interrupt;
    }
  else
    {
      vic[VIC_ADDR + slot] = (uint) handler;
      vic[VIC_CNTL + slot] = 0x20 | interrupt;
    }

  if (enable)
    vic[VIC_ENABLE] = 1 << interrupt;
}


//------------------------------------------------------------------------------


uint v2p_mask (uint virt_mask)
{
  uint phys_mask = 0;

  for (uint i = 0; i < NUM_CPUS; i++)
    if (virt_mask & (1 << i))
      phys_mask |= 1 << sv->v2p_map[i];

  return phys_mask;
}


//------------------------------------------------------------------------------

// Set up GPIO port for LED driving

#if 0	// Superceded...

const uchar s0_leds[] = {0}; // Dummy
const uchar s1_leds[] = {4, 0x01, 0x02, 0x40, 0x80};
const uchar s2_leds[] = {3, 0x01, 0x02, 0x40};
const uchar s3_leds[] = {2, 0x01, 0x20};
const uchar s4_leds[] = {1, 0x01};

const uchar * const led_list[] = {s0_leds, s1_leds, s2_leds, s3_leds, s4_leds};
const uchar led_bits[] = {0x00, 0xc3, 0x43, 0x21, 0x01};

void led_init (void)
{
  uint hw_ver = sv->hw_ver;

  if (hw_ver < HW_VER_MIN || hw_ver > HW_VER_MAX)
    hw_ver = 0;

  sark_memcpy (sv->leds, led_list[hw_ver], 8);  // Point to appropriate table
  sc[GPIO_SET] = led_bits[hw_ver];              // Turn LEDs off
  sc[GPIO_DIR] &= ~led_bits[hw_ver];            // Clear bits in GPIO_DIR
}
#endif


static uint get4 (uint *word, uint index)
{
  uint w = index / 8;
  uint i = index & 7;
  return (word[w] >> (4 * i)) & 15;
}


void sark_led_init (void)
{
  uint count = get4 (&sv->led0, 0);
  uint mask = 0;

  for (uint i = 1; i <= count; i++)
    mask |= 1 << get4 (&sv->led0, i);

  sc[GPIO_SET] = mask;		// Turn LEDs off
  sc[GPIO_DIR] &= ~mask;	// Clear bits in GPIO_DIR
}


// Configure one or more LEDs using LED_XX macros

void sark_led_set (uint leds)
{
  uint count = get4 (&sv->led0, 0);

  for (uint i = 1; i <= count; i++)
    {
      uint led = 1 << get4 (&sv->led0, i);
      uint v = leds & 1;
      uint c = leds & 2;

      leds = leds >> 2;

      if (c == 0)
	{
	  if (v != 0)
	    v = sc[GPIO_PORT] & led;
	  else
	    continue;
	}

      if (v)
	sc[GPIO_CLR] = led;
      else
	sc[GPIO_SET] = led;
    }
}


//------------------------------------------------------------------------------

// Initialise PL340 SDRAM controller

// Micron 166MHz SDRAM - 1Gb - 128MB
// Config = 0x00018012
// Refresh period 7.8us

const ushort pl340_data[] = {
  0x0006,  	// [0]  MC_CASL CAS latency =3
  0x0001,  	// [1]  MC_DQSS T_dqss
  0x0002,  	// [2]  MC_MRD  T_mrd
  0x0007,  	// [3]  MC_RAS  T_ras
  0x000A,  	// [4]  MC_RC   T_rc
  0x0003,  	// [5]  MC_RCD  T_rcd
  0x0275,  	// [6]  MC_RFC  T_rfc
  0x0003,  	// [7]  MC_RP   T_rp
  0x0002,  	// [8]  MC_RRD  T_rrd
  0x0003,  	// [9]  MC_WR   T_wr
  0x0001,  	// [10] MC_WTR  T_wtr
  0x0005,  	// [11] MC_XP   T_xp
  0x0017,  	// [12] MC_XSR  T_xsr
  0x0014  	// [13] MC_ESR  T_esr
};


// Refresh count can be computed as 7.8 * memclk (MHz)
// 1300 for 165 MHz, 1014 for 130 MHz

uint pl340_init (uint mem_clk)
{
  uint refresh = (1997 * mem_clk) / 256; // * 7.8 !

  // Next 3 lines only really needed if re-initialising

  mc[MC_CMD] = 0x00000003;	// -> Pause
  mc[MC_CMD] = 0x00000004;	// -> Configure
  mc[DLL_CONFIG0] = 0;		// Disable DLL

  for (uint i = 0; i < 14; i++)
    mc[MC_CASL + i] = pl340_data[i];

  mc[MC_MCFG] = 0x00018012;	// Init Memory_Cfg

  mc[MC_REFP] = refresh;	// Init refresh_prd reg

  mc[MC_CCFG0] = 0x000060e0;	// Set config reg

  mc[MC_DIRC] = 0x000c0000;	// NOP
  mc[MC_DIRC] = 0x00000000;	// PRECHARGEALL
  mc[MC_DIRC] = 0x00040000;	// AUTOREFRESH
  mc[MC_DIRC] = 0x00040000;	// AUTOREFRESH

  mc[MC_DIRC] = 0x00080033;	// MODEREG (burst=8)

  mc[MC_DIRC] = 0x000a0000;	// EXTMODEREG

  mc[MC_CMD] = 0x00000000;	// Enable memory

  mc[DLL_CONFIG1] = 0x00000000;	// Clear fine-tune
  mc[DLL_CONFIG0] = 0x01000000;	// Enable DLL

  uint t = 100;	// Wait for lock with 100us timeout

  while ((mc[DLL_STATUS] & (1 << 18)) == 0 && t != 0)
    {
      sark_delay_us (1);
      t--;
    }

  return t;
}


//------------------------------------------------------------------------------


// Initialise router multicast tables to empty state


uint rtr_mc_clear (uint start, uint count)
{
  if (start + count > MC_TABLE_SIZE)
    return 0;

  rtr_entry_t *router = sv->rtr_copy;

  for (uint i = start; i < start + count; i++)
    {
      router[i].route  = rtr_ram[i]  = 0xff000000;
      router[i].mask   = rtr_mask[i] = 0;
      router[i].key    = rtr_key[i] =  0xffffffff;
    }

  return 1;
}


void rtr_mc_init (uint start)
{
  (void) rtr_mc_clear (start, MC_TABLE_SIZE-start);

  rtr_entry_t *router = sv->rtr_copy;

  //  for (uint i = 0; i < MC_TABLE_SIZE; i++)
  //    {
  //      router[i].next = router[i].free = 0;
  //    }

  sv->rtr_free = RTR_ALLOC_FIRST;

  // Set up allocation entries

  router[RTR_ALLOC_FIRST].next = RTR_ALLOC_LAST;
  router[RTR_ALLOC_FIRST].free = 0;
  router[RTR_ALLOC_LAST].next = 0;
  router[RTR_ALLOC_LAST].free = 0;

  // Clear out extra entry in copy

  router[MC_TABLE_SIZE].route = 0xff000000;
}


// Load router MC table from a table in memory. The "next" field is
// used to hold the entry number to be loaded and the "free" field (of
// the first entry) holds the count which is used if the count
// parameter is zero. The parameter offset is added to the entry
// number to address each router table entry
// If the "app_id" parameter is non-zero it is used to force the
// app_id in the copy table.

uint rtr_mc_load (rtr_entry_t *e, uint count, uint offset, uint app_id)
{
  if (count == 0)
    count = e->free;

  if (count > MC_TABLE_SIZE)
    return 0;

  app_id <<= 24;

  for (uint i = 0; i < count; i++)
    {
      uint entry = e->next + offset;

      if (entry >= MC_TABLE_SIZE)
	return 0;

      rtr_mc_set (entry, e->key, e->mask, app_id + e->route);
      e++;
    }

  return 1;
}


// Set a router multicast table entry 

uint rtr_mc_set (uint entry, uint key, uint mask, uint route)
{
  if (entry >= MC_TABLE_SIZE)
    return 0;

  rtr_entry_t *copy = sv->rtr_copy + entry;
  uint app_id = route >> 24;

  if (app_id == 0)
    app_id = sark_vec->app_id;

  route &= 0x00ffffff;

  copy->route = route;
  copy->free = 0x8000 + (sark.virt_cpu << 8) + app_id;

  uint phys_mask = v2p_mask (route >> NUM_LINKS);
  route &= (1 << NUM_LINKS) - 1;
  route |= phys_mask << NUM_LINKS;

  rtr_ram[entry] = route;

  copy->key = rtr_key[entry] = key;
  copy->mask = rtr_mask[entry] = mask;

  return 1;
}


// Get a router multicast table entry. Fills in a rtr_entry_t
// whose address is passed in. 

uint rtr_mc_get (uint entry, rtr_entry_t *r)
{
  if (entry >= MC_TABLE_SIZE)
    return 0;

  rtr_entry_t *copy = sv->rtr_copy + entry;

  r->route = copy->route & 0x00ffffff;
  r->mask = copy->mask;
  r->key = copy->key;

  return 1;
}

// Set router fixed-route entry 

void rtr_fr_set (uint route)
{
  uint t = rtr[RTR_FR] & 0xff000000;

  sv->fr_copy = route &= 0x00ffffff;

  uint phys_mask = v2p_mask (route >> NUM_LINKS);
  route &= (1 << NUM_LINKS) - 1;
  route |= phys_mask << NUM_LINKS;

  rtr[RTR_FR] = t | route;
}


// Get router fixed-route entry 

uint rtr_fr_get (void)
{
  return sv->fr_copy;
}


// Initialise router point-to-point tables (every entry set to 6)

void rtr_p2p_init ()
{
  for (uint i = 0; i < P2P_TABLE_SIZE; i++)
    rtr_p2p[i] = P2P_INIT;
}


// Set a router point-to-point table entry

void rtr_p2p_set (uint entry, uint value)
{
  uint word = entry >> P2P_LOG_EPW;

  if (word >= P2P_TABLE_SIZE)
    return;

  uint offset = P2P_BPE * (entry & P2P_EMASK);
  uint data = rtr_p2p[word];

  data &= ~(P2P_BMASK << offset);
  data |= value << offset;

  rtr_p2p[word] = data;
}


// Read a router point-to-point table entry

uint rtr_p2p_get (uint entry)
{
  uint word = entry >> P2P_LOG_EPW;

  if (word >= P2P_TABLE_SIZE)
    return 6;

  uint offset = P2P_BPE * (entry & P2P_EMASK);
  uint data = rtr_p2p[word];

  return (data >> offset) & P2P_BMASK;
}


static const uint dgf_init[] =
{
  0x01fe7cf1,   // 0  local MC packets
  0x01febcf1,   // 1  non-local MC packets
  0x01fe7cf2,   // 2  local P2P packets
  0x01febcf2,   // 3  non-local P2P packets
  0x01fe7cf4,   // 4  local NN packets
  0x01febcf4,   // 5  non-local NN packets
  0x01fe7cf8,   // 6  local FR packets
  0x01febcf8,   // 7  non-local FR packets
  0x0001fcf1,   // 8  dumped MC packets
  0x0001fcf2,   // 9  dumped P2P packets
  0x0001fcf4,   // 10 dumped NN packets
  0x0001fcf8,   // 11 dumped FR packets
  0x00000000,	// 12
  0x00000000,	// 13
  0x00000000,	// 14
  0x00000000	// 15
};


void rtr_diag_init (const uint *table)
{
  rtr[RTR_DGEN] = 0xffff0000; 	// Disable and clear all 16 counters

  volatile uint *rtr_dgf = &rtr[RTR_DGF0];

  for (uint i = 0; i < 16; i++)	// Copy filter registers
    rtr_dgf[i] = table[i];

  rtr[RTR_DGEN] = 0x0000ffff;	// Enable 16 counters
}


void rtr_init (uint monitor)
{
  rtr_p2p_init ();		// Clear P2P table
  rtr_mc_init (0);		// and MC tables
  rtr_diag_init (dgf_init);	// Set up diagnostic regs

  // Enable, MP=0, Wait1=256
  rtr[RTR_CONTROL] = 0x00400001 | (monitor << 8);
}


//------------------------------------------------------------------------------
