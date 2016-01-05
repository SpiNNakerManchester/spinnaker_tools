//------------------------------------------------------------------------------
//
// sark_alloc.c	    Memory management routines for SARK
//
// Copyright (C)    The University of Manchester - 2009-2013
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------

#include <sark.h>

//------------------------------------------------------------------------------

// Allocate a memory block from the specified heap. The block will be
// tagged with the supplied "id" which must be <= 255. Returns NULL on
// failure (not enough memory, bad tag or tag in use).

// The flag parameter contains two flags in the bottom two bits. If bit 0 is
// set (ALLOC_LOCK), the heap manipulation is done behind a lock with
// interrupts disabled. If bit 1 is set (ALLOC_ID), the block is tagged with
// the AppID provided in bits 15:8 of the flag, otherwise the AppID of the
// current application is used.

// An 8 bit "tag" can be provided which is combined with the AppID
// and is stored in the "free" field while the block is allocated. If
// the tag is non-zero, the block is only allocated if the tag is not in use.
// The tag (and AppID) is stored in the "alloc_tag" table while the
// block is allocated.


void *sark_xalloc (heap_t *heap, uint size, uint tag, uint flag)
{
  if (size == 0 || tag > 255)
    return NULL;

  uint app_id = (flag & ALLOC_ID) ? (flag >> 8) : sark_vec->app_id;
  uint entry = (app_id << 8) + tag;

  if (tag != 0 && sv->alloc_tag[entry] != NULL)
    return NULL;

  // Word align and add overhead

  size = ((size + 3) & ~3) + sizeof (block_t);

  uint cpsr;

  if (flag & ALLOC_LOCK)
    cpsr = sark_lock_get (LOCK_HEAP);

  block_t *prev = NULL;
  block_t *free = heap->free;

  while (free != NULL)
    {
      block_t *next = free->next;
      block_t *new = (block_t *) ((uchar *) free + size);

      if (new <= free) // Request way too big?
	break;

      if (new > next) // Free block too small
	{
	  prev = free;
	  free = free->free;
	  continue;
	}

      if (new + 1 < next) // too big - split but don't make zero-size frag.
	{
	  new->next = next;
	  free->next = new;
	  new->free = free->free;
	  free->free = new;
	}

      if (prev)
	prev->free = free->free;
      else
	heap->free = free->free;

      heap->free_bytes -= size;

      if (flag & ALLOC_LOCK)
	sark_lock_free (cpsr, LOCK_HEAP);

      free->free = (block_t *) (0xffff0000 + entry);

      if (tag != 0)
	sv->alloc_tag[entry] = free + 1;

      return free + 1;
    }

  if (flag & ALLOC_LOCK)
    sark_lock_free (cpsr, LOCK_HEAP);

  return NULL;
}


//------------------------------------------------------------------------------

// Allocate a memory block from the SARK heap (in DTCM). Returns NULL on
// failure

void *sark_alloc (uint count, uint size)
{
  return sark_xalloc (sark.heap, count * size, 0, 0);
}


//------------------------------------------------------------------------------

// Free a memory block in the specified heap. If a non-zero lock is
// provided, the manipulation of the heap is done behind a lock. This
// will turn interrupts off for some time while the heap is searched.

void sark_xfree (heap_t *heap, void *ptr, uint flag)
{
  if (ptr == NULL)
    rt_error (RTE_NULL);

  uint cpsr;

  if (flag & ALLOC_LOCK)
    cpsr = sark_lock_get (LOCK_HEAP);

  block_t *block = (block_t *) ptr - 1;
  block_t *prev = NULL;
  block_t *next = heap->free;

  uint entry = (uint) block->free & 0xffff;

  if ((entry & 255) != 0)
    sv->alloc_tag[entry] = NULL;

  heap->free_bytes += (uchar *) block->next - (uchar *) block;

  // Scan free list to find free block higher than us

  while (next != NULL)
    {
      if (block < next)
	break;
      prev = next;
      next = next->free;
    }

  // Link into free list

  block->free = next;

  if (prev)
    prev->free = block;
  else
    heap->free = block;

  // Merge with previous and/or next blocks if possible

  if (next != NULL && block->next == next)
    {
      block->next = next->next;
      block->free = next->free;
    }

  if (prev != NULL && prev->next == block)
    {
      prev->next = block->next;
      prev->free = block->free;
    }

  if (flag & ALLOC_LOCK)
    sark_lock_free (cpsr, LOCK_HEAP);
}


//------------------------------------------------------------------------------

// Free a memory block in the SARK heap (in DTCM)

void sark_free (void *ptr)
{
  sark_xfree (sark.heap, ptr, 0);
}


//------------------------------------------------------------------------------

// Free all allocated blocks in the specified heap which are owned by the
// given "app_id". A lock parameter specifies if locking needed in the
// "sark_xfree" call. Returns number of blocks freed.

uint sark_xfree_id (heap_t *heap, uint app_id, uint lock)
{
  block_t *block = heap->first;
  uint count = 0;

  //## Do we need to lock all of this?

  while (block->next != NULL)
    {
      uint block_id = ((uint) block->free >> 8) & 255;

      if (app_id == block_id)
	{
	  sark_xfree (heap, block + 1, lock);
	  count++;
	}

      block = block->next;
    }

  return count;
}


//------------------------------------------------------------------------------

// Return the size of the largest free block in the supplied heap

uint sark_heap_max (heap_t *heap, uint flag)
{
  block_t *p = heap->free;
  uint max = 0;
  uint cpsr;

  if (flag & ALLOC_LOCK)
    cpsr = sark_lock_get (LOCK_HEAP);

  while (p != NULL)
    {
      if (p->next != NULL)
	{
	  uint free = (uchar *) p->next - (uchar *) p - sizeof (block_t);

	  if (free > max)
	    max = free;
	}

      p = p->free;
    }

  if (flag & ALLOC_LOCK)
    sark_lock_free (cpsr, LOCK_HEAP);

  return max;
}


//------------------------------------------------------------------------------

// Initialise an area of memory as a heap. Arguments are (uint)
// pointers to base and top of the area. Returns a pointer to the heap
// (same address as the base). Assumes the area is large enough to
// hold a minimal heap (needs minimum 32 bytes for zero size heap!).

heap_t *sark_heap_init (uint *base, uint *top)
{
  heap_t *heap = (heap_t *) base;

  block_t *first = (block_t *) heap->buffer;
  block_t *last = (block_t *) ((uchar *) top - sizeof (block_t));

  heap->free = heap->first = first;
  heap->last = first->next = last;
  heap->free_bytes = (uchar *) last - (uchar *) first - sizeof (block_t);

  last->next = NULL;
  first->free = NULL;

  last->free = NULL;	// Not really necessary

  return heap;
}


//------------------------------------------------------------------------------

// Allocate and free routines for entries in the router MC table.

// The first entry is reserved. This allows the use of zero to signal
// failure and NULL. The data structures are similar to those used in
// alloc/free above and stored in the router copy. The "free" field of
// allocated blocks is used to hold an "app_id".

// The data structures are initialised in "rtr_mc_init" in "sark_hw.c"

//------------------------------------------------------------------------------

// Allocate a block of MC table entries of given size. Returns index of
// first entry on success, zero on failure. The block is associated with
// the supplied AppID

uint rtr_alloc_id (uint size, uint app_id)
{
  if (size == 0)
    return 0;

  rtr_entry_t *router = sv->rtr_copy;

  uint cpsr = sark_lock_get (LOCK_RTR);

  uint prev = 0;
  uint free = sv->rtr_free;

  while (free != 0)
    {
      uint next = router[free].next;
      uint new = free + size;

      if (new > next) 	// Too small, keep looking
	{
	  prev = free;
	  free = router[free].free;
	  continue;
	}

      if (new < next)	// Too big, split
	{
	  router[new].next = next;
	  router[free].next = new;
	  router[new].free = router[free].free;
	  router[free].free = new;
	}

      if (prev != 0)
	router[prev].free = router[free].free;
      else
	sv->rtr_free = router[free].free;

      router[free].free = app_id | 0x8000;

      break;
    }

  sark_lock_free (cpsr, LOCK_RTR);

  return free;
}


uint rtr_alloc (uint size)
{
  return rtr_alloc_id (size, sark_vec->app_id);
}


//------------------------------------------------------------------------------

// Free a block of MC table entries which starts with the supplied
// entry. Argument clear causes the relevant router registers to be
// re-initialised.

void rtr_free (uint entry, uint clear)
{
  if (entry < RTR_ALLOC_FIRST || entry >= RTR_ALLOC_LAST)
    return;

  rtr_entry_t *router = sv->rtr_copy;

  uint cpsr = sark_lock_get (LOCK_RTR);

  uint prev = 0;
  uint next = sv->rtr_free;

  if (clear)
    {
      uint size = router[entry].next - entry;
      rtr_mc_clear (entry, size);
    }

  while (next != 0)
    {
      if (entry < next)
	break;
      prev = next;
      next = router[next].free;
    }

  router[entry].free = next;

  if (prev != 0)
    router[prev].free = entry;
  else
    sv->rtr_free = entry;

  if (next != 0 && router[entry].next == next)
    {
      router[entry].next = router[next].next;
      router[entry].free = router[next].free;
    }

  if (prev != 0 && router[prev].next == entry)
    {
      router[prev].next = router[entry].next;
      router[prev].free = router[entry].free;
    }

  sark_lock_free (cpsr, LOCK_RTR);
}

//------------------------------------------------------------------------------

// Free all allocated blocks in the router which are tagged with the
// given "app_id". Argument clear causes the relevant router registers
// to be re-initialised. Returns number of blocks freed.

#define FREE_MASK 0xe0ff

uint rtr_free_id (uint app_id, uint clear)
{
  rtr_entry_t *router = sv->rtr_copy;
  uint block = RTR_ALLOC_FIRST;
  uint count = 0;

  app_id |= 0x8000;	// Stored as "ushort"

  //## Do we need to lock all of this?

  while (router[block].next != 0)
    {
      if ((router[block].free & FREE_MASK) == app_id)
	{
	  rtr_free (block, clear);
	  count++;
	}

      block = router[block].next;
    }

  return count;
}

//------------------------------------------------------------------------------

// Return the size of the largest free block in the router heap

uint rtr_alloc_max (void)
{
  rtr_entry_t *router = sv->rtr_copy;
  uint block = sv->rtr_free;

  uint max = 0;

  uint cpsr = sark_lock_get (LOCK_HEAP);

  while (block != 0)
    {
      if (router[block].next != 0)
	{
	  uint free = router[block].next - block;

	  if (free > max)
	    max = free;
	}

      block = router[block].free;
    }

  sark_lock_free (cpsr, LOCK_HEAP);

  return max;
}

//------------------------------------------------------------------------------

// Get a pointer to a tagged allocation. If the "app_id" parameter is zero
// uses the core's app_id.

void *sark_tag_ptr (uint tag, uint app_id)
{
  if (app_id == 0)
    app_id = sark_vec->app_id;
  
  return (void *) sv->alloc_tag[(app_id << 8) + tag];
}

//------------------------------------------------------------------------------
