//------------------------------------------------------------------------------
//
//! \file bmp_init.c
//! \brief          System initialisation code for BMP LPC1768
//!
//! \copyright      &copy; The University of Manchester - 2012-2015
//!
//! \author         Steve Temple, APT Group, School of Computer Science
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


#include "lpc17xx.h"
#include "bmp.h"


//------------------------------------------------------------------------------

//! \name Linker generated symbols
//! \{
//! \brief These are all very special, imported directly from the linker.

#ifndef DOXYGEN
#define RO_BASE         Image$$PROGRAM$$RO$$Base
#define RO_LIMIT        Image$$PROGRAM$$RO$$Limit
#define RO_LENGTH       Image$$PROGRAM$$RO$$Length

#define RW_BASE         Image$$SRAM$$RW$$Base
#define RW_LIMIT        Image$$SRAM$$RW$$Limit
#define RW_LENGTH       Image$$SRAM$$RW$$Length

#define ZI_BASE         Image$$SRAM$$ZI$$Base
#define ZI_LIMIT        Image$$SRAM$$ZI$$Limit

#define STACK_LIMIT     Image$$STACK$$ZI$$Limit
#define STACK_BASE      Image$$STACK$$ZI$$Base
#endif


extern uint32_t RO_BASE;        //!< Base address of RO section
extern uint32_t RO_LIMIT;       //!< Limit address of RO section
extern uint32_t RO_LENGTH;      //!< Length of RO section
extern uint32_t RW_BASE;        //!< Base address of RW section
extern uint32_t RW_LIMIT;       //!< Limit address of RW section
extern uint32_t RW_LENGTH;      //!< Length of RW section
extern uint32_t ZI_BASE;        //!< Base address of ZI section
extern uint32_t ZI_LIMIT;       //!< Limit address of ZI section
extern uint32_t STACK_LIMIT;    //!< Limit address of STACK section
extern uint32_t STACK_BASE;     //!< Base address of STACK section

//! \}

//! Fudge to force 4 byte alignment
const int pad SECTION(".padding") = 0xdeaddead;

//------------------------------------------------------------------------------


// Cortex M3 vector tables for BMP LPC1768

extern void c_main(cortex_vec_t *, bool, uint32_t, uint32_t);

//! \name Cortex M3 core interrupt handlers
//! \{

//! Critical error handler
void NORETURN error_han(void)
{
    // remember active interrupts (sticky),
    uni_vec[5] |= NVIC->IABR[0];

    // remember exception type
    uni_vec[6] = __get_IPSR();

    // bump error count
    uni_vec[7]++;

    // and trigger a SYSRESET
    uint32_t aircr = SCB->AIRCR & ~SCB_AIRCR_VECTKEY_Msk;
    aircr |= (0x5FA << SCB_AIRCR_VECTKEY_Pos);  // insert security key
    aircr |= SCB_AIRCR_SYSRESETREQ_Msk;         // set SYSRESET bit
    SCB->AIRCR = aircr;
    while (1) {
        refresh_wdt();
    }
}

//! \brief Hardware fault handler: copy relevant data to SRAM buffer and reset
//! \param[in] stack_frame: The stack frame address
//! \param[in] lr_value: The link register value
void NORETURN HardFault_Handler_C(
	uint32_t * stack_frame, uint32_t lr_value)
{
    // store relevant register contents in SRAM,
    dbg_vec[0]  = stack_frame[0];  // stacked r0
    dbg_vec[1]  = stack_frame[1];  // stacked r1
    dbg_vec[2]  = stack_frame[2];  // stacked r2
    dbg_vec[3]  = stack_frame[3];  // stacked r3
    dbg_vec[4]  = stack_frame[4];  // stacked r12
    dbg_vec[5]  = stack_frame[5];  // stacked lr
    dbg_vec[6]  = stack_frame[6];  // stacked pc
    dbg_vec[7]  = stack_frame[7];  // stacked psr
    dbg_vec[8]  = SCB->CFSR;       // Configurable Fault Status Register
    dbg_vec[9]  = SCB->HFSR;       // Hard Fault Status Register
    dbg_vec[10] = SCB->DFSR;       // Debug Fault Status Register
    dbg_vec[11] = SCB->AFSR;       // Auxiliary Fault Status Register
    dbg_vec[12] = SCB->BFAR;       // bus fault address
    dbg_vec[13] = SCB->MMFAR;      // memory mgmt fault address
    dbg_vec[14] = lr_value;        // exc_return

    // and transfer to error handler
    error_han();
}

//! \brief Hardware fault handler; assembly portion
//! \details Extracts the location of the stack frame and lr/exc_return and
//! passes them to HardFault_Handler_C()
__asm void HardFault_Handler(void)
{
    tst     lr, #4
    ite     eq
    mrseq   r0, msp
    mrsne   r0, psp
    mov     r1, lr
    b       __cpp(HardFault_Handler_C)
}

//! Non-maskable interrupt handler
void NMI_Handler            (void) ALIAS(error_han);
//! Memory management interrupt handler
void MemManage_Handler      (void) ALIAS(error_han);
//! Bus fault interrupt handler
void BusFault_Handler       (void) ALIAS(error_han);
//! Usage fault interrupt handler
void UsageFault_Handler     (void) ALIAS(error_han);
//! Service mode interrupt handler
void SVC_Handler            (void) ALIAS(error_han);
//! Debug monitor interrupt handler
void DebugMon_Handler       (void) ALIAS(error_han);
//! Pending SV interrupt handler
void PendSV_Handler         (void) ALIAS(error_han);
//! System tick interrupt handler
void SysTick_Handler        (void) ALIAS(error_han);

// LPC17xx specific interrupt handlers

//! Watchdog timer interrupt handler
void WDT_IRQHandler         (void) ALIAS(error_han);
//! Timer 0 interrupt handler
void TIMER0_IRQHandler      (void) ALIAS(error_han);
//! Timer 1 interrupt handler
void TIMER1_IRQHandler      (void) ALIAS(error_han);
//! Timer 2 interrupt handler
void TIMER2_IRQHandler      (void) ALIAS(error_han);
//! Timer 3 interrupt handler
void TIMER3_IRQHandler      (void) ALIAS(error_han);
//! UART 0 interrupt handler
void UART0_IRQHandler       (void) ALIAS(error_han);
//! UART 1 interrupt handler
void UART1_IRQHandler       (void) ALIAS(error_han);
//! UART 2 interrupt handler
void UART2_IRQHandler       (void) ALIAS(error_han);
//! UART 3 interrupt handler
void UART3_IRQHandler       (void) ALIAS(error_han);
//! PWM 1 interrupt handler
void PWM1_IRQHandler        (void) ALIAS(error_han);
//! I<sup>2</sup>C 1 interrupt handler
void I2C0_IRQHandler        (void) ALIAS(error_han);
//! I<sup>2</sup>C 1 interrupt handler
void I2C1_IRQHandler        (void) ALIAS(error_han);
//! I<sup>2</sup>C 2 interrupt handler
void I2C2_IRQHandler        (void) ALIAS(error_han);
//! S{I interrupt handler
void SPI_IRQHandler         (void) ALIAS(error_han);
//! SSP 0 interrupt handler
void SSP0_IRQHandler        (void) ALIAS(error_han);
//! SSP 1 interrupt handler
void SSP1_IRQHandler        (void) ALIAS(error_han);
//! PLL 0 interrupt handler
void PLL0_IRQHandler        (void) ALIAS(error_han);
//! Realtime clock interrupt handler
void RTC_IRQHandler         (void) ALIAS(error_han);
//! EINT 0 interrupt handler
void EINT0_IRQHandler       (void) ALIAS(error_han);
//! EINT 1 interrupt handler
void EINT1_IRQHandler       (void) ALIAS(error_han);
//! EINT 2 interrupt handler
void EINT2_IRQHandler       (void) ALIAS(error_han);
//! EINT 3 interrupt handler
void EINT3_IRQHandler       (void) ALIAS(error_han);
//! Analog-to-digital converter ready interrupt handler
void ADC_IRQHandler         (void) ALIAS(error_han);
//! BOD interrupt handler
void BOD_IRQHandler         (void) ALIAS(error_han);
//! USB request interrupt handler
void USB_IRQHandler         (void) ALIAS(error_han);
//! CAN request interrupt handler
void CAN_IRQHandler         (void) ALIAS(error_han);
//! DMA controller interrupt handler
void DMA_IRQHandler         (void) ALIAS(error_han);
//! I2S interrupt handler
void I2S_IRQHandler         (void) ALIAS(error_han);
//! Ethernet interrupt handler
void ENET_IRQHandler        (void) ALIAS(error_han);
//! RIT interrupt handler
void RIT_IRQHandler         (void) ALIAS(error_han);
//! MCPWM interrupt handler
void MCPWM_IRQHandler       (void) ALIAS(error_han);
//! QEI interrupt handler
void QEI_IRQHandler         (void) ALIAS(error_han);
//! PLL 1 interrupt handler
void PLL1_IRQHandler        (void) ALIAS(error_han);
//! USB activity interrupt handler
void USBActivity_IRQHandler (void) ALIAS(error_han);
//! CAN activity interrupt handler
void CANActivity_IRQHandler (void) ALIAS(error_han);

//! \}

//! Main Cortex initialisation vector
const cortex_vec_t main_vec SECTION(".vectors") = {
    (uint32_t *) &STACK_LIMIT,     // 0:  Stack top
    (main_proc) c_main,            // 1:  Entry point

    NMI_Handler,                   // 2:  NMI Handler
    HardFault_Handler,             // 3:  Hard Fault Handler
    MemManage_Handler,             // 4:  Memory Manager Handler
    BusFault_Handler,              // 5:  Bus Fault Handler
    UsageFault_Handler,            // 6:  Usage Fault Handler
    (handler) 0,                   // 7:  (Reserved)
    (handler) 0,                   // 8:  (Reserved)
    (handler) 0,                   // 9:  (Reserved)
    (handler) 0,                   // 10: (Reserved)
    SVC_Handler,                   // 11: SVC Handler
    DebugMon_Handler,              // 12: Debug Monitor Handler
    (handler) 0,                   // 13: (Reserved)
    PendSV_Handler,                // 14: Pending SV Handler
    SysTick_Handler,               // 15: SysTick Handler

    WDT_IRQHandler,                // 16: Watchdog Timer
    TIMER0_IRQHandler,             // 17: Timer0
    TIMER1_IRQHandler,             // 18: Timer1
    TIMER2_IRQHandler,             // 19: Timer2
    TIMER3_IRQHandler,             // 20: Timer3
    UART0_IRQHandler,              // 21: UART0
    UART1_IRQHandler,              // 22: UART1
    UART2_IRQHandler,              // 23: UART2
    UART3_IRQHandler,              // 24: UART3
    PWM1_IRQHandler,               // 25: PWM1
    I2C0_IRQHandler,               // 26: I2C0
    I2C1_IRQHandler,               // 27: I2C1
    I2C2_IRQHandler,               // 28: I2C2
    SPI_IRQHandler,                // 29: SPI
    SSP0_IRQHandler,               // 30: SSP0
    SSP1_IRQHandler,               // 31: SSP1

    PLL0_IRQHandler,               // 32: PLL0 Lock (Main PLL)
    RTC_IRQHandler,                // 33: Real Time Clock
    EINT0_IRQHandler,              // 34: External Interrupt 0
    EINT1_IRQHandler,              // 35: External Interrupt 1
    EINT2_IRQHandler,              // 36: External Interrupt 2
    EINT3_IRQHandler,              // 37: External Interrupt 3
    ADC_IRQHandler,                // 38: A/D Converter
    BOD_IRQHandler,                // 39: Brown-Out Detect
    USB_IRQHandler,                // 40: USB
    CAN_IRQHandler,                // 41: CAN
    DMA_IRQHandler,                // 42: General Purpose DMA
    I2S_IRQHandler,                // 43: I2S
    ENET_IRQHandler,               // 44: Ethernet
    RIT_IRQHandler,                // 45: Repetitive Interrupt Timer
    MCPWM_IRQHandler,              // 46: Motor Control PWM
    QEI_IRQHandler,                // 47: Quadrature Encoder Interface
    PLL1_IRQHandler,               // 48: PLL1 Lock (USB PLL)

    USBActivity_IRQHandler,        // 49: USB Activity
    CANActivity_IRQHandler,        // 50: CAN Activity

    (handler) 0,                   // 51:
    (handler) 0,                   // 52:
    BUILD_DATE,                    // 53: Build date
    BMP_VER_NUM,                   // 54: Version number

    (uint32_t) &RO_LENGTH,         // 55: RO_length - NB: causes compiler warn
    (uint32_t) &RW_LENGTH,         // 56: RW_length - NB: causes compiler warn
    &RO_LIMIT,                     // 57: RO_limit
    &RW_BASE,                      // 58: RW_base
    &RW_LIMIT,                     // 59: RW_limit
    &ZI_BASE,                      // 60: ZI_base
    &ZI_LIMIT,                     // 61: ZI_limit
    &STACK_BASE,                   // 62: stack_base
    &STACK_LIMIT                   // 63: stack_limit
};


//------------------------------------------------------------------------------
