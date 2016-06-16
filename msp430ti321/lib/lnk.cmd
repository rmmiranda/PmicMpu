/****************************************************************************/
/* LNK.CMD - V3.0.0  COMMAND FILE FOR LINKING MSP430 PROGRAMS               */
/*                                                                          */
/*   Usage:  cl430 <src files> -z -o <out file> -m <map file> lnk.cmd       */
/*                                                                          */
/*   Description: This file is a sample command file that can be used       */
/*                for linking programs built with the MSP430 C              */
/*                Compiler.   Use it as a guideline; you may want to change */
/*                the allocation scheme according to the size of your       */
/*                program and the memory layout of your target system.      */
/*                                                                          */
/*   Notes: (1)   You must specify the directory in which rts430.lib is     */
/*                located.  Either add a "-i<directory>" line to this       */
/*                file, or use the system environment variable C_DIR to     */
/*                specify a search path for libraries.                      */
/*                                                                          */
/*          (2)   If the run-time support library you are using is not      */
/*                named rts430.lib, be sure to use the correct name here.   */
/*                                                                          */
/****************************************************************************/
-c                                         /* LINK USING C CONVENTIONS      */
-stack  0x1400                             /* SOFTWARE STACK SIZE           */
-heap   0x0c00                             /* HEAP AREA SIZE                */

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
    SFR(R)           : origin = 0x0000, length = 0x0010
    PERIPHERALS_8BIT : origin = 0x0010, length = 0x00F0
    PERIPHERALS_16BIT: origin = 0x0100, length = 0x0100
    MEM(RW)          : origin = 0x0200, length = 0xFDE0
    VECTORS(R)       : origin = 0xFFE0, length = 0x001E
    RESET            : origin = 0xFFFE, length = 0x0002
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
    .intvecs : {} > VECTORS          /* INTERRUPT VECTORS                 */
    .bss     : {} > MEM              /* GLOBAL & STATIC VARS              */
    .sysmem  : {} > MEM              /* DYNAMIC MEMORY ALLOCATION AREA    */
    .stack   : {} > MEM              /* SOFTWARE SYSTEM STACK             */

    .text    : {} > MEM              /* CODE                              */
    .cinit   : {} > MEM              /* INITIALIZATION TABLES             */
    .const   : {} > MEM              /* CONSTANT DATA                     */
    .cio     : {} > MEM              /* C I/O BUFFER                      */

    .pinit   : {} > MEM              /* C++ CONSTRUCTOR TABLES            */
    .reset   : > RESET 
}

