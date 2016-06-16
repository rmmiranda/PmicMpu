;/****************************************************************************
; * Copyright (C) 2011 Maxim Integrated Products, All Rights Reserved.
; *
; * Permission is hereby granted, free of charge, to any person obtaining a
; * copy of this software and associated documentation files (the "Software"),
; * to deal in the Software without restriction, including without limitation
; * the rights to use, copy, modify, merge, publish, distribute, sublicense,
; * and/or sell copies of the Software, and to permit persons to whom the
; * Software is furnished to do so, subject to the following conditions:
; *
; * The above copyright notice and this permission notice shall be included
; * in all copies or substantial portions of the Software.
; *
; * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
; * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
; * MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
; * IN NO EVENT SHALL MAXIM INTEGRATED PRODUCTS BE LIABLE FOR ANY CLAIM, DAMAGES
; * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
; * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
; * OTHER DEALINGS IN THE SOFTWARE.
; *
; * Except as contained in this notice, the name of Maxim Integrated Products
; * shall not be used except as stated in the Maxim Integrated Products
; * Branding Policy.
; *
; * The mere transfer of this software does not imply any licenses
; * of trade secrets, proprietary technology, copyrights, patents,
; * trademarks, maskwork rights, or any other form of intellectual
; * property whatsoever. Maxim Integrated Products retains all ownership rights.
; *
; ********************************************************************************
; */

#define UPA SC.3

    MODULE flash

    EXTERN  ?UTIL_FLASH_WRITE
    EXTERN  ?UTIL_FLASH_ERASE_PAGE
    EXTERN  ?UTIL_CRC16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: flash_writeWord
; Description: writes a 16-bit value to the flash
; Inputs: A[0] = address to write
;         A[1] = data to write
; Outputs: A[0] = 0 on error, A[0] = 1 on success.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    RSEG CODE
    PUBLIC flash_writeWord
flash_writeWord:
    push    SC                    ; Save the current value
    move    UPA, #0               ; Make sure the ROM is mapped in
    call    ?UTIL_FLASH_WRITE     ; Do the Write
    pop     SC                    ; Restore value
    move    A[0], #0              ; Setup return value
    ret     C
    move    A[0], #1
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: flash_readWord
; Description: reads a 16-bit valule from the flash
; Inputs: A[0] = word address to read
; Outputs: A[0] = value read
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    RSEG CODE
    PUBLIC flash_readWord
flash_readWord:
    push    DPC                   ; Save current value
    move    DPC, #5Ch             ; Put pointers in word mode
    move    CP, A[0]              ; Point to value wanted
    move    A[0], @CP             ; Read it
    pop     DPC                   ; Restore value
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: flash_crc
; Description: Computes the CRC-16 of a word.
; Inputs: A[0] = value to CRC-16
;         A[1] = starting CRC-16 value (seed)
; Outputs: A[0] = value read
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    RSEG CODE
    PUBLIC flash_crc
flash_crc:
    push    SC                    ; Save current value
    move    UPA, #0               ; Make sure ROM is accessible
    move    APC, #80h             ; Reset AP
    move    GR, A[1]              ; Setup input to ROM function
    call    flash_readWord        ; Read the word to be CRC'ed
    push    ACC                   ; Save it
    and     #0FFh                 ; Get the low byte
    call    ?UTIL_CRC16           ; CRC it
    pop     ACC                   ; Restore the original word value
    xch                           ; Get the high byte
    and     #0FFh
    call    ?UTIL_CRC16           ; CRC it
    pop     SC                    ; Restore SC
    move    A[0], GR              ; Setup the return value
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: flash_erasePage
; Description: Erases a page of flash
; Inputs: A[0] = an address within the page to be erased.
; Outputs: A[0] = 0 on error, A[0] = 1 on success.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    RSEG CODE
    PUBLIC flash_erasePage
flash_erasePage:
    push    SC                    ; Save current value
    move    UPA, #0               ; Make sure ROM is accessible
    call    ?UTIL_FLASH_ERASE_PAGE; Do the erase
    pop     SC                    ; Restore SC
    move    A[0], #0              ; Setup the return value
    ret     C
    move    A[0], #1
    ret

    END
