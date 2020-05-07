//------------------------------------------------------------------------------
//! \file  spinnaker.h
//! \brief Header file describing SpiNNaker hardware
//!
//! \copyright &copy; The University of Manchester - 2009-2019
//!
//! \author Steve Temple, APT Group, School of Computer Science
//! Email:            steven.temple@manchester.ac.uk
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009-2019 The University of Manchester
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

#ifndef __SPINNAKER_H__
#define __SPINNAKER_H__

//------------------------------------------------------------------------------

// Type definitions for unsigned integers

typedef unsigned char uchar;        //!< Unsigned integer - 8 bits
typedef unsigned short ushort;      //!< Unsigned integer - 16 bits
typedef unsigned int uint;          //!< Unsigned integer - 32 bits
typedef unsigned long long uint64;  //!< Unsigned integer - 64 bits

// Used in register offset definitions below

#define INT_AT(t) ((t) / 4)
#define BYTE_AT(t) (t)

// Miscellaneous...

#ifndef NULL
#define NULL            0           //!< Null pointer value
#endif

#define BIT_31          0x80000000  //!< Bit number 31

//! Define BUF_WRITE to place all peripherals & memories in buffered write space

#define BUF_WRITE       1

//------------------------------------------------------------------------------

// Numbers of CPUS, links

#define NUM_CPUS        18
#define NUM_LINKS       6

//------------------------------------------------------------------------------

//! \name Memory Layout
//! \brief Memory definitions
//! \{

//! Base address of Instruction Tightly Coupled Memory
#define ITCM_BASE           0x00000000
//! Size of Instruction Tightly Coupled Memory (32 kB)
#define ITCM_SIZE           0x00008000  // 32 KB
//! Top address of Instruction Tightly Coupled Memory
#define ITCM_TOP            (ITCM_BASE + ITCM_SIZE)

//! Base address of Data Tightly Coupled Memory
#define DTCM_BASE           0x00400000
//! Size of Data Tightly Coupled Memory (64 kB)
#define DTCM_SIZE           0x00010000  // 64 KB
//! Top address of Data Tightly Coupled Memory
#define DTCM_TOP            (DTCM_BASE + DTCM_SIZE)

//! Base address of SDRAM: writes unbuffered
#define SDRAM_BASE_UNBUF    0x70000000
//! Base address of SDRAM: writes buffered
#define SDRAM_BASE_BUF      0x60000000

//! Base address of SDRAM
#ifdef BUF_WRITE
#define SDRAM_BASE          SDRAM_BASE_BUF
#else
#define SDRAM_BASE          SDRAM_BASE_UNBUF
#endif

//! Size of SDRAM (128 MB)
#define SDRAM_SIZE          (128 * 1024 * 1024) // 128 MB
//! Top address of SDRAM
#define SDRAM_TOP           (SDRAM_BASE + SDRAM_SIZE)

//! Base address of System RAM: writes unbuffered
#define SYSRAM_BASE_UNBUF   0xf5000000
//! Base address of System RAM: writes buffered
#define SYSRAM_BASE_BUF     0xe5000000

//! Base address of System RAM
#ifdef BUF_WRITE
#define SYSRAM_BASE         SYSRAM_BASE_BUF
#else
#define SYSRAM_BASE         SYSRAM_BASE_UNBUF
#endif

//! Size of System RAM (32 kB)
#define SYSRAM_SIZE         0x00008000  // 32 KB
//! Top address of System RAM
#define SYSRAM_TOP          (SYSRAM_BASE + SYSRAM_SIZE)

//! Base address of ROM
#define ROM_BASE            0xf6000000
//! Size of ROM (32 kB)
#define ROM_SIZE            0x00008000  // 32 KB
//! Top address of ROM
#define ROM_TOP             (ROM_BASE + ROM_SIZE)
//! \}

//------------------------------------------------------------------------------

//! \name Communications Controller
//! \brief Communications controller definitions
//! \{

//! Communications controller base address: writes unbuffered
#define CC_BASE_UNBUF   0x10000000  // Unbuffered
//! Communications controller base address: writes buffered
#define CC_BASE_BUF     0x20000000  // Buffered

//! Communications controller base address
#ifdef BUF_WRITE
#define CC_BASE         CC_BASE_BUF
#else
#define CC_BASE         CC_BASE_UNBUF
#endif

//! Comms controller r0: Transmit control register offset
#define CC_TCR          INT_AT(0x00)
//! Comms controller r1: Transmit data register offset
#define CC_TXDATA       INT_AT(0x04)
//! Comms controller r2: Transmit key register offset
#define CC_TXKEY        INT_AT(0x08)
//! Comms controller r3: Receive status register offset
#define CC_RSR          INT_AT(0x0c)
//! Comms controller r4: Receive data register offset
#define CC_RXDATA       INT_AT(0x10)
//! Comms controller r5: Receive key register offset
#define CC_RXKEY        INT_AT(0x14)
//! Comms controller r6: Source address register offset
#define CC_SAR          INT_AT(0x18)
//! Comms controller r7: Test register offset
#define CC_TEST         INT_AT(0x1c)
//! \}

//------------------------------------------------------------------------------

//! \name Timer Controller
//! \brief Timer controller definitions
//! \{

//! Timer controller base address: writes unbuffered
#define TIMER_BASE_UNBUF    0x11000000
//! Timer controller base address: writes buffered
#define TIMER_BASE_BUF      0x21000000

//! Timer controller base address
#ifdef BUF_WRITE
#define TIMER_BASE      TIMER_BASE_BUF
#else
#define TIMER_BASE      TIMER_BASE_UNBUF
#endif

#define TIMER_OFFSET    0x20        //!< Offset of second timer

//! Timer 1 base address
#define TIMER1_BASE     (TIMER_BASE)
//! Timer 2 base address
#define TIMER2_BASE     (TIMER_BASE + TIMER_OFFSET)

//! Timer register r0: Load value register offset
#define TC_LOAD         INT_AT(0x00)
//! Timer register r1: Current count register offset
#define TC_COUNT        INT_AT(0x04)
//! Timer register r2: Control register offset
#define TC_CONTROL      INT_AT(0x08)
//! Timer register r3: Interrupt clear register offset
#define TC_INT_CLR      INT_AT(0x0c)
//! Timer register r4: Raw interrupt status register offset
#define TC_RAW_INT      INT_AT(0x10)
//! Timer register r5: Masked interrupt status register offset
#define TC_MASK_INT     INT_AT(0x14)
//! Timer register r6: Background load value register offset
#define TC_BG_LOAD      INT_AT(0x18)

//! `== TC_LOAD`
#define T1_LOAD         INT_AT(0x00)
//! `== TC_COUNT`
#define T1_COUNT        INT_AT(0x04)
//! `== TC_CONTROL`
#define T1_CONTROL      INT_AT(0x08)
//! `== TC_INT_CLR`
#define T1_INT_CLR      INT_AT(0x0c)
//! `== TC_RAW_INT`
#define T1_RAW_INT      INT_AT(0x10)
//! `== TC_MASK_IN`
#define T1_MASK_INT     INT_AT(0x14)
//! `== TC_BG_LOAD`
#define T1_BG_LOAD      INT_AT(0x18)

//! `== TC_LOAD + TIMER_OFFSET`
#define T2_LOAD         INT_AT(0x20)
//! `== TC_COUNT + TIMER_OFFSET`
#define T2_COUNT        INT_AT(0x24)
//! `== TC_CONTROL + TIMER_OFFSET`
#define T2_CONTROL      INT_AT(0x28)
//! `== TC_INT_CLR + TIMER_OFFSET`
#define T2_INT_CLR      INT_AT(0x2c)
//! `== TC_RAW_INT + TIMER_OFFSET`
#define T2_RAW_INT      INT_AT(0x30)
//! `== TC_MASK_INT + TIMER_OFFSET`
#define T2_MASK_INT     INT_AT(0x34)
//! `== TC_BG_LOAD + TIMER_OFFSET`
#define T2_BG_LOAD      INT_AT(0x38)
//! \}

//------------------------------------------------------------------------------

//! \name VIC
//! \brief Vectored interrupt controller (VIC) definitions
//! \{

//! Vectored interrupt controller base address: writes unbuffered
#define VIC_BASE_UNBUF  0x1f000000
//! Vectored interrupt controller base address: writes buffered
#define VIC_BASE_BUF    0x2f000000

//! Vectored interrupt controller base address
#ifdef BUF_WRITE
#define VIC_BASE        VIC_BASE_BUF
#else
#define VIC_BASE        VIC_BASE_UNBUF
#endif

//! VIC: IRQ status register offset
#define VIC_IRQST       INT_AT(0x00)
//! VIC: FIQ status register offset
#define VIC_FIQST       INT_AT(0x04)
//! VIC: Raw interrupt status register offset
#define VIC_RAW         INT_AT(0x08)
//! VIC: Interrupt type select register offset
#define VIC_SELECT      INT_AT(0x0c)
//! VIC: Interrupt enable register offset
#define VIC_ENABLE      INT_AT(0x10)
//! VIC: Interrupt disable register offset
#define VIC_DISABLE     INT_AT(0x14)
//! VIC: Soft interrupt register offset
#define VIC_SOFT_SET    INT_AT(0x18)
//! VIC: Soft interrupt clear register offset
#define VIC_SOFT_CLR    INT_AT(0x1c)
//! VIC: Protected-mode register offset
#define VIC_PROTECT     INT_AT(0x20)
//! VIC: Current vector address register offset
#define VIC_VADDR       INT_AT(0x30)
//! VIC: Default vector address register offset
#define VIC_DEFADDR     INT_AT(0x34)

//! VIC: Vector address register base
#define VIC_ADDR        INT_AT(0x100)

#define VIC_ADDR0       INT_AT(0x100)
#define VIC_ADDR1       INT_AT(0x104)
#define VIC_ADDR2       INT_AT(0x108)
#define VIC_ADDR3       INT_AT(0x10c)
#define VIC_ADDR4       INT_AT(0x110)
#define VIC_ADDR5       INT_AT(0x114)
#define VIC_ADDR6       INT_AT(0x118)
#define VIC_ADDR7       INT_AT(0x11c)
#define VIC_ADDR8       INT_AT(0x120)
#define VIC_ADDR9       INT_AT(0x124)
#define VIC_ADDR10      INT_AT(0x128)
#define VIC_ADDR11      INT_AT(0x12c)
#define VIC_ADDR12      INT_AT(0x130)
#define VIC_ADDR13      INT_AT(0x134)
#define VIC_ADDR14      INT_AT(0x138)
#define VIC_ADDR15      INT_AT(0x13c)

//! VIC: Vector control register base
#define VIC_CNTL        INT_AT(0x200)

#define VIC_CNTL0       INT_AT(0x200)
#define VIC_CNTL1       INT_AT(0x204)
#define VIC_CNTL2       INT_AT(0x208)
#define VIC_CNTL3       INT_AT(0x20c)
#define VIC_CNTL4       INT_AT(0x210)
#define VIC_CNTL5       INT_AT(0x214)
#define VIC_CNTL6       INT_AT(0x218)
#define VIC_CNTL7       INT_AT(0x21c)
#define VIC_CNTL8       INT_AT(0x220)
#define VIC_CNTL9       INT_AT(0x224)
#define VIC_CNTL10      INT_AT(0x228)
#define VIC_CNTL11      INT_AT(0x22c)
#define VIC_CNTL12      INT_AT(0x230)
#define VIC_CNTL13      INT_AT(0x234)
#define VIC_CNTL14      INT_AT(0x238)
#define VIC_CNTL15      INT_AT(0x23c)

//! Interrupt numbers for SpiNNaker
enum spinnaker_interupt_numbers_e {
    WDOG_INT = 0,       //!< Watchdog timer interrupt
    SOFTWARE_INT = 1,   //!< Used only for local software interrupt generation
    COMM_RX_INT = 2,    //!< Debug communications receiver interrupt
    COMM_TX_INT = 3,    //!< Debug communications transmitter interrupt
    TIMER1_INT = 4,     //!< Local counter/timer interrupt 1
    TIMER2_INT = 5,     //!< Local counter/timer interrupt 2
    CC_RDY_INT = 6,     //!< Local comms controller packet received
    CC_RPE_INT = 7,     //!< Local comms controller received packet parity error
    CC_RFE_INT = 8,     //!< Local comms controller received packet framing error
    CC_TFL_INT = 9,     //!< Local comms controller transmit buffer full
    CC_TOV_INT = 10,    //!< Local comms controller transmit buffer overflow
    CC_TMT_INT = 11,    //!< Local comms controller transmit buffer empty
    DMA_DONE_INT = 12,  //!< Local DMA controller transfer complete
    DMA_ERR_INT = 13,   //!< Local DMA controller error
    DMA_TO_INT = 14,    //!< Local DMA controller transfer timed out
    RTR_DIAG_INT = 15,  //!< Router diagnostic counter event has occurred
    RTR_DUMP_INT = 16,  //!< Router packet dumped - indicates failed delivery
    RTR_ERR_INT = 17,   //!< Router error - packet parity, framing, or time stamp error
    CPU_INT = 18,       //!< System Controller interrupt bit set for this processor
    ETH_TX_INT = 19,    //!< Ethernet transmit frame interrupt
    ETH_RX_INT = 20,    //!< Ethernet receive frame interrupt
    ETH_PHY_INT = 21,   //!< Ethernet PHY/external interrupt
    SLOW_CLK_INT = 22,  //!< System-wide slow (nominally 32 KHz) timer interrupt
    CC_TNF_INT = 23,    //!< Local comms controller can accept new Tx packet
    CC_MC_INT = 24,     //!< Local comms controller multicast packet received
    CC_P2P_INT = 25,    //!< Local comms controller point-to-point packet received
    CC_NN_INT = 26,     //!< Local comms controller nearest neighbour packet receive
    CC_FR_INT = 27,     //!< Local comms controller fixed route packet received
    INT0_INT = 28,      //!< External interrupt request: Pin INT[0]
    INT1_INT = 29,      //!< External interrupt request: Pin INT[1]
    GPIO8_INT = 30,     //!< Signal on Pin GPIO[8]
    GPIO9_INT = 31,     //!< Signal on Pin GPIO[9]
};
//! \}

//------------------------------------------------------------------------------

//! \name DMA Controller
//! \brief DMA controller definitions
//! \{

//! DMA controller base address: writes unbuffered
#define DMA_BASE_UNBUF  0x30000000
//! DMA controller base address: writes buffered
#define DMA_BASE_BUF    0x40000000

//! DMA controller base address
#ifdef BUF_WRITE
#define DMA_BASE        DMA_BASE_BUF
#else
#define DMA_BASE        DMA_BASE_UNBUF
#endif

//! DMA controller: system address register offset
#define DMA_ADRS        INT_AT(0x04)
//! DMA controller: TCM address register offset
#define DMA_ADRT        INT_AT(0x08)
//! DMA controller: transfer description register offset
#define DMA_DESC        INT_AT(0x0c)
//! DMA controller: control register offset
#define DMA_CTRL        INT_AT(0x10)
//! DMA controller: status register offset
#define DMA_STAT        INT_AT(0x14)
//! DMA controller: global control register offset
#define DMA_GCTL        INT_AT(0x18)
//! DMA controller: calculated CRC register offset
#define DMA_CRCC        INT_AT(0x1c)
//! DMA controller: received CRC register offset
#define DMA_CRCR        INT_AT(0x20)
//! DMA controller: timeout value register offset
#define DMA_TMTV        INT_AT(0x24)
//! DMA controller: statistics counters control register offset
#define DMA_SCTL        INT_AT(0x28)

#define DMA_STAT0       INT_AT(0x40)
#define DMA_STAT1       INT_AT(0x44)
#define DMA_STAT2       INT_AT(0x48)
#define DMA_STAT3       INT_AT(0x4c)
#define DMA_STAT4       INT_AT(0x50)
#define DMA_STAT5       INT_AT(0x54)
#define DMA_STAT6       INT_AT(0x58)
#define DMA_STAT7       INT_AT(0x5c)

//! DMA controller: current system address register offset
#define DMA_AD2S        INT_AT(0x104)
//! DMA controller: current TCM address register offset
#define DMA_AD2T        INT_AT(0x108)
//! DMA controller: current descriptor register offset
#define DMA_DES2        INT_AT(0x10c)

//! DMA controller: CRC polynomial base address
#define DMA_CRCT        INT_AT(0x180)
//! \}

//------------------------------------------------------------------------------

//! \name Memory Controler
//! \brief PL340 definitions
//! \{

//! Memory controller base address: writes unbuffered
#define PL340_BASE_UNBUF    0xf0000000
//! Memory controller base address: writes buffered
#define PL340_BASE_BUF      0xe0000000

//! Memory controller base address
#ifdef BUF_WRITE
#define PL340_BASE      PL340_BASE_BUF
#else
#define PL340_BASE      PL340_BASE_UNBUF
#endif

#define MC_STAT         INT_AT(0x00)
#define MC_CMD          INT_AT(0x04)
#define MC_DIRC         INT_AT(0x08)
#define MC_MCFG         INT_AT(0x0c)

#define MC_REFP         INT_AT(0x10)
#define MC_CASL         INT_AT(0x14)
#define MC_DQSS         INT_AT(0x18)
#define MC_MRD          INT_AT(0x1c)
#define MC_RAS          INT_AT(0x20)
#define MC_RC           INT_AT(0x24)
#define MC_RCD          INT_AT(0x28)
#define MC_RFC          INT_AT(0x2c)
#define MC_RP           INT_AT(0x30)
#define MC_RRD          INT_AT(0x34)
#define MC_WR           INT_AT(0x38)
#define MC_WTR          INT_AT(0x3c)
#define MC_XP           INT_AT(0x40)
#define MC_XSR          INT_AT(0x44)
#define MC_ESR          INT_AT(0x48)

#define MC_MCFG2        INT_AT(0x4c)
#define MC_MCFG3        INT_AT(0x50)

#define MC_QOS0         INT_AT(0x100)
#define MC_QOS1         INT_AT(0x104)
#define MC_QOS2         INT_AT(0x108)
#define MC_QOS3         INT_AT(0x10c)
#define MC_QOS4         INT_AT(0x110)
#define MC_QOS5         INT_AT(0x114)
#define MC_QOS6         INT_AT(0x118)
#define MC_QOS7         INT_AT(0x11c)
#define MC_QOS8         INT_AT(0x120)
#define MC_QOS9         INT_AT(0x124)
#define MC_QOS10        INT_AT(0x128)
#define MC_QOS11        INT_AT(0x12c)
#define MC_QOS12        INT_AT(0x130)
#define MC_QOS13        INT_AT(0x134)
#define MC_QOS14        INT_AT(0x138)
#define MC_QOS15        INT_AT(0x13c)

#define MC_CCFG0        INT_AT(0x200)
#define MC_CCFG1        INT_AT(0x204)
#define MC_CCFG2        INT_AT(0x208)
#define MC_CCFG3        INT_AT(0x20c)

#define DLL_STATUS      INT_AT(0x300)
#define DLL_CONFIG0     INT_AT(0x304)
#define DLL_CONFIG1     INT_AT(0x308)
//! \}

//------------------------------------------------------------------------------

//! \name Router Controller
//! \brief Router definitions
//! \{

//! Router base address: writes unbuffered
#define RTR_BASE_UNBUF  0xf1000000
//! Router base address: writes buffered
#define RTR_BASE_BUF    0xe1000000

//! Router base address
#ifdef BUF_WRITE
#define RTR_BASE        RTR_BASE_BUF
#else
#define RTR_BASE        RTR_BASE_UNBUF
#endif

#define RTR_CONTROL     INT_AT(0x00)
#define RTR_STATUS      INT_AT(0x04)

#define RTR_EHDR        INT_AT(0x08)
#define RTR_EKEY        INT_AT(0x0c)
#define RTR_EDAT        INT_AT(0x10)
#define RTR_ESTAT       INT_AT(0x14)
#define RTR_DHDR        INT_AT(0x18)
#define RTR_DKEY        INT_AT(0x1c)
#define RTR_DDAT        INT_AT(0x20)
#define RTR_DLINK       INT_AT(0x24)
#define RTR_DSTAT       INT_AT(0x28)
#define RTR_DGEN        INT_AT(0x2c)

#define RTR_FR          INT_AT(0x84)

#define RTR_DGF0        INT_AT(0x200)
#define RTR_DGF1        INT_AT(0x204)
#define RTR_DGF2        INT_AT(0x208)
#define RTR_DGF3        INT_AT(0x20c)
#define RTR_DGF4        INT_AT(0x210)
#define RTR_DGF5        INT_AT(0x214)
#define RTR_DGF6        INT_AT(0x218)
#define RTR_DGF7        INT_AT(0x21c)
#define RTR_DGF8        INT_AT(0x220)
#define RTR_DGF9        INT_AT(0x224)
#define RTR_DGF10       INT_AT(0x228)
#define RTR_DGF11       INT_AT(0x22c)
#define RTR_DGF12       INT_AT(0x230)
#define RTR_DGF13       INT_AT(0x234)
#define RTR_DGF14       INT_AT(0x238)
#define RTR_DGF15       INT_AT(0x23c)

#define RTR_DGC0        INT_AT(0x300)
#define RTR_DGC1        INT_AT(0x304)
#define RTR_DGC2        INT_AT(0x308)
#define RTR_DGC3        INT_AT(0x30c)
#define RTR_DGC4        INT_AT(0x310)
#define RTR_DGC5        INT_AT(0x314)
#define RTR_DGC6        INT_AT(0x318)
#define RTR_DGC7        INT_AT(0x31c)
#define RTR_DGC8        INT_AT(0x320)
#define RTR_DGC9        INT_AT(0x324)
#define RTR_DGC10       INT_AT(0x328)
#define RTR_DGC11       INT_AT(0x32c)
#define RTR_DGC12       INT_AT(0x330)
#define RTR_DGC13       INT_AT(0x334)
#define RTR_DGC14       INT_AT(0x338)
#define RTR_DGC15       INT_AT(0x33c)

#define RTR_TST1        INT_AT(0xf00)
#define RTR_TST2        INT_AT(0xf04)

#define P2P_TABLE_SIZE  8192
#define MC_TABLE_SIZE   1024

#define MC_RAM_WIDTH    (NUM_CPUS + NUM_LINKS)
#define MC_RAM_MASK     ((1 << MC_RAM_WIDTH) - 1)

#define MC_CORE_ROUTE(x)   (1 << ((x) + NUM_LINKS))
#define MC_LINK_ROUTE(x)   (1 << (x))

#define P2P_EPW         8           //!< Entries per word
#define P2P_LOG_EPW     3           //!< Log of entries per word
#define P2P_EMASK       (P2P_EPW-1) //!< Entries per word - 1
#define P2P_BPE         3           //!< Bits per entry
#define P2P_BMASK       7           //!< Mask for entry bits

#define P2P_INIT        0x00db6db6  // All thrown away!

#define RTR_MCRAM_BASE      (RTR_BASE + 0x00004000)
#define RTR_MCRAM_TOP       (RTR_MCRAM_BASE + MC_TABLE_SIZE * 4)

#define RTR_MCKEY_BASE      (RTR_BASE + 0x00008000)
#define RTR_MCKEY_TOP       (RTR_MCKEY_BASE + MC_TABLE_SIZE * 4)

#define RTR_MCMASK_BASE     (RTR_BASE + 0x0000c000)
#define RTR_MCMASK_TOP      (RTR_MCMASK_BASE + MC_TABLE_SIZE * 4)

#define RTR_P2P_BASE        (RTR_BASE + 0x00010000)
#define RTR_P2P_TOP         (RTR_P2P_BASE + P2P_TABLE_SIZE * 4)

#define MC_CAM_WIDTH        32
//! \}

//------------------------------------------------------------------------------

//! \name System Controller
//! \brief System controller definitions
//! \{

//! System controller base address: writes unbuffered
#define SYSCTL_BASE_UNBUF   0xf2000000
//! System controller base address: writes buffered
#define SYSCTL_BASE_BUF     0xe2000000

//! System controller base address
#ifdef BUF_WRITE
#define SYSCTL_BASE     SYSCTL_BASE_BUF
#else
#define SYSCTL_BASE     SYSCTL_BASE_UNBUF
#endif

//! Magic code for System Controller
#define SC_CODE         0x5ec00000

#define CHIP_ID_CODE    0x59111012

#define SC_CHIP_ID      INT_AT(0x00)
#define SC_CPU_DIS      INT_AT(0x04)
#define SC_SET_IRQ      INT_AT(0x08)
#define SC_CLR_IRQ      INT_AT(0x0c)
#define SC_SET_OK       INT_AT(0x10)
#define SC_CPU_OK       INT_AT(0x10)
#define SC_CLR_OK       INT_AT(0x14)

#define SC_SOFT_RST_L   INT_AT(0x18)
#define SC_HARD_RST_L   INT_AT(0x1c)
#define SC_SUBS_RST_L   INT_AT(0x20)

#define SC_SOFT_RST_P   INT_AT(0x24)
#define SC_HARD_RST_P   INT_AT(0x28)
#define SC_SUBS_RST_P   INT_AT(0x2c)

#define SC_RST_CODE     INT_AT(0x30)
#define SC_MON_ID       INT_AT(0x34)

#define SC_MISC_CTRL    INT_AT(0x38)

#define GPIO_RES        INT_AT(0x3c)

#define GPIO_PORT       INT_AT(0x40)
#define GPIO_DIR        INT_AT(0x44)
#define GPIO_SET        INT_AT(0x48)
#define GPIO_CLR        INT_AT(0x4c)
#define GPIO_READ       INT_AT(0x48)

#define SC_PLL1         INT_AT(0x50)
#define SC_PLL2         INT_AT(0x54)

#define SC_FLAG         INT_AT(0x58)
#define SC_SETFLAG      INT_AT(0x58)
#define SC_CLRFLAG      INT_AT(0x5c)

#define SC_CLKMUX       INT_AT(0x60)
#define SC_SLEEP        INT_AT(0x64)

#define SC_TS0          INT_AT(0x68)
#define SC_TS1          INT_AT(0x6c)
#define SC_TS2          INT_AT(0x70)

#define SC_ARB0         INT_AT(0x080)
#define SC_TAS0         INT_AT(0x100)
#define SC_TAC0         INT_AT(0x180)

#define SC_LINK_DIS     INT_AT(0x200)

#define RST_POR         0
#define RST_WDT         1
#define RST_USER        2
#define RST_SW          3
#define RST_WDI         4
//! \}

//------------------------------------------------------------------------------

//! \name Watchdog Controller
//! \brief Watchdog timer definitions
//! \{

//! Watchdog timer base address: writes unbuffered
#define WDOG_BASE_UNBUF     0xf3000000
//! Watchdog timer base address: writes buffered
#define WDOG_BASE_BUF       0xe3000000

//! Watchdog timer base address
#ifdef BUF_WRITE
#define WDOG_BASE       WDOG_BASE_BUF
#else
#define WDOG_BASE       WDOG_BASE_UNBUF
#endif

#define WD_LOAD         INT_AT(0x00)
#define WD_COUNT        INT_AT(0x04)
#define WD_CTRL         INT_AT(0x08)
#define WD_INTCLR       INT_AT(0x0c)
#define WD_RAWINT       INT_AT(0x10)
#define WD_MSKINT       INT_AT(0x14)
#define WD_LOCK         INT_AT(0xc00)

#define WD_CODE         0x1acce551

#define WD_INT_B        1
#define WD_RST_B        2
//! \}

//------------------------------------------------------------------------------

//! \name Ethernet Controller
//! Ethernet controller definitions
//! \{

//! Ethernet controller base address: writes unbuffered
#define ETH_BASE_UNBUF      0xf4000000
//! Ethernet controller base address: writes buffered
#define ETH_BASE_BUF        0xe4000000

//! Ethernet controller base address
#ifdef BUF_WRITE
#define ETH_BASE        ETH_BASE_BUF
#else
#define ETH_BASE        ETH_BASE_UNBUF
#endif

//! Ethernet transmit buffer: base address
#define ETH_TX_BASE     (ETH_BASE + 0x0000)
//! Ethernet transmit buffer: size
#define ETH_TX_SIZE     0x0600
//! Ethernet transmit buffer: top address
#define ETH_TX_TOP      (ETH_TX_BASE + ETH_TX_SIZE)

//! Ethernet receive buffer: base address
#define ETH_RX_BASE     (ETH_BASE + 0x4000)
//! Ethernet receive buffer: size
#define ETH_RX_SIZE     0x0c00
//! Ethernet receive buffer: top address
#define ETH_RX_TOP      (ETH_RX_BASE + ETH_RX_SIZE)

//! Ethernet receive descriptors: base address
#define ETH_RX_DESC_RAM (ETH_BASE + 0x8000)
//! Ethernet controller registers: base address
#define ETH_REGS        (ETH_BASE + 0xc000)

#define ETH_CONTROL     INT_AT(0x00)
#define ETH_STATUS      INT_AT(0x04)
#define ETH_TX_LEN      INT_AT(0x08)
#define ETH_TX_CMD      INT_AT(0x0c)
#define ETH_RX_CMD      INT_AT(0x10)
#define ETH_MAC_LO      INT_AT(0x14)
#define ETH_MAC_HI      INT_AT(0x18)
#define ETH_PHY_CTRL    INT_AT(0x1c)
#define ETH_INT_CLR     INT_AT(0x20)
#define ETH_RX_BUF_RP   INT_AT(0x24)
#define ETH_RX_DESC_RP  INT_AT(0x2c)

#define ETH_TX_CLR      0x01
#define ETH_RX_CLR      0x10

// Bits in ETH_PHY_CTRL

#define PHY_CTRL_NRST       1
#define PHY_CTRL_DIN        2
#define PHY_CTRL_DOUT       4
#define PHY_CTRL_OE         8
#define PHY_CTRL_CLK        16
//! \}

//------------------------------------------------------------------------------

//! \name Memory Map
//! \brief Handy constants that point to hardware
//! \{

//! Communications controller
static volatile uint * const cc  = (uint *) CC_BASE;
//! Timer controller
static volatile uint * const tc  = (uint *) TIMER_BASE;
//! Timer 1
static volatile uint * const tc1 = (uint *) TIMER1_BASE;
//! Timer 2
static volatile uint * const tc2 = (uint *) TIMER2_BASE;
//! Vectored interrupt controller
static volatile uint * const vic = (uint *) VIC_BASE_UNBUF; // NB unbuffered
//! DMA controller
static volatile uint * const dma = (uint *) DMA_BASE;

//! System controller
static volatile uint * const sc  = (uint *) SYSCTL_BASE;
//! Router controller
static volatile uint * const rtr = (uint *) RTR_BASE;
//! Ethernet controller
static volatile uint * const er  = (uint *) ETH_REGS;
//! PL340 memory controller
static volatile uint * const mc  = (uint *) PL340_BASE;
//! Watchdog controller
static volatile uint * const wd  = (uint *) WDOG_BASE;

//! Base address of SDRAM
static uint * const sdram        = (uint *) SDRAM_BASE;
//! Base address of System RAM
static uint * const sysram       = (uint *) SYSRAM_BASE;

//! Router memory
static uint * const rtr_ram      = (uint *) RTR_MCRAM_BASE;
//! Router key array
static uint * const rtr_key      = (uint *) RTR_MCKEY_BASE;
//! Router mask array
static uint * const rtr_mask     = (uint *) RTR_MCMASK_BASE;
//! Router peer-to-peer table
static uint * const rtr_p2p      = (uint *) RTR_P2P_BASE;

//! Ethernet transmit memory
static uchar * const eth_tx_ram  = (uchar *) ETH_TX_BASE;
//! Ethernet receive memory
static uchar * const eth_rx_ram  = (uchar *) ETH_RX_BASE;
//! Ethernet receive descriptor array
static uint  * const eth_rx_desc = (uint *)  ETH_RX_DESC_RAM;

//------------------------------------------------------------------------------

//! \name SpiNNaker Packets
//! \brief SpiNNaker packet definitions
//! \{

#define PKT_MC          0x00000000  //!< Bits in control byte
#define PKT_P2P         0x00400000
#define PKT_NN          0x00800000  //!< Normal NN
#define PKT_NND         0x00a00000  //!< Peek/poke NN
#define PKT_FR          0x00c00000

#define PKT_PL          0x00020000  // Payload bit

#define PKT_MC_PL       (PKT_MC + PKT_PL)
#define PKT_P2P_PL      (PKT_P2P + PKT_PL)
#define PKT_NN_PL       (PKT_NN + PKT_PL)
#define PKT_FR_PL       (PKT_FR + PKT_PL)
//! \}

//------------------------------------------------------------------------------

//! \name PLL Definitions
//! \brief Clock & PLL definitions
//! \{

#define CLK_XTAL_MHZ    10          //!< Crystal frequency (MHz)

#define PLL_260         0x0007011a  // Assuming 10 MHz in
#define PLL_300         0x0007011e  //
#define PLL_330         0x00070121  //
#define PLL_400         0x00070128  //

#define PLL_LOCK_TIME   80          // Microseconds

#define PLL_CLK_SEL     0x809488a5  //!< CPU/2, SYS/3, RTR/3, MEM/1
//! \}

//------------------------------------------------------------------------------

//! \name GPIO
//! \brief Bits in GPIO[31:0]
//! \{

// Serial ROM

#define SERIAL_NCS      0x00000020  // Out
#define SERIAL_CLK      0x00000010  // Out
#define SERIAL_SI       0x00000008  // Out
#define SERIAL_SO       0x00000004  // In

#define SERIAL_OE       (SERIAL_NCS + SERIAL_CLK + SERIAL_SI)

// On-chip SDRAM

#define SDRAM_TQ        (1 << 31)
#define SDRAM_DPD       (1 << 30)
#define SDRAM_HERE      (1 << 29)

#define JTAG_NTRST      (1 << 27)
#define JTAG_TMS        (1 << 26)
#define JTAG_TDI        (1 << 25)
#define JTAG_TCK        (1 << 24)

// Bits in MISC_CTRL

#define JTAG_RTCK       (1 << 20)
#define JTAG_TDO        (1 << 19)
#define JTAG_INT        (1 << 15)

// LEDs

#define LED_0           0x00000001  //!< LED on bit 0
//! \}

//------------------------------------------------------------------------------

//! \name CPSR Misc
//! \brief ARM CPSR bits
//! \{

#define MODE_USER       0x10
#define MODE_FIQ        0x11
#define MODE_IRQ        0x12
#define MODE_SVC        0x13
#define MODE_ABT        0x17
#define MODE_UND        0x1b
#define MODE_SYS        0x1f

#define THUMB_BIT       0x20

#define IMASK_IRQ       0x80
#define IMASK_FIQ       0x40
#define IMASK_ALL       0xc0
//! \}

//------------------------------------------------------------------------------

#endif
