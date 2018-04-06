//------------------------------------------------------------------------------
//
// scamp-del.c	    SC&MP monitor delegation routines
//
// Copyright (C)    The University of Manchester - 2017, 2018
//
// Author           Luis Plana, APT Group, School of Computer Science
// Email            luis.plana@manchester.ac.uk
//
//------------------------------------------------------------------------------

#include "spinnaker.h"
#include "sark.h"
#include "scamp.h"


//------------------------------------------------------------------------------


void img_cp_exe (void) {
    typedef void (*boot_img_t)(void);
    boot_img_t boot_img = (boot_img_t) ITCM_BASE;

    // start boot image DMA to top-half of DTCM,
    dma[DMA_ADRS] = (uint) SDRAM_BASE;
    dma[DMA_ADRT] = (uint) DTCM_BASE + 0x00008000;
    dma[DMA_DESC] = 1 << 24 | 4 << 21 | 0 << 19 | 0x7100;

    // start boot image DMA to ITCM,
    dma[DMA_ADRT] = (uint) ITCM_BASE;
    dma[DMA_DESC] = 1 << 24 | 4 << 21 | 0 << 19 | 0x7100;

    // wait for DTCM DMA to complete,
    while (!(dma[DMA_STAT] & (1 << 10))) {
        continue;
    }

    // clear DMA transfer complete,
    dma[DMA_CTRL] = 1 << 3;

    // wait for ITCM DMA to complete,
    while (!(dma[DMA_STAT] & (1 << 10))) {
        continue;
    }

    // clear DMA transfer complete,
    dma[DMA_CTRL] = 1 << 3;

    // and jump to boot image
    boot_img();
}


//------------------------------------------------------------------------------
