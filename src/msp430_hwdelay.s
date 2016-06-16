;*****************************************************************************
;* Filename:    msp430_hwdelay.s
;*
;* Copyright 2008, Research In Motion Ltd
;*
;* Author:      Bryon Hummel
;*
;* Created:     December 19, 2008
;*
;* Description: Assembler routine for HwDelay (for a MCLK frequency of 16Mhz )
;*****************************************************************************

            .global HwDelay
            .text
;============================================================================
; HwDelay ( DWORD delayInNanoSec )
;============================================================================
HwDelay

;           HwDelayCyclePeriod = 62.5nS ... using 64 and adding some error in the delay loop to account for it.
;           Thr delay loop is ~17 cycles but we will calculate as if it was 16 to add the required error.
;           Calculate number of DelayLoops we need to run given the input nanosecond delay request
;           DelayLoopCount = NanosecDelay / ( 64 + 16 )    (using shifts for division, so this must be a multiple of 2)
            MOV.W  #10,R14   ; i=10
divLoop     CLRC            ; clear carry bit( so MSWord shift shifts in 0)
            RRC.W  R13      ; MSWord>>1
            RRC.W  R12      ; LSWord>>1 w/ carry from MSWord
            DEC.W  R14      ; i--
            JNZ    divLoop

            ; subtract ~divLoop overhead from loop duration  (4*16 is Approx. the divLoop overhead in DelayLoop 'time')
            SUB.W  #4,R12
            SUBC.W #0,R13

; R13, R12 now contain the number of loops to run through @16Mhz for the input nS delay requested

            INC.W  R13       ; add one to MSWord to account for zero exit test later    ; 1 cycle * 1

DelayLoop   MOV.W  #4,R14

InnerLoop   SUB.W  #1,R14
            JNZ    InnerLoop

            SUB.W  #1,R12    ; (First parameter LSWord) decrement by 1                  ; 1 cycle * loopCount
            SUBC.W #0,R13    ; (second parameter MSWord ) decrement by carry            ; 1 cycle * loopCount
            JNZ    DelayLoop                                                            ; 2 cycle * loopCount
            ret

