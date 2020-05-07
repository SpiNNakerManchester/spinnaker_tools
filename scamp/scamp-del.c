//------------------------------------------------------------------------------
//! \file
//! \brief     SC&MP monitor delegation routines
//!
//! \copyright &copy; The University of Manchester - 2017-2019
//!
//! \author    Luis Plana, APT Group, School of Computer Science
//! Email:     luis.plana@manchester.ac.uk
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2017-2019 The University of Manchester
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
