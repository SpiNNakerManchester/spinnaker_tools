//------------------------------------------------------------------------------
//
// bmp.h            Low-level hardware interface code for BMP LPC1768
//
// Copyright (C)    The University of Manchester - 2012-2018
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012-2019 The University of Manchester
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

//
//! \file
//! \brief Header file for Spin4/Spin5 Board Management Processor (BMP, LPC1768)
//

#include <stdint.h>
#include <stdbool.h>

#ifndef DOXYGEN
#define NORETURN        __attribute__((noreturn))
#define SECTION(sec)    __attribute__((section (sec)))
#define ALIAS(fun)      __attribute__((weak, alias(#fun)))
#define ALIGNED(a)      __attribute__ ((aligned(a)))
#endif

//------------------------------------------------------------------------------

#define BMP_VER_STR             "2.1.2"
#define BMP_VER_NUM             0x020102

#define BMP_ID_STR              "BC&MP/Spin5-BMP"

//------------------------------------------------------------------------------

#define CCLK                    100000000       // CPU clock - 100 MHz

#ifndef NULL
#define NULL 0
#endif

//------------------------------------------------------------------------------

enum bmp_led_code {
    LED_0 = (1 << 4),   //!< Green
    LED_1 = (1 << 5),   //!< Orange
    LED_2 = (1 << 19),  //!< Red
    LED_3 = (1 << 20),  //!< Green
    LED_4 = (1 << 21),  //!< Green
    LED_5 = (1 << 22),  //!< Green
    LED_6 = (1 << 29),  //!< Green
    LED_7 = (1 << 30),  //!< Red
};

#define LED_MASK                (LED_0 + LED_1 + LED_2 + LED_3 + \
                                 LED_4 + LED_5 + LED_6 + LED_7)

#define SF_NCS                  (1 << 16)

#define XFSEL_0                 (1 << 25)       // Port 3
#define XFSEL_1                 (1 << 26)       // Port 3
#define XFSEL_2                 (1 << 29)       // Port 4

//------------------------------------------------------------------------------

//! Number of FPGAs available (on SpiNN-5 board)
#define NUM_FPGAS               3

//! Directions for fpga_word()
enum bmp_fpga_command {
    FPGA_READ = 0,      //!< Read from FPGA
    FPGA_WRITE = 1      //!< Write to FPGA
};

//------------------------------------------------------------------------------

enum bmp_commands {
    CMD_VER = 0,
    CMD_RUN = 1,
    CMD_READ = 2,
    CMD_WRITE = 3,
    CMD_FILL = 5,

    CMD_FPGA_READ = 17,      // SPI interface to FPGAs
    CMD_FPGA_WRITE = 18,      //

    CMD_LED = 25,
    CMD_IPTAG = 26,

    CMD_BMP_INFO = 48,
    CMD_FLASH_COPY = 49,
    CMD_FLASH_ERASE = 50,
    CMD_FLASH_WRITE = 51,
    CMD_XXX_52 = 52,
    CMD_BMP_SF = 53,
    CMD_BMP_EE = 54,
    CMD_RESET = 55,
    CMD_XILINX = 56,
    CMD_POWER = 57,

    CMD_BMP_TEST = 63,

    CMD_TUBE = 64
};

enum bmp_type_code {
    TYPE_BYTE = 0,
    TYPE_HALF = 1,
    TYPE_WORD = 2
};

enum bmp_return_code {
    RC_OK = 0x80,       //!< Command completed OK
    RC_LEN = 0x81,      //!< Bad packet length
    RC_SUM = 0x82,      //!< Bad checksum
    RC_CMD = 0x83,      //!< Bad/invalid command
    RC_ARG = 0x84,      //!< Invalid arguments
    RC_PORT = 0x85,     //!< Bad port number
    RC_TIMEOUT = 0x86,  //!< Timeout
    RC_ROUTE = 0x87,    //!< No P2P route
    RC_CPU = 0x88       //!< Bad CPU number
};

#define BOARD_MASK              31

#define FLASH_BYTES             4096    //!< Size of flash buffer (bytes)
#define FLASH_WORDS             1024    //!< Size of flash buffer (words)

// IPTAG definitions

enum bmp_iptag_commands {
    IPTAG_NEW = 0,
    IPTAG_SET = 1,
    IPTAG_GET = 2,
    IPTAG_CLR = 3,
    IPTAG_TTO = 4
};

#define IPTAG_MAX               4

enum bmp_iptag_flags {
    IPTAG_VALID = 0x8000,       //!< Entry is valid
    IPTAG_TRANS = 0x4000,       //!< Entry is transient
    IPTAG_ARP = 0x2000          //!< Awaiting ARP resolution
};

#define TAG_NONE                255     // Invalid tag/transient request
#define TAG_HOST                0       // Reserved for host


#define TAG_FIXED_SIZE          8       // At bottom of table
#define TAG_POOL_SIZE           8

#define FIRST_POOL_TAG          TAG_FIXED_SIZE
#define LAST_POOL_TAG           (TAG_FIXED_SIZE + TAG_POOL_SIZE - 1)

#define TAG_TABLE_SIZE          (TAG_FIXED_SIZE + TAG_POOL_SIZE)
#define SDPF_REPLY              0x80


typedef struct {                //!< IPTAG entry (24 bytes)
    uint8_t ip[4];
    uint8_t mac[6];
    uint16_t port;
    uint16_t timeout;
    uint16_t flags;
    uint32_t count;
    uint32_t PAD;
} iptag_t;

//------------------------------------------------------------------------------

#define PORT_SHIFT              5
#define PORT_MASK               7
#define BOARD_MASK              31

#define PORT_ETH                (7 << PORT_SHIFT)       // Port 7

#define SDP_BUF_SIZE            256

// ------------------------------------------------------------------------
//! \brief SDP message type definition
// ------------------------------------------------------------------------
//! \details
//! Note that the length field is the number of bytes following
//! the checksum. It will be a minimum of 8 as the SDP header
//! should always be present.

typedef struct sdp_msg {        // SDP message (=292 bytes)
    struct sdp_msg *next;       //!< Next in free list
    uint16_t length;            //!< length
    uint16_t checksum;          //!< checksum (if used)

    // sdp_hdr_t

    uint8_t flags;              //!< SDP flag byte
    uint8_t tag;                //!< SDP IPtag
    uint8_t dest_port;          //!< SDP destination port/CPU
    uint8_t srce_port;          //!< SDP source port/CPU
    uint16_t dest_addr;         //!< SDP destination address
    uint16_t srce_addr;         //!< SDP source address

    // cmd_hdr_t (optional)

    uint16_t cmd_rc;            //!< Command/Return Code
    uint16_t seq;               //!< Sequence number
    uint32_t arg1;              //!< Arg 1
    uint32_t arg2;              //!< Arg 2
    uint32_t arg3;              //!< Arg 3

    // user data (optional)

    uint8_t data[SDP_BUF_SIZE]; //!< User data (256 bytes)

    uint32_t _PAD;              // Private padding
} sdp_msg_t;

//------------------------------------------------------------------------------

//! IO stream that goes immediately to host (tubotron)
#define IO_STD          ((char *) 0)            // Stream numbers
//! IO stream for debugging
#define IO_DBG          ((char *) 1)
//! IO stream that goes to the LCD panel
#define IO_LCD          ((char *) 2)
//! IO stream that gets dumped
#define IO_NULL         ((char *) 3)

#define LCD_POS(x, y)   (0x80 + (y) * 64 + (x))

//------------------------------------------------------------------------------

#define FL_DIR_SIZE     16              //!< Number of fl_dir entries

//! Values in fl_dir->type
enum bmp_fl_dir_type {
    FL_BMP_IP = 1,
    FL_SPIN_IP = 2,
    FL_FPGA = 3,
    FL_XREG = 4,
};

typedef struct {        // 128 bytes
    uint8_t type;
    uint8_t size;
    uint16_t flags;
    uint32_t time;
    uint32_t crc;
    uint32_t base;
    uint32_t length;
    uint32_t p0;
    uint32_t p1;
    uint32_t p2;
    uint8_t data[96];
} fl_dir_t;

typedef struct {        // 256 bytes
    uint8_t marker;             //!< 0: == 0x96
    uint8_t sw_ver;             //!< 1: EE Data format version
    uint8_t hw_ver;             //!< 2: Backplane HW version (0..7)
    uint8_t frame_ID;           //!< 3: Frame Identifier

    uint32_t mod_date;          //!< 4: Date written
    uint8_t gw_addr[4];         //!< 8: Gateway address (& IP base)
    uint8_t flags;              //!< 12: 8 flag bits
    uint8_t mask_bits;          //!< 13: IP mask bits (0..31)
    uint8_t mac_byte;           //!< 14: Byte 1 of MAC address
    uint8_t LCD_time;           //!< 15: Time (secs) for initial display

    uint8_t __PAD1[16];

    int8_t warn_int[4];         //!< 32: Int Temp Warning settings
    int8_t shut_int[4];         //!< 36: Int Temp Shutdown settings

    int8_t warn_ext[4];         //!< 40: Ext Temp Warning settings
    int8_t shut_ext[4];         //!< 44: Ext Temp Shutdown settings

    uint8_t warn_fan[4];        //!< 48: Fan Speed Warning settings
    uint8_t shut_fan[4];        //!< 52: Fan Speed Shutdown settings

    uint8_t warn_vlow[8];       //!< 56: Under-voltage Warning settings
    uint8_t shut_vlow[8];       //!< 64: Under-voltage Shutdown settings

    uint8_t warn_vhigh[8];      //!< 72: Over-voltage Warning settings
    uint8_t shut_vhigh[8];      //!< 80: Over-voltage Shutdown settings

    uint8_t __PAD2[164];        // 88
    uint32_t CRC32;             //!< 252: CRC
} ee_data_t;

typedef struct {        // 48 bytes
    uint16_t adc[8];
    int16_t t_int[4];
    int16_t t_ext[4];
    uint16_t fan[4];
    uint32_t warning;
    uint32_t shutdown;
} board_stat_t;

typedef struct {        // 32 bytes
    uint16_t flags;
    uint8_t mac_addr[6];
    uint8_t ip_addr[4];
    uint8_t gw_addr[4];
    uint8_t net_mask[4];
    uint16_t udp_port;
    uint16_t __PAD1;
    uint32_t __PAD2;
    uint32_t __PAD3;
} ip_data_t;

//------------------------------------------------------------------------------

// Cortex boot vector formats

typedef void (*handler) (void);
typedef void (*proc4) (uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*main_proc) (void *, uint32_t, uint32_t, uint32_t);

typedef struct {
    uint32_t *stack_top;        // 0
    main_proc main;

    handler NMI;                // 2
    handler HardFault;
    handler MemManage;
    handler BusFault;
    handler UsageFault;
    handler Rsvd_7;
    handler Rsvd_8;
    handler Rsvd_9;
    handler Rsvd_10;
    handler SVC;
    handler DebugMon;
    handler Rsvd_13;
    handler PendSV;
    handler SysTickH;

    handler WDT;                // 16
    handler TIMER0;
    handler TIMER1;
    handler TIMER2;
    handler TIMER3;
    handler UART0;
    handler UART1;
    handler UART2;
    handler UART3;
    handler PWM1;
    handler I2C0;
    handler I2C1;
    handler I2C2;
    handler SPI;
    handler SSP0;
    handler SSP1;

    handler PLL0;               // 32
    handler RTC;
    handler EINT0;
    handler EINT1;
    handler EINT2;
    handler EINT3;
    handler ADC;
    handler BOD;
    handler USB;
    handler CAN;
    handler DMA;
    handler I2S;
    handler ENET;
    handler RIT;
    handler MCPWM;
    handler QEI;

    handler PLL1;               // 48
    handler USBActivity;
    handler CANActivity;

    handler Rsvd_51;            // 51
    handler Rsvd_52;

    uint32_t build_date;        // 53
    uint32_t sw_ver;

    uint32_t RO_length;         // 55
    uint32_t RW_length;
    uint32_t *RO_limit;
    uint32_t *RW_base;
    uint32_t *RW_limit;
    uint32_t *ZI_base;
    uint32_t *ZI_limit;
    uint32_t *stack_base;
    uint32_t *stack_limit;      // 63
} cortex_vec_t;

typedef struct {
    uint32_t *stack_top;        // 0
    handler boot_proc;          // 1
    proc4 flash_copy;           // 2
    proc4 Rsvd_3;               // 3
    proc4 Rsvd_4;               // 4
    uint32_t build_date;        // 5
    uint32_t sw_ver;            // 6
    uint32_t checksum;          // 7
} boot_vec_t;

static boot_vec_t * const boot_vec = (boot_vec_t *) 0;

extern cortex_vec_t *cortex_vec;

//------------------------------------------------------------------------------

#define PROC_RESET 0
#define PROC_POWER 1
#define PROC_LED   2

//------------------------------------------------------------------------------

static fl_dir_t  *const fl_dir =        (fl_dir_t *)  0x1000;
static ip_data_t *const bmp_flash_ip =  (ip_data_t *) 0x1020;
static ip_data_t *const spin_flash_ip = (ip_data_t *) 0x10a0;

//! \brief LPC17xx In-Application Programming entry point
//! \param[in] cmd: Command to apply
//! \param[out] res: Result
typedef void (*LPC_IAP) (uint32_t *cmd, uint32_t *res);

//! \brief LPC17xx In Application Programming entry point
//! \note This is a THUMB address in ROM
static LPC_IAP const lpc_iap = (LPC_IAP) 0x1fff1ff1;

//! \brief 4096 byte buffer used for writing to Flash and as a general purpose
//! buffer

static uint32_t * const flash_buf = (uint32_t *) 0x10000000;

// 32 byte (8 word) uninitialised vector
// 0 - copy of last RSID register
// 1 - count of WDT timeouts
// 2 - up time (seconds)
// 3 - time of last WDT/SYSRESET (copy of up time)
// 4 - count of SYSRESETs
// 5 - NVIC->IABR[0]: active interrupts @ call to 'error_han'
// 6 - IPSR: exception that caused call to 'error_han'
// 7 - count of calls to 'error_han'

#define UNI_VEC_SIZE            8
static uint32_t * const uni_vec = (uint32_t *) 0x10001000;

// 64 byte (16 word) uninitialised fault debug vector
//  0 - stacked r0
//  1 - stacked r1
//  2 - stacked r2
//  3 - stacked r3
//  4 - stacked r12
//  5 - stacked lr
//  6 - stacked pc
//  7 - stacked psr
//  8 - Configurable Fault Status Register
//  9 - Hard Fault Status Register
// 10 - Debug Fault Status Register
// 11 - Auxiliary Fault Status Register
// 12 - bus fault address
// 13 - memory mgmt fault address
// 14 - exc_return

#define DBG_VEC_SIZE            16
static uint32_t * const dbg_vec = (uint32_t *) 0x10001020;

//------------------------------------------------------------------------------
//! \name bmp_event.c
//! \{

//! The type of an event handler
typedef void (*event_proc) (uint32_t, uint32_t);

//! Record of how to handle an event
typedef struct event {
    event_proc proc;    //!< Proc to be called or NULL
    uint32_t arg1;      //!< First arg to proc
    uint32_t arg2;      //!< Second arg to proc
    uint32_t time;      //!< Time (CPU ticks) until event due (0 if at head of Q)
    uint32_t ID;        //!< Unique ID for active event (0 if inactive)
    struct event *next; //!< Next in Q or NULL
} event_t;

void event_init (uint32_t priority);
event_t* event_new (event_proc proc, uint32_t a1, uint32_t a2);

void event_set_byte (uint32_t addr, uint32_t data);

void event_schedule (event_t *e, uint32_t t);
void event_cancel (event_t *e, uint32_t ID);

void proc_queue_add (event_t *e);
void proc_queue_run (void);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_heap.c
//! \{

void heap_init (char *base, char *top);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_i2c.c
//! \{

void configure_i2c (void);

uint32_t i2c_poll (LPC_I2C_TypeDef *restrict i2c, uint32_t ctrl);

uint32_t i2c_receive (LPC_I2C_TypeDef *restrict i2c, uint32_t ctrl,
	uint32_t addr, uint32_t length, void *buf);

uint32_t i2c_send (LPC_I2C_TypeDef *restrict i2c, uint32_t ctrl,
	uint32_t addr, uint32_t length, const void *buf);

int16_t read_ts (LPC_I2C_TypeDef *restrict i2c, uint32_t addr);

uint32_t read_ee (uint32_t addr, uint32_t count, void *buf);

uint32_t write_ee (uint32_t addr, uint32_t count, const void *buf);

void configure_lcd (void);
void lcd_ctrl (uint32_t c);
void lcd_putc (uint32_t c);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_hw.c
//! \{

void clock_div (uint32_t bit_pos, uint32_t value);

void delay_us (uint32_t n);
void delay_ms (uint32_t n);
uint32_t is_blank (void *buf, uint32_t len);

void configure_hw (void);
void read_adc (void);
void read_temp (void);
void read_fans (void);

void set_power (uint32_t state);

void reset_spin (uint32_t code);

void fpga_reset (uint32_t code);
uint32_t fpga_init (uint32_t mask);

void refresh_wdt (void);

void led_set (uint32_t leds);

void ssp0_pins (uint32_t on);

#if 0
void die(uint32_t code) NORETURN;
#endif

//! \}
//------------------------------------------------------------------------------
//! \name bmp_flash.c
//! \{

uint32_t is_blank (const void *buf, uint32_t len);
uint32_t flash_sector (uint32_t addr);
uint32_t flash_write (uint32_t addr, uint32_t length, const uint32_t *buffer);
uint32_t flash_erase (uint32_t start, uint32_t end);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_clock.c
//! \{

void configure_clocks (void);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_cmd.c
//! \{

void flash_buf_init (void);

void proc_power (uint32_t arg1, uint32_t arg2);
void proc_reset (uint32_t arg1, uint32_t arg2);
void proc_led   (uint32_t arg1, uint32_t arg2);

uint32_t debug (sdp_msg_t *);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_net.c
//! \{

void eth_receive (void);
void copy_ip_data (void);
void iptag_timer (void);
void copy_ip (const uint8_t *f, uint8_t *t);
void arp_lookup (iptag_t *iptag);
uint32_t iptag_new (void);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_eth.c
//! \{

uint32_t eth_rx_rdy (void);
uint32_t eth_tx_rdy (void);
uint32_t eth_rx_size (void);
void eth_update_tx (void);
void eth_rx_discard (void);
void eth_copy_txbuf (uint32_t *buffer, uint32_t length);
void eth_copy_rxbuf (uint32_t *buffer, uint32_t length);

void configure_eth (const uint8_t *mac_addr);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_can.c
//! \{

void configure_can (uint32_t id);
uint32_t can_send_msg (uint32_t dest, sdp_msg_t *msg);
void can_timer (void);
void can_proc_cmd (uint32_t dest, uint32_t op, uint32_t arg1, uint32_t arg2);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_crc.c
//! \{

uint32_t crc32 (void *buf, uint32_t len, uint32_t crc);
uint32_t crc32_chk (void *buf, uint32_t len);
void     crc32_buf (void *buf, uint32_t len);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_io.c
//! \{

void io_printf (char *stream, char *f, ...);

//! \}
//------------------------------------------------------------------------------
//! \name bmp_ssp.c
//! \{

void configure_ssp (void);

void ssp1_fast (void);
void ssp1_slow (void);

void ssp0_read (uint32_t cmd, uint32_t addr, uint32_t len, uint8_t *buf);
void ssp0_write (uint32_t cmd, uint32_t addr, uint32_t len, const uint8_t *buf);
void ssp0_copy (uint32_t addr, uint32_t len);

void ssp1_copy (uint32_t count, const uint8_t *buf);

void sf_read (uint32_t addr, uint32_t len, uint8_t *buf);
void sf_write (uint32_t addr, uint32_t len, const uint8_t *buf);
uint32_t sf_crc32 (uint32_t addr, uint32_t len);

void fpga_word (uint32_t addr, uint32_t fpga, uint32_t *buf, uint32_t dir);

//! \}
//------------------------------------------------------------------------------

void msg_init (void);

sdp_msg_t* msg_get (void);

uint32_t msg_queue_size (void);
uint32_t msg_queue_insert (sdp_msg_t *msg);
sdp_msg_t* msg_queue_remove (void);

void route_msg (sdp_msg_t * msg);

//------------------------------------------------------------------------------

void *bmp_malloc (uint32_t size);
uint32_t cpu_int_off (void);
void cpu_int_restore (uint32_t cpsr);

//------------------------------------------------------------------------------

#define POWER_ON 2
#define POWER_OFF 0
#define POWER_SHUT 1

extern uint8_t can_ID;
extern uint8_t board_ID;
extern bool bp_ctrl;
extern bool fan_sense;
extern uint8_t power_state;

extern ip_data_t spin_ip;
extern ip_data_t bmp_ip;

#define CAN_SIZE 24

extern uint8_t can_status[CAN_SIZE];
extern board_stat_t board_stat[CAN_SIZE];

extern ee_data_t ee_data;

extern bool lcd_active;

extern uint32_t boot_sec;

extern uint32_t up_time;

extern const uint8_t *can2board;
extern const uint8_t *board2can;

extern bool data_ok;

extern iptag_t tag_table[];

extern const uint32_t hw_ver;

//------------------------------------------------------------------------------
