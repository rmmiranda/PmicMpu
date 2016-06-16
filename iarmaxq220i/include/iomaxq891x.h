/********************************************************************
 *
 * Register and bit definitions for Maxim  MAXQ891X.
 *
 * This file supports assembler and C development for MAXQ891X
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * >> Modified for MaxQ891X by Maxim <<
 *
 * $Revision: 1167 $
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

__no_init volatile __io unsigned char PO0 @ _M(0, 0x00);   /* Port 0 Output Register */
__no_init volatile __io unsigned char PO1 @ _M(0, 0x01);   /* Port 1 Output Register */


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
} @ _M(0, 0x02);


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
} @ _M(0, 0x03);


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
  } EIF1_bit;
} @ _M(0, 0x04);


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
  } EIE1_bit;
} @ _M(0, 0x05);


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
} @ _M(0, 0x06);


__no_init const volatile __io unsigned char PI0 @ _M(0, 0x08);  /* Port 0 Input Register */
__no_init const volatile __io unsigned char PI1 @ _M(0, 0x09);  /* Port 1 Input Register */


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
} @ _M(0, 0x0A);


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
  } EIES1_bit;
} @ _M(0, 0x0B);


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
} @ _M(0, 0x0C);


__no_init volatile __io unsigned char PID0  @ _M(0, 0x0D);   /* Port 0 Input Disable */

__no_init volatile __io unsigned char PD0  @ _M(0, 0x10);   /* Port 0 Direction Register */
__no_init volatile __io unsigned char PD1  @ _M(0, 0x11);   /* Port 1 Direction Register */


/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

/* Serial Port Control Register */
__no_init volatile __io union
{
  unsigned char SCON;
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
  } SCON_bit;
} @ _M(1, 0x00);


__no_init volatile __io unsigned char SBUF @ _M(1, 0x01);   /* Serial Data Buffer */


/* SPI Control Register */
__no_init volatile __io union
{
  unsigned char SPICN0;
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
  } SPICN0_bit;
} @ _M(1, 0x02);

/* SPI Control Register */
__no_init volatile __io union
{
  unsigned char SPICN1;
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
  } SPICN1_bit;
} @ _M(4, 0x00);


__no_init volatile __io unsigned short SPIB0 @ _M(1, 0x03);      /* SPI Data Buffer Register */
__no_init volatile __io unsigned short SPIB1 @ _M(4, 0x01);      /* SPI Data Buffer Register */


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
} @ _M(1, 0x04);


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
} @ _M(1, 0x05);


__no_init volatile __io unsigned short I2CBUF @ _M(1, 0x06);


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
} @ _M(1, 0x07);


/* Serial Port Mode Register */
__no_init volatile __io union
{
  unsigned char SMD;
  struct
  {
    unsigned char FEDE           : 1;
    unsigned char SMOD           : 1;
    unsigned char ESI            : 1;
  } SMD_bit;
} @ _M(1, 0x08);

__no_init volatile __io unsigned short PR @ _M(1, 0x09);   /* Phase Register */


/* SPI Configuration Register */
__no_init volatile __io union
{
  unsigned char SPICF0;
  struct
  {
    unsigned char CKPOL          : 1;
    unsigned char CKPHA          : 1;
    unsigned char CHR            : 1;
    unsigned char                : 4;
    unsigned char ESPII          : 1;
  } SPICF0_bit;
} @ _M(1, 0x0A);

/* SPI Configuration Register */
__no_init volatile __io union
{
  unsigned char SPICF1;
  struct
  {
    unsigned char CKPOL          : 1;
    unsigned char CKPHA          : 1;
    unsigned char CHR            : 1;
    unsigned char                : 4;
    unsigned char ESPII          : 1;
  } SPICF1_bit;
} @ _M(4, 0x08);


__no_init volatile __io unsigned char SPICK0 @ _M(1, 0x0B);   /* SPI Clock Register */
__no_init volatile __io unsigned char SPICK1 @ _M(4, 0x09);   /* SPI Clock Register */


/* I2C Clock Register */
__no_init volatile __io union
{
  unsigned short I2CCK;
  struct
  {
    unsigned char I2CCKL         : 8;
    unsigned char I2CCKH         : 8;
  } I2CCK_bit;
} @ _M(1, 0x0C);

__no_init volatile __io unsigned short I2CTO  @ _M(1, 0x0D);   /* I2C Timeout Register       */
__no_init volatile __io unsigned short I2CSLA @ _M(1, 0x0E);   /* I2C Slave Address Register */



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
} @ _M(2, 0x06);

__no_init volatile __io unsigned short TB0R  @ _M(2, 0x07);    /* Timer B 0 Reload Register   */

__no_init volatile __io unsigned short MC1R  @ _M(2, 0x08);   /* Multiplier Read Register 1 */
__no_init volatile __io unsigned short MC0R  @ _M(2, 0x09);   /* Multiplier Read Register 0 */
__no_init volatile __io unsigned short TB0V  @ _M(2, 0x0A);    /* Timer B 0 Value Register   */
__no_init volatile __io unsigned short TB0C  @ _M(2, 0x0B);    /* Timer B 0 Capture Register  */


/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

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
} @ _M(3, 0x00);


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
} @ _M(3, 0x01);


__no_init volatile __io unsigned short DADATA1 @ _M(3, 0x02);   /* DAC 1 Output */
__no_init volatile __io unsigned short DADATA2 @ _M(3, 0x03);   /* DAC 2 Output */
__no_init volatile __io unsigned short DADATA3 @ _M(3, 0x04);   /* DAC 3 Output */
__no_init volatile __io unsigned short DADATA4 @ _M(3, 0x05);   /* DAC 4 Output */
__no_init volatile __io unsigned short AMPCN   @ _M(3, 0x06);
__no_init volatile __io unsigned short ISINKCN @ _M(3, 0x07);


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
} @ _M(3, 0x08);

__no_init volatile __io unsigned short ADDATA  @ _M(3, 0x09);    /* ADC Sample Data Register   */

__no_init volatile __io unsigned short OPMCN   @ _M(3, 0x0A);
__no_init volatile __io union
{
  unsigned short DACN;
  struct
  {
    unsigned char DACEN1        :1;
    unsigned char DACEN2        :1;
    unsigned char DACEN3        :1;
    unsigned char DACEN4        :1;
    unsigned char DACOUT1       :1;
    unsigned char DACOUT2       :1;
    unsigned char               :2;
  }DACN_bit;
}@ _M(3, 0x0B);
__no_init volatile __io unsigned short TEMPEN  @ _M(3, 0x0C);


/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/



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


