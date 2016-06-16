;*****************************************************************************
;* Filename:    msp430_inout.s
;*
;* Copyright 2010, Research In Motion Ltd
;*
;* Author:      Jesse Schwartzentruber
;*
;* Created:     June 8, 2010
;*
;* Description: Assembler routine for HwDelay (for a MCLK frequency of 16Mhz )
;*****************************************************************************
            ; imports

            .global InterruptHandlerActual
            .global GetQueueCount

            ; exports

            .global IsrVect0
            .global IsrVect1
            .global IsrVect2
            .global IsrVect3
            .global IsrVect4
            .global IsrVect5
            .global IsrVect6
            .global IsrVect7
            .global IsrVect8
            .global IsrVect9
            .global IsrVect10
            .global IsrVect11
            .global IsrVect12
            .global IsrVect13
            .global IsrVect14
            .global IsrVect15
            .global IsrVect16
            .global IsrVect17
            .global IsrVect18
            .global Vect5Base

            .text

IsrVect0    push    r15
            mov.w   #0x00, r15
            jmp     handleInterrupt
IsrVect1    push    r15
            mov.w   #0x02, r15
            jmp     handleInterrupt
IsrVect2    push    r15
            mov.w   #0x04, r15
            jmp     handleInterrupt
IsrVect4    push    r15
            mov.w   #0x08, r15
            jmp     handleInterrupt
; first 4 functions are 6 bytes each, below are 8 bytes each
IsrVect3    push    r15
            mov.w   #0x06, r15
            jmp     handleInterrupt
Vect5Base
IsrVect5    push    r15
            mov.w   #0x0A, r15
            jmp     handleInterrupt
IsrVect6    push    r15
            mov.w   #0x0C, r15
            jmp     handleInterrupt
IsrVect7    push    r15
            mov.w   #0x0E, r15
            jmp     handleInterrupt
IsrVect8    push    r15
            mov.w   #0x10, r15
            jmp     handleInterrupt
IsrVect9    push    r15
            mov.w   #0x12, r15
            jmp     handleInterrupt
IsrVect10   push    r15
            mov.w   #0x14, r15
            jmp     handleInterrupt
IsrVect11   push    r15
            mov.w   #0x16, r15
            jmp     handleInterrupt
IsrVect12   push    r15
            mov.w   #0x18, r15
            jmp     handleInterrupt
IsrVect13   push    r15
            mov.w   #0x1A, r15
            jmp     handleInterrupt
IsrVect14   push    r15
            mov.w   #0x1C, r15
            jmp     handleInterrupt
IsrVect15   push    r15
            mov.w   #0x1E, r15
            jmp     handleInterrupt
IsrVect16   push    r15
            mov.w   #0x20, r15
            jmp     handleInterrupt
IsrVect17   push    r15
            mov.w   #0x22, r15
            jmp     handleInterrupt
IsrVect18   push    r15
            mov.w   #0x24, r15
            ;jmp     handleInterrupt

;============================================================================
; void HandleInterrupt( FUNCTION isrNum )
;============================================================================
handleInterrupt
            push    r14
            push    r13
            push    r12
            push    r11
            call    InterruptHandlerActual(r15)
            call    #GetQueueCount
            tst.b   r12
            jz      doneInterrupt
            bic     #0xF0, 0x0A(sp) ; __bic_SR_register_on_exit( LPM4_SR_BITS )
doneInterrupt
            pop     r11
            pop     r12
            pop     r13
            pop     r14
            pop     r15
            reti

