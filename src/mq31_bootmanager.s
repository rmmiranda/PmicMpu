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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This code assumes the flash is divided into four sections: the fuel gauge
; code, the boot manager code, the application code, and the upgrade area.  The
; application code area and the upgrade area must be the same size.  Upon system
; reset, the fuel gauge code jumps to the boot manager.  The boot manager then
; checks the upgrade area for a valid upgrade image.  If a valid upgrade image is
; not found, the boot manager checks the upgrade area for valid application code,
; sets a flag indicating if valid code was found, and then jumps to the main
; application code.  Execution will then continue as normal.  If the boot manager
; does find a valid upgrade image, it performs these steps:
;
; 1) erases the application code,
; 2) copies the upgrade image to the application area,
; 3) verifies the copy was successful,
; 4) erases the upgrade image, and
; 5) jumps to the application area.
;
; The upgrade area is then free to have the secondary application code loaded
; into it or left blank waiting for another upgrade image.  If power is lost
; during steps 1, 2, or 3, when the system resets the boot manager will still
; see a valid upgrade image in the upgrade area and start the process again at
; step 1.  If power is lost during step 4 or 5, upon reset the boot manager will
; no longer find a valid image in the upgrade area and jump to application code.
; Since step 2 was executed before power was lost, that application code will
; be the new version.
;
; At any time, the application code can erase the image area and write new data
; into it.  This data can be marked as an upgrade image or secondary application
; code.  Once the application is satisfied the upgrade image is complete, it can
;
; A) mark the image as a valid upgrade and then jump to step 1 to apply it, or
; B) mark the image as valid secondary code and jump to the beginning of the
;    application.
;
; If the main application wishes to use the secondary code, it should first
; check the state of the valid code variable before calling into the secondary
; code.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define UPA     SC.3
#define CDA1    SC.5
#define CPA     SC.6
#define SHDNCN  M5[4]

#define FLASH_PAGE_SIZE     0x100

#define BOOT_MANAGER_START  0x1F00
#define BOOT_MANAGER_END    0x1FFF

#define APP_START           0x2000
#define APP_END             0x8FFB
#define APP_SIZE            APP_END + 1 - APP_START
#define APP_PAGE_COUNT      (APP_SIZE + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE

#define IMAGE_START         0x9000
#define IMAGE_END           0xFFFB
#define IMAGE_SIZE          IMAGE_END + 1 - IMAGE_START
#define IMAGE_PAGE_COUNT    (IMAGE_SIZE + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE

#define TYPE_ADDR           IMAGE_END
#define PAIR_CRC_ADDR       IMAGE_END + 1
#define CRC_ADDR            IMAGE_END + 2
#define BOOT_MSG_ADDR		IMAGE_END + 3
#define BOOT_KEY_ADDR		IMAGE_END + 4

#define CODE_VALID_ADDR     0xA0

#define TYPE_UPGRADE        0x01
#define TYPE_APPLICATION    0x02

#define BOOT_MSG_UPDATE     0x01

    NAME    bootManager
    ASEGN	BOOT_MANAGER:CODE:ROOT,(BOOT_MANAGER_START << 1)

    EXTERN  ?UTIL_FLASH_WRITE
    EXTERN  ?UTIL_FLASH_ERASE_PAGE
    EXTERN  ?UTIL_MOVE_DP0_INC
    EXTERN  ?UTIL_CRC16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: bootManager
; Description: Examines the image area for a valid upgrade image or valid
;              application code.  If an upgrade image is found, the old code is
;              erased and the new code copied to the application area.  The
;              image area is then erased, the 'codeValid' variable is set to
;              false.  If valid application code is found, the 'codeValid'
;              variable is set to true.  Otherwise, the 'codeValid' varible is
;              set to false.  Finally, the main application code is executed.
; Inputs: None
; Outputs: None
; Destroys: AP = 0, APC = 0, DPC = 0x40, CP, DP[0], LC[0], LC[1],
;           A[0], A[1], A[3], A[4], GR, UPA = 0, CDA1 = 0, CPA = 1
; Assumes: None
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bootManager:
    jump    bootStart
    ORG (BOOT_MANAGER_START << 1) + 0x40
    DC16    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
    DC16    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
bootStart:
    move    WDCN.1, #0              ; Turn off watchdog timer
    move    SHDNCN.0, #0            ; Turn off shutdown timer
    move    CDA1, #0                ; Let the first half of flash be seen with CP
    move    UPA, #0                 ; Make sure UROM is mapped into code space
    move    APC, #0x80              ; Reset AP, disable auto increment/decrement
    call    isBootUpdateAvailable   ; Check if boot update is available, returns 0 if there is
    jump    NZ, mainAppJump         ; If no update is available, false and jump to the main application
    call    isCRCValid              ; Check CRC of image, returns 0 if match
    jump    NZ, setValidVariable    ; If a bad CRC was found, set 'codeValid' to
                                    ;  false and jump to the main application
    call    isApplicationCode       ; Check image type to see if it is application
    jump    Z, setValidVariable     ;  code or an upgrade image.  Returns 0 for app
                                    ; If an app image was found, set 'codeValid'
                                    ;  to true and jump to the main application
updateToNewCode:
    call    eraseApplication        ; Erase the current application code
    call    copyImage               ; Copy the new code in from the image area
    jump    C, updateToNewCode      ; If copy failed, retry indefinately
    call    verifyImage             ; Make sure the contents copied correctly
    jump    NZ, updateToNewCode     ; If copy failed, retry indefinately
    call    eraseImage              ; Erase the upgrade area
    move    ACC, #1                 ; Setup input to setValidVariable to
    jump    setValidVariable        ;  "not-valid", and jump to it.  It will
                                    ;  then set the codeValid variable and jump
                                    ;  to the main application

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: isBootUpdateAvailable
; Description: Determines data stored in the image area is a boot update.
; Inputs: None
; Outputs: ACC = 0 if boot update is availiable, non-zero otherwise
; Destroys: CP
; Assumes: CP in word mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
isBootUpdateAvailable:
    move    CP, #BOOT_MSG_ADDR      ; Point to the boot message value
    move    ACC, @CP                ; Read it
    sub     #BOOT_MSG_UPDATE        ; Set return value in ACC.
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: isCRCValid
; Description: Computes CRC of ImageArea and compares it against the stored CRC.
; Inputs: None
; Outputs: ACC = 0 if CRC matches, ACC = non-zero otherwise
; Destroys: GR, CP, LC[0], A[0], A[3], A[4], DPC = 0x40, CPA = 1
; Assumes: AP = 0, APC = 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
isCRCValid:
    move    CPA, #1                 ; Select upper 64KB for CP access.
    move    DPC, #0                 ; Set all pointers to byte mode
    move    LC[0], #IMAGE_SIZE << 1 ; Get the number of bytes to be CRC'ed
    move    CP, #IMAGE_START << 1   ; Point to the first byte to be CRC'ed
    move    GR, #0                  ; Initialize the CRC seed
isCRCValid_Loop:
    move    ACC, @CP++              ; Get the next byte to be CRC'ed
    call    ?UTIL_CRC16             ; Do the CRC
    djnz    LC[0], isCRCValid_Loop  ; Keep going until all bytes are CRC'ed
    move    DPC, #0x40              ; Set all pointers to byte mode except CP
    move    CP, #CRC_ADDR           ; Get the stored CRC
    move    ACC, @CP
    sub     GR                      ; Compare it against the calculated CRC
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: setValidVariable
; Description: Sets the 'codeValid' variable and jumps to the main application.
; Inputs: ACC = 0 if image area holds valid code, non-zero otherwise
; Outputs: None
; Destroys: DP[0]
; Assumes: DP[0] in byte mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
setValidVariable:
    move    DP[0], #CODE_VALID_ADDR ; Point to 'codeValid' variable
    jump    Z, setValidTrue         ; Check input to function
setValidFalse:
    move    @DP[0], #0              ; Set the variable to 0 (false)
    jump    mainAppJump
setValidTrue:
    move    @DP[0], #1              ; Set the variable to 1 (true)
mainAppJump:
    ljump   #APP_START              ; Jump to the application code


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: isApplicationCode
; Description: Determines the type of data stored in the image area.
; Inputs: None
; Outputs: ACC = 0 if type is application code, non-zero otherwise
; Destroys: CP
; Assumes: CP in word mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
isApplicationCode:
    move    CP, #TYPE_ADDR          ; Point to the type value
    move    ACC, @CP                ; Read it
    sub     #TYPE_APPLICATION       ; Set return value in ACC.
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: copyImage
; Description:
; Inputs: None
; Outputs: Carry bit set on error, clear on success
; Destroys: LC[0], LC[1], CP, A[0], A[1]
; Assumes: APC = 0, AP = 0, CP in word mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
copyImage:
    move    LC[0], #IMAGE_SIZE      ; Set the number of words to copy
    move    CP, #IMAGE_START        ; Point to beginning of source
    move    A[0], #APP_START        ; Point to beginning of destination
copyImage_Loop:
    move    A[1], @CP++             ; Read a word from source, increment pointer
    call    ?UTIL_FLASH_WRITE       ; Write the word to the destination
    ret     C                       ; Return if write failed
    add     #1                      ; Increment the destination pointer
    djnz    LC[0], copyImage_Loop   ; Keep going until all words are copied
    move    C, #0                   ; Indicate success
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: verifyImage
; Description:
; Inputs: None
; Outputs: ACC = 0 on success, non-zero otherwise
; Destroys: LC[0], DP[0], CP, GR, CDA1 = 0
; Assumes: AP = 0, APC = 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
verifyImage:
    move    DPC, #0x5C              ; Put all pointers in word mode.
    move    LC[0], #IMAGE_SIZE      ; Set the number of words to compare
    move    CP, #IMAGE_START        ; Point to beginning of image
    move    DP[0], #APP_START+0x8000; Point to beginning of app area.  Add
verifyImage_Loop:                   ;  0x8000 for data addr as seen from ROM.
    move    ACC, DP[0]              ; See if we wrapped into RAM
    jump    NZ, verifyImage_cont
    move    CDA1, #1                ; Map the upper flash into the data area
    move    DP[0], #0x8000          ; Move the data pointer to the upper flash
verifyImage_cont:
    move    ACC, @CP++              ; Read a word from image, increment ptr
    call    ?UTIL_MOVE_DP0_INC      ; Read a word from app, increment ptr
    sub     GR                      ; Compare the two values
    jump    NZ, verifyImage_Exit    ; If compare failed, exit with error
    djnz    LC[0], verifyImage_Loop ; Keep going until all words are compared
verifyImage_Exit:
    move    CDA1, #0                ; Restore CDA1 back to default state
    move    DPC, #0x40              ; Set all pointers to byte mode except CP
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: eraseImage
; Description: Erases the image area, skipping pages that are empty.
; Inputs: None
; Outputs: None
; Destroys: LC[0], LC[1], GR, A[0]
; Assumes: CP in word mode, AP = 0, APC = 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
eraseImage:
    move    LC[0], #IMAGE_PAGE_COUNT; Set the number of pages to erase
    move    A[0], #IMAGE_START      ; Set the address of the first page
    jump    erasePages              ; Do the erase


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: eraseApplication
; Description: Erases the application area, skipping pages that are empty.
; Inputs: None
; Outputs: None
; Destroys: LC[0], LC[1], GR, A[0]
; Assumes: CP in word mode, AP = 0, APC = 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
eraseApplication:
    move    LC[0], #APP_PAGE_COUNT  ; Set the number of pages to erase
    move    A[0], #APP_START        ; Set the address of the first page
    jump    erasePages              ; Do the erase


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: erasePages
; Description: Erases a sequential number of flash pages.  If a particular page
;              is already blank, it will be skipped to save wear on the flash.
; Inputs: LC[0] = num pages to erase
;         A[0] = word address of first page to erase
; Outputs: None
; Destroys: LC[0], LC[1], GR, A[0]
; Assumes: CP in word mode, AP = 0, APC = 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
erasePages:
    call    isPageEmpty             ; Is the current page already empty?
    jump    E, erasePages_Next      ; If so, skip the erase step
    push    A[0]
    call    ?UTIL_FLASH_ERASE_PAGE  ; Erase the page
    pop     A[0]
erasePages_Next:
    add     #FLASH_PAGE_SIZE        ; Change address to next page
    djnz    LC[0], erasePages       ; Keep going until all pages are erased
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Function: isPageEmpty
; Description: Determines is a given page is empty (contains all 0xFFFF).
; Inputs: A[0] = first address of page to check
; Outputs: equal flag set if page is empty, cleared otherwise
; Destroys: LC[1], CP
; Assumes: CP in word mode, AP = 0, APC = 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
isPageEmpty:
    move    LC[1], #FLASH_PAGE_SIZE ; Get number of words in a page
    move    CP, A[0]                ; Point to beginning of page
    push    ACC
isPageEmpty_Loop:
    move    ACC, @CP++              ; Get the next word in the page
    cmp     #0xFFFF                 ; If word != 0xFFFF, page is not empty
    jump    NE, isPageEmpty_Exit    ;
    djnz    LC[1], isPageEmpty_Loop ; Keep going until whole page is checked
isPageEmpty_Exit:
    pop     ACC
    ret

;    ORG 1FFFCh
;    DC16    TYPE_APPLICATION
;    ORG 1FFFEh
;    DC16    0x0046
    END

