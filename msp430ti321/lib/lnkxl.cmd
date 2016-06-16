/****************************************************************************/
/* LNKXL.CMD - V3.0.0  COMMAND FILE FOR LINKING LARGE-DATA MSP430X PROGRAMS */
/*                                                                          */
/*   Usage:  cl430 -vmspx -ml <src files> -z -o <out file>                  */
/*                                           -m <map file> lnkxl.cmd        */
/*           cl430  <src files...> -z -o <out file> -m <map file> lnk.cmd   */
/*                                                                          */
/*   Description: This file is a sample command file that can be used       */
/*                for linking programs built with the MSP430 C              */
/*                Compiler.   Use it as a guideline; you may want to change */
/*                the allocation scheme according to the size of your       */
/*                program and the memory layout of your target system.      */
/*                                                                          */
/*   Notes: (1)   You must specify the directory in which rts430xl.lib is   */
/*                located.  Either add a "-i<directory>" line to this       */
/*                file, or use the system environment variable C_DIR to     */
/*                specify a search path for libraries.                      */
/*                                                                          */
/*          (2)   If the run-time support library you are using is not      */
/*                named rts430xl.lib, be sure to use the correct name here. */
/*                                                                          */
/****************************************************************************/
-c                                         /* LINK USING C CONVENTIONS      */
-stack  0x2000                             /* SOFTWARE STACK SIZE           */
-heap   0x3000                             /* HEAP AREA SIZE                */

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
    SFR(R)           : origin = 0x0000, length = 0x0010
    PERIPHERALS_8BIT : origin = 0x0010, length = 0x00F0
    PERIPHERALS_16BIT: origin = 0x0100, length = 0x0100
    MEM(RW)          : origin = 0x0200, length = 0xFDE0
    HIMEM            : origin = 0x10000,length = 0xEFFFF
    VECTORS(R)       : origin = 0xFFE0, length = 0x001E
    RESET            : origin = 0xFFFE, length = 0x0002
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
    .intvecs : {} > VECTORS          /* INTERRUPT VECTORS                 */
    .bss     : {} > HIMEM            /* GLOBAL & STATIC VARS              */
    .sysmem  : {} > HIMEM            /* DYNAMIC MEMORY ALLOCATION AREA    */
    .stack   : {} > HIMEM            /* SOFTWARE SYSTEM STACK             */

    .text:_isr : {} > MEM            /* CODE FOR ISR'S                    */
    .text    : {} > HIMEM            /* CODE                              */
    .cinit   : {} > HIMEM            /* INITIALIZATION TABLES             */
    .const   : {} > HIMEM            /* CONSTANT DATA                     */
    .cio     : {} > HIMEM            /* C I/O BUFFER                      */

    .pinit   : {} > HIMEM            /* C++ CONSTRUCTOR TABLES            */
    .reset   : > RESET 
}

