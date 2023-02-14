//------------------------------------------------------------------------------
//
//! \file bmp_eth.c
//! \brief          Ethernet hardware interface code for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012-2023 The University of Manchester
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

#include <string.h>

#include "lpc17xx.h"
#include "lpc17xx_emac.h"

#include "bmp.h"

//! Number of receive buffers
#define ETH_RX_BUFS 4
//! Number of transmit buffers
#define ETH_TX_BUFS 3

//! Ethernet receive descriptor
typedef struct {
    uint32_t Packet;    //!< Receive Packet Descriptor
    uint32_t Ctrl;      //!< Receive Control Descriptor
} rx_desc_t;

//! Ethernet receive status
typedef struct {
    uint32_t Info;      //!< Receive Information Status
    uint32_t HashCRC;   //!< Receive Hash CRC Status
} rx_stat_t;

//! Ethernet transmit descriptor
typedef struct {
    uint32_t Packet;    //!< Transmit Packet Descriptor
    uint32_t Ctrl;      //!< Transmit Control Descriptor
} tx_desc_t;

//! Ethernet transmit status
typedef struct {
    uint32_t Info;      //!< Transmit Information Status
} tx_stat_t;


//! Ethernet receive descriptors
static rx_desc_t rx_desc[ETH_RX_BUFS];
//! Ethernet receive status
static ALIGNED(8) rx_stat_t rx_stat[ETH_RX_BUFS];
//! Ethernet receive buffers
static uint32_t  rx_buf[ETH_RX_BUFS][EMAC_ETH_MAX_FLEN >> 2];

//! Ethernet transmit descriptors
static tx_desc_t tx_desc[ETH_TX_BUFS];
//! Ethernet transmit status
static tx_stat_t tx_stat[ETH_TX_BUFS];
//! Ethernet transmit buffers
static uint32_t  tx_buf[ETH_TX_BUFS][EMAC_ETH_MAX_FLEN >> 2];


//! \brief Is the ethernet hardware ready to receive?
//! \return true if a packet has been received
uint32_t eth_rx_rdy(void)
{
    return LPC_EMAC->RxConsumeIndex != LPC_EMAC->RxProduceIndex;
}


//! \brief Is the ethernet hardware ready to transmit?
//! \return true if a packet can be sent now
uint32_t eth_tx_rdy(void)
{
    return LPC_EMAC->TxProduceIndex != (LPC_EMAC->TxConsumeIndex - 1);
}


//! \brief Select the next transmission buffer.
void eth_update_tx(void)
{
    uint32_t idx = LPC_EMAC->TxProduceIndex + 1;

    if (idx == ETH_TX_BUFS) {
        idx = 0;
    }
    LPC_EMAC->TxProduceIndex = idx;
}

//! Discard received packet, releasing buffer for reuse
void eth_rx_discard(void)
{
    uint32_t idx = LPC_EMAC->RxConsumeIndex + 1;

    if (idx == ETH_RX_BUFS) {
        idx = 0;
    }
    LPC_EMAC->RxConsumeIndex = idx;
}


//! \brief Copy supplied buffer into transmit hardware
//! \param[in] buffer: Buffer to copy the message from
//! \param[in] length: Length of message
void eth_copy_txbuf(uint32_t *buffer, uint32_t length)
{
    uint32_t idx = LPC_EMAC->TxProduceIndex;
    uint32_t *dest = (uint32_t *) tx_desc[idx].Packet;
    uint32_t alen = (length + 3) & ~3;

    memcpy(dest, buffer, alen);

    tx_desc[idx].Ctrl = (length - 1) | (EMAC_TCTRL_INT | EMAC_TCTRL_LAST);
}


//! \brief Copy received message into supplied buffer
//! \param[out] buffer: Buffer to copy the message into
//! \param[in] length: Length of message. Buffer must be at least this large.
void eth_copy_rxbuf(uint32_t *buffer, uint32_t length)
{
    uint32_t idx = LPC_EMAC->RxConsumeIndex;
    uint32_t *srce = (uint32_t *) rx_desc[idx].Packet;
    uint32_t alen = (length + 3) & ~3;

    memcpy(buffer, srce, alen);
}


//! \brief Get size of received data.
//! \return Size of message, in bytes
uint32_t eth_rx_size(void)
{
    uint32_t idx = LPC_EMAC->RxConsumeIndex;

    return rx_stat[idx].Info & EMAC_RINFO_SIZE;
}


//! Initialise receive descriptors
static void rx_desc_init(void)
{
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

//! Initialise transmit descriptors
static void tx_desc_init(void)
{
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

//! \brief Configure the ethernet hardware
//! \param[in] mac_addr: The MAC address to use
void configure_eth(const uint8_t *mac_addr)
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

    // !! ST ?? Reset Reduced MII Logic
    /*
    LPC_EMAC->SUPP = EMAC_SUPP_RES_RMII;

    delay_us(5);

    LPC_EMAC->SUPP = 0;
    */

    LPC_EMAC->SUPP = EMAC_SUPP_SPEED;   // !! Bodge - assume 100 MHz for now

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
    // !!  LPC_EMAC->IntEnable = EMAC_INT_RX_DONE | EMAC_INT_TX_DONE;

    // Reset all interrupts
    // !! LPC_EMAC->IntClear  = 0xFFFF;

    // Enable receive and transmit mode of MAC Ethernet core
    LPC_EMAC->Command |= (EMAC_CR_RX_EN | EMAC_CR_TX_EN);
    LPC_EMAC->MAC1 |= EMAC_MAC1_REC_EN;
}
