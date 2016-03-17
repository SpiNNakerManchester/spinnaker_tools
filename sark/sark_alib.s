
;;------------------------------------------------------------------------------
;;
;; sark_alib.s	    Initialisation & assembly code for SARK
;;
;; Copyright (C)    The University of Manchester - 2010-2013
;;
;; Author           Steve Temple, APT Group, School of Computer Science
;; Email            temples@cs.man.ac.uk
;;
;;------------------------------------------------------------------------------

		get	spinnaker.s
		get	sark.s

		preserve8

;------------------------------------------------------------------------------

; Need to import the following to force inclusion in the ELF symbol table
; where they are needed to build the APLX header

	if	:lnot: :def: GNU

		import	||Image$$ITCM$$RO$$Base||	; Base/length of ITCM
		import	||Image$$ITCM$$RO$$Length||	; RO data

		import	||Image$$DTCM$$RW$$Base||	; Base/length of DTCM
		import	||Image$$DTCM$$RW$$Length||	; RW data

		import	||Image$$DTCM$$ZI$$Base||	; Base/length of DTCM
		import	||Image$$DTCM$$ZI$$Length||	; ZI data
	
		import	||Image$$DTCM$$ZI$$Limit||	; ZI limit (heap base)

		import	||Image$$STACK$$ZI$$Limit||	; Stack top
		import	||Image$$STACK$$ZI$$Length||	; Stack size
	endif

;------------------------------------------------------------------------------

		area	_alib_reset, code
		code32

		import	sark_init
		import	sark_pre_main
		import	sark_post_main
		import	rte_handler
		import	c_main

		export	cpu_reset

cpu_reset	ldr	pc, reset_vec		; Reset
undef		ldr	pc, undef_vec		; Undefined instr
		ldr	pc, svc_vec		; SVC
		ldr	pc, pabt_vec		; Prefetch abort
		ldr	pc, dabt_vec		; Data abort
		bl	undef			; (Former address exception)
		ldr	pc, [pc, #-0xff0]	; IRQ (via VIC)
		ldr	pc, fiq_vec  		; FIQ

;------------------------------------------------------------------------------

; The following table is available to C as "sark_vec", a pointer to a
; struct of type "sark_vec_t". It must be at 0x20.

; 8 words at 0x20...

reset_vec	dcd	reset_entry		; 20
undef_vec	dcd	undef_error		; 24
svc_vec  	dcd	svc_error		; 28
pabt_vec 	dcd	pabt_error		; 2c
dabt_vec 	dcd	dabt_error		; 30
aplx_proc	dcd	sark_aplx		; 34 Must be @ offset 0x34 !
irq_vec  	dcd	irq_error		; 38 Possibly spare ??
fiq_vec  	dcd	fiq_error		; 3c

; 0x40 ...

svc_stack	dcw	SVC_STACK / 4		; 40 Default stack sizes (words)
irq_stack	dcw	IRQ_STACK / 4		; 42
fiq_stack	dcw	FIQ_STACK / 4		; 44

	if	:def:	GNU
stack_size	dcw	STACK_SIZE		; 46 Total stack size in bytes
code_top	dcd	RO_LENGTH		; 48 Top of code
heap_base	dcd	HEAP_BASE		; 4c Base of heap
stack_top	dcd	STACK_TOP		; 50 Top of stack
	else
stack_size	dcw	||Image$$STACK$$ZI$$Length||
code_top	dcd	||Image$$ITCM$$RO$$Length||
heap_base	dcd	||Image$$DTCM$$ZI$$Limit||
stack_top	dcd	||Image$$STACK$$ZI$$Limit||
	endif

stack_fill	dcd	DEAD_WORD		; 54 Stack fill word
	
num_msgs	dcb	NUM_SDP_MSGS		; 58 Number of SDP message bufs
sark_slot	dcb	SARK_SLOT		; 59 VIC slot for int handler
num_events	dcb	INIT_EVENTS		; 5a Number of initial events
api		dcb	0			; 5b Running API

app_id		dcb	0			; 5c App ID
app_flags	dcb	0			; 5d App flags
__PAD		dcw	0			; 5e Pad

	if	:def:	SARK_EVENT
event_vec	space	EVENT_COUNT * 4		; Space for Event vectors.
	endif

;------------------------------------------------------------------------------

reset_entry	adr	r1, reset_vec		; Point to sark_vec
		strh	r0, [r1, #app_id-reset_vec] ; AppID passed in at reset
		adr	r0, reset_error		; then update reset vector to
		str	r0, [r1, #0]		; trap branch through zero

; Set CP15 control register

		ldr	r0, =0x1f7e 	    	; Thumb on, Low vecs, ITCM on
        	mcr     p15, 0, r0, c1, c0, 0	; LE, WB on, DTCM on, Align on

; Go to SVC mode, set stack pointer to top of stack

		msr	cpsr_c, #IMASK_ALL+MODE_SVC ; SVC mode
		mov	sp, #ITCM_TOP		; Set temporary stack
		bl	sark_config		; Call config routine
		ldr	sp, stack_top		; Set proper stack top

; Fill all stacks with fill word

		ldrh	r2, stack_size		; Fill all stacks
		ldr	r1, stack_fill		; with fill word
		sub	r0, sp, r2
		bl	sark_word_set		; Must use no stack!

; Call initialisation, main and cleanup routines (if present)

		mov	r0, sp			; Copy stack top to r0
		bl	sark_init		; SARK initialisation
		msr	cpsr_c, r0		; Set CPU mode from result

		bl	sark_wait     		; Wait for go signal

		bl	sark_pre_main		; Runtime system init
	
		bl 	c_main			; C main procedure
	
		bl	sark_post_main		; RTS clean-up

		bl	cpu_sleep		; Finally sleep...

;------------------------------------------------------------------------------

; Run time error handlers

		import	sark

		export	rt_error
		export	vic_error

reset_error	mov	r0, #A_RTE_RESET	; Probably branch through zero
		b	rt_error		; No mode change

undef_error	mov	r11, #A_RTE_UNDEF	; Undefined instruction
		b	exc_error		; Mode changes

svc_error	mov	r11, #A_RTE_SVC		; SVC instruction
		b	exc_error		; Mode changes

pabt_error	mov	r11, #A_RTE_PABT	; Prefetch abort
		b	exc_error		; Mode changes

dabt_error	mov	r11, #A_RTE_DABT	; Data abort
		b	exc_error		; Mode changes

irq_error	mov	r11, #A_RTE_IRQ		; Unhandled IRQ (unlikely)
		b	exc_error		; Mode changes

fiq_error	mov	r11, #A_RTE_FIQ		; Unhandled FIQ (unlikely)
		b	exc_error		; Mode changes

vic_error	mov	r11, #A_RTE_VIC		; Unconfigured VIC vector

		mov	r0, #VIC_BASE 		; Pick up some VIC state
		ldr	r0, [r0, #VIC_VADDR] 	; Drop through...

; Come here if error has changed mode.

exc_error	mov	r10, lr			; LR to r10
		mrs	r8, spsr		; SPSR to r8
		orr	r9, r8, #IMASK_ALL	; Modified (ints off) to r9
		bic	r9, #THUMB_BIT		; and force ARM mode
		msr	cpsr_cxsf, r9		; then go back to caller mode
		b	cc_error		; and join common code

; Come here if error did not change mode.

rt_error	proc

		mov  	r11, r0			; Error code to r11
		mrs	r8, cpsr		; CPSR to r8
		mov	r10, lr	    	       	; LR to r10

		endp

; Now dump state - r0-r7, psr, sp, lr

; Note that offsets/sizes here must track definition of "vcpu_block_t"
; in "sark.h"

cc_error	ldr	r12, =sark    		; Get virt_cpu
		ldr	r12, [r12, #SARK_VIRT_CPU]
		mov	r9, #VCPU_SIZE		; Size of struct
		ldr	lr, =SV_VCPU		; Point to base of dumps
		mla	lr, r12, r9, lr		; Compute struct address
		mov	r9, sp	      		; Get sp
		stm	lr, {r0-r10} 		; and dump registers
		strb	r11, [lr, #VCPU_RT_CODE] ; then error code

		mov	r8, #A_STATE_RTE	; Update state
		strb	r8, [lr, #VCPU_CPU_STATE]

		mov	r0, r11			; Error code back to r0
		msr	cpsr_c, #IMASK_ALL+MODE_SVC ; SVC mode, ints off
		bl	rte_handler		; Call handler then shut down

;------------------------------------------------------------------------------

; void cpu_shutdown (void);

		export	cpu_shutdown
cpu_shutdown	proc

		msr	cpsr_c, #IMASK_ALL+MODE_SVC ; SVC mode, ints off
		mov	r0, #VIC_BASE
		mov	r1, #0xffffffff
		str	r1, [r0, #VIC_DISABLE]

		endp

; void cpu_sleep (void);

		export	cpu_sleep
cpu_sleep	proc

		mcr 	p15, 0, r0, c7, c0, 4	; Enter WFI mode
		b	cpu_sleep	    	; Loop forever

		endp

;------------------------------------------------------------------------------

		area	alib_get_cp15, code, readonly
		code32

; uint cpu_get_cp15_cr (void);

       	        export	cpu_get_cp15_cr
cpu_get_cp15_cr	proc

		mrc 	p15, 0, r0, c1, c0, 0
		bx	lr

		endp

		area	alib_set_cp15, code, readonly
		code32

; void cpu_set_cp15_cr (uint value);

       	        export	cpu_set_cp15_cr
cpu_set_cp15_cr	proc

        	mcr     p15, 0, r0, c1, c0, 0
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_init_mode, code, readonly
		code32

; uchar *cpu_init_mode (uint *stack, uint mode, uint size);

		export	cpu_init_mode
cpu_init_mode	proc

		mrs	r3, cpsr		; Caller mode to r3
		msr	cpsr_c, r1		; Set CPSR from r1
		mov	sp, r0			; and SP from r0
		msr	cpsr_csxf, r3		; Restore mode and
		sub	r0, r2, lsl #2		; return new stack base
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_irq_on, code, readonly
		code32

; uint cpu_irq_enable (void);

		export	cpu_irq_enable
		export	spin1_irq_enable

spin1_irq_enable proc
		endp

cpu_irq_enable	proc

		mrs	r0, cpsr
		bic	r1, r0, #IMASK_IRQ
		msr	cpsr_c, r1
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_irq_disable, code, readonly
		code32

; uint cpu_irq_disable (void);

		export	cpu_irq_disable
		export	spin1_irq_disable

spin1_irq_disable proc
		endp

cpu_irq_disable	proc

		mrs	r0, cpsr
		orr	r1, r0, #IMASK_IRQ
		msr	cpsr_c, r1
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_fiq_enable, code, readonly
		code32

; uint cpu_fiq_enable (void);

		export	cpu_fiq_enable
		export	spin1_fiq_enable

spin1_fiq_enable proc
		endp

cpu_fiq_enable	proc

		mrs	r0, cpsr
		bic	r1, r0, #IMASK_FIQ
		msr	cpsr_c, r1
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_fiq_disable, code, readonly
		code32

; uint cpu_fiq_disable (void);

		export	cpu_fiq_disable
		export	spin1_fiq_disable

spin1_fiq_disable proc
		endp

cpu_fiq_disable	proc

		mrs	r0, cpsr
		orr	r1, r0, #IMASK_FIQ
		msr	cpsr_c, r1
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_int_enable, code, readonly
		code32

; uint cpu_int_enable (void);

		export	cpu_int_enable
		export	spin1_int_enable

spin1_int_enable proc
		endp

cpu_int_enable	proc

		mrs	r0, cpsr
		bic	r1, r0, #IMASK_ALL
		msr	cpsr_c, r1
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_int_disable, code, readonly
		code32

; uint cpu_int_disable (void);

		export	cpu_int_disable
		export	spin1_int_disable

spin1_int_disable proc
		endp

cpu_int_disable	proc

		mrs	r0, cpsr
		orr	r1, r0, #IMASK_ALL
		msr	cpsr_c, r1
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_int_restore, code, readonly
		code32

; void cpu_int_restore (uint cpsr);

		export	cpu_int_restore
		export	spin1_mode_restore

spin1_mode_restore proc
		endp

cpu_int_restore	proc

		msr	cpsr_cxsf, r0
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_get_cpsr, code, readonly
		code32

; uint cpu_get_cspr (void);

		export	cpu_get_cpsr
cpu_get_cpsr	proc

		mrs	r0, cpsr
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_set_cpsr, code, readonly
		code32

; void cpu_set_cpsr (uint cpsr);

		export	cpu_set_cpsr
cpu_set_cpsr	proc

		mov	r1, lr
		msr	cpsr_cxsf, r0
		bx	r1

		endp

;------------------------------------------------------------------------------

		area	alib_wfi, code, readonly
		code32

; void cpu_wfi (void);

       	        export	cpu_wfi
       	        export	spin1_wfi

spin1_wfi	proc
		endp

cpu_wfi		proc

		mcr 	p15, 0, r0, c7, c0, 4
		bx	lr

		endp

;------------------------------------------------------------------------------

 		area 	alib_lock, readonly, code
		code32

; uint sark_lock_get (uint lock);

       	        export	sark_lock_get
sark_lock_get	proc

		mov      r12, #SYSCTL_BASE
		add      r12, r12, r0, lsl #2

		mrs      r0, apsr
		orr      r1, r0, #IMASK_ALL
		msr      cpsr_c, r1

lg1		ldr      r1, [r12, #0x100]
		tst      r1, #0x80000000
		bne      lg1

		bx       lr

		endp

; void sark_lock_free (uint cpsr, uint lock);

       	        export	sark_lock_free
sark_lock_free	proc

		mov     r12, #SYSCTL_BASE
		add     r1, r12, r1, lsl #2
		ldr     r1, [r1, #0x180]
		msr     cpsr_c, r0
		bx      lr

		endp

;------------------------------------------------------------------------------

 		area 	alib_sema_raise, readonly, code
		code16

; uint sema_raise (uchar *sema);

       	        export	sark_sema_raise
sark_sema_raise	proc

		push	{r4, lr}		; Save return address
		mov	r3, r0			; Arg to r3
		mov	r0, #A_LOCK_SEMA	; Get this lock
		bl	sark_lock_get		; Returns CPSR in r0

		ldrb	r2, [r3, #0]		; Increment the variable
		add	r2, #1
		strb	r2, [r3, #0]		; Put back

		mov	r1, #A_LOCK_SEMA	; Free the lock
		bl	sark_lock_free

		mov	r0, r2			; Return post-inc value
		pop	{r4, pc}

		endp

 		area 	alib_sema_lower, readonly, code
		code16

; uint sema_lower (uchar *sema);

       	        export	sark_sema_lower
sark_sema_lower	proc

		push	{r4, lr}		; Save return address
		mov	r3, r0			; Arg to r3
		mov	r0, #A_LOCK_SEMA	; Get this lock
		bl	sark_lock_get		; Returns CPSR in r0

		ldrb	r2, [r3, #0]		; Get the variable
		cmp	r2, #0	 		; Test for zero
		beq	lower1

		sub	r1, r2, #1		; Decrement if not
		strb	r1, [r3, #0]		; and put back

lower1		mov	r1, #A_LOCK_SEMA	; Free lock
		bl	sark_lock_free

		mov	r0, r2			; Return pre-dec value
		pop	{r4, pc}

		endp

;------------------------------------------------------------------------------

 		area 	alib_memcpy, readonly, code
		code32

; void sark_mem_cpy (void *dest, const void *src, uint n);

		export	sark_mem_cpy
sark_mem_cpy	proc

		subs	r2, r2, #1
		ldrhsb	r3, [r1], #1
		strhsb	r3, [r0], #1
		bhs	sark_mem_cpy
		bx	lr

		endp

;------------------------------------------------------------------------------

 		area 	alib_strcpy, readonly, code
		code32

; void sark_str_cpy (char *dest, const char *src);

		export	sark_str_cpy
sark_str_cpy	proc

		ldrb	r2, [r1], #1
		cmp	r2, #0
		strb	r2, [r0], #1
		bne	sark_str_cpy
		bx	lr

		endp

;------------------------------------------------------------------------------

 		area 	alib_word_set, readonly, code
		code32

; void sark_word_set (void *dest, uint data, uint n);

		export	sark_word_set
sark_word_set	proc

		mov	r3, r1			; Copy data to r3

ws1	  	subs    r2, r2, #16		; Drop count by 16
  		stmcs   r0!, {r1, r3}		; Fill 16 bytes...
  		stmcs   r0!, {r1, r3}		;
  		bcs     ws1 			; Loop while >= 0

		lsls    r2, r2, #29 		; Shift count to flags
  		stmcs   r0!, {r1, r3}		; and fill remainder
  		stmmi   r0!, {r1}

  		bx	lr

		endp

;------------------------------------------------------------------------------

 		area 	alib_word_cpy, readonly, code
		code32

; void copy_msg (msg_t *to, msg_t *from)

		export	sark_msg_cpy
sark_msg_cpy	proc

		add	 r0, r0, #4		; Point past link word (to)
		add	 r1, r1, #4		; Point past link word (from)
		ldrh     r2, [r1, #0]		; Get length (bytes)
		add	 r2, r2, #4		; Bump length by 4 (len/sum)

		endp

; Fast copy for word aligned buffers. Byte count "n" need not
; be a multiple of 4 but it will be rounded up to be so.

; void sark_word_cpy (void *dest, const void *src, uint n)

		export	sark_word_cpy
sark_word_cpy	proc

		tst	r2, #3
  		bicne	r2, r2, #3
  		addne	r2, r2, #4

		push    {r4, lr}

	  	subs    r2, r2, #32
  		bcc     wc2

wc1		ldm	r1!, {r3, r4, r12, lr}
  		stm     r0!, {r3, r4, r12, lr}
  		ldm     r1!, {r3, r4, r12, lr}
  		stm     r0!, {r3, r4, r12, lr}
  		subs    r2, r2, #32
  		bcs     wc1

wc2		lsls    r12, r2, #28
  		ldmcs   r1!, {r3, r4, r12, lr}
  		stmcs   r0!, {r3, r4, r12, lr}
  		ldmmi   r1!, {r3, r4}
  		stmmi   r0!, {r3, r4}

  		lsls    r12, r2, #30
  		ldrcs   r3, [r1], #4
  		strcs   r3, [r0], #4

  		pop     {r4, pc}

		endp

;------------------------------------------------------------------------------

 		area 	alib_rand, readonly, code
		code32

; uint sark_rand (void);

		export	sark_rand
		import	sark
sark_rand	proc

		ldr	r3, =sark + SARK_RANDOM
		ldmia	r3, {r0, r1}
		tst	r1, r1, lsr #1
		movs	r2, r0, rrx
		adc	r1, r1
		eor	r2, r2, r0, lsl #12
		eor	r0, r2, r2, lsr #20
		stmia	r3, {r0, r1}
		bx	lr

		endp

;------------------------------------------------------------------------------

		area	alib_srand, code, readonly
		code16

; void sark_srand (uint val);

		export	sark_srand
		import	sark
sark_srand	proc

		ldr	r2, =sark + SARK_RANDOM
		mov	r1, #0
		stmia	r2!, {r0, r1}
		bx	lr

		endp

 ;------------------------------------------------------------------------------

		area	alib_delay, code, readonly
		code16

; void sark_delay_us (uint delay);

		export	sark_delay_us
		import	sark
sark_delay_us	proc

        	ldr      r1, =sark + SARK_CPU_CLK ; Get clock speed to r1
        	ldrh     r1, [r1]
        	mul      r1, r0			; Multiply by delay
        	lsr      r0, r1, #2		; Shift to divide by 4

delay1        	sub      r0, #1			; Loop takes 4 clocks
        	bcs      delay1

        	bx       lr

		endp

 ;------------------------------------------------------------------------------

 		area 	alib_chksum, readonly, code
		code32

; Compute 4-bit 1's complement checksum of 64/32 bits. Sum is returned in
; top 4 bits of result & other bits are zero.

; uint chksum_64 (uint a, uint b)
; uint chksum_32 (uint a)

		export	chksum_64
chksum_64	proc

	  	adds  	r0, r0, r1 		; s = a + b
  		addcs 	r0, r0, #1		; Add back carry

		endp

		export	chksum_32
chksum_32	proc

		adds	r0, r0, r0, lsl #16	; s = s + s << 16;
  		addcs 	r0, r0, #0x00010000	; Add back carry
  		bic   	r0, r0, #0x0000ff00	; Ensure no carry in
  		adds	r0, r0, r0, lsl #8	; s = s + s << 8
  		addcs 	r0, r0, #0x01000000	; Add back carry
  		bic   	r0, r0, #0x00ff0000	; Ensure no carry in
  		adds	r0, r0, r0, lsl #4	; s = s + s << 4
  		addcs 	r0, r0, #0x10000000	; Add back carry
  		and   	r0, r0, #0xf0000000	; Isolate checksum
  		eor	r0, r0, #0xf0000000	; Complement top 4
  		bx	lr

		endp

;------------------------------------------------------------------------------

 		area 	alib_div10, readonly, code
		code32

; divmod_t sark_div10 (uint n);

		export	sark_div10
sark_div10	proc

	if 	:def:  DIV10_VIR	; value_in_regs version (not used)

    		sub    r1, r0, #10
    		sub    r0, r0, r0, lsr #2
    		add    r0, r0, r0, lsr #4
    		add    r0, r0, r0, lsr #8
    		add    r0, r0, r0, lsr #16
    		mov    r0, r0, lsr #3
    		add    r2, r0, r0, lsl #2
    		subs   r1, r1, r2, lsl #1
    		addpl  r0, r0, #1
    		addmi  r1, r1, #10
    		bx     lr
	else
		sub    r2, r1, #10             ; keep (x-10) for later
		sub    r1, r1, r1, lsr #2
		add    r1, r1, r1, lsr #4
		add    r1, r1, r1, lsr #8
		add    r1, r1, r1, lsr #16
		mov    r1, r1, lsr #3
		add    r3, r1, r1, lsl #2
		subs   r2, r2, r3, lsl #1      ; calc (x-10) - (x/10)*10
		addpl  r1, r1, #1              ; fix-up quotient
		addmi  r2, r2, #10             ; fix-up remainder
		stm    r0, {r1, r2}
		bx     lr
	endif
		endp

;------------------------------------------------------------------------------

; IRQ_HANDLER sark_int_han (void);

 		area 	alib_sark_int, readonly, code
		code32
	
		export	sark_int_han
		import	sark_int

sark_int_han	proc

		push	{r0-r3, r12, lr}	; Save lr and working regs
		sub	r0, lr, #4	  	; Return address to r0

    		mrs    	r12, spsr		; Get SPSR_irq to r12
    		msr    	cpsr_c, #MODE_SYS    	; Go to SYS mode, interrupts enabled

    		push	{r12, lr} 		; Save SPSR_irq and LR_sys

    		bl     	sark_int     		; Call main handler

    		pop	{r12, lr} 		; Restore SPSR_irq & LR_sys

    		msr    	cpsr_c, #MODE_IRQ+IMASK_IRQ ; Back to IRQ mode, IRQ disabled
    		msr    	spsr_cxsf, r12		; Restore SPSR_irq

		mov    	r0, #VIC_BASE		; Tell VIC we are done
		str    	r0, [r0, #VIC_VADDR]

    		pop	{r0-r3, r12, lr}	; Restore working regs and lr

		endp

; IRQ_HANDLER null_int_han (void);

		export	null_int_han

null_int_han	proc

		subs	pc, lr, #4

		endp
	
;------------------------------------------------------------------------------

; void schedule (uchar event_id, uint arg0, uint arg1)

 		area 	alib_schedule, readonly, code
		code32

		export	schedule
       		import	schedule_sysmode [weak]

schedule	proc
		
		push	{r12, lr}		; save r12 and lr_irq

		mrs	r12, cpsr		; Go to SYS mode
		bic	lr, r12, #0x1f
		orr	lr, lr, #MODE_SYS
		msr	cpsr_c, lr

		push	{r12, lr}		; save lr_sys and cpsr_c

		bl	schedule_sysmode

        	pop	{r12, lr}		; restore lr_sys and cpsr_c
		msr	cpsr_c, r12		; back to IRQ mode

		pop	{r12, lr}		; restore r12 and lr_irq
		bx	lr			; return using lr_irq

		endp

;------------------------------------------------------------------------------

		area	alib_rts, code, readonly
		code16

; Dummy functions. All drop through to "bx lr"

		export	sark_wait [weak]
sark_wait  	proc
		endp

		export	sark_config [weak]
sark_config  	proc
		endp

		bx	lr

; void bx (uint addr)

		export	sark_bx
sark_bx	  	proc
		bx	r0			; Branch to r0
		endp

;------------------------------------------------------------------------------

		area	alib_t1_case_uqi, code, readonly
		code16

; This is a support routine from the GNU library (rewritten slightly
; for efficiency). There is a nicer 32 bit version but GCC won't link
; to it correctly.

		export	__gnu_thumb1_case_uqi

__gnu_thumb1_case_uqi proc

		mov      r12, r1		; Save r1 in scratch reg
		mov      r1, lr			; Copy return address
		sub      r1, #1			; and make even
		ldrb     r1, [r1, r0]		; Get a byte from table
		lsl      r1, #1	  		; Multiply by two and
		add      lr, r1			; point to target code
		mov      r1, r12		; Restore r1
		bx       lr  			; and go to code

;		code32
;		bic	r12, lr, #1		; Point to table
;		ldrb	r12, [r12, r0]		; Read offset from table
;		add	pc, lr, r12, lsl #1	; Form new PC

		endp

;------------------------------------------------------------------------------

; APLX Loader

; Format of APLX table - each entry except last is 4 words. First word
; is an opcode - ACOPY, RCOPY, FILL, EXEC, END. Next three words are
; operands. Copy/Fill always rounds up to multiple of 32 bytes and
; zero length is not sensible.

;	APLX_ACOPY, APLX_RCOPY
;	Dest address
;	Srce address (relative to start of this entry if RCOPY)
;	Length (bytes)

;	APLX_FILL
;	Dest address
;	Length (bytes)
;	Fill data

;	APLX_EXEC
;	Program counter
;	(Unused)
;	(Unused)

;	APLX_END (or invalid opcode)
;	(Unused)
;	(Unused)
;	(Unused)

; void sark_aplx (uint *table, uint app_id)

		area	alib_aplx, code, readonly
		code16

		export	sark_aplx
sark_aplx	proc

		push	{r1, r4-r7, lr}		; Save link, r1, r4-r7
		ldr	r1, aplx_buffer		; Get buffer to r1
		adr	r6, aplx_loader		; r6 -> loader code
		mov	r7, #APLX_BUF_SIZE	; Move 64 bytes...

aplx_l0		ldm	r6!, {r2-r5}		; ... 16 at a time
		stm	r1!, {r2-r5}
		sub	r7, #16
		bne	aplx_l0

		mov	r4, r0			; Table pointer to r4
		sub	r1, #APLX_BUF_SIZE-1	; r1 -> load point + 1
		bx	r1

  	     	align	4

aplx_buffer	dcd	APLX_BUFFER

; Come here with table address in r4. Following section of code must
; fit in 64 bytes (currently 60)

aplx_loader	ldm	r4!, {r0-r3}		; Get opcode & operands

		cmp	r0, #APLX_ACOPY		; Copy absolute
		beq	aplx_copy
		cmp	r0, #APLX_RCOPY		; Copy relative
		beq	aplx_rcopy
		cmp	r0, #APLX_FILL		; Fill
		beq	aplx_fill
		cmp	r0, #APLX_EXEC		; Execute
		beq	aplx_exec

		pop	{r1, r4-r7, pc}		; Restore & return

aplx_rcopy	add	r2, r4			; Copy relative
		sub	r2, #APLX_ENTRY_SIZE	; Reduce by table entry size

aplx_copy	ldm	r2!, {r0, r5-r7}	; Copy absolute
		stm	r1!, {r0, r5-r7}	; r1 -> to   
		ldm	r2!, {r0, r5-r7}	; r2 -> from 
		stm	r1!, {r0, r5-r7}	; r3 = length
		sub	r3, #32
		bhi	aplx_copy
		b	aplx_loader

aplx_fill	movs	r5, r3			; Fill
		movs	r6, r3			; r1 -> to
		movs	r7, r3			; r2 = length
aplx_l1		stm     r1!, {r3, r5-r7}	; r3 = data
  		stm     r1!, {r3, r5-r7}
  		sub     r2, #32
  		bhi     aplx_l1
		b	aplx_loader

aplx_exec	ldr	r0, [sp, #0] 		; Get app_id to r0
		blx	r1			; Exec absolute
		b	aplx_loader		; r1 = address

		endp

		align	4

;------------------------------------------------------------------------------

; This section is placed last, after all other code to be copied to ITCM.
; It forces the length of the section to be a multiple of 4 bytes as it
; was previously Thumb aligned and this caused problems in copying the RW
; data that followed. This is a bodge!

		area	_alib_align, code, readonly
		code32

		dcd	DEAD_WORD

;------------------------------------------------------------------------------

		end
