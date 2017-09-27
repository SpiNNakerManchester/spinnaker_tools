//------------------------------------------------------------------------------
//
// bmp_eth.c	    Ethernet hardware interface code for BMP LPC1768
//
// Copyright (C)    The University of Manchester - 2012-2015
//
// Author           Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

#include <string.h>

#include "lpc17xx.h"
#include "lpc17xx_emac.h"

#include "bmp.h"

#define ETH_RX_BUFS 4
#define ETH_TX_BUFS 3


typedef struct {
    uint32_t Packet;	// Receive Packet Descriptor
    uint32_t Ctrl;	// Receive Control Descriptor
} rx_desc_t;


typedef struct {
    uint32_t Info;	// Receive Information Status
    uint32_t HashCRC;	// Receive Hash CRC Status
} rx_stat_t;

typedef struct {
    uint32_t Packet;	// Transmit Packet Descriptor
    uint32_t Ctrl;	// Transmit Control Descriptor
} tx_desc_t;

typedef struct {
    uint32_t Info;	// Transmit Information Status
} tx_stat_t;


static rx_desc_t rx_desc[ETH_RX_BUFS];
static __attribute__ ((aligned(8))) rx_stat_t rx_stat[ETH_RX_BUFS];
static uint32_t  rx_buf[ETH_RX_BUFS][EMAC_ETH_MAX_FLEN>>2];

static tx_desc_t tx_desc[ETH_TX_BUFS];
static tx_stat_t tx_stat[ETH_TX_BUFS];
static uint32_t  tx_buf[ETH_TX_BUFS][EMAC_ETH_MAX_FLEN>>2];


uint32_t eth_rx_rdy(void)
{
    return LPC_EMAC->RxConsumeIndex != LPC_EMAC->RxProduceIndex;
}


uint32_t eth_tx_rdy(void)
{
    return LPC_EMAC->TxProduceIndex != (LPC_EMAC->TxConsumeIndex - 1);
}


void eth_update_tx(void)
{
    uint32_t idx = LPC_EMAC->TxProduceIndex + 1;

    if (idx == ETH_TX_BUFS) {
	idx = 0;
    }
    LPC_EMAC->TxProduceIndex = idx;
}

void eth_rx_discard(void)
{
    uint32_t idx = LPC_EMAC->RxConsumeIndex + 1;

    if (idx == ETH_RX_BUFS) {
	idx = 0;
    }
    LPC_EMAC->RxConsumeIndex = idx;
}


void eth_copy_txbuf(uint32_t *buffer, uint32_t length)
{
    uint32_t idx = LPC_EMAC->TxProduceIndex;
    uint32_t *dest = (uint32_t *) tx_desc[idx].Packet;
    uint32_t alen = (length + 3) & ~3;

    memcpy(dest, buffer, alen);

    tx_desc[idx].Ctrl = (length - 1) | (EMAC_TCTRL_INT | EMAC_TCTRL_LAST);
}


void eth_copy_rxbuf(uint32_t *buffer, uint32_t length)
{
    uint32_t idx = LPC_EMAC->RxConsumeIndex;
    uint32_t *srce = (uint32_t *) rx_desc[idx].Packet;
    uint32_t alen = (length + 3) & ~3;

    memcpy(buffer, srce, alen);
}


uint32_t eth_rx_size(void)
{
    uint32_t idx = LPC_EMAC->RxConsumeIndex;

    return rx_stat[idx].Info & EMAC_RINFO_SIZE;
}


static void rx_desc_init(void)
{
    // Initialise receive descriptors
    for (uint32_t i = 0; i < ETH_RX_BUFS; i++) {
	rx_desc[i].Packet  = (uint32_t) &rx_buf[i];
	rx_desc[i].Ctrl    = EMAC_RCTRL_INT | (EMAC_ETH_MAX_FLEN - 1);

	rx_stat[i].Info    = 0;
	rx_stat[i].HashCRC = 0;
    }

    // Set EMAC Receive Descriptor Registers.
    LPC_EMAC->RxDescriptor = (uint32_t) rx_desc;
    LPC_EMAC->RxStatus = (uint32_t) rx_stat;
    LPC_EMAC->RxDescriptorNumber = ETH_RX_BUFS - 1;

    // Rx Descriptors Point to 0
    LPC_EMAC->RxConsumeIndex  = 0;
}

static void tx_desc_init(void)
{
    // Initialise transmit descriptors
    for (uint32_t i = 0; i < ETH_TX_BUFS; i++) {
	tx_desc[i].Packet = (uint32_t) &tx_buf[i];
	tx_desc[i].Ctrl   = 0;
	tx_stat[i].Info   = 0;
    }

    // Set EMAC Transmit Descriptor Registers
    LPC_EMAC->TxDescriptor = (uint32_t) tx_desc;
    LPC_EMAC->TxStatus = (uint32_t) tx_stat;
    LPC_EMAC->TxDescriptorNumber = ETH_TX_BUFS - 1;

    // Tx Descriptors Point to 0 */
    LPC_EMAC->TxProduceIndex = 0;
}


void configure_eth(uint8_t *mac_addr)
{
    // Reset all EMAC internal modules
    LPC_EMAC->MAC1 = EMAC_MAC1_RES_TX | EMAC_MAC1_RES_MCS_TX
	    | EMAC_MAC1_RES_RX | EMAC_MAC1_RES_MCS_RX | EMAC_MAC1_SIM_RES
	    | EMAC_MAC1_SOFT_RES;
    LPC_EMAC->Command = EMAC_CR_REG_RES | EMAC_CR_TX_RES | EMAC_CR_RX_RES
	    | EMAC_CR_PASS_RUNT_FRM;

    delay_us(5);

    // Initialize MAC control registers
    LPC_EMAC->MAC1 = EMAC_MAC1_PASS_ALL;
    LPC_EMAC->MAC2 = EMAC_MAC2_CRC_EN | EMAC_MAC2_PAD_EN;
    LPC_EMAC->MAXF = EMAC_ETH_MAX_FLEN;

    // Write to MII configuration register and reset
    LPC_EMAC->MCFG = EMAC_MCFG_CLK_SEL(9) | EMAC_MCFG_RES_MII;
    // Release reset
    LPC_EMAC->MCFG &= ~(EMAC_MCFG_RES_MII);

    LPC_EMAC->CLRT = EMAC_CLRT_DEF;
    LPC_EMAC->IPGR = EMAC_IPGR_P2_DEF;

    // Enable Reduced MII interface.
    LPC_EMAC->Command = EMAC_CR_RMII | EMAC_CR_PASS_RUNT_FRM;

    //!! ST ?? Reset Reduced MII Logic
    /*
    LPC_EMAC->SUPP = EMAC_SUPP_RES_RMII;

    delay_us(5);

    LPC_EMAC->SUPP = 0;
    */

    LPC_EMAC->SUPP = EMAC_SUPP_SPEED;	//!! Bodge - assume 100 MHz for now

    // Set EMAC address
    LPC_EMAC->SA0 = (mac_addr[5] << 8) | mac_addr[4];
    LPC_EMAC->SA1 = (mac_addr[3] << 8) | mac_addr[2];
    LPC_EMAC->SA2 = (mac_addr[1] << 8) | mac_addr[0];

    // Initialize Tx and Rx DMA Descriptors
    rx_desc_init();
    tx_desc_init();

    // Set Receive Filter register: enable broadcast and multicast
    LPC_EMAC->RxFilterCtrl = EMAC_RFC_MCAST_EN | EMAC_RFC_BCAST_EN
	    | EMAC_RFC_PERFECT_EN;

    // Enable Rx Done and Tx Done interrupt for EMAC
    //!!  LPC_EMAC->IntEnable = EMAC_INT_RX_DONE | EMAC_INT_TX_DONE;

    // Reset all interrupts
    //!! LPC_EMAC->IntClear  = 0xFFFF;

    // Enable receive and transmit mode of MAC Ethernet core
    LPC_EMAC->Command |= (EMAC_CR_RX_EN | EMAC_CR_TX_EN);
    LPC_EMAC->MAC1 |= EMAC_MAC1_REC_EN;
}
