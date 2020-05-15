//------------------------------------------------------------------------------
//! \file  spinnaker.h
//! \brief Header file describing SpiNNaker hardware
//! \details See [SpiNNaker Datasheet
//!     ](https://spinnakermanchester.github.io/docs/SpiNN2DataShtV202.pdf)
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
//! \brief Converts a byte offset into a word offset
//! \param[in] t: byte offset
//! \return word offset
#define INT_AT(t) ((t) / 4)

// Miscellaneous...

#ifndef NULL
#define NULL            0           //!< Null pointer value
#endif

#define BIT_31          0x80000000  //!< Bit number 31

//! Define BUF_WRITE to place all peripherals & memories in buffered write space

#define BUF_WRITE       1

//------------------------------------------------------------------------------

// Numbers of CPUS, links

#define NUM_CPUS        18      //!< Number of CPU cores per chip
#define NUM_LINKS       6       //!< Number of SpiNNaker links per chip

//------------------------------------------------------------------------------

//! \name Memory Layout
//! \brief Memory definitions
//! \details Datasheet, section 3.3
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
//! \details Datasheet, section 8
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
//! \details Datasheet, section 6
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

//! Timer 1 load value: `== TC_LOAD`
#define T1_LOAD         INT_AT(0x00)
//! Timer 1 current count: `== TC_COUNT`
#define T1_COUNT        INT_AT(0x04)
//! Timer 1 control: `== TC_CONTROL`
#define T1_CONTROL      INT_AT(0x08)
//! Timer 1 interrupt clear: `== TC_INT_CLR`
#define T1_INT_CLR      INT_AT(0x0c)
//! Timer 1 raw interrupt status: `== TC_RAW_INT`
#define T1_RAW_INT      INT_AT(0x10)
//! Timer 1 masked interrupt status: `== TC_MASK_IN`
#define T1_MASK_INT     INT_AT(0x14)
//! Timer 1 background load: `== TC_BG_LOAD`
#define T1_BG_LOAD      INT_AT(0x18)

//! Timer 2 load value: `== TC_LOAD + TIMER_OFFSET`
#define T2_LOAD         INT_AT(0x20)
//! Timer 2 current count: `== TC_COUNT + TIMER_OFFSET`
#define T2_COUNT        INT_AT(0x24)
//! Timer 2 control: `== TC_CONTROL + TIMER_OFFSET`
#define T2_CONTROL      INT_AT(0x28)
//! Timer 2 interrupt clear: `== TC_INT_CLR + TIMER_OFFSET`
#define T2_INT_CLR      INT_AT(0x2c)
//! Timer 2 raw interrupt status: `== TC_RAW_INT + TIMER_OFFSET`
#define T2_RAW_INT      INT_AT(0x30)
//! Timer 2 masked interrupt status: `== TC_MASK_INT + TIMER_OFFSET`
#define T2_MASK_INT     INT_AT(0x34)
//! Timer 2 background load: `== TC_BG_LOAD + TIMER_OFFSET`
#define T2_BG_LOAD      INT_AT(0x38)
//! \}

//------------------------------------------------------------------------------

//! \name VIC
//! \brief Vectored interrupt controller (VIC) definitions
//! \details Datasheet, section 5
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

//! VIC register r0: IRQ status register offset
#define VIC_IRQST       INT_AT(0x00)
//! VIC register r1: FIQ status register offset
#define VIC_FIQST       INT_AT(0x04)
//! VIC register r2: Raw interrupt status register offset
#define VIC_RAW         INT_AT(0x08)
//! VIC register r3: Interrupt type select register offset
#define VIC_SELECT      INT_AT(0x0c)
//! VIC register r4: Interrupt enable register offset
#define VIC_ENABLE      INT_AT(0x10)
//! VIC register r5: Interrupt disable register offset
#define VIC_DISABLE     INT_AT(0x14)
//! VIC register r6: Soft interrupt register offset
#define VIC_SOFT_SET    INT_AT(0x18)
//! VIC register r7: Soft interrupt clear register offset
#define VIC_SOFT_CLR    INT_AT(0x1c)
//! VIC register r8: Protected-mode register offset
#define VIC_PROTECT     INT_AT(0x20)
//! VIC register r9: Current vector address register offset
#define VIC_VADDR       INT_AT(0x30)
//! VIC register r10: Default vector address register offset
#define VIC_DEFADDR     INT_AT(0x34)

//! VIC: Vector address register base
#define VIC_ADDR        INT_AT(0x100)

//! VIC: Vector address register 0 offset
#define VIC_ADDR0       INT_AT(0x100)
//! VIC: Vector address register 1 offset
#define VIC_ADDR1       INT_AT(0x104)
//! VIC: Vector address register 2 offset
#define VIC_ADDR2       INT_AT(0x108)
//! VIC: Vector address register 3 offset
#define VIC_ADDR3       INT_AT(0x10c)
//! VIC: Vector address register 4 offset
#define VIC_ADDR4       INT_AT(0x110)
//! VIC: Vector address register 5 offset
#define VIC_ADDR5       INT_AT(0x114)
//! VIC: Vector address register 6 offset
#define VIC_ADDR6       INT_AT(0x118)
//! VIC: Vector address register 7 offset
#define VIC_ADDR7       INT_AT(0x11c)
//! VIC: Vector address register 8 offset
#define VIC_ADDR8       INT_AT(0x120)
//! VIC: Vector address register 9 offset
#define VIC_ADDR9       INT_AT(0x124)
//! VIC: Vector address register 10 offset
#define VIC_ADDR10      INT_AT(0x128)
//! VIC: Vector address register 11 offset
#define VIC_ADDR11      INT_AT(0x12c)
//! VIC: Vector address register 12 offset
#define VIC_ADDR12      INT_AT(0x130)
//! VIC: Vector address register 13 offset
#define VIC_ADDR13      INT_AT(0x134)
//! VIC: Vector address register 14 offset
#define VIC_ADDR14      INT_AT(0x138)
//! VIC: Vector address register 15 offset
#define VIC_ADDR15      INT_AT(0x13c)

//! VIC: Vector control register base
#define VIC_CNTL        INT_AT(0x200)

//! VIC: Vector control register 0 offset
#define VIC_CNTL0       INT_AT(0x200)
//! VIC: Vector control register 1 offset
#define VIC_CNTL1       INT_AT(0x204)
//! VIC: Vector control register 2 offset
#define VIC_CNTL2       INT_AT(0x208)
//! VIC: Vector control register 3 offset
#define VIC_CNTL3       INT_AT(0x20c)
//! VIC: Vector control register 4 offset
#define VIC_CNTL4       INT_AT(0x210)
//! VIC: Vector control register 5 offset
#define VIC_CNTL5       INT_AT(0x214)
//! VIC: Vector control register 6 offset
#define VIC_CNTL6       INT_AT(0x218)
//! VIC: Vector control register 7 offset
#define VIC_CNTL7       INT_AT(0x21c)
//! VIC: Vector control register 8 offset
#define VIC_CNTL8       INT_AT(0x220)
//! VIC: Vector control register 9 offset
#define VIC_CNTL9       INT_AT(0x224)
//! VIC: Vector control register 10 offset
#define VIC_CNTL10      INT_AT(0x228)
//! VIC: Vector control register 11 offset
#define VIC_CNTL11      INT_AT(0x22c)
//! VIC: Vector control register 12 offset
#define VIC_CNTL12      INT_AT(0x230)
//! VIC: Vector control register 13 offset
#define VIC_CNTL13      INT_AT(0x234)
//! VIC: Vector control register 14 offset
#define VIC_CNTL14      INT_AT(0x238)
//! VIC: Vector control register 15 offset
#define VIC_CNTL15      INT_AT(0x23c)

//! Interrupt numbers for SpiNNaker
enum spinnaker_interrupt_numbers_e {
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
//! \details Datasheet, section 7
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

//! DMA controller r1: system address register offset
#define DMA_ADRS        INT_AT(0x04)
//! DMA controller r2: TCM address register offset
#define DMA_ADRT        INT_AT(0x08)
//! DMA controller r3: transfer description register offset
#define DMA_DESC        INT_AT(0x0c)
//! DMA controller r4: control register offset
#define DMA_CTRL        INT_AT(0x10)
//! DMA controller r5: status register offset
#define DMA_STAT        INT_AT(0x14)
//! DMA controller r6: global control register offset
#define DMA_GCTL        INT_AT(0x18)
//! DMA controller r7: calculated CRC register offset
#define DMA_CRCC        INT_AT(0x1c)
//! DMA controller r8: received CRC register offset
#define DMA_CRCR        INT_AT(0x20)
//! DMA controller r9: timeout value register offset
#define DMA_TMTV        INT_AT(0x24)
//! DMA controller r10: statistics counters control register offset
#define DMA_SCTL        INT_AT(0x28)

//! DMA controller statistics counter 0 offset
#define DMA_STAT0       INT_AT(0x40)
//! DMA controller statistics counter 1 offset
#define DMA_STAT1       INT_AT(0x44)
//! DMA controller statistics counter 2 offset
#define DMA_STAT2       INT_AT(0x48)
//! DMA controller statistics counter 3 offset
#define DMA_STAT3       INT_AT(0x4c)
//! DMA controller statistics counter 4 offset
#define DMA_STAT4       INT_AT(0x50)
//! DMA controller statistics counter 5 offset
#define DMA_STAT5       INT_AT(0x54)
//! DMA controller statistics counter 6 offset
#define DMA_STAT6       INT_AT(0x58)
//! DMA controller statistics counter 7 offset
#define DMA_STAT7       INT_AT(0x5c)

//! DMA controller r65: current system address register offset
#define DMA_AD2S        INT_AT(0x104)
//! DMA controller r66: current TCM address register offset
#define DMA_AD2T        INT_AT(0x108)
//! DMA controller r67: current descriptor register offset
#define DMA_DES2        INT_AT(0x10c)

//! DMA controller: CRC polynomial base address offset
#define DMA_CRCT        INT_AT(0x180)
//! \}

//------------------------------------------------------------------------------

//! \name Memory Controler
//! \brief PL340 definitions
//! \details Datasheet, section 13
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

//! Memory controller r0: status register offset
#define MC_STAT         INT_AT(0x00)
//! Memory controller r1: command register offset
#define MC_CMD          INT_AT(0x04)
//! Memory controller r2: direct command register offset
#define MC_DIRC         INT_AT(0x08)
//! Memory controller r3: memory configuration register offset
#define MC_MCFG         INT_AT(0x0c)

//! Memory controller r4: refresh period register offset
#define MC_REFP         INT_AT(0x10)
//! Memory controller r5: CAS latency register offset
#define MC_CASL         INT_AT(0x14)
//! Memory controller r6: write to DQS time register offset
#define MC_DQSS         INT_AT(0x18)
//! Memory controller r7: mode register command time register offset
#define MC_MRD          INT_AT(0x1c)
//! Memory controller r8: RAS to precharge delay register offset
#define MC_RAS          INT_AT(0x20)
//! Memory controller r9: active bank x to active bank x delay register offset
#define MC_RC           INT_AT(0x24)
//! Memory controller r10: RAS to CAS minimum delay register offset
#define MC_RCD          INT_AT(0x28)
//! Memory controller r11: auto-refresh command time register offset
#define MC_RFC          INT_AT(0x2c)
//! Memory controller r12: precharge to RAS delay register offset
#define MC_RP           INT_AT(0x30)
//! Memory controller r13: active bank x to active bank y delay register offset
#define MC_RRD          INT_AT(0x34)
//! Memory controller r14: write to precharge delay register offset
#define MC_WR           INT_AT(0x38)
//! Memory controller r15: write to read delay register offset
#define MC_WTR          INT_AT(0x3c)
//! Memory controller r16: exit power-down command time register offset
#define MC_XP           INT_AT(0x40)
//! Memory controller r17: exit self-refresh command time register offset
#define MC_XSR          INT_AT(0x44)
//! Memory controller r18: self-refresh command time register offset
#define MC_ESR          INT_AT(0x48)

#define MC_MCFG2        INT_AT(0x4c)
#define MC_MCFG3        INT_AT(0x50)

//! Memory controller QOS settings 0
#define MC_QOS0         INT_AT(0x100)
//! Memory controller QOS settings 1
#define MC_QOS1         INT_AT(0x104)
//! Memory controller QOS settings 2
#define MC_QOS2         INT_AT(0x108)
//! Memory controller QOS settings 3
#define MC_QOS3         INT_AT(0x10c)
//! Memory controller QOS settings 4
#define MC_QOS4         INT_AT(0x110)
//! Memory controller QOS settings 5
#define MC_QOS5         INT_AT(0x114)
//! Memory controller QOS settings 6
#define MC_QOS6         INT_AT(0x118)
//! Memory controller QOS settings 7
#define MC_QOS7         INT_AT(0x11c)
//! Memory controller QOS settings 8
#define MC_QOS8         INT_AT(0x120)
//! Memory controller QOS settings 9
#define MC_QOS9         INT_AT(0x124)
//! Memory controller QOS settings 10
#define MC_QOS10        INT_AT(0x128)
//! Memory controller QOS settings 11
#define MC_QOS11        INT_AT(0x12c)
//! Memory controller QOS settings 12
#define MC_QOS12        INT_AT(0x130)
//! Memory controller QOS settings 13
#define MC_QOS13        INT_AT(0x134)
//! Memory controller QOS settings 14
#define MC_QOS14        INT_AT(0x138)
//! Memory controller QOS settings 15
#define MC_QOS15        INT_AT(0x13c)

//! Memory controller chip 0 configuration
#define MC_CCFG0        INT_AT(0x200)
//! Memory controller chip 1 configuration
#define MC_CCFG1        INT_AT(0x204)
//! Memory controller chip 2 configuration
#define MC_CCFG2        INT_AT(0x208)
//! Memory controller chip 3 configuration
#define MC_CCFG3        INT_AT(0x20c)

//! Memory controller DLL test and status inputs register offset
#define DLL_STATUS      INT_AT(0x300)
//! Memory controller DLL test and control outputs register offset
#define DLL_CONFIG0     INT_AT(0x304)
//! Memory controller DLL fine-tune control register offset
#define DLL_CONFIG1     INT_AT(0x308)
//! \}

//------------------------------------------------------------------------------

//! \name Router Controller
//! \brief Router definitions
//! \details Datasheet, section 10
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

//! Router controller r0: control register offset
#define RTR_CONTROL     INT_AT(0x00)
//! Router controller r1: status register offset
#define RTR_STATUS      INT_AT(0x04)

//! Router controller r2: error header register offset
#define RTR_EHDR        INT_AT(0x08)
//! Router controller r3: error key word register offset
#define RTR_EKEY        INT_AT(0x0c)
//! Router controller r4: error payload word register offset
#define RTR_EDAT        INT_AT(0x10)
//! Router controller r5: error status register offset
#define RTR_ESTAT       INT_AT(0x14)
//! Router controller r6: dump header register offset
#define RTR_DHDR        INT_AT(0x18)
//! Router controller r7: dump key word register offset
#define RTR_DKEY        INT_AT(0x1c)
//! Router controller r8: dump payload word register offset
#define RTR_DDAT        INT_AT(0x20)
//! Router controller r9: dump outputs register offset
#define RTR_DLINK       INT_AT(0x24)
//! Router controller r10: dump status register offset
#define RTR_DSTAT       INT_AT(0x28)
//! Router controller r11: diagnostic counter enable register offset
#define RTR_DGEN        INT_AT(0x2c)

// registers r12-r32 are NOT described here!

//! Router controller r33: fixed route packet route register offset
#define RTR_FR          INT_AT(0x84)

//! Router controller diagnostic filter 0
#define RTR_DGF0        INT_AT(0x200)
//! Router controller diagnostic filter 1
#define RTR_DGF1        INT_AT(0x204)
//! Router controller diagnostic filter 2
#define RTR_DGF2        INT_AT(0x208)
//! Router controller diagnostic filter 3
#define RTR_DGF3        INT_AT(0x20c)
//! Router controller diagnostic filter 4
#define RTR_DGF4        INT_AT(0x210)
//! Router controller diagnostic filter 5
#define RTR_DGF5        INT_AT(0x214)
//! Router controller diagnostic filter 6
#define RTR_DGF6        INT_AT(0x218)
//! Router controller diagnostic filter 7
#define RTR_DGF7        INT_AT(0x21c)
//! Router controller diagnostic filter 8
#define RTR_DGF8        INT_AT(0x220)
//! Router controller diagnostic filter 9
#define RTR_DGF9        INT_AT(0x224)
//! Router controller diagnostic filter 10
#define RTR_DGF10       INT_AT(0x228)
//! Router controller diagnostic filter 11
#define RTR_DGF11       INT_AT(0x22c)
//! Router controller diagnostic filter 12
#define RTR_DGF12       INT_AT(0x230)
//! Router controller diagnostic filter 13
#define RTR_DGF13       INT_AT(0x234)
//! Router controller diagnostic filter 14
#define RTR_DGF14       INT_AT(0x238)
//! Router controller diagnostic filter 15
#define RTR_DGF15       INT_AT(0x23c)

//! Router controller diagnostic counter 0
#define RTR_DGC0        INT_AT(0x300)
//! Router controller diagnostic counter 1
#define RTR_DGC1        INT_AT(0x304)
//! Router controller diagnostic counter 2
#define RTR_DGC2        INT_AT(0x308)
//! Router controller diagnostic counter 3
#define RTR_DGC3        INT_AT(0x30c)
//! Router controller diagnostic counter 4
#define RTR_DGC4        INT_AT(0x310)
//! Router controller diagnostic counter 5
#define RTR_DGC5        INT_AT(0x314)
//! Router controller diagnostic counter 6
#define RTR_DGC6        INT_AT(0x318)
//! Router controller diagnostic counter 7
#define RTR_DGC7        INT_AT(0x31c)
//! Router controller diagnostic counter 8
#define RTR_DGC8        INT_AT(0x320)
//! Router controller diagnostic counter 9
#define RTR_DGC9        INT_AT(0x324)
//! Router controller diagnostic counter 10
#define RTR_DGC10       INT_AT(0x328)
//! Router controller diagnostic counter 11
#define RTR_DGC11       INT_AT(0x32c)
//! Router controller diagnostic counter 12
#define RTR_DGC12       INT_AT(0x330)
//! Router controller diagnostic counter 13
#define RTR_DGC13       INT_AT(0x334)
//! Router controller diagnostic counter 14
#define RTR_DGC14       INT_AT(0x338)
//! Router controller diagnostic counter 15
#define RTR_DGC15       INT_AT(0x33c)

//! Router controller: hardware test register offset
#define RTR_TST1        INT_AT(0xf00)
//! Router controller: hardware test (CAM input key) register offset
#define RTR_TST2        INT_AT(0xf04)

//! Peer-to-peer table size (entries)
#define P2P_TABLE_SIZE  8192
//! Multicast table size (entries)
#define MC_TABLE_SIZE   1024

//! Multicast route select width (bits)
#define MC_RAM_WIDTH    (NUM_CPUS + NUM_LINKS)
//! Multicast route select mask
#define MC_RAM_MASK     ((1 << MC_RAM_WIDTH) - 1)

//! \brief Multicast route to CPU core
//! \param[in] x: ID of core to route to
//! \return Bit in multicast route to use
#define MC_CORE_ROUTE(x)   (1 << ((x) + NUM_LINKS))
//! \brief Multicast route to SpiNNaker link
//! \param[in] x: ID of link to route to
//! \return Bit in route to use
#define MC_LINK_ROUTE(x)   (1 << (x))

#define P2P_EPW         8           //!< Entries per word
#define P2P_LOG_EPW     3           //!< Log of entries per word
#define P2P_EMASK       (P2P_EPW-1) //!< Entries per word - 1
#define P2P_BPE         3           //!< Bits per entry
#define P2P_BMASK       7           //!< Mask for entry bits

//! P2P table initial entry entry; all thrown away
#define P2P_INIT        0x00db6db6

//! Router controller: multicast route table base address
#define RTR_MCRAM_BASE      (RTR_BASE + 0x00004000)
//! Router controller: multicast route table top address
#define RTR_MCRAM_TOP       (RTR_MCRAM_BASE + MC_TABLE_SIZE * 4)

//! Router controller: multicast key table base address (write only)
#define RTR_MCKEY_BASE      (RTR_BASE + 0x00008000)
//! Router controller: multicast key table top address
#define RTR_MCKEY_TOP       (RTR_MCKEY_BASE + MC_TABLE_SIZE * 4)

//! Router controller: multicast mask table base address (write only)
#define RTR_MCMASK_BASE     (RTR_BASE + 0x0000c000)
//! Router controller: multicast mask table top address
#define RTR_MCMASK_TOP      (RTR_MCMASK_BASE + MC_TABLE_SIZE * 4)

//! Router controller: peer-to-peer table base address
#define RTR_P2P_BASE        (RTR_BASE + 0x00010000)
//! Router controller: peer-to-peer table top address
#define RTR_P2P_TOP         (RTR_P2P_BASE + P2P_TABLE_SIZE * 4)

//! Multicast routing table CAM width (bits)
#define MC_CAM_WIDTH        32
//! \}

//------------------------------------------------------------------------------

//! \name System Controller
//! \brief System controller definitions
//! \details Datasheet, section 14
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

//! Expected value of chip ID (see #SC_CHIP_ID)
#define CHIP_ID_CODE    0x59111012

//! System controller r0: chip ID register offset
#define SC_CHIP_ID      INT_AT(0x00)
//! System controller r1: CPU disable register offset
#define SC_CPU_DIS      INT_AT(0x04)
//! System controller r2: set CPU IRQ register offset
#define SC_SET_IRQ      INT_AT(0x08)
//! System controller r3: clear CPU IRQ register offset
#define SC_CLR_IRQ      INT_AT(0x0c)
//! System controller r4: set CPU OK register offset
#define SC_SET_OK       INT_AT(0x10)
//! Alias for #SC_SET_OK
#define SC_CPU_OK       INT_AT(0x10)
//! System controller r5: clear CPU OK register offset
#define SC_CLR_OK       INT_AT(0x14)

//! System controller r6: CPU reset (level) register offset
#define SC_SOFT_RST_L   INT_AT(0x18)
//! System controller r7: node reset (level) register offset
#define SC_HARD_RST_L   INT_AT(0x1c)
//! System controller r8: subsystem reset (level) register offset
#define SC_SUBS_RST_L   INT_AT(0x20)

//! System controller r9: CPU reset (pulse) register offset
#define SC_SOFT_RST_P   INT_AT(0x24)
//! System controller r10: node reset (pulse) register offset
#define SC_HARD_RST_P   INT_AT(0x28)
//! System controller r11: subsystem reset (pulse) register offset
#define SC_SUBS_RST_P   INT_AT(0x2c)

//! System controller r12: reset code register offset
#define SC_RST_CODE     INT_AT(0x30)
//! System controller r13: monitor ID register offset
#define SC_MON_ID       INT_AT(0x34)

//! System controller r14: miscellaneous control register offset
#define SC_MISC_CTRL    INT_AT(0x38)

//! System controller r15: GPIO pull up/down enable register offset
#define GPIO_RES        INT_AT(0x3c)

//! System controller r16: GPIO port register offset
#define GPIO_PORT       INT_AT(0x40)
//! System controller r17: GPIO direction register offset
#define GPIO_DIR        INT_AT(0x44)
//! System controller r18: GPIO set/read register offset
#define GPIO_SET        INT_AT(0x48)
//! System controller r19: GPIO clear register offset
#define GPIO_CLR        INT_AT(0x4c)
//! Alias for #GPIO_SET
#define GPIO_READ       INT_AT(0x48)

//! System controller r20: PLL1 frequency control register offset
#define SC_PLL1         INT_AT(0x50)
//! System controller r21: PLL2 frequency control register offset
#define SC_PLL2         INT_AT(0x54)

//! System controller r22: set flags register offset
#define SC_FLAG         INT_AT(0x58)
//! Alias for #SC_FLAG
#define SC_SETFLAG      INT_AT(0x58)
//! System controller r23: clear flags register offset
#define SC_CLRFLAG      INT_AT(0x5c)

//! System controller r24: clock multiplexer control register offset
#define SC_CLKMUX       INT_AT(0x60)
//! System controller r25: CPU sleep status register offset
#define SC_SLEEP        INT_AT(0x64)

//! System controller r26: temperature sensor 0 register offset
#define SC_TS0          INT_AT(0x68)
//! System controller r27: temperature sensor 1 register offset
#define SC_TS1          INT_AT(0x6c)
//! System controller r28: temperature sensor 2 register offset
#define SC_TS2          INT_AT(0x70)

//! System controller r32: arbiter 0 register offset
#define SC_ARB0         INT_AT(0x080)
//! System controller r64: test-and-set 0 register offset
#define SC_TAS0         INT_AT(0x100)
//! System controller r96: test-and-clear 0 register offset
#define SC_TAC0         INT_AT(0x180)

//! System controller r128: link disable register offset
#define SC_LINK_DIS     INT_AT(0x200)

//! Reset codes (see #SC_RST_CODE)
enum {
    RST_POR =       0, //!< Power-on reset
    RST_WDT =       1, //!< Watchdog reset
    RST_USER =      2, //!< User reset
    RST_SW =        3, //!< Reset entire chip
    RST_WDI =       4  //!< Watchdog interrupt (monitor only)
};
//! \}

//------------------------------------------------------------------------------

//! \name Watchdog Controller
//! \brief Watchdog timer definitions
//! \details Datasheet, section 16
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

//! Watchdog timer r0: count load register offset
#define WD_LOAD         INT_AT(0x00)
//! Watchdog timer r1: current count register offset
#define WD_COUNT        INT_AT(0x04)
//! Watchdog timer r2: control register offset
#define WD_CTRL         INT_AT(0x08)
//! Watchdog timer r3: interrupt clear register offset
#define WD_INTCLR       INT_AT(0x0c)
//! Watchdog timer r4: raw interrupt status register offset
#define WD_RAWINT       INT_AT(0x10)
//! Watchdog timer r5: masked interrupt status register offset
#define WD_MSKINT       INT_AT(0x14)
//! Watchdog timer r6: lock register offset
#define WD_LOCK         INT_AT(0xc00)

//! Watchdog timer: magic
#define WD_CODE         0x1acce551

//! Flags for #WD_CTRL
enum {
    WD_INT_B = 1, //!< enable watchdog counter and interrupt
    WD_RST_B = 2  //!< enable watchdog reset output
};
//! \}

//------------------------------------------------------------------------------

//! \name Ethernet Controller
//! Ethernet controller definitions
//! \details Datasheet, section 15
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

//! Ethernet controller r0: general command register offset
#define ETH_CONTROL     INT_AT(0x00)
//! Ethernet controller r1: general status register offset
#define ETH_STATUS      INT_AT(0x04)
//! Ethernet controller r2: transmit frame length register offset
#define ETH_TX_LEN      INT_AT(0x08)
//! Ethernet controller r3: transmit command register offset
#define ETH_TX_CMD      INT_AT(0x0c)
//! Ethernet controller r4: receive command register offset
#define ETH_RX_CMD      INT_AT(0x10)
//! Ethernet controller r5: MAC address low bytes register offset
#define ETH_MAC_LO      INT_AT(0x14)
//! Ethernet controller r6: MAC address high bytes register offset
#define ETH_MAC_HI      INT_AT(0x18)
//! Ethernet controller r7: PHY control register offset
#define ETH_PHY_CTRL    INT_AT(0x1c)
//! Ethernet controller r8: interrupt clear register offset
#define ETH_INT_CLR     INT_AT(0x20)
//! Ethernet controller r9: receive frame buffer read pointer register offset
#define ETH_RX_BUF_RP   INT_AT(0x24)
//! Ethernet controller r10: receive frame buffer write pointer register offset
#define ETH_RX_DESC_RP  INT_AT(0x2c)

//! Bits in #ETH_INT_CLR
enum {
    ETH_TX_CLR =    0x01, //!< Clear transmit interrupt request
    ETH_RX_CLR =    0x10  //!< Clear receive interrupt request
};

//! Bits in #ETH_PHY_CTRL
enum {
    PHY_CTRL_NRST =     1, //!< PHY reset (active low)
    PHY_CTRL_DIN =      2, //!< SMI data input
    PHY_CTRL_DOUT =     4, //!< SMI data output
    PHY_CTRL_OE =       8, //!< SMI data output enable
    PHY_CTRL_CLK =     16  //!< SMI clock (active rising)
};
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
//! \}

//------------------------------------------------------------------------------

//! \name SpiNNaker Packets
//! \brief SpiNNaker packet definitions
//! \{

//! Bits in SpiNNaker packet control word
enum {
    PKT_MC =        0x00000000, //!< Packet is multicast packet
    PKT_P2P =       0x00400000, //!< Packet is peer-to-peer packet
    PKT_NN =        0x00800000, //!< Packet is normal nearest-neighbour packet
    PKT_NND =       0x00a00000, //!< Packet is peek/poke nearest-neighbour
    PKT_FR =        0x00c00000, //!< Packet is fixed route packet

    PKT_PL =        0x00020000, //!< Packet has payload word

    PKT_MC_PL =     (PKT_MC + PKT_PL),  //!< Multicast with payload
    PKT_P2P_PL =    (PKT_P2P + PKT_PL), //!< Peer-to-peer with payload
    PKT_NN_PL =     (PKT_NN + PKT_PL),  //!< Nearest-neighbour with payload
    PKT_FR_PL =     (PKT_FR + PKT_PL)   //!< Fixed-route with payload
};
//! \}

//------------------------------------------------------------------------------

//! \name PLL Definitions
//! \brief Clock & PLL definitions
//! \{

#define CLK_XTAL_MHZ    10          //!< Crystal frequency (MHz)

//! PLL 260 MHz clock (assuming 10 MHz input, #CLK_XTAL_MHZ)
#define PLL_260         0x0007011a
//! PLL 300 MHz clock (assuming 10 MHz input, #CLK_XTAL_MHZ)
#define PLL_300         0x0007011e
//! PLL 330 MHz clock (assuming 10 MHz input, #CLK_XTAL_MHZ)
#define PLL_330         0x00070121
//! PLL 400 MHz clock (assuming 10 MHz input, #CLK_XTAL_MHZ)
#define PLL_400         0x00070128

//! Time for PLL to lock (&mu;s)
#define PLL_LOCK_TIME   80

#define PLL_CLK_SEL     0x809488a5  //!< CPU/2, SYS/3, RTR/3, MEM/1
//! \}

//------------------------------------------------------------------------------

//! \name GPIO
//! \brief Bits in GPIO[31:0]
//! \{

//! Serial ROM (GPIO[5:2])
enum {
    SERIAL_NCS =    0x00000020, // Out
    SERIAL_CLK =    0x00000010, // Out
    SERIAL_SI =     0x00000008, // Out
    SERIAL_SO =     0x00000004, // In

    SERIAL_OE =     (SERIAL_NCS + SERIAL_CLK + SERIAL_SI)
};

// On-chip SDRAM
//! Bits in #GPIO_PORT, also on-chip SDRAM
enum {
    SDRAM_TQ =      (1 << 31),
    SDRAM_DPD =     (1 << 30),
    SDRAM_HERE =    (1 << 29),

    JTAG_NTRST =    (1 << 27),
    JTAG_TMS =      (1 << 26),
    JTAG_TDI =      (1 << 25),
    JTAG_TCK =      (1 << 24)
};

//! Bits in #SC_MISC_CTRL
enum {
    JTAG_RTCK =     (1 << 20), //!< read value on JTAG_RTCK pin
    JTAG_TDO =      (1 << 19), //!< read value on JTAG_TDO pin
    JTAG_INT =      (1 << 15)  //!< select on-chip (1) or off-chip (0) control of JTAG pins
};

// LEDs

#define LED_0           0x00000001  //!< LED on bit 0
//! \}

//------------------------------------------------------------------------------

//! \name CPSR Miscellany
//! \brief ARM CPSR bits
//! \{

//! \brief Bits in the ARM Current Program Status Register
//! \details
//! 	https://www.keil.com/pack/doc/CMSIS/Core_A/html/group__CMSIS__CPSR.html
enum {
    // Mode bits: [4:0]
    MODE_USER =     0x10, //!< User mode
    MODE_FIQ =      0x11, //!< Fast interrupt mode
    MODE_IRQ =      0x12, //!< Interrupt mode
    MODE_SVC =      0x13, //!< Supervisor mode
    MODE_ABT =      0x17, //!< Abort mode
    MODE_UND =      0x1b, //!< Undefined mode
    MODE_SYS =      0x1f, //!< System mode

    // Thumb bit: [5]
    THUMB_BIT =     0x20, //!< Thumb instruction mode

    // Interrupt control: [7:6]
    IMASK_IRQ =     0x80, //!< Mask: IRQ enabled
    IMASK_FIQ =     0x40, //!< Mask: FIQ enabled
    IMASK_ALL =     0xc0  //!< Mask: All interrupts enabled
};
//! \}

//------------------------------------------------------------------------------

#endif
