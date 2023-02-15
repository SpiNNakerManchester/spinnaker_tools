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
 * Copyright (c) 2017 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "spinnaker.h"
#include "sark.h"
#include "scamp.h"


//------------------------------------------------------------------------------

//! Copy executable image from SDRAM to ITCM and boot it
void img_cp_exe (void) {
    typedef void (*boot_img_t)(void);
    boot_img_t boot_img = (boot_img_t) ITCM_BASE;

    // start boot image DMA to top-half of DTCM,
    dma[DMA_ADRS] = (uint) SDRAM_BASE;
    dma[DMA_ADRT] = (uint) DTCM_BASE + 0x00008000;
    dma[DMA_DESC] = 1 << 24 | 4 << 21 | 0 << 19 | BOOT_IMAGE_SIZE;

    // start boot image DMA to ITCM,
    dma[DMA_ADRT] = (uint) ITCM_BASE;
    dma[DMA_DESC] = 1 << 24 | 4 << 21 | 0 << 19 | BOOT_IMAGE_SIZE;

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
