/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ200X.
 *
 * This file supports assembler and C development for MAXQ61X
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1344 $
 *
 ********************************************************************/


#ifndef __IOMAXQ61x
#define __IOMAXQ61x

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x42)     /* 0x42 = 66 dec */
#error iomaxqxx.h file for use with ICCMAXQ/AMAXQ only
#endif

#include "iomaxq.h"

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended

/*-------------------------------------------------------------------------
 *   Special Function Registers
 *-------------------------------------------------------------------------*/

/*-------------------------------*
 *           Module 0            *
 *-------------------------------*/

/* Port 0 Output Register */
__no_init volatile __io union
{
  unsigned char PO0;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PO0_bit;
} @ _M(0,0);

/* Port 1 Output Register */
__no_init volatile __io union
{
  unsigned char PO1;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PO1_bit;
} @ _M(0,1);

/* Port 2 Output Register */
__no_init volatile __io union
{
  unsigned char PO2;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PO2_bit;
} @ _M(0,2);

/* Port 3 Output Register */
__no_init volatile __io union
{
  unsigned char PO3;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PO3_bit;
} @ _M(0,3);

/* External Interrupt Flag 0 Register */
__no_init volatile __io union
{
  unsigned char EIF0;
  struct
  {
    unsigned char IE0            : 1;
    unsigned char IE1            : 1;
    unsigned char IE2            : 1;
    unsigned char IE3            : 1;
    unsigned char IE4            : 1;
    unsigned char IE5            : 1;
    unsigned char IE6            : 1;
    unsigned char IE7            : 1;
  } EIF0_bit;
} @ _M(0,4);


/* External Interrupt Enable 0 Register */
__no_init volatile __io union
{
  unsigned char EIE0;
  struct
  {
    unsigned char EX0            : 1;
    unsigned char EX1            : 1;
    unsigned char EX2            : 1;
    unsigned char EX3            : 1;
    unsigned char EX4            : 1;
    unsigned char EX5            : 1;
    unsigned char EX6            : 1;
    unsigned char EX7            : 1;
  } EIE0_bit;
} @ _M(0x0, 0x5);

/* External Interrupt Flag 1 Register */
__no_init volatile __io union
{
  unsigned char EIF1;
  struct
  {
    unsigned char IE8            : 1;
    unsigned char IE9            : 1;
    unsigned char IE10           : 1;
    unsigned char IE11           : 1;
    unsigned char IE12           : 1;
    unsigned char IE13           : 1;
    unsigned char IE14           : 1;
    unsigned char IE15           : 1;
  } EIF1_bit;
} @ _M(0,6);


/* External Interrupt Enable 1 Register  */
__no_init volatile __io union
{
  unsigned char EIE1;
  struct
  {
    unsigned char EX8            : 1;
    unsigned char EX9            : 1;
    unsigned char EX10           : 1;
    unsigned char EX11           : 1;
    unsigned char EX12           : 1;
    unsigned char EX13           : 1;
    unsigned char EX14           : 1;
    unsigned char EX15           : 1;
  } EIE1_bit;
} @ _M(0, 7);

/* Port 0 Input Register */
__no_init volatile __io union
{
  unsigned char PI0;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PI0_bit;
} @ _M(0,0x8);

/* Port 1 Input Register */
__no_init volatile __io union
{
  unsigned char PI1;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PI1_bit;
} @ _M(0,0x9);

/* Port 2 Input Register */
__no_init volatile __io union
{
  unsigned char PI2;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PI2_bit;
} @ _M(0,0xA);

/* Port 3 Input Register */
__no_init volatile __io union
{
  unsigned char PI3;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PI3_bit;
} @ _M(0,0xB);

/* External Interrupt Edge Select 0 Register */
__no_init volatile __io union
{
  unsigned char EIES0;
  struct
  {
    unsigned char IT0            : 1;
    unsigned char IT1            : 1;
    unsigned char IT2            : 1;
    unsigned char IT3            : 1;
    unsigned char IT4            : 1;
    unsigned char IT5            : 1;
    unsigned char IT6            : 1;
    unsigned char IT7            : 1;
  } EIES0_bit;
} @ _M(0,0xC);

/* External Interrupt Edge Select 1 Register */
__no_init volatile __io union
{
  unsigned char EIES1;
  struct
  {
    unsigned char IT8            : 1;
    unsigned char IT9            : 1;
    unsigned char IT10           : 1;
    unsigned char IT11           : 1;
    unsigned char IT12           : 1;
    unsigned char IT13           : 1;
    unsigned char IT14           : 1;
    unsigned char IT15           : 1;
  } EIES1_bit;
} @ _M(0,0xD);

/* Port 0 Direction Register */
__no_init volatile __io union
{
  unsigned char PD0;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PD0_bit;
} @ _M(0,0x10);

/* Port 1 Direction Register */
__no_init volatile __io union
{
  unsigned char PD1;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PD1_bit;
} @ _M(0,0x11);

/* Port 2 Direction Register */
__no_init volatile __io union
{
  unsigned char PD2;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PD2_bit;
} @ _M(0,0x12);

/* Port 3 Direction Register */
__no_init volatile __io union
{
  unsigned char PD3;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PD3_bit;
} @ _M(0,0x13);


/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

/* Port 4 Output Register */
__no_init volatile __io union
{
  unsigned char PO4;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PO4_bit;
} @ _M(1,0);

__no_init volatile __io union
{
  unsigned char WUTC;
  struct
  {
    unsigned char WTE    : 1;
    unsigned char WTF    : 1;
  } WUTC_bit;
} @ _M(1,4);

__no_init volatile __io unsigned short WUT @ _M(1,5);

/* Port 4 Input Register */
__no_init volatile __io union
{
  unsigned char PI4;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PI4_bit;
} @ _M(1,0x8);

__no_init volatile __io union
{
  unsigned short PWCN;
  struct
  {
    unsigned short PFD          :1;
    unsigned short PFIE         :1;
    unsigned short PFI          :1;
    unsigned short REGEN        :1;
    unsigned short IRTXOE       :1;
    unsigned short IRTXOUT      :1;
    unsigned short IRRXWP       :1;
    unsigned short PWRLOSS      :1;
    unsigned short PFRCK        :2;
  } PWCN_bit;
} @ _M(1,0xC);

/* Port 4 Direction Register */
__no_init volatile __io union
{
  unsigned char PD4;
  struct
  {
    unsigned char bit0          : 1;
    unsigned char bit1          : 1;
    unsigned char bit2          : 1;
    unsigned char bit3          : 1;
    unsigned char bit4          : 1;
    unsigned char bit5          : 1;
    unsigned char bit6          : 1;
    unsigned char bit7          : 1;
  } PD4_bit;
} @ _M(1,0x10);

/*-------------------------------*
 *           Module 2            *
 *-------------------------------*/

__no_init volatile __io unsigned short TB0R @ _M(2,0);

__no_init volatile __io union
{
  unsigned short TB0CN;
  struct
  {
    unsigned short CP_RLB       :1;
    unsigned short ETB          :1;
    unsigned short TRB          :1;
    unsigned short EXENB        :1;
    unsigned short DCEN         :1;
    unsigned short TBOE         :1;
    unsigned short EXFB         :1;
    unsigned short TFB          :1;
    unsigned short TBPS         :3;
    unsigned short TBCR         :1;
    unsigned short TBCS         :1;
    unsigned short              :2;
    unsigned short C_TB         :1;
  } TB0CN_bit;
} @ _M(2,1);

__no_init volatile __io unsigned short TB1R @ _M(2,2);

__no_init volatile __io union
{
  unsigned short TB1CN;
  struct
  {
    unsigned short CP_RLB       :1;
    unsigned short ETB          :1;
    unsigned short TRB          :1;
    unsigned short EXENB        :1;
    unsigned short DCEN         :1;
    unsigned short TBOE         :1;
    unsigned short EXFB         :1;
    unsigned short TFB          :1;
    unsigned short TBPS         :3;
    unsigned short TBCR         :1;
    unsigned short TBCS         :1;
    unsigned short C_TB         :1;
  } TB1CN_bit;
} @ _M(2,3);

__no_init volatile __io union
{
  unsigned short IRCN;
  struct
  {
    unsigned short IREN       : 1;
    unsigned short IRMODE     : 1;
    unsigned short IRTXPOL    : 1;
    unsigned short IRDATA     : 1;
    unsigned short IRRXSEL    : 2;
    unsigned short IRCFME     : 1;
    unsigned short IRXRL      : 1;
    unsigned short IRENV      : 2;
    unsigned short IRDIV      : 3;
  } IRCN_bit;
} @ _M(2,4);

__no_init volatile __io union
{
  unsigned short IRCNB;
  struct
  {
    unsigned short IROV       : 1;
    unsigned short IRIF       : 1;
    unsigned short IRIE       : 1;
    unsigned short RXBCNT     : 1;
  } IRCNB_bit;
} @ _M(2,7);

__no_init volatile __io union
{
  unsigned short IRCA;
  struct
  {
    unsigned short IRCAL        : 8;
    unsigned short IRCAH        : 8;
  } IRCA_bit;
} @ _M(2,5);

__no_init volatile __io unsigned short IRMT @ _M(2,6);

__no_init volatile __io unsigned short TB0C @ _M(2,8);
__no_init volatile __io unsigned short TB0V @ _M(2,9);
__no_init volatile __io unsigned short TB1C @ _M(2,10);
__no_init volatile __io unsigned short TB1V @ _M(2,11);
__no_init volatile __io unsigned short IRV @ _M(2,12);

/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

/* Serial Port Control Register */
__no_init volatile __io union
{
  unsigned char SCON0;
  struct
  {
    unsigned char RI             : 1;
    unsigned char TI             : 1;
    unsigned char RB8            : 1;
    unsigned char TB8            : 1;
    unsigned char REN            : 1;
    unsigned char SM2            : 1;
    unsigned char SM1            : 1;
    unsigned char SM0FE          : 1;
  } SCON0_bit;
} @ _M(3,0);

__no_init volatile __io unsigned char SBUF0 @ _M(3,1);   /* Serial Data Buffer */

/* Serial Port Control Register */
__no_init volatile __io union
{
  unsigned char SCON1;
  struct
  {
    unsigned char RI             : 1;
    unsigned char TI             : 1;
    unsigned char RB8            : 1;
    unsigned char TB8            : 1;
    unsigned char REN            : 1;
    unsigned char SM2            : 1;
    unsigned char SM1            : 1;
    unsigned char SM0FE          : 1;
  } SCON1_bit;
} @ _M(3,2);

__no_init volatile __io unsigned char SBUF1 @ _M(3,3);   /* Serial Data Buffer */

/* SPI Data Buffer Register */
__no_init volatile __io unsigned short SPIB @ _M(3,4);

/* SPI Control Register */
__no_init volatile __io union
{
  unsigned char SPICN;
  struct
  {
    unsigned char SPIEN          : 1;
    unsigned char MSTM           : 1;
    unsigned char MODFE          : 1;
    unsigned char MODF           : 1;
    unsigned char WCOL           : 1;
    unsigned char ROVR           : 1;
    unsigned char SPIC           : 1;
    unsigned char STBY           : 1;
  } SPICN_bit;
} @ _M(3,5);

__no_init volatile __io unsigned short PR0 @ _M(3,8);   /* Phase Register */

/* Serial Port Mode Register */
__no_init volatile __io union
{
  unsigned char SMD0;
  struct
  {
    unsigned char FEDE           : 1;
    unsigned char SMOD           : 1;
    unsigned char ESI            : 1;
  } SMD0_bit;
} @ _M(3,9);

__no_init volatile __io unsigned short PR1 @ _M(3,10);   /* Phase Register */

/* Serial Port Mode Register */
__no_init volatile __io union
{
  unsigned char SMD1;
  struct
  {
    unsigned char FEDE           : 1;
    unsigned char SMOD           : 1;
    unsigned char ESI            : 1;
  } SMD1_bit;
} @ _M(3,11);

/* SPI Configuration Register */
__no_init volatile __io union
{
  unsigned char SPICF;
  struct
  {
    unsigned char CKPOL          : 1;
    unsigned char CKPHA          : 1;
    unsigned char CHR            : 1;
    unsigned char                : 3;
    unsigned char SAS            : 1;
    unsigned char ESPII          : 1;
  } SPICF_bit;
} @ _M(3,0xC);

__no_init volatile __io unsigned char SPICK @ _M(3,0xD);   /* SPI Clock Register */

/* Clock Control register */
__no_init volatile __io union
{
  unsigned char CKCN;
  struct
  {
    unsigned char CD0            : 1;
    unsigned char CD1            : 1;
    unsigned char PMME           : 1;
    unsigned char SWB            : 1;
    unsigned char STOP           : 1;
    unsigned char RGMD           : 1;
    unsigned char RGSL           : 1;
    unsigned char XTRC           : 1;
  } CKCN_bit;
} @ _M(8,0x0E);

__no_init volatile __io union
{
  unsigned char WDCN;
  struct
  {
    unsigned char RWT       : 1;
    unsigned char EWT       : 1;
    unsigned char WTRF      : 1;
    unsigned char WDIF      : 1;
    unsigned char WD0       : 1;
    unsigned char WD1       : 1;
    unsigned char EWDI      : 1;
    unsigned char POR       : 1;
  } WDCN_bit;
} @ _M(8, 0xF);

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ61x */


