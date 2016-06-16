/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ2010.
 *
 * This file supports assembler and C development for MAXQ200X
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * >> Modified for MaxQ2010 by Dallas/Maxim <<
 *
 * $Revision: 1167 $  Added bit structures for all Port Input,
 *                   Output and Direction registers
 *
 ********************************************************************/


#ifndef __IOMAXQ2010
#define __IOMAXQ2010

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x42)     /* 0x42 = 66 dec */
#error iomaxqxx.h file for use with ICCMAXQ/AMAXQ only
#endif


#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended

#include "iomaxq.h"


/*-------------------------------------------------------------------------
 *   Special Function Registers
 *-------------------------------------------------------------------------*/

/*-------------------------------*
 *           Module 0            *
 *-------------------------------*/

/* Port 0 Output Register */
__no_init volatile __io union
{
  unsigned char PO0;  /* Yeah, that is an O followed by a ZERO */
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
} @ _M(0, 0x04);


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
    unsigned char IT0            : 1;
    unsigned char IT1            : 1;
  } EIE0_bit;
} @ _M(0, 0x05);

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
} @ _M(0, 0x0C);


/* Power Control Register */
__no_init volatile __io union
{
  unsigned short PWCN;
  struct
  {
    unsigned char FLLEN          : 1;
    unsigned char FLOCK          : 1;
    unsigned char X32D           : 1;
    unsigned char X32KRDY        : 1;
    unsigned char HFXD           : 1;
    unsigned char X32KBYP        : 1;
    unsigned char REGEN          : 1;
    unsigned char BOD            : 1;
    unsigned char X32KMD         : 2;
    unsigned char                : 6;
  } PWCN_bit;
} @ _M(0, 0x0F);

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



__no_init volatile __io unsigned char RTRM @ _M(0, 0x18);   /* RTC Trim Register         */

/* Real Time Clock Control Register */
__no_init volatile __io union
{
  unsigned short RCNT;
  struct
  {
    unsigned char RTCE           : 1;
    unsigned char ADE            : 1;
    unsigned char ASE            : 1;
    unsigned char BUSY           : 1;
    unsigned char RDY            : 1;
    unsigned char RDYE           : 1;
    unsigned char ALDF           : 1;
    unsigned char ALSF           : 1;
    unsigned char SQE            : 1;
    unsigned char FT             : 1;
    unsigned char                : 3;
    unsigned char ACS            : 1;
    unsigned char                : 1;
    unsigned char WE             : 1;
  } RCNT_bit;
} @ _M(0, 0x19);

__no_init volatile __io unsigned char  RTSS @ _M(0, 0x1A);  /* RTC Subsecond Counter       */
__no_init volatile __io unsigned short RTSH @ _M(0, 0x1B);  /* RTC Second Counter High     */
__no_init volatile __io unsigned short RTSL @ _M(0, 0x1C);  /* RTC Second Counter Low      */
__no_init volatile __io unsigned char  RSSA @ _M(0, 0x1D);  /* RTC Subsecond Alarm         */
__no_init volatile __io unsigned char  RASH @ _M(0, 0x1E);  /* RTC Time of Day Alarm High  */
__no_init volatile __io unsigned short RASL @ _M(0, 0x1F);  /* RTC Time of Day Alarm Low   */


/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

__no_init volatile __io unsigned short SPIB @ _M(1, 0x03);      /* SPI Data Buffer Register */

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

/* Port 5 Direction Register */
__no_init volatile __io union
{
  unsigned char PD5;
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
  } PD5_bit;
} @ _M(1,0x11);

/* Port 6 Direction Register */
__no_init volatile __io union
{
  unsigned char PD6;
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
  } PD6_bit;
} @ _M(1,0x12);

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

/* Port 5 Output Register */
__no_init volatile __io union
{
  unsigned char PO5;
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
  } PO5_bit;
} @ _M(1,1);

/* Port 6 Output Register */
__no_init volatile __io union
{
  unsigned char PO6;
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
  } PO6_bit;
} @ _M(1,2);

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
  } EIF1_bit;
} @ _M(1, 0x04);


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
  } EIE1_bit;
} @ _M(1, 0x05);


/* External Interrupt Flag 2 Register */
__no_init volatile __io union
{
  unsigned char EIF2;
  struct
  {
    unsigned char IE15           : 1;
    unsigned char IE16           : 1;
    unsigned char IE17           : 1;
    unsigned char IE18           : 1;
    unsigned char IE19           : 1;
    unsigned char IE20           : 1;
    unsigned char IE21           : 1;
    unsigned char IE22           : 1;
  } EIF2_bit;
} @ _M(1, 0x06);


/* External Interrupt Enable 2 Register  */
__no_init volatile __io union
{
  unsigned char EIE2;
  struct
  {
    unsigned char EX15           : 1;
    unsigned char EX16           : 1;
    unsigned char EX17           : 1;
    unsigned char EX18           : 1;
    unsigned char EX19           : 1;
    unsigned char EX20           : 1;
    unsigned char EX21           : 1;
    unsigned char EX22           : 1;
  } EIE2_bit;
} @ _M(1, 0x07);

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

/* Port 5 Input Register */
__no_init volatile __io union
{
  unsigned char PI5;
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
  } PI5_bit;
} @ _M(1,0x9);

/* Port 6 Input Register */
__no_init volatile __io union
{
  unsigned char PI6;
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
  } PI6_bit;
} @ _M(1,0x0A);

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
  } EIES1_bit;
} @ _M(1, 0x0B);


/* External Interrupt Edge Select 2 Register */
__no_init volatile __io union
{
  unsigned char EIES2;
  struct
  {
    unsigned char IT15           : 1;
    unsigned char IT16           : 1;
    unsigned char IT17           : 1;
    unsigned char IT18           : 1;
    unsigned char IT19           : 1;
    unsigned char IT20           : 1;
    unsigned char IT21           : 1;
    unsigned char IT22           : 1;
  } EIES2_bit;
} @ _M(1, 0x0C);


/* Supply Voltage Monitor Register */
__no_init volatile __io union
{
  unsigned short SVM;
  struct
  {
    unsigned char SVMEN          : 1;
    unsigned char SVMRDY         : 1;
    unsigned char SVMIE          : 1;
    unsigned char SVMI           : 1;
    unsigned char SVMSTOP        : 1;
    unsigned char                : 3;
    unsigned char SVTH           : 4;
    unsigned char                : 4;
  } SVM_bit;
} @ _M(1, 0x0D);

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
} @ _M(1, 0x15);


/* SPI Configuration Register */
__no_init volatile __io union
{
  unsigned char SPICF;
  struct
  {
    unsigned char CKPOL          : 1;
    unsigned char CKPHA          : 1;
    unsigned char CHR            : 1;
    unsigned char                : 4;
    unsigned char ESPII          : 1;
  } SPICF_bit;
} @ _M(1, 0x16);


__no_init volatile __io unsigned char SPICK @ _M(1, 0x17);   /* SPI Clock Register */


/*-------------------------------*
 *           Module 2            *
 *-------------------------------*/

/* Multiplier Control Register */
__no_init volatile __io union
{
  unsigned char MCNT;
  struct
  {
    unsigned char SUS            : 1;
    unsigned char MMAC           : 1;
    unsigned char CLD            : 1;
    unsigned char OPCS           : 1;
    unsigned char SQU            : 1;
    unsigned char                : 2;
    unsigned char OF             : 1;
  } MCNT_bit;
} @ _M(2, 0x00);


__no_init volatile __io unsigned short MA  @ _M(2, 0x01);   /* Multiplier Operand A Register    */
__no_init volatile __io unsigned short MB  @ _M(2, 0x02);   /* Multiplier Operand B Register    */
__no_init volatile __io unsigned short MC2 @ _M(2, 0x03);   /* Multiplier Accumulate Register 2 */
__no_init volatile __io unsigned short MC1 @ _M(2, 0x04);   /* Multiplier Accumulate Register 1 */
__no_init volatile __io unsigned short MC0 @ _M(2, 0x05);   /* Multiplier Accumulate Register 0 */


/* LCD Configuration register */
__no_init volatile __io union
{
  unsigned char LCFG;
  struct
  {
    unsigned char DPE            : 1;
    unsigned char OPM            : 1;
    unsigned char SMO            : 1;
    unsigned char PCF            : 5;
  } LCFG_bit;
} @ _M(2, 0x6);


__no_init volatile __io unsigned short MC1R @ _M(2, 0x08);   /* Multiplier Read Register 1 */
__no_init volatile __io unsigned short MC0R @ _M(2, 0x09);   /* Multiplier Read Register 0 */


/* LCD Adjust Register */
__no_init volatile __io union
{
  unsigned short LCRA;
  struct
  {
    unsigned short LRA            : 4;
    unsigned short                : 1;
    unsigned short LRIG           : 1;
    unsigned short LCCS           : 1;
    unsigned short FRM            : 4;
    unsigned short DUTY           : 2;
  } LCRA_bit;
} @ _M(2, 0x0A);


/* LCD Display registers */

__no_init volatile __io unsigned char LCD0   @ _M(2, 0x0B);
__no_init volatile __io unsigned char LCD1   @ _M(2, 0x0C);
__no_init volatile __io unsigned char LCD2   @ _M(2, 0x0D);
__no_init volatile __io unsigned char LCD3   @ _M(2, 0x0E);
__no_init volatile __io unsigned char LCD4   @ _M(2, 0x0F);
__no_init volatile __io unsigned char LCD5   @ _M(2, 0x10);
__no_init volatile __io unsigned char LCD6   @ _M(2, 0x11);
__no_init volatile __io unsigned char LCD7   @ _M(2, 0x12);
__no_init volatile __io unsigned char LCD8   @ _M(2, 0x13);
__no_init volatile __io unsigned char LCD9   @ _M(2, 0x14);
__no_init volatile __io unsigned char LCD10  @ _M(2, 0x15);
__no_init volatile __io unsigned char LCD11  @ _M(2, 0x16);
__no_init volatile __io unsigned char LCD12  @ _M(2, 0x17);
__no_init volatile __io unsigned char LCD13  @ _M(2, 0x18);
__no_init volatile __io unsigned char LCD14  @ _M(2, 0x19);
__no_init volatile __io unsigned char LCD15  @ _M(2, 0x1A);
__no_init volatile __io unsigned char LCD16  @ _M(2, 0x1B);
__no_init volatile __io unsigned char LCD17  @ _M(2, 0x1C);
__no_init volatile __io unsigned char LCD18  @ _M(2, 0x1D);
__no_init volatile __io unsigned char LCD19  @ _M(2, 0x1E);
__no_init volatile __io unsigned char LCD20  @ _M(2, 0x1F);


/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

__no_init volatile __io unsigned short I2CBUF @ _M(3, 0x00);


/* I2C Status Register */
__no_init volatile __io union
{
  unsigned short I2CST;
  struct
  {
    unsigned char I2CSRI         : 1;
    unsigned char I2CTXI         : 1;
    unsigned char I2CRXI         : 1;
    unsigned char I2CSTRI        : 1;
    unsigned char I2CTOI         : 1;
    unsigned char I2CAMI         : 1;
    unsigned char I2CALI         : 1;
    unsigned char I2CNACKI       : 1;
    unsigned char I2CGCI         : 1;
    unsigned char I2CROI         : 1;
    unsigned char I2CSCL         : 1;
    unsigned char I2CSPI         : 1;
    unsigned char                : 2;
    unsigned char I2CBUSY        : 1;
    unsigned char I2CBUS         : 1;
  } I2CST_bit;
} @ _M(3, 0x01);


/* I2C Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned short I2CIE;
  struct
  {
    unsigned char I2CSRIE        : 1;
    unsigned char I2CTXIE        : 1;
    unsigned char I2CRXIE        : 1;
    unsigned char I2CSTRIE       : 1;
    unsigned char I2CTOIE        : 1;
    unsigned char I2CAMIE        : 1;
    unsigned char I2CALIE        : 1;
    unsigned char I2CNACKIE      : 1;
    unsigned char I2CGCIE        : 1;
    unsigned char I2CROIE        : 1;
    unsigned char I2CSTREN       : 1;
    unsigned char I2CSPIE        : 1;
  } I2CIE_bit;
} @ _M(3, 0x02);


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
} @ _M(3, 0x04);

__no_init volatile __io unsigned char SBUF0 @ _M(3, 0x05);   /* Serial Data Buffer */


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
} @ _M(3, 0x06);

__no_init volatile __io unsigned char SBUF1 @ _M(3, 0x07);   /* Serial Data Buffer */


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
} @ _M(3, 0x08);

__no_init volatile __io unsigned short PR0 @ _M(3, 0x09);   /* Phase Register */


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
} @ _M(3, 0x0A);

__no_init volatile __io unsigned short PR1 @ _M(3, 0x0B);   /* Phase Register */


/* I2C Control Register */
__no_init volatile __io union
{
  unsigned char I2CCN;
  struct
  {
    unsigned char I2CEN          : 1;
    unsigned char I2CMST         : 1;
    unsigned char I2CMODE        : 1;
    unsigned char I2CEA          : 1;
    unsigned char I2CSTRS        : 1;
    unsigned char I2CACK         : 1;
    unsigned char I2CSTART       : 1;
    unsigned char I2CSTOP        : 1;
  } I2CCN_bit;
} @ _M(3, 0x0C);


/* I2C Clock Register */
__no_init volatile __io union
{
  unsigned short I2CCK;
  struct
  {
    unsigned char I2CCKL         : 8;
    unsigned char I2CCKH         : 8;
  } I2CCK_bit;
} @ _M(3, 0x0D);

__no_init volatile __io unsigned short I2CTO  @ _M(3, 0x0E);   /* I2C Timeout Register       */
__no_init volatile __io unsigned short I2CSLA @ _M(3, 0x0F);   /* I2C Slave Address Register */


/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/

__no_init volatile __io unsigned short TB0R  @ _M(4, 0x00);    /* Timer B 0 Reload Register   */
__no_init volatile __io unsigned short TB0C  @ _M(4, 0x01);    /* Timer B 0 Capture Register  */
__no_init volatile __io unsigned short TB1R  @ _M(4, 0x02);    /* Timer B 1 Reload Register   */
__no_init volatile __io unsigned short TB1C  @ _M(4, 0x03);    /* Timer B 1 Capture Register  */
__no_init volatile __io unsigned short TB2R  @ _M(4, 0x04);    /* Timer B 2 Reload Register   */
__no_init volatile __io unsigned short TB2C  @ _M(4, 0x05);    /* Timer B 2 Capture Register  */


/* ADC Status Register */
__no_init volatile __io union
{
  unsigned short ADST;
  struct
  {
    unsigned char ADIDX          : 4;
    unsigned char ADCFG          : 1;
    unsigned char ADDAI          : 1;
    unsigned char ADCONV         : 1;
    unsigned char REFOK          : 1;
    unsigned char ADDAT          : 4;
  } ADST_bit;
} @ _M(4, 0x06);


/* ADC Address Register */
__no_init volatile __io union
{
  unsigned short ADADDR;
  struct
  {
    unsigned char SEQEND         : 3;
    unsigned char                : 1;
    unsigned char SEQSTART       : 3;
    unsigned char                : 1;
    unsigned char SEQSTORE       : 4;
  } ADADDR_bit;
} @ _M(4, 0x07);


/* Timer B 0 Control Register */
__no_init volatile __io union
{
  unsigned short TB0CN;
  struct
  {
    unsigned char CPnRLB         : 1;
    unsigned char ETB            : 1;
    unsigned char TRB            : 1;
    unsigned char EXENB          : 1;
    unsigned char DCEN           : 1;
    unsigned char TBOE           : 1;
    unsigned char EXFB           : 1;
    unsigned char TFB            : 1;
    unsigned char TBPS           : 3;
    unsigned char TBCR           : 1;
    unsigned char TBCS           : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;
  } TB0CN_bit;
} @ _M(4, 0x08);

__no_init volatile __io unsigned short TB0V  @ _M(4, 0x09);    /* Timer B 0 Value Register   */


/* Timer B 1 Control Register */
__no_init volatile __io union
{
  unsigned short TB1CN;
  struct
  {
    unsigned char CPnRLB         : 1;
    unsigned char ETB            : 1;
    unsigned char TRB            : 1;
    unsigned char EXENB          : 1;
    unsigned char DCEN           : 1;
    unsigned char TBOE           : 1;
    unsigned char EXFB           : 1;
    unsigned char TFB            : 1;
    unsigned char TBPS           : 3;
    unsigned char TBCR           : 1;
    unsigned char TBCS           : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;
  } TB1CN_bit;
} @ _M(4, 0x0A);

__no_init volatile __io unsigned short TB1V  @ _M(4, 0x0B);    /* Timer B 1 Value Register   */


/* Timer B 2 Control Register */
__no_init volatile __io union
{
  unsigned short TB2CN;
  struct
  {
    unsigned char CPnRLB         : 1;
    unsigned char ETB            : 1;
    unsigned char TRB            : 1;
    unsigned char EXENB          : 1;
    unsigned char DCEN           : 1;
    unsigned char TBOE           : 1;
    unsigned char EXFB           : 1;
    unsigned char TFB            : 1;
    unsigned char TBPS           : 3;
    unsigned char TBCR           : 1;
    unsigned char TBCS           : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;
  } TB2CN_bit;
} @ _M(4, 0x0C);

__no_init volatile __io unsigned short TB2V  @ _M(4, 0x0D);    /* Timer B 2 Value Register   */



/* ADC Control Register */
__no_init volatile __io union
{
  unsigned short ADCN;
  struct
  {
    unsigned char ADACQ          : 4;
    unsigned char ADPMO          : 1;
    unsigned char ADDAIE         : 1;
    unsigned char ADCONT         : 1;
    unsigned char IREFEN         : 1;
    unsigned char ADCLK          : 2;
    unsigned char ADINT          : 2;
  } ADCN_bit;
} @ _M(4, 0x0E);

__no_init volatile __io unsigned short ADDATA  @ _M(4, 0x0F);    /* ADC Sample Data Register   */



/*-------------------------------*
 *           Module 8            *
 *-------------------------------*/

/* Only CKCN and WDCN are defined in this file, the rest of the */
/* registers in Module 8 can be found in iomaxq.h.              */

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
    unsigned char FLLMD          : 1;
    unsigned char FLLSL          : 1;
  } CKCN_bit;
} @ _M(8, 0x0E);


/* Watchdog Control register */
__no_init volatile __io union
{
  unsigned char WDCN;
  struct
  {
    unsigned char RWT            : 1;
    unsigned char EWT            : 1;
    unsigned char WTRF           : 1;
    unsigned char WDIF           : 1;
    unsigned char WD0            : 1;
    unsigned char WD1            : 1;
    unsigned char EWDI           : 1;
    unsigned char POR            : 1;
  } WDCN_bit;
} @ _M(8, 0x0F);


#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ2010 */


