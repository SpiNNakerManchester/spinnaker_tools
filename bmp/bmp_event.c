//------------------------------------------------------------------------------
//
//! \file bmp_event.c
//! \brief          Event handling routines for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
// Email            steven.temple@manchester.ac.uk
//
//------------------------------------------------------------------------------

/*
 * Copyright (c) 2012 The University of Manchester
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

#include "lpc17xx.h"
#include "bmp.h"

//------------------------------------------------------------------------------

static event_t *event_list;     //!< List of free events
static event_t *event_queue;    //!< List of active events

static event_t *proc_head;      //!< List of queued "proc" events
static event_t *proc_tail;      //!< and tail of that list

static uint32_t event_id;       //!< Makes unique ID for active events
static uint32_t event_count;    //!< Number of events currently in use
static uint32_t event_max;      //!< Maximum number ever used

#define NUM_EVENTS 16           //!< Number of events available for use

//! Storage for events
static event_t events[NUM_EVENTS];

//------------------------------------------------------------------------------

//! \brief Set a _byte_ at an address
//! \param[in] addr: The address to write at.
//! \param[in] data: The value to write there.
void event_set_byte(uint32_t addr, uint32_t data)
{
    * (uint8_t *) addr = data;
}


//! \brief Initialise the event processing system.
//! \param[in] priority: Priority to use for the driving timer interrupt
void event_init(uint32_t priority)
{
    event_t *e = event_list = events;

    for (uint32_t i = 0; i < NUM_EVENTS - 1; i++) {
        e->next = e + 1;
        e = e + 1;
    }

    e->next = NULL;

    NVIC_SetPriority(TIMER3_IRQn, priority);
    NVIC_EnableIRQ(TIMER3_IRQn);
}


//! \brief Adds an event to a list of events which can (all) be executed at
//! some later time.
//!
//! The order of execution is the same as that of addition to the list.
//! \param[in] e: The event to add.
void proc_queue_add(event_t *e)
{
    uint32_t cpsr = cpu_int_off();

    if (proc_head == NULL) {
        proc_head = proc_tail = e;
    } else {
        proc_tail->next = e;
        proc_tail = e;
    }

    cpu_int_restore(cpsr);
}


//! \brief Execute a list of events (in the order in which they were added to
//! the list).
//!
//! Events are returned to the free queue after execution.
void proc_queue_run(void)
{
    uint32_t cpsr = cpu_int_off();

    event_t *e = proc_head;     // Get list of events and
    proc_head = NULL;           // update list head

    cpu_int_restore(cpsr);

    while (e != NULL) {
        e->proc(e->arg1, e->arg2);// No need to check for NULL here

        event_t *next = e->next;

        uint32_t cpsr = cpu_int_off();

        e->next = event_list;   // Return to free queue
        event_list = e;
        e->ID = 0;
        event_count--;

        cpu_int_restore(cpsr);

        e = next;
    }
}


//! \brief Interrupt handler for the timer which is handling events.
//!
//! Events added to the queue while this interrupt is processing will not be
//! processed until the _next_ time this interrupt is called.
void TIMER3_IRQHandler(void)
{
    LPC_TIM3->IR = 1;                   // Clear MR0 interrupt

    uint32_t cpsr = cpu_int_off();

    event_t *e = event_queue;           // Pointer to event queue
    event_t *eq = NULL;                 // Accumulates list of events to execute

    while (e != NULL && e->time == 0) { // Process all in queue with time=0
        event_t *next = e->next;        // Remove head of event queue
        event_queue = next;

        e->next = eq;                   // Move to exec queue
        eq = e;

        e = next;                       // Move to next entry
    }

    if (e != NULL) {                    // If entries remain in queue
        LPC_TIM3->MR0 = e->time;        // Put first one into timer
        e->time = 0;                    // Set head of queue time to zero

        LPC_TIM3->TC = 0;               // Set timer back to zero
        LPC_TIM3->MCR = 5;              // Interrupt and stop on match
        LPC_TIM3->TCR = 1;              // Enable counting
    }

    cpu_int_restore(cpsr);

    while (eq != NULL) {                // Run the execute queue
        if (eq->proc != NULL) {         // Execute proc if non-NULL
            eq->proc(eq->arg1, eq->arg2);
        }
        event_t *next = eq->next;

        uint32_t cpsr = cpu_int_off();

        eq->next = event_list;          // Return to free queue
        event_list = eq;
        eq->ID = 0;
        event_count--;

        cpu_int_restore(cpsr);

        eq = next;
    }
}


//! \brief Cancel an event that was previously scheduled.
//!
//! The ID that was allocated when the event was created must be given in
//! case the event has already executed and possibly been recycled.
//!
//! It is potentially quite difficult to cancel an event at the head
//! of the event queue so in this case the "proc" is made NULL and
//! the event left to terminate on the timer interrupt.
//!
//! \param[in] e: The event to cancel.
//! \param[in] ID: The ID that the event is expected to have.
void event_cancel(event_t *e, uint32_t ID)
{
    uint32_t cpsr = cpu_int_off();

    if (e->ID == ID) {                  // Still active (return if not)
        event_t *q = event_queue;

        if (q == e) {                   // At head of queue - nullify proc
            e->proc = NULL;
        } else {                        // Further down queue - remove
            event_t *pq = q;
            q = q->next;

            while (q != NULL) {
                if (q == e) {
                    pq->next = e->next;

                    if (e->next != NULL) { // Add our time to next in queue
                        e->next->time += e->time;
                    }
                    e->next = event_list;  // Return to free queue
                    event_list = e;
                    e->ID = 0;
                    event_count--;

                    break;
                }

                pq = q;
                q = q->next;
            }
        }
    }

    cpu_int_restore(cpsr);
}


//! \brief Allocates a new event.
//!
//! Allocates a new event from the free queue and intialise "proc",
//! "arg1" and "arg2" fields. The "ID", "next" and "time" fields are also
//! set.
//! \param[in] proc: The event handler
//! \param[in] arg1: First argument to pass to \p proc when event fires
//! \param[in] arg2: Second argument to pass to \p proc when event fires
//! \return The allocated event, or `NULL` if no event can be allocated
event_t* event_new(event_proc proc, uint32_t arg1, uint32_t arg2)
{
    uint32_t cpsr = cpu_int_off();

    event_t *e = event_list;

    if (e != NULL) {
        uint32_t next_id = event_id + 1;

        if (next_id == 0) {
            next_id = 1;
        }
        event_list = e->next;
        e->next = NULL;

        e->ID = event_id = next_id;

        e->proc = proc;
        e->arg1 = arg1;
        e->arg2 = arg2;
        e->time = 0;

        event_count++;
        if (event_count > event_max) {
            event_max = event_count;
        }
    }

    cpu_int_restore(cpsr);

    return e;
}

//! \brief Schedules an event to occur some time in the future.
//! \param[in] e: The event to schedule
//! \param[in] time: How many &mu;s in the future to schedule the event
//!     firing at
void event_schedule(event_t *e, uint32_t time)
{
    time *= CCLK / 1000000;     // Convert us to timer ticks

    uint32_t cpsr = cpu_int_off();

    if (event_queue == NULL) {  // Queue empty - just insert
        event_queue = e;

        LPC_TIM3->MR0 = time;   // Set match register
        LPC_TIM3->TC = 0;       // Set timer back to zero
        LPC_TIM3->MCR = 5;      // Interrupt and stop on match
        LPC_TIM3->TCR = 1;      // Enable counting

        cpu_int_restore(cpsr);
        return;
    }

    LPC_TIM3->TCR = 0;          //## Stop counting

    uint32_t t2c = LPC_TIM3->MR0 - LPC_TIM3->TC;

    if (time < t2c) {           // Earlier than current head - replace
        LPC_TIM3->MR0 = LPC_TIM3->TC + time;    // Set counter to new time
        LPC_TIM3->TCR = 1;      //## Start counting

        event_queue->time = t2c - time; // Adjust old head time

        e->next = event_queue;          // and insert new event at head
        event_queue = e;

        cpu_int_restore(cpsr);
        return;
    }

    LPC_TIM3->TCR = 1;          //## Start counting

    // (time >= t2c) - Insert in correct place in queue (tricky!)

    uint32_t total = 0;
    uint32_t ptotal = 0;

    event_t *pq = event_queue;
    event_t *q = pq->next;

    time -= t2c;

    while (q != NULL) {
        total += q->time;

        if (total > time) {
            q->time = total - time;
            break;
        }

        pq = q;
        ptotal += q->time;
        q = q->next;
    }

    e->time = time - ptotal;

    e->next = pq->next;
    pq->next = e;

    cpu_int_restore(cpsr);
}


//------------------------------------------------------------------------------
