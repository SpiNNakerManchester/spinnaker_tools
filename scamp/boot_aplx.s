
;;------------------------------------------------------------------------------
;;
;; boot_aplx.s	    Self-loading preamble for SC&MP APLX files
;;
;; Copyright (C)    The University of Manchester - 2009-2013
;;
;; Author           Steve Temple, APT Group, School of Computer Science
;; Email            temples@cs.man.ac.uk
;;
;;------------------------------------------------------------------------------

		get	spinnaker.s
		get	sark.s

		preserve8

 		area 	boot_aplx, readonly, code, align=7

		entry

aplx_start	mov	sp, #DTCM_TOP		; Set up stack
		adr	r0, aplx_args		; APLX block in DTCM
		ldm	r0, {r0-r1, r4-r5}	; Get address & args
		blx	proc_aplx		; Go to loader

; When booting via bootROM, APLX table ends up in DTCM 128 bytes above
; start of image		

aplx_args	dcd	DTCM_BASE + 0x8080	; Address of APLX table
		dcd	0			; Arg passed in r0 (app_id)

aplx_svc	msr	cpsr_c, #IMASK_ALL+MODE_SVC
		bx	lr

;-------------------------------------------------------------------------------

; proc_aplx (uint table, uint arg)

  		code16

proc_aplx	push	{r1, r4-r7, lr}		; Save link and r1, r4-r7
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

; Come here with table address in r4

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

aplx_exec	ldr	r0, [sp, #0] 		; Get arg to r0
		blx	r1			; Exec absolute
		b	aplx_loader		; r1 = address

		align	128
		code32
aplx_end

;------------------------------------------------------------------------------
		end
