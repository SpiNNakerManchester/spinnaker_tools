//------------------------------------------------------------------------------
//! \file
//! \brief     Application support for SC&MP
//!
//! \copyright &copy; The University of Manchester - 2009-2012
//!
//! \author    Steve Temple, APT Group, School of Computer Science
//!
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2009 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "sark.h"
#include "scamp.h"


//------------------------------------------------------------------------------


//! \brief Enable/disable clocks to a set of cores specified by a bit mask
//! \param[in] phys_mask: Which _physical_ cores to enable or disable the
//!     clock for.
//! \param[in] enable: True to switch on the clock for the cores, false to
//!     switch the clock off for them.
void clock_ap(uint phys_mask, uint enable)
{
    uint state = sc[SC_CPU_DIS];

    if (enable) {
        state &= ~phys_mask;
    } else {
        state |= phys_mask;
    }

    sc[SC_CPU_DIS] = SC_CODE + state;
}


//! \brief Soft reset a set of cores specified by a bit mask.
//!
//! Clocks for those cores are enabled.
//! \param[in] virt_mask: Which (virtual) cores to soft reset
void reset_ap(uint virt_mask)
{
    uint phys_mask = v2p_mask(virt_mask);

    sc[SC_SOFT_RST_L] = SC_CODE + phys_mask;
    clock_ap(phys_mask, 1);
    sark_delay_us(5);
    sc[SC_SOFT_RST_L] = SC_CODE;
}


//! \brief Boot APs.
//!
//! Assumes the "boot_aplx" image has been placed at 0x7f00 and
//! the default "sark" image at 0x7000. Copies "boot_aplx" to start of
//! SysRAM and the default image to SDRAM. Then starts the default
//! app on all cores.
void boot_ap(void)
{
    uint virt_mask = (1 << num_cpus) - 2;

    sark_word_cpy(sysram, (void *) 0x7f00, 256);
    sark_word_cpy(sv->sdram_sys, (void *) 0x7000, 3840);

    proc_start_app((uint) sv->sdram_sys, virt_mask);
}


//! \brief Send a signal to a set of cores
//! \param[in] cmd: The ::shm_cmd_e to use (probably ::SHM_SIGNAL)
//! \param[in] data: The signal to send, from ::signal_e
//! \param[in] virt_mask: Which virtual cores to signal
//! \param[in] phys_mask: Which physical cores to signal
static void signal_sark(uint cmd, uint data, uint virt_mask, uint phys_mask)
{
    for (uint cpu = 1; cpu < num_cpus; cpu++) {
        if (virt_mask & (1 << cpu)) {
            vcpu_t *vcpu = sv_vcpu + cpu;
            vcpu->mbox_ap_cmd = cmd;
            vcpu->mbox_ap_msg = (void *) data;
        }
    }

    sc[SC_SET_IRQ] = SC_CODE + phys_mask;
}


//! \brief Update some status variables for a set of cores
//! \param[in] virt_mask: Which cores to update for
//! \param[in] state: The state to mark them as being in
//! \param[in] app_id: The ID of the application on those cores
void set_cpu_info(uint virt_mask, uint state, uint app_id)
{
    for (uint cpu = 1; cpu < num_cpus; cpu++) {
        if (virt_mask & (1 << cpu)) {
            vcpu_t *vcpu = sv_vcpu + cpu;

            vcpu->cpu_state = state;
            vcpu->time = sv->unix_time;

            vcpu->app_id = app_id;
            core_app[cpu] = app_id;
        }
    }
}

//------------------------------------------------------------------------------


//! \brief Find the position of the least significant bit in a word.
//! \details Used to find the lead application processor.
//! \param[in] mask: The word mask to look in. Assumed to be non-zero.
//! \return The bit index of the least significant set bit
static inline uint find_lead(uint mask)
{
    uint count = 0;

    while ((mask != 0) && ((mask & (1 << count)) == 0)) {
        count++;
    }

    return count;
}

//! \brief Start an application on a set of cores
//! \param[in] aplx_addr: Where the APLX has been loaded in SDRAM
//! \param[in] id_op_mask: AppID, and flags to pass to APLX loader
void proc_start_app(uint aplx_addr, uint id_op_mask)
{
    uint virt_mask = id_op_mask & 0x0003ffff;

    if (virt_mask == 0) {
        return;
    }

    uint app_id = id_op_mask >> 24;
    uint app_flags = (id_op_mask >> 18) & 63;

    app_data_t *app = sv->app_data + app_id;

    if (app->cores == 0) {
        app->clean = 0;
        app->sema = 0;
        app->mask = 0;
        app->lead = find_lead(virt_mask);
    }

    app->cores += sark_count_bits(virt_mask);
    app->mask |= virt_mask;

    sc[SC_SETFLAG] = virt_mask;

    set_cpu_info(virt_mask, CPU_STATE_INIT, app_id);

    sysram[4] = aplx_addr;
    sysram[5] = (app_flags << 8) + app_id;

    reset_ap(virt_mask);
}


//! \brief Clean up shared data belonging to a given AppID
//! \param[in] app_id: The AppID to clean up
void clean_app_id(uint app_id)
{
    app_data_t *app = sv->app_data + app_id;

    if (app->clean) {
        return;
    }

    sark_xfree_id(sv->sdram_heap,  app_id, 1);
    sark_xfree_id(sv->sysram_heap, app_id, 1);
    sark_xfree_id(sv->sys_heap, app_id, 1);

    rtr_free_id(app_id, 1);

    app->cores = 0;
    app->clean = 1;
}


//! \brief Initialise a set of cores so that they run the default application
//! \param[in] virt_mask: Which virtual cores to power up
//! \param[in] phys_mask: Which physical cores to power up
void proc_init_cores(uint virt_mask, uint phys_mask)
{
    clock_ap(phys_mask, 0);

    sark_word_cpy(sv->sdram_sys, (void *) 0x7000, 3840);

    proc_start_app((uint) sv->sdram_sys, virt_mask); // AppID=0, Flags=0
}


//! \brief Stop an application on a set of cores and free up any global
//!     resources associated with the application
//! \param[in] app_id_and_virt_mask: What AppID to stop, and which virtual
//!     cores to stop
//! \param[in] app_mask_and_phys_mask: Mask to select which AppIDs to stop,
//!     and which physical cores to stop
void proc_stop_app(uint app_id_and_virt_mask, uint app_mask_and_phys_mask)
{
    uint app_id = app_id_and_virt_mask >> 24;
    uint virt_mask = app_id_and_virt_mask & 0x00FFFFFF;

    uint app_mask = app_mask_and_phys_mask >> 24;
    uint phys_mask = app_mask_and_phys_mask & 0x00FFFFFF;

    clock_ap(phys_mask, 0);

    uint candidate_app_id = app_id;
    while ((candidate_app_id & app_mask) == app_id) {
        clean_app_id(candidate_app_id);
        candidate_app_id++;
    }

    proc_init_cores(virt_mask, phys_mask);
}


//! \brief Power down a set of cores
//! \param[in] virt_mask: Which virtual cores to power down
//! \param[in] phys_mask: Which physical cores to power down
void proc_power_down(uint virt_mask, uint phys_mask)
{
    clock_ap(phys_mask, 0);
    set_cpu_info(virt_mask, CPU_STATE_PWRDN, 0);
}


//! \brief Signal cores running a range of applications identified by an AppID
//!     and a mask
//! \param[in] data: What signal, cores and app ID
//! \details Delegates to:
//! * proc_init_cores() for ::SIG_INIT
//! * proc_power_down() for ::SIG_PWRDN
//! * proc_stop_app() for ::SIG_STOP
//! * signal_sark() for most other signals
void signal_app(uint data)
{
    uint sig = (data >> 16) & 15;
    uint app_mask = (data >> 8) & 255;
    uint app_id = data & 255;

    uint virt_mask = 0;
    uint phys_mask = 0;
    uint cpsr;

    for (uint i = 1; i < num_cpus; i++) {
        uint b = (core_app[i] & app_mask) == app_id;
        virt_mask |= b << i;
        phys_mask |= b << v2p_map[i];
    }

    switch (sig) {
    case SIG_INIT:
        event_queue_proc(proc_init_cores, virt_mask, phys_mask, PRIO_0);
        break;

    case SIG_PWRDN:
        event_queue_proc(proc_power_down, virt_mask, phys_mask, PRIO_0);
        break;

    case SIG_STOP:
        event_queue_proc(proc_stop_app, (app_id << 24) | virt_mask,
                (app_mask << 24) | phys_mask, PRIO_0);
        break;

    case SIG_SYNC0:
        // wait to align signal arrival with farthest chip
        // do not allow interrupts to extend the delay!
        cpsr = cpu_int_disable();
        sark_delay_us(sv->sync_alignment);
        sc[SC_CLRFLAG] = virt_mask;
        sc[SC_SET_IRQ] = SC_CODE + phys_mask;
        cpu_int_restore(cpsr);
        break;

    case SIG_SYNC1:
        // wait to align signal arrival with farthest chip
        // do not allow interrupts to extend the delay!
        cpsr = cpu_int_disable();
        sark_delay_us(sv->sync_alignment);
        sc[SC_SETFLAG] = virt_mask;
        sc[SC_SET_IRQ] = SC_CODE + phys_mask;
        cpu_int_restore(cpsr);
        break;

    case SIG_TIMER:
    case SIG_USR0:
    case SIG_USR1:
    case SIG_USR2:
    case SIG_USR3:
    case SIG_PAUSE:
    case SIG_CONT:
    case SIG_EXIT:
    case SIG_START:
        signal_sark(SHM_SIGNAL, sig, virt_mask, phys_mask);
        break;
    }
}
