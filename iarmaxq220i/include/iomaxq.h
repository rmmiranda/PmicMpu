/********************************************************************
 *
 * System register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ.
 *
 * This file supports assembler and C development for MAXQ
 *
 * Copyright 2004-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1344 $
 *
 ********************************************************************/

#ifndef __IOMAXQ
#define __IOMAXQ

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x42)     /* 0x42 = 66 dec */
#error iomaxqxx.h file for use with ICCMAXQ/AMAXQ only
#endif

#ifdef __ICCMAXQ__
#if(__CORE__ == __MAXQ68__)
#define USE_MAXQ68_SYSTEM_REGISTERS
#endif
#else
#if((__TID__ & 0x00FF) == 68)
#define USE_MAXQ68_SYSTEM_REGISTERS
#endif
#endif

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended

#include "iomacro.h"

/*-------------------------------------------------------------------------
 *   Special Function Registers
 *-------------------------------------------------------------------------*/

/*-------------------------------*
 *           Module 8            *
 *-------------------------------*/

/* Processor Status Flags register */
__no_init volatile __io union
{
  unsigned char PSF;
  struct
  {
    unsigned char E              : 1;
    unsigned char C              : 1;
    unsigned char OV             : 1;
    unsigned char GPF0           : 1;
    unsigned char GPF1           : 1;
    unsigned char                : 1;
    unsigned char S              : 1;
    unsigned char Z              : 1;
  } PSF_bit;
} @ _M(8,4);


#ifndef USE_MAXQ68_SYSTEM_REGISTERS

/* Interrupt and Control register */
__no_init volatile __io union
{
  unsigned char IC;
  struct
  {
    unsigned char IGE            : 1;
    unsigned char INS            : 1;
    unsigned char                : 1;
    unsigned char                : 1;
    unsigned char                : 1;
    unsigned char CGDS           : 1;
    unsigned char                : 1;
    unsigned char                : 1;
  } IC_bit;
} @ _M(8,5);

/* Interrupt Mask Register */
__no_init volatile __io union
{
  unsigned char IMR;
  struct
  {
    unsigned char IM0           : 1;
    unsigned char IM1           : 1;
    unsigned char IM2           : 1;
    unsigned char IM3           : 1;
    unsigned char IM4           : 1;
    unsigned char IM5           : 1;
    unsigned char               : 1;
    unsigned char IMS           : 1;
  } IMR_bit;
} @ _M(8,6);

/* Interrupt Identification Register */
__no_init volatile __io union
{
  unsigned char IIR;
  struct
  {
    unsigned char II0           : 1;
    unsigned char II1           : 1;
    unsigned char II2           : 1;
    unsigned char II3           : 1;
    unsigned char II4           : 1;
    unsigned char II5           : 1;
    unsigned char               : 1;
    unsigned char IIS           : 1;
  } IIR_bit;
} @ _M(8,0x0B);

/* System Control register */
__no_init volatile __io union
{
  unsigned char SC;
  struct
  {
    unsigned char                : 1;
    unsigned char PWL            : 1;
    unsigned char ROD            : 1;
    unsigned char UPA            : 1;
    unsigned char CDA0           : 1;
    unsigned char CDA1           : 1;
    unsigned char                : 1;
    unsigned char TAP            : 1;
  } SC_bit;
} @ _M(8,8);

#else

#undef IIR
#undef IMR

/* MODULE 8 */
/* privilege Register */
__no_init volatile __io union
{
  unsigned char PRIV;
  struct
  {
      unsigned char PULR         : 1;
      unsigned char PULW         : 1;
      unsigned char PSYR         : 1;
      unsigned char PSYW         : 1;
  } PRIV_bit;
} @ _M(8,2);

__no_init volatile __io unsigned char PRIVT0 @ _M(8,3);

/* Interrupt and Control register */
__no_init volatile __io union
{
  unsigned char IC;
  struct
  {
    unsigned char IGE            : 1;
    unsigned char                : 1;
    unsigned char IPS            : 2;
    unsigned char                : 1;
    unsigned char                : 1;
    unsigned char                : 1;
    unsigned char                : 1;
  } IC_bit;
} @ _M(8,5);

__no_init volatile __io union
{
  unsigned short IPR0;
  struct
  {
    unsigned short IVP0           : 2;
    unsigned short IVP1           : 2;
    unsigned short IVP2           : 2;
    unsigned short IVP3           : 2;
    unsigned short IVP4           : 2;
    unsigned short IVP5           : 2;
    unsigned short IVP6           : 2;
    unsigned short IVP7           : 2;
  } IPR0_bit;
} @ _M(8,9);

__no_init volatile __io union
{
  unsigned short IPR1;
  struct
  {
    unsigned short IVP8           : 2;
    unsigned short IVP9           : 2;
    unsigned short IVP10          : 2;
    unsigned short IVP11          : 2;
    unsigned short IVP12          : 2;
    unsigned short IVP13          : 2;
    unsigned short IVP14          : 2;
    unsigned short IVP15          : 2;
  } IPR1_bit;
} @ _M(8,10);

__no_init volatile __io union
{
  unsigned char PRIVF;
  struct
  {
    unsigned short              : 4;
    unsigned short PULRF		    : 1;
    unsigned short PULWF        : 1;
    unsigned short PSYRF        : 1;
    unsigned short PSYWF        : 1;
 } PRIVF_bit;
} @ _M(8,11);

__no_init volatile __io unsigned short ULDR @ _M(8,12);

__no_init volatile __io unsigned short UAPP @ _M(8,13);

/* System Control register */
__no_init volatile __io union
{
  unsigned int SC;
  struct
  {
    unsigned char                : 1;
    unsigned char PWL            : 1;
    unsigned char ROD            : 1;
    unsigned char UPA            : 1;
    unsigned char CDA0           : 1;
    unsigned char CDA1           : 1;
    unsigned char                : 1;
    unsigned char TAP            : 1;
    unsigned char PWLS           : 1;
    unsigned char PWLL           : 1;
    unsigned char MPE            : 1;
  } SC_bit;
} @ _M(8,8);

#endif

/* CKCN and WDCN are defined in each of the implementation specific header files */

/*-------------------------------*
 *           Module 13            *
 *-------------------------------*/

 /* Interrupt Vector register */
__no_init volatile __io unsigned short IV @ _M(0x0D,2);   /* Interrupt Vector register */


#pragma language=default
#endif /* __IAR_SYSTEMS_ICC__  */

#ifdef __IAR_SYSTEMS_ASM__
#ifdef USE_MAXQ68_SYSTEM_REGISTERS

#define PRIV M8[2]
#define PRIVT0 M8[3]
#define PRIVT1 M8[6]
#define IPR0 M8[9]
#define IPR1 M8[10]
#define PRIVF M8[11]
#define ULDR M8[12]
#define UAPP M8[13]
#undef IIR
#undef IMR

#endif

#endif
#endif /* __IOMAXQ */
