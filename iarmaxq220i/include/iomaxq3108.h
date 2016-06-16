/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ3108.
 *
 * This file supports assembler and C development for MAXQ3108
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1241 $
 *
 ********************************************************************/


#ifndef __IOMAXQ3108
#define __IOMAXQ3108

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

__no_init volatile __io unsigned short AD0 @ _M(0,0);   /* A/D Converter 0 Output */
__no_init volatile __io unsigned short AD1 @ _M(0,1);   /* A/D Converter 1 Output */
__no_init volatile __io unsigned short AD2 @ _M(0,2);   /* A/D Converter 2 Output */
__no_init volatile __io unsigned short AD3 @ _M(0,3);   /* A/D Converter 3 Output */
__no_init volatile __io unsigned short AD4 @ _M(0,4);   /* A/D Converter 4 Output */
__no_init volatile __io unsigned short AD5 @ _M(0,5);   /* A/D Converter 5 Output */


/* Slave Response Register 0 */
__no_init volatile __io union
{
  unsigned char SRSP0;
  struct
  {
    unsigned char RSPST          : 4;
    unsigned char REQE           : 1;
    unsigned char RSPSDV         : 1;
  } SRSP0_bit
} @ _M(0,6);

__no_init volatile __io unsigned short SRSP1 @ _M(0,7);   /* Slave Response Register 1 */

__no_init volatile __io unsigned char AD0LSB @ _M(0,8);   /* A/D Converter 0 Output LSB */
__no_init volatile __io unsigned char AD1LSB @ _M(0,9);   /* A/D Converter 1 Output LSB */
__no_init volatile __io unsigned char AD2LSB @ _M(0,10);   /* A/D Converter 2 Output LSB */
__no_init volatile __io unsigned char AD3LSB @ _M(0,11);   /* A/D Converter 3 Output LSB */
__no_init volatile __io unsigned char AD4LSB @ _M(0,12);   /* A/D Converter 4 Output LSB */
__no_init volatile __io unsigned char AD5LSB @ _M(0,13);   /* A/D Converter 5 Output LSB */

/* Master Request Register 0 */
__no_init volatile __io union
{
  unsigned char MREQ0;
  struct
  {
    unsigned char REQCM          : 4;
    unsigned char RSPIE          : 1;
    unsigned char REQCDV         : 1;
  } MREQ0_bit
} @ _M(0,14);

__no_init volatile __io unsigned short MREQ1 @ _M(0,15);   /* Master Request Register 1 */

__no_init volatile __io unsigned short MREQ2 @ _M(0,16);   /* Master Request Register 2 */

/* ADC Control Register */
__no_init volatile __io union
{
  unsigned short ADCN;
  struct
  {
    unsigned short ABF0           : 1;
    unsigned short ABF1           : 1;
    unsigned short ABF2           : 1;
    unsigned short ABF3           : 1;
    unsigned short ABF4           : 1;
    unsigned short ABF5           : 1;
    unsigned short OSR            : 2;
    unsigned short MD0E           : 1;
    unsigned short MD1E           : 1;
    unsigned short MD2E           : 1;
    unsigned short MDCKS          : 1;
    unsigned short IF10E          : 1;
    unsigned short IF32E          : 1;
    unsigned short IF54E          : 1;
    unsigned short IFCSEL         : 1;
  } ADCN_bit
} @ _M(0,17);

__no_init volatile __io unsigned short ADCC @ _M(0,18);   /* ADC Clock Correction */

/* Manchester Decoder Status */
__no_init volatile __io union
{
  unsigned char MSTC;
  struct
  {
    unsigned char MD0SNC         : 1;
    unsigned char MD1SNC         : 1;
    unsigned char MD2SNC         : 1;
    unsigned char                : 1;
    unsigned char CCSL           : 2;
  } MSTC_bit
} @ _M(0,19);   

/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

__no_init volatile __io union
{
  unsigned char PO0;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PO0_bit;
} @ _M(1,0);   /* Port 0 Output Register */
__no_init volatile __io union
{
  unsigned char PO1;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PO1_bit;
} @ _M(1,1);   /* Port 1 Output Register */
__no_init const volatile __io union
{
  unsigned char PI0;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PI0_bit;
} @ _M(1,2);  /* Port 0 Input Register */
__no_init const volatile __io union
{
  unsigned char PI1;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PI1_bit;
} @ _M(1,3);  /* Port 1 Input Register */

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
} @ _M(1,4);

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
} @ _M(1,5);

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
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } EIF1_bit;
} @ _M(1,6);

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
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } EIE1_bit;
} @ _M(1, 7);

__no_init volatile __io union
{
  unsigned char PD0;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PD0_bit;
} @ _M(1,8);   /* Port 0 Direction Register */

__no_init volatile __io union
{
  unsigned char PD1;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PD1_bit;
} @ _M(1,9);   /* Port 1 Direction Register */

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
} @ _M(1,10);

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
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } EIES1_bit;
} @ _M(1,11);

/* Supply Voltage Monitor Register */
__no_init volatile __io union
{
  unsigned char SVM;
  struct
  {
    unsigned short SVMEN          : 1;
    unsigned short SVMRDY         : 1;
    unsigned short SVMIE          : 1;
    unsigned short SVMI           : 1;
    unsigned short SVMSTOP        : 1;
    unsigned short                : 3;
    unsigned short SVTH           : 4;
  } SVM_bit
} @ _M(1,12);

/* Power Control Register */
__no_init volatile __io union
{
  unsigned short PWCN;
  struct
  {
    unsigned short FLLEN          : 1;
    unsigned short FLOCK          : 1;
    unsigned short ECLKO          : 1;
    unsigned short                : 1;
    unsigned short HFXD           : 1;
    unsigned short RSTD           : 1;
    unsigned short REGEN          : 1;
    unsigned short BOD            : 1;
    unsigned short                : 2;
    unsigned short ENDSP          : 1;
  } PWCN_bit
} @ _M(1,15);

__no_init volatile __io unsigned short BB0 @ _M(1,16);   /* Battery Backed Register 0 */
__no_init volatile __io unsigned short BB1 @ _M(1,17);   /* Battery Backed Register 1 */
__no_init volatile __io unsigned short BB2 @ _M(1,18);   /* Battery Backed Register 2 */
__no_init volatile __io unsigned short BB3 @ _M(1,19);   /* Battery Backed Register 3 */
__no_init volatile __io unsigned short BB4 @ _M(1,20);   /* Battery Backed Register 4 */
__no_init volatile __io unsigned short BB5 @ _M(1,21);   /* Battery Backed Register 5 */
__no_init volatile __io unsigned short BB6 @ _M(1,22);   /* Battery Backed Register 6 */
__no_init volatile __io unsigned short BB7 @ _M(1,23);   /* Battery Backed Register 7 */

/* RTC Trim Register */
__no_init volatile __io union
{
  unsigned char RTRM;
  struct
  {
    unsigned char TRM   : 6;
    unsigned char TSGN  : 1;
  } RTRM_bit;
} @ _M(1,24);

/* Real Time Clock Control Register */
__no_init volatile __io union
{
  unsigned short RCNT;
  struct
  {
    unsigned short RTCE           : 1;
    unsigned short ADE            : 1;
    unsigned short ASE            : 1;
    unsigned short BUSY           : 1;
    unsigned short RDY            : 1;
    unsigned short RDYE           : 1;
    unsigned short ALDF           : 1;
    unsigned short ALSF           : 1;
    unsigned short SQE            : 1;
    unsigned short FT             : 1;
    unsigned short X32KMD         : 2;
    unsigned short X32KBYP        : 1;
    unsigned short X32KRDY        : 1;
    unsigned short X32D           : 1;
    unsigned short WE             : 1;
  } RCNT_bit;
} @ _M(1,25);

__no_init volatile __io unsigned char  RTSS @ _M(1,26); /* RT Clock Subsecond Counter  */
__no_init volatile __io unsigned short RTSH @ _M(1,27); /* RT Clock Second Counter High*/
__no_init volatile __io unsigned short RTSL @ _M(1,28); /* RT Clock Second Counter Low */
__no_init volatile __io unsigned char  RSSA @ _M(1,29); /* RT Clock Subsecond Alarm    */
__no_init volatile __io unsigned short RASH @ _M(1,30); /* RT Clock T-O-D Alarm High   */
__no_init volatile __io unsigned short RASL @ _M(1,31); /* RT Clock T-O-D Alarm Low   */

/*-------------------------------*
 *           Module 2            *
 *-------------------------------*/

/* Timer 0: Type 2 Control Register A */
__no_init volatile __io union
{
  unsigned char T2CNA;
  struct
  {
    unsigned char G2EN           : 1;
    unsigned char SS2            : 1;
    unsigned char CPRL2          : 1;
    unsigned char TR2            : 1;
    unsigned char TR2L           : 1;
    unsigned char T2POL0         : 1;
    unsigned char T2OE0          : 1;
    unsigned char ET2            : 1;
  } T2CNA_bit;
} @ _M(2,0);

__no_init volatile __io unsigned char T2H0 @ _M(2,1);   /* Timer 0: Type 2 Most Significant Byte Value */
__no_init volatile __io unsigned char T2RH0 @ _M(2,2);   /* Timer 0: Type 2 Most Significant Byte Reload */
__no_init volatile __io unsigned char T2CH0 @ _M(2,3);   /* Timer 0: Type 2 Most Significant Byte Capture/Compare */

__no_init volatile __io union
{
  unsigned char PO2;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PO2_bit;
} @ _M(2,4);   /* Port 2 Output Register */
__no_init const volatile __io union
{
  unsigned char PI2;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PI2_bit;
} @ _M(2,5);  /* Port 2 Input Register */

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
} @ _M(2,6);

__no_init volatile __io unsigned char SBUF0 @ _M(2,7);   /* Serial Data Buffer */

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
} @ _M(2,8);

__no_init volatile __io unsigned short PR0 @ _M(2,9);   /* Phase Register */

__no_init volatile __io union
{
  unsigned char PD2;
  struct
  {
    unsigned char bit0           : 1;
    unsigned char bit1           : 1;
    unsigned char bit2           : 1;
    unsigned char bit3           : 1;
    unsigned char bit4           : 1;
    unsigned char bit5           : 1;
    unsigned char bit6           : 1;
    unsigned char bit7           : 1;
  } PD2_bit;
} @ _M(2,10);   /* Port 2 Direction Register */

/* Timer 0: Type 2 Control Register B */
__no_init volatile __io union
{
  unsigned char T2CNB;
  struct
  {
    unsigned char TC2L           : 1;
    unsigned char TCC2           : 1;
    unsigned char TF2L           : 1;
    unsigned char TF2            : 1;
    unsigned char                : 1;
    unsigned char T2POL1         : 1;
    unsigned char T2OE1          : 1;
    unsigned char ET2L           : 1;
  } T2CNB_bit;
} @ _M(2,11);

__no_init volatile __io unsigned short T2V @ _M(2,12);   /* Timer 0: Type 2 Value Register */
__no_init volatile __io unsigned short T2R @ _M(2,13);   /* Timer 0: Type 2 Reload Register */
__no_init volatile __io unsigned short T2C @ _M(2,14);   /* Timer 0: Type 2 Capture/Compare Register */

/* Timer 0: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG;
  struct
  {
    unsigned char CT             : 1;
    unsigned char CCF            : 2;
    unsigned char T2MD           : 1;
    unsigned char T2DIV          : 3;
    unsigned char T2CI           : 1;
  } T2CFG_bit;
} @ _M(2,15);

/* Test Mode 2 Register */
__no_init volatile __io union
{
  unsigned char TM2;
  struct
  {
    unsigned char VBGO           : 1;
    unsigned char AFDM           : 1;
    unsigned char LDOOFF         : 1;
    unsigned char MDTOE          : 1;
    unsigned char XTAL           : 2;
  } TM2_bit;
} @ _M(2,23);

__no_init volatile __io unsigned short ICDT0 @ _M(2,24);   /* In-circuit Debug Temp 0 Register */
__no_init volatile __io unsigned short ICDT1 @ _M(2,25);   /* In-circuit Debug Temp 1 Register */

/* In-circuit Debug Control Register */
__no_init volatile __io union
{
  unsigned char ICDC;
  struct
  {
    unsigned char CMD0           : 1;
    unsigned char CMD1           : 1;
    unsigned char CMD2           : 1;
    unsigned char CMD3           : 1;
    unsigned char                : 1;
    unsigned char REGE           : 1;
    unsigned char                : 1;
    unsigned char DME            : 1;
  } ICDC_bit;
} @ _M(2,26);


/* In-circuit Debug Flag Register */
__no_init volatile __io union
{
  unsigned char ICDF;
  struct
  {
    unsigned char TXC            : 1;
    unsigned char SPE            : 1;
    unsigned char PSS0           : 1;
    unsigned char PSS1           : 1;
  } ICDF_bit;
} @ _M(2,27);

__no_init volatile __io unsigned char ICDB @ _M(2,28);   /* In-circuit Debug Buffer Register */
__no_init volatile __io unsigned char ICDA @ _M(2,29);   /* In-circuit Debug Address Register */
__no_init volatile __io unsigned char ICDD @ _M(2,30);   /* In-circuit Debug Data Register */

/* Test Mode Register */
__no_init volatile __io union
{
  unsigned short TM;
  struct
  {
    unsigned short TME           : 1;
    unsigned short CKT           : 1;
    unsigned short SRT           : 1;
    unsigned short ROUT          : 1;
    unsigned short DOFF          : 1;
    unsigned short IDDQ          : 1;
    unsigned short DCW           : 1;
    unsigned short COUT          : 1;
  } TM_bit;
} @ _M(2,31);

/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

/* Multiplier Control Register */
__no_init volatile __io union
{
  unsigned char MCNT;
  struct
  {
    unsigned char SUS            : 1;
    unsigned char MMAC           : 1;
    unsigned char MSUB           : 1;
    unsigned char OPCS           : 1;
    unsigned char SQU            : 1;
    unsigned char CLD            : 1;
    unsigned char MCW            : 1;
    unsigned char OF             : 1;
  } MCNT_bit;
} @ _M(3,0x0);

__no_init volatile __io unsigned short MA  @ _M(3,1); /* Multiplier Operand A Register */
__no_init volatile __io unsigned short MB  @ _M(3,2); /* Multiplier Operand B Register */
__no_init volatile __io unsigned short MC2 @ _M(3,3); /* Multiplier Accumulate Register 2 */
__no_init volatile __io unsigned short MC1 @ _M(3,4); /* Multiplier Accumulate Register 1 */
__no_init volatile __io unsigned short MC0 @ _M(3,5); /* Multiplier Accumulate Register 0 */

__no_init volatile __io unsigned short SPIB @ _M(3,7);   /* SPI Data Buffer Register */

__no_init volatile __io unsigned short MC1R @ _M(3,8); /* Multiplier Read Register 1 */
__no_init volatile __io unsigned short MC0R @ _M(3,9); /* Multiplier Read Register 0 */

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
} @ _M(3,13);


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
} @ _M(3,14);

__no_init volatile __io unsigned char SPICK @ _M(3,15);   /* SPI Clock Register */

/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/

__no_init volatile __io unsigned short I2CBUF @ _M(4,0); /* I2C Buffer Register */

/* I2C Status Register */
__no_init volatile __io union
{
  unsigned short I2CST;
  struct
  {
    unsigned short I2CSRI        : 1;
    unsigned short I2CTXI        : 1;
    unsigned short I2CRXI        : 1;
    unsigned short I2CSTRI       : 1;
    unsigned short I2CTOI        : 1;
    unsigned short I2CAMI        : 1;
    unsigned short I2CALI        : 1;
    unsigned short I2CNACKI      : 1;
    unsigned short I2CGCI        : 1;
    unsigned short I2CROI        : 1;
    unsigned short I2CSCL        : 1;
    unsigned short I2CSPI        : 1;
    unsigned short               : 2;
    unsigned short I2CBUSY       : 1;
    unsigned short I2CBUS        : 1;
  } I2CST_bit;
} @ _M(4,1);

/* I2C Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned short I2CIE;
  struct
  {
    unsigned short I2CSRIE       : 1;
    unsigned short I2CTXIE       : 1;
    unsigned short I2CRXIE       : 1;
    unsigned short I2CSTRIE      : 1;
    unsigned short I2CTOIE       : 1;
    unsigned short I2CAMIE       : 1;
    unsigned short I2CALIE       : 1;
    unsigned short I2CNACKIE     : 1;
    unsigned short I2CGCIE       : 1;
    unsigned short I2CROIE       : 1;
    unsigned short               : 1;
    unsigned short I2CCSPIE      : 1;
  } I2CIE_bit;
} @ _M(4,2);

__no_init volatile __io unsigned short TB0R @ _M(4,4); /* Timer B Reload Register */
__no_init volatile __io unsigned short TB0C @ _M(4,5); /* Timer B Compare Register */

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
} @ _M(4,6);

__no_init volatile __io unsigned char SBUF1 @ _M(4,7);   /* Serial Data Buffer */

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
} @ _M(4,8);

__no_init volatile __io unsigned short PR1 @ _M(4,9);   /* Phase Register */

/* Timer B Control Register */
__no_init volatile __io union
{
  unsigned short TB0CN;
  struct
  {
    unsigned short CPRLB         : 1;
    unsigned short ETB           : 1;
    unsigned short TRB           : 1;
    unsigned short EXENB         : 1;
    unsigned short DCEN          : 1;
    unsigned short TBOE          : 1;
    unsigned short EXFB          : 1;
    unsigned short TFB           : 1;
    unsigned short TBPS          : 3;
    unsigned short TBCR          : 1;
    unsigned short TBCS          : 1;
    unsigned short               : 2;
    unsigned short CTB           : 1;
  } TB0CN_bit;
} @ _M(4,10);

__no_init volatile __io unsigned short TB0V @ _M(4,11);   /* Timer B Value Register */

/* I2C Control Register */
__no_init volatile __io union
{
  unsigned short I2CCN;
  struct
  {
    unsigned short I2CEN         : 1;
    unsigned short I2CMST        : 1;
    unsigned short I2CMODE       : 1;
    unsigned short               : 1;
    unsigned short I2CSTRS       : 1;
    unsigned short I2CACK        : 1;
    unsigned short I2CSTART      : 1;
    unsigned short I2CSTOP       : 1;
    unsigned short I2CGCEN       : 1;
    unsigned short I2CSTREN      : 1;
    unsigned short               : 5;
    unsigned short I2CRST        : 1;
  } I2CCN_bit;
} @ _M(4,12);

/* I2C Clock Register */
__no_init volatile __io union
{
  unsigned short I2CCK;
  struct
  {
    unsigned short I2CCKL        : 8;
    unsigned short I2CCKH        : 8;
  } I2CCK_bit;
} @ _M(4,13);

__no_init volatile __io unsigned char I2CTO @ _M(4,14);   /* I2C Timeout Register */

__no_init volatile __io unsigned short I2CSLA @ _M(4,15);   /* I2C Slave Address Register */

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
} @ _M(8,0x0E);


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
} @ _M(8,0x0F);

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ3108 */


