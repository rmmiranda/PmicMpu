/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ200X.
 *
 * This file supports assembler and C development for MAXQ200X
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1241 $
 *
 ********************************************************************/


#ifndef __IOMAXQ200X
#define __IOMAXQ200X

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
  unsigned char PO0;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PO0_bit;
} @ _M(0,0); 
  
/* Port 1 Output Register */
__no_init volatile __io union
{
  unsigned char PO1;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PO1_bit;
} @ _M(0,1);
   
/* Port 2 Output Register */
__no_init volatile __io union
{
  unsigned char PO2;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PO2_bit;
} @ _M(0,2);  
 
/* Port 3 Output Register */
__no_init volatile __io union
{
  unsigned char PO3 ;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
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
} @ _M(0,6);


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
} @ _M(0x0, 0x7);

/* Port 0 Input Register */
__no_init const volatile __io union
{
  unsigned char PI0;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PI0_bit;
} @ _M(0,0x8); 
  
/* Port 1 Input Register */
__no_init const volatile __io union
{
  unsigned char PI1;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PI1_bit;
} @ _M(0,0x9);
   
/* Port 2 Input Register */
__no_init const volatile __io union
{
  unsigned char PI2;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PI2_bit;
} @ _M(0,0xA);  
 
/* Port 3 Input Register */
__no_init const volatile __io union
{
  unsigned char PI3;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
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

/* Port 0 Direction Register */
__no_init volatile __io union
{
  unsigned char PD0;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PD0_bit;
} @ _M(0,0x10);  

/* Port 1 Direction Register */
__no_init volatile __io union
{
  unsigned char PD1;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PD1_bit;
} @ _M(0,0x11);   

/* Port 2 Direction Register */
__no_init volatile __io union
{
  unsigned char PD2;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PD2_bit;
} @ _M(0,0x12);   

/* Port 3 Direction Register */
__no_init volatile __io union
{
  unsigned char PD3;   
  struct
  {
    unsigned char bit0            : 1;
    unsigned char bit1            : 1;
    unsigned char bit2            : 1;
    unsigned char bit3            : 1;
    unsigned char bit4            : 1;
    unsigned char bit5            : 1;
    unsigned char bit6            : 1;
    unsigned char bit7            : 1;
  } PD3_bit;
} @ _M(0,0x13);   

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
    unsigned short                : 5;
    unsigned short ACS            : 1;
    unsigned short X32D           : 1;
    unsigned short WE             : 1;
  } RCNT_bit;
} @ _M(0,0x19);

__no_init volatile __io unsigned char  RTSS @ _M(0,0x1A); /* RT Clock Subsecond Counter  */
__no_init volatile __io unsigned short RTSH @ _M(0,0x1B); /* RT Clock Second Counter High*/
__no_init volatile __io unsigned short RTSL @ _M(0,0x1C); /* RT Clock Second Counter Low */
__no_init volatile __io unsigned char  RSSA @ _M(0,0x1D); /* RT Clock Subsecond Alarm    */
__no_init volatile __io unsigned short RASH @ _M(0,0x1E); /* RT Clock T-O-D Alarm High   */
__no_init volatile __io unsigned short RASL @ _M(0,0x1F); /* RT Clock T-O-D Alarm Low   */

/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

__no_init volatile __io unsigned char PO4 @ _M(1,0);      /* Port 4 Output Register */
__no_init volatile __io unsigned char PO5 @ _M(1,1);      /* Port 5 Output Register */
__no_init volatile __io unsigned char PO6 @ _M(1,2);      /* Port 6 Output Register */
__no_init volatile __io unsigned char PO7 @ _M(1,3);      /* Port 7 Output Register */

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
    unsigned char EX12           : 1;
    unsigned char EX13           : 1;
    unsigned char EX14           : 1;
    unsigned char EX15           : 1;
  } EIE1_bit;
} @ _M(1, 7);

__no_init const volatile __io unsigned char PI4 @ _M(1,0x8);  /* Port 1 Input Register */
__no_init const volatile __io unsigned char PI5 @ _M(1,0x9);  /* Port 2 Input Register */
__no_init const volatile __io unsigned char PI6 @ _M(1,0xA);  /* Port 3 Input Register */
__no_init const volatile __io unsigned char PI7 @ _M(1,0xB);  /* Port 4 Input Register */

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
} @ _M(1,0xC);

__no_init volatile __io unsigned char PD4 @ _M(1,0x10);   /* Port 4 Direction Register */
__no_init volatile __io unsigned char PD5 @ _M(1,0x11);   /* Port 5 Direction Register */
__no_init volatile __io unsigned char PD6 @ _M(1,0x12);   /* Port 6 Direction Register */
__no_init volatile __io unsigned char PD7 @ _M(1,0x13);   /* Port 7 Direction Register */

/* Supply Voltage Selector register */

__no_init volatile __io union
{
  unsigned char SVS;
  struct
  {
    unsigned char SV70           : 1;
    unsigned char SV71           : 1;
    unsigned char                : 2;
    unsigned char SV64           : 1;
    unsigned char SV65           : 1;
    unsigned char SV66           : 1;
    unsigned char SV67           : 1;
  } SVS_bit;
} @ _M(1,0x1E);

/* Wake-Up Output Register */

__no_init volatile __io union
{
  unsigned char WK0;
  struct
  {
    unsigned char WKE0           : 1;
    unsigned char WKE1           : 1;
    unsigned char WKL            : 1;
    unsigned char                : 5;
  } WKO_bit;
} @ _M(1,0x1F);

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
} @ _M(2,0x0);

__no_init volatile __io unsigned short MA  @ _M(2,1); /* Multiplier Operand A Register */
__no_init volatile __io unsigned short MB  @ _M(2,2); /* Multiplier Operand B Register */
__no_init volatile __io unsigned short MC2 @ _M(2,3); /* Multiplier Accumulate Register 2 */
__no_init volatile __io unsigned short MC1 @ _M(2,4); /* Multiplier Accumulate Register 1 */
__no_init volatile __io unsigned short MC0 @ _M(2,5);   /* Multiplier Accumulate Register 0 */


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

/* LCD Adjust Register */
__no_init volatile __io union
{
  unsigned short LCRA;
  struct
  {
    unsigned short LRA            : 5;
    unsigned short LRIGC          : 1;
    unsigned short LCCS           : 1;
    unsigned short FRM            : 4;
    unsigned short DUTY           : 2;
  } LCRA_bit;
} @ _M(2,0xD);

/* LCD Configuration register */
__no_init volatile __io union
{
  unsigned char LCFG;
  struct
  {
    unsigned char DPE            : 1;
    unsigned char OPM            : 1;
    unsigned char                : 2;
    unsigned char PCF            : 4;
  } LCFG_bit;
} @ _M(2,0xE);

/* LCD Display registers */

__no_init volatile __io unsigned char LCD16  @ _M(2,0x0F);
__no_init volatile __io unsigned char LCD0   @ _M(2,0x10);
__no_init volatile __io unsigned char LCD1   @ _M(2,0x11);
__no_init volatile __io unsigned char LCD2   @ _M(2,0x12);
__no_init volatile __io unsigned char LCD3   @ _M(2,0x13);
__no_init volatile __io unsigned char LCD4   @ _M(2,0x14);
__no_init volatile __io unsigned char LCD5   @ _M(2,0x15);
__no_init volatile __io unsigned char LCD6   @ _M(2,0x16);
__no_init volatile __io unsigned char LCD7   @ _M(2,0x17);
__no_init volatile __io unsigned char LCD8   @ _M(2,0x18);
__no_init volatile __io unsigned char LCD9   @ _M(2,0x19);
__no_init volatile __io unsigned char LCD10  @ _M(2,0x1A);
__no_init volatile __io unsigned char LCD11  @ _M(2,0x1B);
__no_init volatile __io unsigned char LCD12  @ _M(2,0x1C);
__no_init volatile __io unsigned char LCD13  @ _M(2,0x1D);
__no_init volatile __io unsigned char LCD14  @ _M(2,0x1E);
__no_init volatile __io unsigned char LCD15  @ _M(2,0x1F);

/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

/* Timer 0: Type 2 Control Register A */
__no_init volatile __io union
{
  unsigned char T2CNA0;
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
  } T2CNA0_bit;
} @ _M(3,0);

__no_init volatile __io unsigned char T2H0 @ _M(3,1);   /* Timer 0: Type 2 Most Significant Byte Value */
__no_init volatile __io unsigned char T2RH0 @ _M(3,2);   /* Timer 0: Type 2 Most Significant Byte Reload */
__no_init volatile __io unsigned char T2CH0 @ _M(3,3);   /* Timer 0: Type 2 Most Significant Byte Capture/Compare */

/* SPI Data Buffer Register */
__no_init volatile __io unsigned short SPIB @ _M(3,5);


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
} @ _M(3,6);

__no_init volatile __io unsigned char SBUF1 @ _M(3,7);   /* Serial Data Buffer */

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
} @ _M(3,8);

__no_init volatile __io unsigned short PR1 @ _M(3,9);   /* Phase Register */

/* Timer 0: Type 2 Control Register B */
__no_init volatile __io union
{
  unsigned char T2CNB0;
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
  } T2CNB0_bit;
} @ _M(3,0xC);

__no_init volatile __io unsigned short T2V0 @ _M(3,0xD);   /* Timer 0: Type 2 Value Register */
__no_init volatile __io unsigned short T2R0 @ _M(3,0xE);   /* Timer 0: Type 2 Reload Register */
__no_init volatile __io unsigned short T2C0 @ _M(3,0xF);   /* Timer 0: Type 2 Capture/Compare Register */

/* Timer 0: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG0;
  struct
  {
    unsigned char CT             : 1;
    unsigned char CCF            : 2;
    unsigned char T2MD           : 1;
    unsigned char T2DIV          : 3;
    unsigned char T2CI           : 1;
  } T2CFG0_bit;
} @ _M(3,0x10);

/* One Wire Master Address */
__no_init volatile __io union
{
  unsigned char OWA;
  struct
  {
    unsigned char A0             : 1;
    unsigned char A1             : 1;
    unsigned char A2             : 1;
  } OWA_bit;
} @ _M(3,0x13);

/* One Wire Master Data */
__no_init volatile __io unsigned char OWD @ _M(3,0x14);

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
} @ _M(3,0x15);


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
} @ _M(3,0x16);

__no_init volatile __io unsigned char SPICK @ _M(3,0x17);   /* SPI Clock Register */

__no_init volatile __io unsigned short ICDT0 @ _M(3,0x18);   /* In-circuit Debug Temp 0 Register */
__no_init volatile __io unsigned short ICDT1 @ _M(3,0x19);   /* In-circuit Debug Temp 1 Register */

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
} @ _M(3,0x1A);


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
} @ _M(3,0x1B);

__no_init volatile __io unsigned char ICDB @ _M(3,0x1C);   /* In-circuit Debug Buffer Register */
__no_init volatile __io unsigned char ICDA @ _M(3,0x1D);   /* In-circuit Debug Address Register */
__no_init volatile __io unsigned char ICDD @ _M(3,0x1E);   /* In-circuit Debug Data Register */


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
} @ _M(3,0x1F);

/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/

/* Timer 1: Type 2 Control Register A */
__no_init volatile __io union
{
  unsigned char T2CNA1;
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
  } T2CNA1_bit;
} @ _M(4,0);

__no_init volatile __io unsigned char T2H1 @ _M(4,1);   /* Timer 1: Type 2 Most Significant Byte Value */
__no_init volatile __io unsigned char T2RH1 @ _M(4,2);   /* Timer 1: Type 2 Most Significant Byte Reload */
__no_init volatile __io unsigned char T2CH1 @ _M(4,3);   /* Timer 1: Type 2 Most Significant Byte Capture/Compare */

/* Timer 2: Type 2 Control Register A */
__no_init volatile __io union
{
  unsigned char T2CNA2;
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
  } T2CNA2_bit;
} @ _M(4,4);

__no_init volatile __io unsigned char T2H2 @ _M(4,5);   /* Timer 2: Type 2 Most Significant Byte Value */
__no_init volatile __io unsigned char T2RH2 @ _M(4,6);   /* Timer 2: Type 2 Most Significant Byte Reload */
__no_init volatile __io unsigned char T2CH2 @ _M(4,7);   /* Timer 2: Type 2 Most Significant Byte Capture/Compare */


/* Timer 1: Type 2 Control Register B */
__no_init volatile __io union
{
  unsigned char T2CNB1;
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
  } T2CNB1_bit;
} @ _M(4,8);

__no_init volatile __io unsigned short T2V1 @ _M(4,9);   /* Timer 1: Type 2 Value Register */
__no_init volatile __io unsigned short T2R1 @ _M(4,0xA); /* Timer 1: Type 2 Reload Register */
__no_init volatile __io unsigned short T2C1 @ _M(4,0xB); /* Timer 1: Type 2 Capture/Compare Register */

/* Timer 2: Type 2 Control Register B */
__no_init volatile __io union
{
  unsigned char T2CNB2;
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
  } T2CNB2_bit;
} @ _M(4,0xC);

__no_init volatile __io unsigned short T2V2 @ _M(4,0xD);  /* Timer 2: Type 2 Value Register */
__no_init volatile __io unsigned short T2R2 @ _M(4,0xE);  /* Timer 2: Type 2 Reload Register */
__no_init volatile __io unsigned short T2C2 @ _M(4,0xF); /* Timer 2: Type 2 Capture/Compare Register */

/* Timer 1: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG1;
  struct
  {
    unsigned char CT             : 1;
    unsigned char CCF            : 2;
    unsigned char T2MD           : 1;
    unsigned char T2DIV          : 3;
    unsigned char T2CI           : 1;
  } T2CFG1_bit;
} @ _M(4,0x10);

/* Timer 0: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG2;
  struct
  {
    unsigned char CT             : 1;
    unsigned char CCF            : 2;
    unsigned char T2MD           : 1;
    unsigned char T2DIV          : 3;
    unsigned char T2CI           : 1;
  } T2CFG2_bit;
} @ _M(4,0x11);


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
    unsigned char RGMD           : 1;
    unsigned char RGSL           : 1;
    unsigned char XTRC           : 1;
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
#endif /* __IOMAXQ200X */


