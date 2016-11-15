
//------------------------------------------------------------------------------
//
// sark_base.c	    SARK - Spinnaker Application Runtime Kernel
//
// Copyright (C)    The University of Manchester - 2010-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------


#include <sark.h>

#ifdef SARK_API

#include "spin1_api.h"
#include "spin1_api_params.h"

cback_t __attribute__((weak)) callback[NUM_EVENTS];

void __attribute__((weak)) schedule_sysmode (uchar event_id,
					     uint arg0, uint arg1) {}

#endif

//------------------------------------------------------------------------------

#define SARK_ID_STR		"SARK/SpiNNaker"

sark_data_t sark;

INT_HANDLER sark_int_han (void);
INT_HANDLER sark_fiq_han (void);

//------------------------------------------------------------------------------

// Simple RTE handler. Regs and state have been saved before we get
// here and if we return we do "cpu_shutdown". Can be replaced by
// user-provided alternative.

void __attribute__((weak)) rte_handler (uint code)
{
  sv->led_period = 8;
}


//------------------------------------------------------------------------------

// Return various IDs

// Chip ID is 16 bits - X in top 8, Y in bottom 8

uint sark_chip_id (void)
{
  return sv->p2p_addr;
}

// Core ID is virtual CPU number (in range 0..17)

uint sark_core_id (void)
{
  return sark.virt_cpu;
}

// App ID is in range 0..255

uint sark_app_id (void)
{
  return sark_vec->app_id;
}


//------------------------------------------------------------------------------

// Count the number of bits set in a word

uint sark_count_bits (uint word)
{
  uint count = 0;

  while (word != 0)
    {
      count++;
      word &= word - 1;
    }

  return count;
}

// Count the number of characters in a zero terminated string

uint sark_str_len (char *string)
{
  uint count = 0;

  while (*string++)
    count++;

  return count;
}

// Set the CPU state

void sark_cpu_state (cpu_state state)
{
  sark.vcpu->cpu_state = state;
}

//------------------------------------------------------------------------------


uint sark_app_raise (void)
{
  uint app_id = sark_vec->app_id;
  uchar *sema = &sv->app_data[app_id].sema;
  return sark_sema_raise (sema);
}


uint sark_app_lower (void)
{
  uint app_id = sark_vec->app_id;
  uchar *sema = &sv->app_data[app_id].sema;
  return sark_sema_lower (sema);
}


uint sark_app_sema (void)
{
  uint app_id = sark_vec->app_id;
  return sv->app_data[app_id].sema;
}


uint sark_app_cores (void)
{
  uint app_id = sark_vec->app_id;
  return sv->app_data[app_id].cores;
}


uint sark_app_lead (void)
{
  uint app_id = sark_vec->app_id;
  return sv->app_data[app_id].lead;
}


//------------------------------------------------------------------------------

// Record a "software error". Logs the file and line number of the most
// recent error and increments a count (to a maximum of 65535). Can cause
// a run time error if mode requires it otherwise just returns.

void sw_error_fl (sw_err_mode mode, char* file, uint line)
{
  sark.vcpu->sw_file = file;
  sark.vcpu->sw_line = line;

  if (sark.vcpu->sw_count != 0xffff)
    sark.vcpu->sw_count++;

  if (sark.virt_cpu != 0)
    sv->led_period = 16;

  if (mode == SW_RTE || (mode == SW_OPT && sark.sw_rte))
    rt_error (RTE_SWERR);
}


//------------------------------------------------------------------------------

#if 0

// Following routines have been replaced by more general alloc/free in
// "sark_alloc.c"

// Simple allocator which allocates linearly from base of heap. There is no
// matching free.

void *sark_alloc (uint count, uint size)
{
  uint bytes = count * size;
  uint words = (bytes + 3) / sizeof (uint);

  if (words == 0)
    return 0;

  uint *base = sark.heap_ptr;
  uint *next = sark.heap_ptr + words;

  if (next > sark.stack_base)
    return 0;

  sark.heap_ptr = next;
  return (void *) base;
}


// Returns number of bytes left in heap

uint sark_heap (void)
{
  return (uchar *) sark.stack_base - (uchar *) sark.heap_ptr;
}

#endif

//------------------------------------------------------------------------------


// Initialise a memory buffer into a linked list of "count" blocks of
// "size" bytes. "size" needs to be at least 4 and a multiple of 4 and
// the memory buffer needs to be at least "size * count" bytes. The
// first word of each block is a pointer to the next block. Returns a
// pointer to the last block.

void *sark_block_init (void *buf, uint count, uint size)
{
  mem_link_t *m = buf;

  while (--count)
    {
      buf = (uchar *) buf + size;
      m->next = buf;
      m = buf;
    }

  m->next = NULL;

  return buf;
}


// Get a free SDP message from our internal buffer pool. Returns pointer
// to message on success, NULL on failure.

sdp_msg_t *sark_msg_get (void)
{
  mem_block_t *root = &sark.msg_root;

  uint cpsr = cpu_int_disable ();

  mem_link_t *blk = root->free;

  if (blk != NULL)
    {
      root->free = blk->next;

      root->count++;
      if (root->count > root->max)
	root->max = root->count;
    }

  cpu_int_restore (cpsr);

  return (sdp_msg_t *) blk;
}


// Return an SDP message buffer to the free pool 

void sark_msg_free (sdp_msg_t *msg)
{
  mem_block_t *root = &sark.msg_root;

  uint cpsr = cpu_int_disable ();

  mem_link_t *m = (mem_link_t *) msg;

  m->next = root->free;
  root->free = m;

  root->count--;

  cpu_int_restore (cpsr);
}


// Generic call to get a buffer from a pool whose "root" is supplied.
// Returns pointer to buffer on success, NULL on failure.

void *sark_block_get (mem_block_t *root)
{
  uint cpsr = cpu_int_disable ();

  mem_link_t *blk = root->free;

  if (blk != NULL)
    {
      root->free = blk->next;

      root->count++;
      if (root->count > root->max)
	root->max = root->count;
    }

  cpu_int_restore (cpsr);

  return (void *) blk;
}


// Generic call to return a buffer from a pool whose "root" is supplied.

void sark_block_free (mem_block_t *root, void *blk)
{
  uint cpsr = cpu_int_disable ();

  mem_link_t *m = (mem_link_t *) blk;

  m->next = root->free;
  root->free = m;

  root->count--;

  cpu_int_restore (cpsr);
}


// Get a free SDP message from the shared SysRAM pool. Returns pointer
// to message on success, NULL on failure.

sdp_msg_t* sark_shmsg_get ()
{
  uint cpsr = sark_lock_get (LOCK_MSG);

  mem_block_t *root = &sv->shm_root;
  mem_link_t *blk = root->free;

  if (blk != NULL)
    {
      root->free = blk->next;

      root->count++;
      if (root->count > root->max)
	root->max = root->count;
    }

  sark_lock_free (cpsr, LOCK_MSG);

  return (sdp_msg_t *) blk;
}


// Return an SDP message buffer to the shared SysRAM pool 

void sark_shmsg_free (sdp_msg_t *msg)
{
  uint cpsr = sark_lock_get (LOCK_MSG);

  mem_block_t *root = &sv->shm_root;
  mem_link_t *m = (mem_link_t *) msg;

  m->next = root->free;
  root->free = m;

  root->count--;

  sark_lock_free (cpsr, LOCK_MSG);
}


#ifdef __GNUC__
extern void (*__init_array_start)();
extern void (*__init_array_end)();
void sark_call_cpp_constructors (void)
{
  // Loop through any C++ constructors that may be present and call them
  for (void (**p)() = &__init_array_start; p < &__init_array_end; p++)
  {
    (*p)();
  }
}
#else  // Not GCC: No C++ support
void sark_call_cpp_constructors (void)
{
  // Do nothing
}
#endif

//------------------------------------------------------------------------------

// "sark_init" sets up SARK stacks and some internal data structures.
// This is a weak declaration so that it may be replaced with another
// routine though not many systems will want to do this.

// "sark_config" (also weak - see sark_alib.s) can be used to preconfigure
// some parameters which are used here.

// Argument "stack" is the top of the SVC stack
// Return value is the mode (CPSR) which will be entered on return
// NB - arrive in SVC mode - must leave in same mode!

uint __attribute__((weak)) sark_init (uint *stack)
{
  // Set up 4 stacks

  uint *stack_top = stack;
  stack -= sark_vec->svc_stack; // SVC stack base

  stack = cpu_init_mode (stack, IMASK_ALL+MODE_IRQ, sark_vec->irq_stack);
  stack = cpu_init_mode (stack, IMASK_ALL+MODE_FIQ, sark_vec->fiq_stack);
  (void)  cpu_init_mode (stack, IMASK_ALL+MODE_SYS, 0);

  // Fill in "sark" data structure. The heap is all memory from the top
  // of the ZINIT section to the bottom of the stack area.

  sark.stack_base = stack_top - (sark_vec->stack_size >> 2);
  sark.heap_base = sark_vec->heap_base;

  sark.heap = sark_heap_init (sark.heap_base, sark.stack_base);

  sark.phys_cpu = dma[DMA_STAT] >> 24;
  sark.virt_cpu = sv->p2v_map[sark.phys_cpu];

  sark.cpu_clk = sv->cpu_clk;
  sark.random[0] = DEAD_WORD;

  // Set up SDP message buffers

  sdp_msg_t *msg_bufs = sark_alloc (sark_vec->num_msgs, sizeof (sdp_msg_t));

  sark.msg_root.free = (mem_link_t *) msg_bufs;
  sark_block_init (msg_bufs, sark_vec->num_msgs, sizeof (sdp_msg_t));

  // Initialise the "vcpu" fields for this virtual CPU. This is only
  // done if we are not the Monitor processor

  if (sark.phys_cpu != (sc[SC_MON_ID] & 31))
    {
      sark.vcpu = sv_vcpu + sark.virt_cpu;
      sark.sdram_buf = (void *) (sv->sdram_bufs +
				 sv->sysbuf_size * sark.virt_cpu);

      sark_word_set (sark.vcpu, 0, sizeof (vcpu_t) - 4 * sizeof (uint));
      sark.vcpu->cpu_state = CPU_STATE_WAIT;
      sark.vcpu->phys_cpu = sark.phys_cpu;

      sark.vcpu->app_id = sark_vec->app_id;
      sark.vcpu->time = sv->unix_time;
      sark.vcpu->sw_ver = SLLT_VER_NUM;
      sark_str_cpy (sark.vcpu->app_name, build_name);

      // Check software version number
      //   Major must match
      //   Minor must be LE
      //   Patch is ignored

      if ((SLLT_VER_NUM & 0x00ff0000) != (sv->sw_ver & 0x00ff0000) ||
	  (SLLT_VER_NUM & 0x0000ff00) > (sv->sw_ver & 0x0000ff00))
	rt_error (RTE_VER);
    }

  // Set up VIC - disable everything

  sark_vic_init ();

  // Install SARK interrupt handler and default packet handler

  sark_vic_set ((vic_slot) sark_vec->sark_slot, CPU_INT, 1, sark_int_han);
  sark_vic_set (SLOT_FIQ, CC_RDY_INT, 1, sark_fiq_han);

#ifdef SARK_EVENT
  event_alloc (sark_vec->num_events);
#endif

  // Finally return target mode

  return MODE_SYS;
}


void __attribute__((weak)) sark_pre_main (void)
{
  sark_call_cpp_constructors();
  sark_cpu_state (CPU_STATE_SARK);
}


void __attribute__((weak)) sark_post_main (void)
{
  sark_cpu_state (CPU_STATE_EXIT);
}


//------------------------------------------------------------------------------


// Send a SDP msg to the Monitor Processor

uint sark_msg_send (sdp_msg_t *msg, uint timeout)
{
  sdp_msg_t *shm_msg = sark_shmsg_get ();

  if (shm_msg == NULL)
    return 0;

  sark_msg_cpy (shm_msg, msg);

  sark.vcpu->mbox_mp_msg = shm_msg;
  sark.vcpu->mbox_mp_cmd = SHM_MSG;

  uint cpsr = sark_lock_get (LOCK_MBOX);

  uint t = sv->mbox_flags;

  sv->mbox_flags = t | (1 << sark.virt_cpu);

  if (t == 0)
    sc[SC_SET_IRQ] = SC_CODE + (1 << sv->v2p_map[0]);

  sark_lock_free (cpsr, LOCK_MBOX);

  // Timeout using bottom 32 bits of clock_ms!

  volatile uint *ms = (uint *) &sv->clock_ms;
  uint start = *ms;

  while (sark.vcpu->mbox_mp_cmd != SHM_IDLE)
    if (*ms - start > timeout)
      break;

  if (sark.vcpu->mbox_mp_cmd != SHM_IDLE)
    {
      sark_shmsg_free (shm_msg);
      return 0;
    }

  sark.msg_sent++;

  return 1;
}


//------------------------------------------------------------------------------


uint sark_cmd_ver (sdp_msg_t *msg)
{
  msg->arg1 = (sv->p2p_addr << 16) + (sark.phys_cpu << 8) + sark.virt_cpu;
  msg->arg2 = 0xffff0000 + SDP_BUF_SIZE;
  msg->arg3 = (uint) build_date;

  sark_str_cpy ((char *) msg->data, SARK_ID_STR);
  sark_str_cpy ((char *) msg->data + sizeof (SARK_ID_STR), SLLT_VER_STR);

  return 12 + sizeof (SARK_ID_STR) + sizeof (SLLT_VER_STR);
}


uint sark_cmd_read (sdp_msg_t *msg) // arg1=addr, arg2=len, arg3=type
{
  uint len = msg->arg2;
  uint type = msg->arg3;

  if (len > SDP_BUF_SIZE || type > TYPE_WORD)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  uint addr = msg->arg1;
  uchar *buffer = (uchar *) &msg->arg1;

  if (type == TYPE_BYTE)
    {
      uchar *mem = (uchar *) addr;
      uchar *buf = (uchar *) buffer;

      for (uint i = 0; i < len; i++)
	buf[i] = mem[i];
    }
  else if (type == TYPE_HALF)
    {
      ushort *mem = (ushort *) addr;
      ushort *buf = (ushort *) buffer;

      for (uint i = 0; i < len / 2; i++)
	buf[i] = mem[i];

    }
  else
    {
      uint *mem = (uint *) addr;
      uint *buf = (uint *) buffer;

      for (uint i = 0; i < len / 4; i++)
	buf[i] = mem[i];
    }

  return len;
}


uint sark_cmd_write (sdp_msg_t *msg) // arg1=addr, arg2=len, arg3=type
{
  uint len = msg->arg2;
  uint type = msg->arg3;

  if (len > SDP_BUF_SIZE || type > TYPE_WORD)
    {
      msg->cmd_rc = RC_ARG;
      return 0;
    }

  uint addr = msg->arg1;
  uchar *buffer = msg->data;

  if (type == TYPE_BYTE)
    {
      uchar *mem = (uchar*) addr;
      uchar *buf = (uchar*) buffer;

      for (uint i = 0; i < len; i++)
	mem[i] = buf[i];
    }
  else if (type == TYPE_HALF)
    {
      ushort* mem = (ushort*) addr;
      ushort* buf = (ushort*) buffer;

      for (uint i = 0; i < len / 2; i++)
	mem[i] = buf[i];
    }
  else
    {
      uint *mem = (uint *) addr;
      uint *buf = (uint *) buffer;

      for (uint i = 0; i < len / 4; i++)
	mem[i] = buf[i];
    }

  return 0;
}


uint sark_cmd_fill (sdp_msg_t *msg)
{
  sark_word_set ((void *) msg->arg1, msg->arg2, msg->arg3);
  return 0;
}


//------------------------------------------------------------------------------


static uint sark_debug (sdp_msg_t *msg)
{
  uint len = msg->length;

  if (len < 24) // !! const
    {
      msg->cmd_rc = RC_LEN;
      return 0;
    }

  uint t = msg->cmd_rc;
  msg->cmd_rc = RC_OK;

  if (t == CMD_VER)
    return sark_cmd_ver (msg);

  if (t == CMD_READ)
    return sark_cmd_read (msg);

  if (t == CMD_WRITE)
    return sark_cmd_write (msg);

  if (t == CMD_FILL)
    return sark_cmd_fill (msg);

  msg->cmd_rc = RC_CMD;
  return 0;
}


static void swap_hdr (sdp_msg_t *msg)
{
  uint dest_port = msg->dest_port;
  uint dest_addr = msg->dest_addr;

  msg->dest_port = msg->srce_port;
  msg->srce_port = dest_port;

  msg->dest_addr = msg->srce_addr;
  msg->srce_addr = dest_addr;
}


//------------------------------------------------------------------------------


#ifdef SARK_EVENT
void sark_wait (void)
{
  while (sark_vec->app_flags & (1 << APP_FLAG_WAIT))
    cpu_wfi ();
}
#endif


//------------------------------------------------------------------------------


// Interrupt handler for CPU interrupt from monitor processor
// Entry and exit are via wrapper code "sark_int_han" in "sark_alib.s"
// This handler executes in System mode with interrupts enabled.


void sark_int (void *pc)
{
  sc[SC_CLR_IRQ] = SC_CODE + (1 << sark.phys_cpu);	// Ack the interrupt

  uint cmd = sark.vcpu->mbox_ap_cmd;			// Get command code

  if (cmd == SHM_IDLE)
    return;

  uint data = (uint) sark.vcpu->mbox_ap_msg;		// and data word

  if (cmd == SHM_NOP)					// Send back PC if NOP
    sark.vcpu->mbox_ap_msg = pc;

  sark.vcpu->mbox_ap_cmd = SHM_IDLE;			// and go back to idle

#ifdef SARK_EVENT
  if (cmd == SHM_SIGNAL)
    switch (data)
      {
      case SIG_PAUSE:
	event_pause (1);
	return;

      case SIG_CONT:
	event_pause (0);
	return;

      case SIG_EXIT:
	event_stop (255);	// !! const
	return;

      case SIG_START:
	sark_vec->app_flags &= ~(1 << APP_FLAG_WAIT);
	return;

      case SIG_USR0:
      case SIG_USR1:
      case SIG_USR2:
      case SIG_USR3:
	event.signal = data;
	vic[VIC_SOFT_SET] = 1 << SARK_SIG_INT;
	return;

      case SIG_TIMER:
	vic[VIC_SOFT_SET] = 1 << TIMER1_INT;
	return;

      default:
	return;
      }
#endif

  if (cmd == SHM_MSG)
    {
      sdp_msg_t *shm_msg = (sdp_msg_t *) data;
      sdp_msg_t *msg = sark_msg_get ();

      if (msg != NULL)
	{
	  sark_msg_cpy (msg, shm_msg);
	  sark_shmsg_free (shm_msg);

	  sark.msg_rcvd++;

	  uint dp = msg->dest_port;

	  if ((dp & PORT_MASK) == 0) // Port 0 is for us
	    {
	      msg->length = 12 + sark_debug (msg);
	      swap_hdr (msg);

	      sark_msg_send (msg, 10);
	      sark_msg_free (msg);
	    }
	  else	  // else send msg to application...
	    {
	      if (sark_vec->api)
		{
#ifdef SARK_API
		  if (callback[SDP_PACKET_RX].cback != NULL)
		    {
		      uint cpsr = cpu_int_disable ();
		      schedule_sysmode (SDP_PACKET_RX, (uint) msg,
					dp >> PORT_SHIFT);
		      cpu_int_restore (cpsr);
		    }
		  else
		    sark_msg_free (msg);
#endif
		}
#ifdef SARK_EVENT
	      else
		{
		  if (vic[VIC_ENABLE] & (1 << SARK_MSG_INT))
		    {
		      event.msg = msg;
		      vic[VIC_SOFT_SET] = 1 << SARK_MSG_INT;
		    }
		  else
		    sark_msg_free (msg);
		}
#endif
	    }
	}
      else
	{
	  sark_shmsg_free (shm_msg);
	}
    }
}


//------------------------------------------------------------------------------
