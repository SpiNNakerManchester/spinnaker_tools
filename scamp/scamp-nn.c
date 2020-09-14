//------------------------------------------------------------------------------
//! \file
//! \brief     NN packet handling for SC&MP
//!
//! \copyright &copy; The University of Manchester - 2009-2011
//!
//! \author    Steve Temple, APT Group, School of Computer Science
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

#include <stdlib.h>

#include "spinnaker.h"
#include "sark.h"
#include "scamp.h"

//------------------------------------------------------------------------------

//! Number of packets to allocate for the nearest neighbour protocols
#define BC_TABLE_SIZE           16

//! Flood fill states
enum ff_states {
    FF_ST_IDLE = 0,             //!< Doing nothing
    FF_ST_EXBLK = 1,            //!< Rcvd FFS or FBE, awaiting FBS, FFE
    FF_ST_INBLK = 2,            //!< Rcvd FBS, awaiting FBD, FBE
};

//! Device identifier (`0x591 << 28`, for SpiNNaker)
#define PART_ID         (CHIP_ID_CODE & 0xfff00000)

//------------------------------------------------------------------------------

//! Nearest neighbour protocol state
typedef struct nn_desc_t {
    uchar state;                //!< FF state (see ::ff_states enum)
    uchar error;                //!< Error flag
    uchar forward;              //!< Rebroadcast code
    uchar retry;                //!< Rebroadcast retry

    uchar id;                   //!< ID of current FF
    uchar block_len;            //!< Block length
    uchar block_count;          //!< Block count
    uchar block_num;            //!< Block number

    uchar app_id;               //!< AppID
    uchar load;                 //!< Set if for us
    ushort srce_addr;           //!< P2P of sender

    ushort word_len;            //!< Number of words in block
    ushort word_count;          //!< Count of received words

    uint gidr_id;               //!< GIDR ID

    uint sum;                   //!< Checksum
    uint region;                //!< Region
    uint cores;                 //!< Selected cores to load
    uint* aplx_addr;            //!< APLX block addr
    uint load_addr;             //!< Block load base
    uint id_set[4];             //!< ID bitmap (128 bits)
    uint biff_id_set[4];        //!< Board-level flood-fill ID bitmap (128 bits)
    uint fbs_set[8];            //!< Block bitmap for FBS (must be >= 8 words)
    uint fbd_set[8];            //!< Word bitmap for FBD (must be >= 8 words)
    uint fbe_set[8];            //!< Block bitmap for FBE (must be >= 8 words)
    uint stats[16];             //!< Counters for each packet type
    uint errors;                //!< Counter for bad checksums
    uint buf[SDP_BUF_SIZE/4];   //!< Holding buffer

    uint64 last_ffcs;           //!< Index of the last FFCS packet
} nn_desc_t;

//------------------------------------------------------------------------------

//! Storage for packets used in ::pkt_buf_list
pkt_buf_t pkt_buf_table[BC_TABLE_SIZE];
//! Free list of packets.
pkt_buf_t *pkt_buf_list;
//! Number of packets (in ::pkt_buf_table) not on free list (::pkt_buf_list)
uint pkt_buf_count;
//! Statistics: maximum value of ::pkt_buf_count
uint pkt_buf_max;

//! \brief Table "hop_table" has entries of type `(id << 24, hops)` for each
//! possible P2P address.
//!
//! Field "hops" contains the number of hops to the address (initialised to
//! 65535) and "id" contains the ID of the last packet used to update the
//! table (initialised to 128). These are packed into a word.
//!
//! Stored in SYSRAM so visible to all cores. Should only be updated by SCAMP
uint *hop_table;

//! Used for tracking signal dispatch
level_t levels[4];

//! What application (or at least AppID) is running on each core.
uchar core_app[MAX_CPUS];
//! Unused?
uint app_mask[256];

//! Nearest neighbour protocol state
static nn_desc_t nn_desc;

//! Packet reserved for performing P2P peeks
pkt_buf_t peek_pkt;
//! Packet reserved for performing P2P pokes
pkt_buf_t poke_pkt;

//! delegate blacklisted monitor
uint mon_del = 0;
//------------------------------------------------------------------------------

//! Initialise the nearest-neighbour data structures
void nn_init(void)
{
    nn_desc.forward = sv->forward; // V104 20jul12 (was 0x3e)

    hop_table = sv->hop_table;

    pkt_buf_t *pkt = (pkt_buf_t *) pkt_buf_table;
    pkt_buf_list = pkt;

    for (uint i = 0; i < BC_TABLE_SIZE; i++) {
        pkt->next = (i != (BC_TABLE_SIZE - 1)) ? pkt + 1 : NULL;
        pkt++;
    }
}

//------------------------------------------------------------------------------

//! \brief Local ethernet chip locator map
//!
//! Given a P2P address, compute P2P address of chip with Ethernet interface
//! on same PCB. Table is for standard 48-chip boards only. Will work with
//! 4-chip boards - may not work for more exotic combinations!
//! Also computes the position of the chip on the PCB as offset from (0,0)
//!
//! Each byte in this table is a pair of X,Y values which are subtracted
//! from a P2P address to give the address of the Ethernet-attached chip
//! on the same 48-chip PCB.

char eth_map[12][12] = {
    {0x00, 0x10, 0x20, 0x30, 0x40, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74},
    {0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75},
    {0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x36, 0x46, 0x56, 0x66, 0x76},
    {0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x47, 0x57, 0x67, 0x77},

    {0x40, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x00, 0x10, 0x20, 0x30},
    {0x41, 0x51, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x01, 0x11, 0x21, 0x31},
    {0x42, 0x52, 0x62, 0x36, 0x46, 0x56, 0x66, 0x76, 0x02, 0x12, 0x22, 0x32},
    {0x43, 0x53, 0x63, 0x73, 0x47, 0x57, 0x67, 0x77, 0x03, 0x13, 0x23, 0x33},

    {0x44, 0x54, 0x64, 0x74, 0x00, 0x10, 0x20, 0x30, 0x40, 0x14, 0x24, 0x34},
    {0x45, 0x55, 0x65, 0x75, 0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x25, 0x35},
    {0x46, 0x56, 0x66, 0x76, 0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x36},
    {0x47, 0x57, 0x67, 0x77, 0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73}
};

//! Computes where our board's ethernet chip is.
void compute_eth(void)
{
    int x = p2p_addr >> 8;
    int y = p2p_addr & 0xFF;
    int x_size = p2p_dims >> 8;
    int y_size = p2p_dims & 0xFF;

    // Compensate for the fact that '0, 0' may not actually lie in a
    // legitimate place for a board corner then compute modulo 12 the cheap
    // way.

    int xm = x - (p2p_root >> 8);
    while (xm < 0) {
        xm += 12;
    }
    while (xm >= 12) {
        xm -= 12;
    }

    int ym = y - (p2p_root & 0xFF);
    while (ym < 0) {
        ym += 12;
    }
    while (ym >= 12) {
        ym -= 12;
    }

    // Get table entry and split fields

    int t = eth_map[ym][xm];
    int xt = t >> 4;
    int yt = t & 15;

    // Add offsets to current address and adjust for wrapping

    x -= xt;
    if (x < 0) {
        x += x_size;
    }

    y -= yt;
    if (y < 0) {
        y += y_size;
    }

    sv->board_addr = (xt << 8) + yt;
    sv->eth_addr = sv->dbg_addr = (x << 8) + y;
}

//------------------------------------------------------------------------------

//! \name Level deltas
//! \{
//! Given a packet arriving from a given direction which is labelled with an X
//! and Y coordinate, gives the dx, dy to get the current chip's coordinate.

const signed char lx[6] = {-1, -1,  0, +1, +1,  0}; //!< X deltas, by link ID
const signed char ly[6] = { 0, -1, -1,  0, +1, +1}; //!< Y deltas, by link ID

//! \}

//------------------------------------------------------------------------------

//! \brief Given a P2P address, compute region addresses for each of the four
//!     levels, updating the levels structure.
//! \param[in] p2p_addr: The address to compute for.
void compute_level(uint p2p_addr)
{
    uint x = p2p_addr >> 8;
    uint y = p2p_addr & 255;

    for (uint lvl = 0; lvl < 4; lvl++) {
        uint shift = 6 - 2 * lvl;
        uint mask = ~((4 << shift) - 1);
        uint bit = ((x >> shift) & 3) + 4 * ((y >> shift) & 3);
        uint nx = x & mask;
        uint ny = y & mask;
        levels[lvl].level_addr = (nx << 24) + ((ny + lvl) << 16) + (1 << bit);
    }
}

//! \brief For all regions at all levels, find a working chip within each of
//!     the 16 subregions and record its coordinates.
void level_config(void)
{
    compute_level(p2p_addr);

    for (uint level = 0; level < 4; level++) {
        uint base = (levels[level].level_addr >> 16) & 0xfcfc;
        uint shift = 6 - 2 * level;     // {6, 4, 2, 0};
        uint width = (1 << shift);      // {64, 16, 4, 1};

        for (uint ix = 0; ix < 4; ix++) {
            for (uint iy = 0; iy < 4; iy++) {
                uint num = ix + 4 * iy;
                uint addr = ((ix << 8) + iy) << shift;

                // Now search for a working chip within the subregion.

                for (uint i = 0; i < (width * width); i++) {
                    uint wx = i & ((1 << shift) - 1);
                    uint wy = i >> shift;
                    uint a = base + addr + (wx << 8) + wy;

                    // Don't bother trying outside the scope of the system
                    if (a >= p2p_dims || (a & 0xFF) >= (p2p_dims & 0xFF)) {
                        continue;
                    }

                    uint link = rtr_p2p_get(a);

                    if (link != 6) {
                        levels[level].addr[num] = a;
                        levels[level].valid[num] = 1;
                        break;
                    }
                }
            }
        }
    }
}


//------------------------------------------------------------------------------


//! \brief Initialises ::peek_pkt to contain the given arguments
//!     (presumably an acknowledge)
//! \param[in] link: Which link to send the packet on
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
void peek_ack_pkt(uint link, uint data, uint key)
{
    if (peek_pkt.flags != 1) {
        pkt_t t = {link, data, key};
        peek_pkt.pkt = t;
        peek_pkt.flags = 1;
    }
}


//! \brief Initialises ::poke_pkt to contain the given arguments
//!     (presumably an acknowledge)
//! \param[in] link: Which link to send the packet on
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
void poke_ack_pkt(uint link, uint data, uint key)
{
    if (poke_pkt.flags != 1) {
        pkt_t t = {link, data, key};
        poke_pkt.pkt = t;
        poke_pkt.flags = 1;
    }
}


//------------------------------------------------------------------------------

//! \brief Read a word from the chip (or device) over a link.
//! \param[in] addr: The address to read from
//! \param[in] link: The link to use
//! \param[out] buf: The data that has been read (4 byte buffer)
//! \param[in] timeout: How long to wait for the other side to respond
//! \return result code
uint link_read_word(uint addr, uint link, uint *buf, uint timeout)
{
    peek_pkt.flags = 0;

    if (! pkt_tx(PKT_NND + (link << 18), 0, addr)) {
        return RC_PKT_TX;
    }

    event_t* e = event_new(proc_byte_set, (uint) &peek_pkt.flags, 2);
    if (e == NULL) {
        sw_error(SW_OPT);
        return RC_BUF;
    }

    uint id = e->ID;
    timer_schedule(e, timeout);

    while (peek_pkt.flags == 0) {
        continue;
    }

    if (peek_pkt.flags == 2) {
        return RC_TIMEOUT;
    }

    timer_cancel(e, id);

    if (buf != NULL) {
        *buf = peek_pkt.pkt.data;
    }

    return RC_OK;
}

//! \brief Write a word to the chip (or device) over a link.
//! \param[in] addr: The address to write to
//! \param[in] link: The link to use
//! \param[in] buf: The data that is to be written (buffer containing 4 bytes)
//! \param[in] timeout: How long to wait for the other side to respond
//! \return result code
uint link_write_word(uint addr, uint link, uint *buf, uint timeout)
{
    poke_pkt.flags = 0;

    if (! pkt_tx(PKT_NND + PKT_PL + (link << 18), *buf, addr)) {
        return RC_PKT_TX;
    }

    event_t* e = event_new(proc_byte_set, (uint) &poke_pkt.flags, 2);
    if (e == NULL) {
        sw_error(SW_OPT);
        return RC_BUF;
    }

    uint id = e->ID;
    timer_schedule(e, timeout);

    while (poke_pkt.flags == 0) {
        continue;
    }

    if (poke_pkt.flags == 2) {
        return RC_TIMEOUT;
    }

    timer_cancel(e, id);
    return RC_OK;
}


//------------------------------------------------------------------------------

//! \brief Allocate a packet (getting one from the free list, ::pkt_buf_list).
//! \return The allocated packet, or `NULL` if none available
pkt_buf_t* pkt_buf_get(void)
{
    pkt_buf_t *pkt = pkt_buf_list;

    if (pkt != NULL) {
        pkt_buf_list = pkt->next;
        pkt_buf_count++;
        if (pkt_buf_count > pkt_buf_max) {
            pkt_buf_max = pkt_buf_count;
        }
    }

    return pkt;
}

//! \brief Add packet to free list (::pkt_buf_list), "freeing" it
//! \param[in] pkt: Packet to free.
void pkt_buf_free(pkt_buf_t *pkt)
{
    pkt->next = pkt_buf_list;
    pkt_buf_list = pkt;

    pkt_buf_count--;
}

//! \name Flood fill
//! \{

//! \brief Get the next flood-fill identifier we should use
//! \return A new flood fill ID (range: 2&ndash;254)
uint next_id(void)
{
    uint t = sv->last_id + 1;
    if (t > 127) {
        t = 1;
    }
    sv->last_id = t;
    return t << 1;
}
//! \}

//! \name Board information flood fill
//! \{

//! \brief Get the next board info flood-fill identifier we should use
//! \return A new flood fill ID (range: 2&ndash;254)
uint next_biff_id(void)
{
    uint t = sv->last_biff_id + 1;
    if (t > 127) {
        t = 1;
    }
    sv->last_biff_id = t;
    return t << 1;
}
//! \}

//! \brief Mark an ID as discovered
//! \param[in] key: The key from the NN packet
// Is this unused?
void nn_mark(uint key)
{
    uint id = (key >> 1) & 127;

    uint w = id >> 5;                   // Word in id_set
    uint b = 1 << (id & 31);            // Bit in word
    uint v = nn_desc.id_set[w];         // Get word from array

    nn_desc.id_set[w] = v | b;
    w = (w + 2) % 4;
    nn_desc.id_set[w] = 0;
}

//! \name Flood fill
//! \{

//! \brief Flood fill NN packet sender.
//! \param[in] key: The key to use
//! \param[in] data: The data to use
//! \param[in] fwd_rty: Forward/delay/retry configuration
//! \param[in] add_id: Whether to merge the next_id() result into the key
void ff_nn_send(uint key, uint data, uint fwd_rty, uint add_id)
{
    if (add_id) {
        key |= next_id();
    }

    uint count = fwd_rty & 7;           // Number of times to send
    uint delay = fwd_rty & 0xf8;        // Delay between transmissions
    uint forward = fwd_rty >> 8;        // Set of links to forward to

    key |= chksum_64(key, data);

    do {
        for (uint link = 0; link < NUM_LINKS; link++) {
            if (forward & (1 << link) & link_en) {
                sark_delay_us(delay);
                (void) pkt_tx(PKT_NN + PKT_PL + (link << 18), data, key);
            }
        }
    } while (count--);
}

//! \}

//! \name Board information flood fill
//! \{

//! \brief Board information flood fill packet sender.
//! \note Should only be called on chips which are at position (0, 0) on
//!     their board.
//! \param[in] data: The payload of the packet to send.
void biff_nn_send(uint data)
{
    uint key = NN_CMD_BIFF << 24;
    key |= next_biff_id();
    // NB: X = Y = 0

    key |= chksum_64(key, data);

    uint forward = 0x07; // East, North-East, North only
    for (uint link = 0; link < NUM_LINKS; link++) {
        if (forward & (1 << link) & link_en) {
            sark_delay_us(8);  // !! const
            (void) pkt_tx(PKT_NN + PKT_PL + (link << 18), data, key);
        }
    }
}
//! \}

//------------------------------------------------------------------------------

//! \name P2PC
//! \{

//! \brief Transmit our current best guess of coordinates to all neighbouring
//!     chips
//! \param arg1: unused
//! \param arg2: unused
void p2pc_addr_nn_send(uint arg1, uint arg2)
{
    // Don't send anything if our address is actually unknown...
    if (p2p_addr_guess_x == NO_IDEA || p2p_addr_guess_y == NO_IDEA) {
        return;
    }

    uint key = (NN_CMD_P2PC << 24) | (P2PC_ADDR << 2);
    uint data = ((p2p_addr_guess_x & 0xFFFF) << 16) |
            (p2p_addr_guess_y & 0xFFFF);

    key |= chksum_64(key, data);

    for (uint link = 0; link < NUM_LINKS; link++) {
        pkt_tx(PKT_NN + PKT_PL + (link << 18), data, key);
    }
}

//! \brief Broadcast the existence of a new P2P coordinate having been
//!     discovered
//! \param[in] x: X coordinate
//! \param[in] y: Y coordinate
void p2pc_new_nn_send(uint x, uint y)
{
    uint key = (NN_CMD_P2PC << 24) | (P2PC_NEW << 2);
    uint data = ((x & 0xFFFF) << 16) | (y & 0xFFFF);

    key |= chksum_64(key, data);

    for (uint link = 0; link < NUM_LINKS; link++) {
        pkt_tx(PKT_NN + PKT_PL + (link << 18), data, key);
    }
}

//! \brief Transmit our current best guess of coordinates to all neighbouring
//!     chips.
//! \param arg1: unused
//! \param arg2: unused
void p2pc_dims_nn_send(uint arg1, uint arg2)
{
    uint key = (NN_CMD_P2PC << 24) | (P2PC_DIMS << 2);
    uint data = ((p2p_min_x & 0xFF) << 24) |
            ((p2p_min_y & 0xFF) << 16) |
            ((p2p_max_x & 0xFF) << 8) |
            ((p2p_max_y & 0xFF) << 0);

    key |= chksum_64(key, data);

    for (uint link = 0; link < NUM_LINKS; link++) {
        pkt_tx(PKT_NN + PKT_PL + (link << 18), data, key);
    }
}

//! \brief Transmit "P2PB" table generating packets
//! \param arg1: unused
//! \param arg2: unused
void p2pb_nn_send(uint arg1, uint arg2)
{
    static int id = 0;

    uint key = (NN_CMD_P2PB << 24) | (0x3f00 << 8) | ((id++ & 0x7F) << 1);
    uint data = p2p_addr << 16;

    key |= chksum_64(key, data);

    for (uint link = 0; link < NUM_LINKS; link++) {
        if (link_en & (1 << link)) {
            pkt_tx(PKT_NN + PKT_PL + (link << 18), data, key);
        }
    }
}

//! \brief Update our current best guess of our coordinates based on a packet
//!     from a neighbour
//! \param[in] link: What link was the packet received on
//! \param[in] data: The payload from the packet
//! \param[in] key: The key from the packet
static void nn_rcv_p2pc_addr_pct(uint link, uint data, uint key)
{
    // Coordinates of neighbour
    int nx = data >> 16;
    int ny = data & 0xFFFF;
    if (nx & 0x8000) {
        nx |= 0xFFFF0000;
    }
    if (ny & 0x8000) {
        ny |= 0xFFFF0000;
    }

    // Work out our coordinates
    int x = nx + (int)lx[link];
    int y = ny + (int)ly[link];

    // Ignore if this address has wrapped around a few too many times...
    if (x < -255 || x > 255 || y < -255 || y > 255) {
        return;
    }

    // Ignore if in board-test mode and coordinate is not within a SpiNN-5
    // board boundary.
    if ((sv->bt_flags & 1) &&
            (x >= 8 || x < 0 || y >= 8 || y < 0 ||
                    eth_map[y][x] != ((x << 4) + y))) {
        return;
    }

    // Update out current guess
    int updated = 0;
    if ((p2p_addr_guess_x < 0 && x >= 0) || // X has gone non-negative or
            (((x < 0) == (p2p_addr_guess_x < 0)) && // Sign is the same and...
                    (abs(x) < abs(p2p_addr_guess_x)))) { // ...magnitude reduced
        p2p_addr_guess_x = x;
        updated |= 1;
    }
    if ((p2p_addr_guess_y < 0 && y >= 0) || // Y has gone non-negative or
            (((y < 0) == (p2p_addr_guess_y < 0)) && // Sign is the same and...
                    (abs(y) < abs(p2p_addr_guess_y)))) { // ...magnitude reduced
        p2p_addr_guess_y = y;
        updated |= 2;
    }

    // If guess was updated, broadcast this fact
    if (updated) {
        ticks_since_last_p2pc_new = 0;
        p2pc_new_nn_send(p2p_addr_guess_x, p2p_addr_guess_y);
    }
}

//! \brief A new P2P coordinate has been broadcast to us.
//! \param[in] link: What link was the packet received on
//! \param[in] data: The payload from the packet
//! \param[in] key: The key from the packet
static void nn_rcv_p2pc_new_pct(uint link, uint data, uint key)
{
    // Coordinates of the new coordinate
    int x = data >> 16;
    int y = data & 0xFFFF;
    if (x & 0x8000) {
        x |= 0xFFFF0000;
    }
    if (y & 0x8000) {
        y |= 0xFFFF0000;
    }

    // Ignore if this address has wrapped around a few too many times (should
    // not occur but here as a sanity check since we'll be indexing with
    // this...)
    if (x < -255 || x > 255 || y < -255 || y > 255) {
        return;
    }

    // Update the bitmap of known coordinates
    uint bit_offset = (((x + 256) << 9) | ((y + 256) << 0));
    uchar byte = p2p_addr_table[bit_offset / 8];
    uchar new_byte = byte | (1 << (bit_offset % 8));
    p2p_addr_table[bit_offset / 8] = new_byte;

    // Re-broadcast only newly discovered coordinates
    if (byte != new_byte) {
        ticks_since_last_p2pc_new = 0;
        p2pc_new_nn_send(x, y);
    }
}

//! \brief A neighbour has reported their P2P coordinates to us
//! \param[in] link: What link was the packet received on
//! \param[in] data: The payload from the packet
//! \param[in] key: The key from the packet
static void nn_rcv_p2pc_dims_pct(uint link, uint data, uint key)
{
    // Bounds of the coordinates
    int new_min_x = (data >> 24) & 0xFF;
    int new_min_y = (data >> 16) & 0xFF;
    if (new_min_x) {
        new_min_x |= 0xFFFFFF00;
    }
    if (new_min_y) {
        new_min_y |= 0xFFFFFF00;
    }

    int new_max_x = (data >> 8) & 0xFF;
    int new_max_y = (data >> 0) & 0xFF;

    // Expand bounds as required
    int changed = 0;
    if (new_min_x < p2p_min_x) {
        p2p_min_x = new_min_x;
        changed = 1;
    }
    if (new_min_y < p2p_min_y) {
        p2p_min_y = new_min_y;
        changed = 1;
    }
    if (new_max_x > p2p_max_x) {
        p2p_max_x = new_max_x;
        changed = 1;
    }
    if (new_max_y > p2p_max_y) {
        p2p_max_y = new_max_y;
        changed = 1;
    }

    // Re-broadcast immediately if bounds expanded to minimise propagation
    // delay
    if (changed) {
        ticks_since_last_p2pc_dims = 0;
        p2pc_dims_nn_send(0, 0);
    }
}

//! \brief Handle P2PC packet
//!
//! Delegates to:
//! * nn_rcv_p2pc_addr_pct()
//! * nn_rcv_p2pc_dims_pct()
//! * nn_rcv_p2pc_new_pct()
//!
//! \param[in] link: What link was the packet received on
//! \param[in] data: The payload from the packet
//! \param[in] key: The key from the packet
void nn_rcv_p2pc_pct(uint link, uint data, uint key)
{
    int p2pc_cmd = (key >> 2) & 0x3;

    switch (p2pc_cmd) {
    case P2PC_ADDR:
        nn_rcv_p2pc_addr_pct(link, data, key);
        break;
    case P2PC_NEW:
        nn_rcv_p2pc_new_pct(link, data, key);
        break;
    case P2PC_DIMS:
        nn_rcv_p2pc_dims_pct(link, data, key);
        break;
    }
}

//! \}

//------------------------------------------------------------------------------

//! \brief Write the Router Control Register.
//! \param[in] data:
//! payload from the NN packet.
//! Various fields may be selected using 'spare' bits in the word.
//! The field is updated if the bit is set. \n
//! bit 5 --- Wait2 \n
//! bit 4 --- Wait1 \n
//! bit 3 --- W bit \n
//! bit 2 --- MP field \n
//! bit 1 --- TP field

// !! resync TP update timer?

static void nn_cmd_rtrc(uint data)
{
    uint t1 = 0;
    uint t2 = 0;

    if (data & 0x20) {                          // Wait2
        t2 |= data & 0xff000000;
        t1 |= 0xff000000;
    }
    if (data & 0x10) {                          // Wait1
        t2 |= data & 0x00ff0000;
        t1 |= 0x00ff0000;
    }
    if (data & 0x08) {                          // W bit
        t2 |= data & 0x00008000;
        t1 |= 0x00008000;
    }
    if (data & 0x04) {                          // MP
        t2 |= data & 0x00001f00;
        t1 |= 0x00001f00;
    }
    if (data & 0x02) {                          // TP
        t2 |= data & 0x000000c0;
        t1 |= 0x000000c0;
    }

    t1 = rtr[RTR_CONTROL] & ~t1;
    rtr[RTR_CONTROL] = t1 | t2;
}


//------------------------------------------------------------------------------

//! Operation codes for nn_cmd_sig0()
enum sig0_operations {
    SIG0_FWD_RTRY,      //!< Set forward and retry
    SIG0_LEDS,          //!< Set LEDs (call sark_led_set())
    SIG0_GTP,           //!< Global Time Phase
    SIG0_ID,            //!< ID set/reset
    SIG0_LEVEL,         //!< Trigger level_config()
    SIG0_APP,           //!< Signal application (call signal_app())
    SIG0_TP,            //!< Router time phase period _(unimplemented)_
    SIG0_RST,           //!< Shut down APs _(unimplemented)_
    SIG0_DOWN,          //!< Minimum power mode _(unimplemented)_
    SIG0_UP             //!< Operational mode _(unimplemented)_
};

//! \brief SIG0 does simple commands whose data fits into 28 bits.
//! \details Delegates to:
//!
//! * sark_led_set()
//! * level_config()
//! * signal_app()
//! \param[in] data: The message payload
//! \return 0 if packet should not be propagated, 1 otherwise.
uint nn_cmd_sig0(uint data)
{
    uint op = data >> 28;

    switch (op) {
    case SIG0_FWD_RTRY:
        nn_desc.forward = sv->forward = data >> 8;      // 8 bits
        nn_desc.retry = sv->retry = data;               // 8 bits
        break;

    case SIG0_LEDS:
        sark_led_set(data);
        break;

    case SIG0_GTP:
        sv->tp_scale = data & 15;
        sv->tp_timer = 0;
        break;

    case SIG0_ID: // what's this? Set last_id here ?
        if (data == nn_desc.gidr_id) {
            return 0;
        }

        nn_desc.gidr_id = data;
        sark_word_set(nn_desc.id_set, (data & 1) ? 0xff : 0x00, 16);
        break;

    case SIG0_LEVEL:
        event_queue_proc((event_proc) level_config, 0, 0, PRIO_0);
        break;

    case SIG0_APP:
        signal_app(data);
        break;
      /*
    case SIG0_TP:       // Router time phase period
    case SIG0_RST:      // Shut down APs
    case SIG0_DOWN:     // Minimum power mode
    case SIG0_UP:       // Operational mode
      */
    }
    return 1;
}

//! \brief Memory write operations
//! \param[in] data: The data to write
//! \param[in] key: The key from the NN packet. Encodes where to write
//!     relative to (::sv, ::sysram, ::sc, or sv_t::mem_ptr), what the offset
//!     should be, and how much to write (1, 2 or 4 bytes).
static void nn_cmd_mem(uint data, uint key)
{
    uint type = (key >> 18) & 3;
    uint op = (key >> 16) & 3;
    uint base = SV_SV;

    if (op == 1) {
        base = SYSRAM_BASE;
    } else if (op == 2) {
        base = SYSCTL_BASE;
    } else if (op == 3) {
        base = sv->mem_ptr;
    }

    uint ptr = base + ((key >> 8) & 255);

    if (type == 0) {
        * ((uchar *) ptr) = data;
    } else if (type == 1) {
        * ((short *) ptr) = data;
    } else {
        * ((uint *) ptr) = data;
    }
}

//! Operation codes for nn_cmd_sig1()
enum sig1_operations {
    SIG1_WRITE          //!< Memory write
};

//! \brief SIG1 does simple commands with a payload whose control fits in
//!     20 bits
//! \details Delegates to:
//!
//! * nn_cmd_mem()
//! \param[in] data: The payload from the NN packet
//! \param[in] key: The key from the NN packet
void nn_cmd_sig1(uint data, uint key)
{
    uint op = (key >> 20) & 15;

    if (op == SIG1_WRITE) {
        nn_cmd_mem(data, key);
    }
}


//------------------------------------------------------------------------------

#define P2PB_STOP_BIT 0x400        //!< Set to stop propagation

//! \brief P2P Broadcast, used to propagate P2P addresses and maintain the P2P
//! routing table.
//!
//! Table "hop_table" has entries of type (id << 24, hops) for each possible
//! P2P address. Field "hops" contains the number of hops to the address
//! (initialised to 65535) and "id" contains the ID of the last packet
//! used to update the table (initialised to 128).
//!
//! An update occurs when a packet with a lower hop count than the current
//! one arrives via an enabled link. The link on which the packet arrived
//! is used to update the P2P routing table.
//!
//! \param[in] id: The ID from the key of the packet
//! \param[in] data: The payload of the packet
//! \param[in] link: What route the packet came from
//! \return \p data, with P2P_STOP_BIT set if the packet should not be
//!     propagated
static uint nn_cmd_p2pb(uint id, uint data, uint link)
{
    uint addr = data >> 16;
    uint hops = data & 0x3ff;   // Bottom 10 bits

    uint table_hops = hop_table[addr] & 0xffff;

    if ((netinit_phase == NETINIT_PHASE_P2P_TABLE) &&
	    (addr != p2p_addr) &&
	    (hops < table_hops) &&
	    (link_en & (1 << link))) {

        // keep a count of P2P addresses we've heard of
        if (table_hops == 0xffff) {
            sv->p2p_active++;
        }

        hop_table[addr] = (id << 24) + hops;

        rtr_p2p_set(addr, link);

        if (hops >= 0x3FF) {
            data |= P2PB_STOP_BIT;
        }
    } else {
        data |= P2PB_STOP_BIT;
    }

    return data + 1;
}


//------------------------------------------------------------------------------

//! \name Flood fill
//! \{

//! \brief Flood fill start
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
static void nn_cmd_ffs(uint data, uint key)
{
    nn_desc.aplx_addr = sv->sdram_sys;
    nn_desc.region = data;
    nn_desc.id = (key >> 17) & 127;     // 8 bits (LSB zero)
    nn_desc.block_len = key >> 8;       // 8 bits
    nn_desc.block_count = 0;            // 8 bits
    nn_desc.error = nn_desc.load = 0;

    sark_word_set(nn_desc.fbs_set, 0, 32);
    sark_word_set(nn_desc.fbe_set, 0, 32);

    // Reset the flood-fill selected cores and FFCS index
    nn_desc.cores = 0;
    nn_desc.last_ffcs = 0;

    uint mask = nn_desc.region & 0xffff;
    uint region = nn_desc.region >> 16;
    uint level = region & 3;

    if ((mask == 0) ||
            (((levels[level].level_addr >> 16) == region)) &&
            (levels[level].level_addr & mask)) {
        nn_desc.load = 1;
    }

    nn_desc.state = FF_ST_EXBLK;
}

//! \brief Flood fill core select
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
//! \return 1 if the packet should not be propagated further
static uint nn_cmd_ffcs(uint data, uint key)
{
    // Select cores to flood-fill to
    // Determine if we're in the region specified by the packet
    uint mask = data & 0xffff;
    uint region = data >> 16;
    uint level = region & 3;

    // Extract the cores from the key.
    uint cores = key & 0x0003ffff;

    // If we've seen this packet before then return 1 to indicate that it
    // shouldn't be forwarded.
    uint64 id = (((uint64) data) << 18) | cores;
    if (id <= nn_desc.last_ffcs) {
        return 1;
    }
    nn_desc.last_ffcs = id;

    if ((mask == 0) ||
            (((levels[level].level_addr >> 16) == region)) &&
            (levels[level].level_addr & mask)) {
        // We're in the region, so ensure that load is set
        nn_desc.load = 1;

        // And include the cores that we wish to load
        nn_desc.cores |= cores;
    }

    return 0;
}

//! \brief Flood fill block start
//! \param[in] id: The ID from the key of the packet
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
//! \return 1 if the packet should not be propagated further
static uint nn_cmd_fbs(uint id, uint data, uint key)
{
    if (id != nn_desc.id || nn_desc.state != FF_ST_EXBLK) {
        return 0;
    }

    uint block_num = (key >> 16) & 255;
    uint word = block_num >> 5;
    uint bit = 1 << (block_num & 31);
    uint mask = nn_desc.fbs_set[word];

    if ((mask & bit) != 0) {
        return 0;
    }

    nn_desc.fbs_set[word] = mask | bit;

    nn_desc.load_addr = data;
    nn_desc.sum = data;
    nn_desc.word_len = ((key >> 8) & 255) + 1;
    nn_desc.block_num = block_num;
    nn_desc.word_count = 0;

    sark_word_set(nn_desc.fbd_set, 0, 32);

    nn_desc.state = FF_ST_INBLK;

    return 1;
}

//! \brief Flood fill block data
//! \param[in] id: The ID from the key of the packet
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
//! \return 1 if the packet should not be propagated further
static uint nn_cmd_fbd(uint id, uint data, uint key)
{
    uint block_num = (key >> 16) & 0xff; // Block num

    if (id != nn_desc.id || block_num != nn_desc.block_num ||
            nn_desc.state != FF_ST_INBLK) {
        return 0;
    }

    uint offset = (key >> 8) & 0xff;
    uint word = offset >> 5;
    uint bit = 1 << (offset & 31);
    uint mask = nn_desc.fbd_set[word];

    if ((mask & bit) != 0) {
        return 0;
    }

    nn_desc.word_count++;
    nn_desc.fbd_set[word] = mask | bit;
    nn_desc.buf[offset] = data;
    nn_desc.sum += data;

    return 1;
}

//! \brief Flood fill block end
//! \param[in] id: The ID from the key of the packet
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
//! \return 1 if the packet should not be propagated further
static uint nn_cmd_fbe(uint id, uint data, uint key)
{
    uint block_num = (key >> 16) & 0xff;

    if (id != nn_desc.id || block_num != nn_desc.block_num ||
            nn_desc.state != FF_ST_INBLK) {
        return 0;
    }

    uint word = block_num >> 5;
    uint bit = 1 << (block_num & 31);
    uint mask = nn_desc.fbe_set[word];

    if ((mask & bit) != 0) {
        return 0;
    }

    nn_desc.fbe_set[word] = mask | bit;
    nn_desc.sum += data;

    if (nn_desc.sum == 0 && nn_desc.word_count == nn_desc.word_len) {
        nn_desc.block_count++;
        sark_word_cpy((void *) nn_desc.load_addr, nn_desc.buf,
                nn_desc.word_len * 4);
    } else {
        nn_desc.error = 1;
    }

    nn_desc.state = FF_ST_EXBLK;
    return 1;
}

//! \brief Flood fill end
//! \param[in] id: The ID from the key of the packet
//! \param[in] data: The payload of the packet
//! \param[in] key: The key of the packet
//! \return 1 if the packet should not be propagated further
static uint nn_cmd_ffe(uint id, uint data, uint key)
{
    if (id != nn_desc.id || nn_desc.state != FF_ST_EXBLK) {
        return 0;
    }

    nn_desc.state = FF_ST_IDLE;

    if (nn_desc.error == 0 && nn_desc.block_count == nn_desc.block_len
            && nn_desc.load) {
        if (!event_queue_proc(proc_start_app, (uint) nn_desc.aplx_addr,
                data | nn_desc.cores, PRIO_0)) {
            sw_error(SW_OPT);
        }
    }

    return 1;
}

//! \}

//! \brief Nearest neighbour packet broadcast handler
//! \param[in] i_pkt: (Type-punned) pkt_buf_t pointer
//! \param[in] count: Number of times to try; should be at least 1
static void proc_pkt_bc(uint i_pkt, uint count)
{
    pkt_buf_t *pkt = (pkt_buf_t *) i_pkt;

    uint forward = pkt->fwd;
    uint link = pkt->link;
    uint offset = (forward & 0x40) ? 0 : link;

    for (uint lnk = 0; lnk < NUM_LINKS; lnk++) {
        if (forward & (1 << lnk)) {
            uint p = lnk + offset;

            if (p >= NUM_LINKS) {
                p -= NUM_LINKS;
            }

            if ((1 << p) & link_en) {
                if (! pkt_tx(pkt->pkt.ctrl + (p << 18), pkt->pkt.data,
                        pkt->pkt.key)) {
                    sw_error(SW_OPT);
                }
            }
        }
    }

    count--;

    if (count != 0) {
        if (!timer_schedule_proc(proc_pkt_bc, (uint) pkt, count,
                pkt->delay)) {
            sw_error(SW_OPT);
        }
    } else {
        pkt_buf_free(pkt);
    }
}

//! \name Board information flood fill
//! \{

//! \brief Board information flood fill command handler
//!
//! Can be invoked directly during setup, or in response to a BIFF message.
//! \param[in] x: The believed X coordinate
//! \param[in] y: The believed Y coordinate
//! \param[in] data: The payload word from the message
void nn_cmd_biff(uint x, uint y, uint data)
{
    // Board info data is formatted like so:
    // data[31:30] - Data type identifier.
    // For data type 00:
    //   Defines which parts of a chip are known to be defective
    //   data[29:27] - X coordinate of chip on board
    //   data[26:24] - Y coordinate of chip on board
    //   data[23:18] - Which links are working (bitmap)
    //   data[17:0] - Which cores are working (bitmap)
    uint type = data >> 30;

    switch (type) {
    case 0: {
        uint target_x = (data >> 27) & 7;
        uint target_y = (data >> 24) & 7;

        // Ignore if not targeted at this chip
        if (target_x != x || target_y != y) {
            return;
        }

        // disable blacklisted links
        // NB: blacklisted links are given as '1'
        sv->link_en = link_en = ((~data) >> 18) & link_en;

        // remember blacklisted cores
        uint dead_cores = data & 0x3ffff;

        // if blacklisted prepare to delegate
        if (dead_cores & (1 << sark.phys_cpu)) {
            uint * boot_img = (uint *) (DTCM_BASE + 0x00008000);

            // modify aplx table to preserve variable sv,
            boot_img[45] = 0;

            // start boot image DMA to SDRAM for delegate,
            dma[DMA_ADRS] = (uint) SDRAM_BASE;
            dma[DMA_ADRT] = (uint) boot_img;
            dma[DMA_DESC] = 1 << 24 | 4 << 21 | 1 << 19 | BOOT_IMAGE_SIZE;

            // don't kill a blacklisted monitor just yet,
            dead_cores &= ~(1 << sark.phys_cpu);

            // copy SROM data to system RAM for delegate,
            sark_word_cpy (sysram + 128, &srom, sizeof(srom_data_t));

            // and remember to delegate when ready
            mon_del = 1;
        }

        // Kill blacklisted cores
        remap_phys_cores(dead_cores);
        break;
    }

    default: // Ignore...
        break;
    }
}

//! Board info flood fill packet receiver
//! \param[in] link: What link was the packet received on
//! \param[in] data: The payload from the packet
//! \param[in] key: The key from the packet
void nn_rcv_biff_pct(uint link, uint data, uint key)
{
    // When this function has been called, the checksum has already been
    // verified and the command checked as being NN_CMD_BIFF.

    // Work out the coordinate of this chip on its board given the link it
    // arrived on and the coordinates in the packet.
    int x = ((key >> 11) & 7) + lx[link];
    int y = ((key >> 8) & 7) + ly[link];

    // Fliter the packet if it came from another board
    if (x >= 8 || x < 0 || y >= 8 || y < 0 ||
            eth_map[y][x] != ((x << 4) + y)) {
        return;
    }

    uint id = (key >> 1) & 127;

    // Packets must have an ID at present
    if (id == 0) {
        return;
    }

    // Filter out packets we've seen before (note that IDs are only unique
    // within a board so this process must occur *after* filtering packets
    // from other boards.
    uint word = id >> 5;                        // Word in biff_id_set
    uint bit = 1 << (id & 31);                  // Bit in word
    uint mask = nn_desc.biff_id_set[word];      // Get mask word from array
    if (mask & bit) {
        return;
    }

    // Flag that this ID has been seen
    nn_desc.biff_id_set[word] = mask | bit;
    word = (word + 2) % 4;
    nn_desc.biff_id_set[word] = 0;

    // Handle the command (passing in the coordinates of the chip within its
    // board)
    nn_cmd_biff(x, y, data);

    // Continue propagation:

    // Update coordinates in packet
    key &= ~0x3f00;
    key |= x << 11;
    key |= y << 8;

    // Recompute checksum
    key &= 0x0fffffff;
    key |= chksum_64(key, data);

    // Schedule packet for forwarding
    pkt_buf_t *pkt = pkt_buf_get();

    if (pkt == NULL) { // !! ??
        sw_error(SW_OPT);
        return;
    }

    pkt->fwd = 0x3e; // Don't return to sender...
    pkt->delay = 0; // Not sent more than once so no delay needed
    pkt->link = link;

    pkt_t tp = {PKT_NN + PKT_PL, data, key};
    pkt->pkt = tp;

    if (!timer_schedule_proc(proc_pkt_bc, (uint) pkt, 1, 8)) {
        sw_error(SW_OPT);
    }
}

//! \}

//! \brief Nearest-neighbour packet received handler
//! \param[in] link: What link was the packet received on
//! \param[in] data: The payload from the packet
//! \param[in] key: The key from the packet
void nn_rcv_pkt(uint link, uint data, uint key)
{
    uint t = chksum_64(key, data);
    if (t != 0) {
        nn_desc.errors++;
        return;
    }

    uint cmd = (key >> 24) & 15;
    uint id = (key >> 1) & 127;

    // NN_CMD_BIFF and NN_CMD_P2PC are handled separately
    if (cmd == NN_CMD_BIFF) {
        nn_rcv_biff_pct(link, data, key);
        return;
    } else if (cmd == NN_CMD_P2PC) {
        nn_rcv_p2pc_pct(link, data, key);
        return;
    }

    nn_desc.stats[cmd]++;

    if (cmd < 7 && id != 0) { //const - use (non-zero) ID to stop propagation
        uint word = id >> 5;                    // Word in id_set
        uint bit = 1 << (id & 31);              // Bit in word
        uint mask = nn_desc.id_set[word];       // Get mask word from array
        if (mask & bit) {
            return;
        }

        nn_desc.id_set[word] = mask | bit;
        word = (word + 2) % 4;
        nn_desc.id_set[word] = 0;
    }

    switch (cmd) {
    case NN_CMD_SIG0:
        if (nn_cmd_sig0(data)) {
            break;
        }
        return;

    case NN_CMD_RTRC:
        nn_cmd_rtrc(data);
        break;

    case NN_CMD_LTPC:
	// Used to keep clocks approximately in phase
        t = sv->tp_timer + 2;
        if ((t <= data) && ((t ^ data) & BIT_31) == 0) {
            sv->tp_timer = data - 1;
        } else {
            return;
        }
        break;

    case NN_CMD_SIG1:
        nn_cmd_sig1(data, key);
        break;

    case NN_CMD_FFS:
        nn_cmd_ffs(data, key);
        break;

    case NN_CMD_FFCS:
        if (nn_cmd_ffcs(data, key)) {
            return;
        }
        break;

    case NN_CMD_P2PB:
        data = nn_cmd_p2pb(id, data, link);
        if (data & P2PB_STOP_BIT) {
            return;
        }
        break;

    case NN_CMD_FBS:
        if (nn_cmd_fbs(id, data, key)) {
            break;
        }
        return;

    case NN_CMD_FBD:
        if (nn_cmd_fbd(id, data, key)) {
            break;
        }
        return;

    case NN_CMD_FBE:
        if (nn_cmd_fbe(id, data, key)) {
            break;
        }
        return;

    case NN_CMD_FFE:
        if (nn_cmd_ffe(id, data, key)) {
            break;
        }
        return;

    default: // Ignore...
        return;
    }

    // Now forward the incoming packet

    // forward[5:0] Which links should the packet be sent down
    // forward[6] If reset (fwd[6]==0), link numbers above will be treated
    //            as relative to the packet's incoming link, i.e., fwd[0]
    //            means send to the incoming link. For example. if a packet
    //            comes in on the south link, fwd[0] means send South, fwd[1]
    //            means send East, and so on counterclockwise.
    //            If set (fwd[6]==1), link numbers above will be treated as
    //            absolute, i.e., fwd[0] means send East, fwd[1] means send
    //            NorthEast, and so on counterclockwise.
    //            To send to all links except the incoming one use:
    //            forward[6:0] = 0x3e [relative and not to incoming link (0)].
    // retry[2:0] number of additional times to repeat the packet (i.e. sent
    //            this many times + 1)
    // retry[7:3] The number of usec to wait between repeated packets is
    //            wait = (8 * retry[7:3]) + 8
    uint forward, retry;

    if (cmd & 4) { //const
        forward = nn_desc.forward;
        retry = nn_desc.retry;
    } else {
        forward = (key >> 16) & 255;
        retry = (key >> 8) & 255;
    }

    key &= 0x0fffffff;
    key |= chksum_64(key, data);

    pkt_buf_t *pkt = pkt_buf_get();

    if (pkt == NULL) { // !! ??
        sw_error(SW_OPT);
        return;
    }

    pkt->fwd = forward;
    pkt->delay = (retry & 0xf8) + 8;
    pkt->link = link;

    pkt_t tp = {PKT_NN + PKT_PL, data, key};
    pkt->pkt = tp;

    if (!timer_schedule_proc(proc_pkt_bc,
            (uint) pkt, (retry & 7) + 1, 8)) { // const
        sw_error(SW_OPT);
    }
}
