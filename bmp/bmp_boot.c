//------------------------------------------------------------------------------
//
// bmp_boot.c	    System bootstrap code for BMP LPC1768
//
// Copyright (C)    The University of Manchester - 2014-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------
//
// History
//
// V1.00 - 30jul14 - initial release to separate boot function from application
// V1.01 - 05aug14 - added support for WDT
// V1.02 - 07aug14 - pass cortex_vec to application
//
//------------------------------------------------------------------------------


#include "lpc17xx.h"
#include "bmp.h"


//------------------------------------------------------------------------------

// Linker generated symbols

#define RO_LENGTH 	Image$$BOOT$$RO$$Length
#define STACK_LIMIT 	Image$$STACK$$ZI$$Limit

extern uint32_t RO_LENGTH;	// Size of this boot image
extern uint32_t STACK_LIMIT;	// Stack top

// Fudge to force 4 byte alignment

const uint32_t pad __attribute__ ((section (".padding"))) = 0xdeaddead;

// LED definitions

static const uint32_t led_bit[] = {LED_3, LED_4, LED_5, LED_6};


//------------------------------------------------------------------------------


// Curl up and die. The top LED is turned on (Red on Spin5) and the
// code is put on the next 4 LEDs (Green on Spin5).

// This is usually called from within code in this source file in which
// case the WDT is inactive and "die" will never exit. When "flash_copy"
// is called from the main code and the WDT is active then the WDT will
// trigger after the WDT period.


static void __attribute__((noreturn)) die(uint32_t code)
{
    __disable_irq();

    uint32_t bits = LED_7;

    for (uint32_t i = 0; i < 4; i++) {
	if (code & (1 << i)) {
	    bits |= led_bit[i];
	}
    }

    LPC_GPIO0->FIOCLR = LED_MASK;
    LPC_GPIO0->FIOSET = bits;

    while (1) {
	continue;
    }
}


//------------------------------------------------------------------------------


// Reset (boot) handler

void __attribute__((noreturn)) boot_proc(void)
{
    // Set some LEDs (code for die(10))
    LPC_GPIO0->FIODIR = LED_MASK;
    LPC_GPIO0->FIOSET = LED_7 + LED_6 + LED_4;

    // Set up system clocks, etc
    configure_clocks();

    // Check CRC of Boot section of Flash (at address 0)

    uint32_t boot_size = (uint32_t) &RO_LENGTH;

    if (crc32_chk(boot_vec, boot_size + 4) != 0) {
	die(14);
    }

    // Check CRC of Code/Data section of Flash. Four 64KB blocks are
    // used, starting at 0x10000. They are checked in sequence until
    // one with a valid CRC is found.

    uint32_t boot_sec = 1;		// Boot block number
    cortex_vec_t *cortex_vec;		// Cortex boot vector

    while (1) {
	cortex_vec = (cortex_vec_t *) (boot_sec * 0x10000);

	uint32_t length = cortex_vec->RO_length + cortex_vec->RW_length + 4;

	// Check length < 256KB
	if (length < 0x40000 && crc32_chk(cortex_vec, length) == 0) {
	    break;
	}
	if (boot_sec++ == 4) {
	    die(13);
	}
    }

    // Check CRC of Data sector unless blank. If CRC is bad we leave
    // the top red LED on but continue nonetheless.
    bool data_ok = false;

    LPC_GPIO0->FIOCLR = LED_6 + LED_5 + LED_4 + LED_3;

    if (is_blank((void *) 0x1000, 4096) ||
	    crc32_chk((void *) 0x1000, 4096) == 0) {
	data_ok = true;
	LPC_GPIO0->FIOCLR = LED_7;
    }

    // Copy RW/DATA section from flash to RAM
    uint32_t *from = cortex_vec->RO_limit;
    uint32_t *base = cortex_vec->RW_base;
    uint32_t *limit = cortex_vec->RW_limit;
    while (base < limit) {
	*base++ = *from++;
    }

    // Clear ZI/BSS section
    base = cortex_vec->ZI_base;
    limit = cortex_vec->ZI_limit;
    while (base < limit) {
	*base++ = 0;
    }

    // Fill stack with 0xdeaddead
    base = cortex_vec->stack_base;
    limit = cortex_vec->stack_limit;
    while (base < limit) {
	*base++ = 0xdeaddead;
    }

    // Initialise heap if needed
    //     heap_init(cortex_vec->ZI_limit, cortex_vec->stack_base);

    // Set Cortex vectors at start of image
    SCB->VTOR = (uint32_t) cortex_vec;

    // Call c_main...
    cortex_vec->main(cortex_vec, data_ok, 0, 0);

    while (1) {
        die(11);
    }
}


//------------------------------------------------------------------------------

// Self-contained flash copy code


// Copy memory by words

static void mem_copy(uint32_t *to, uint32_t *from, uint32_t n)
{
    while (n--) {
	*to++ = *from++;
    }
}


// Refresh WDT

static void refresh_wdt(void)
{
    LPC_WDT->WDFEED = 0xaa;
    LPC_WDT->WDFEED = 0x55;
}


static void __attribute__ ((section (".flash_copy")))
flash_copy(uint32_t to, uint32_t from, int32_t size, int32_t arg4)
{
    uint32_t *flash_srce = (uint32_t *) from;
    uint32_t flash_dest = to;

    __disable_irq();

    flash_erase(flash_dest, flash_dest + size - 1);

    while (size > 0) {
	refresh_wdt();

	mem_copy(flash_buf, flash_srce, FLASH_WORDS);

	flash_write(flash_dest, FLASH_BYTES, flash_buf);

	flash_srce += FLASH_WORDS;
	flash_dest += FLASH_BYTES;
	size -= FLASH_BYTES;
    }

    die(15);	// WDT should reset now...
}


//------------------------------------------------------------------------------


// Cortex M3 vector table

// This short one lives at the bottom of flash memory. It's only used for
// booting. The checksum at word 7 needs to be computed and filled in by
// someone. The "openocd" JTAG application can do this as can "sum.pl"

static const boot_vec_t boot_data __attribute__ ((section (".boot_vec"))) = {
    &STACK_LIMIT,   		// 0: Boot stack pointer
    boot_proc,			// 1; Boot routine
    (proc4) flash_copy,		// 2: Flash copy routine
    (proc4) 0,   		// 3:
    (proc4) 0,         		// 4:
    BUILD_DATE,  		// 5: Build date
    BMP_VER_NUM,        	// 6: Version
    0				// 7: Checksum
};


//------------------------------------------------------------------------------
