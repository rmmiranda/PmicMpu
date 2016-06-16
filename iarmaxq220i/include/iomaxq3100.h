
/********************************************************************
 *
 * Standard register and bit definitions for the Dallas MAXQ.
 *
 * CUSTOMIZED FOR THE MAXQ3100 by BAS
 *
 *
 * Copyright 2003 IAR Systems. All rights reserved.
 *
 * $Revision: 1106 $
 *
 ********************************************************************/

#ifndef __IOMAXQ
#define __IOMAXQ

#if (((__TID__ >> 8) & 0x7F) != 0x42)     /* 0x42 = 66 dec */
#error iomaxqxx.h file for use with ICCMAXQ/AMAXQ only
#endif

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended
#include "iomacro.h"


  // module 8
  __SFR_BITS(__io,PSF,0x208,,,OV,GPF0,GPF1,,,)
  __SFR_BITS(__io,IC,0x20A,IGE,INS,,,,CGDS,,)
  __SFR_BITS(__io,IMR,0x20C,IM0,IM1,,IM3,IM4,,,IMS)
  __SFR_BITS(__io,SC,0x210,,PWL,ROD,UPA,CDA0,CDA1,,TAP)
  __SFR(__io,IIR,0x216)
  __SFR_BITS(__io,CKCN,0x21C,CD0,CD1,PMME,SWB,STOP,,,)
  __SFR_BITS(__io,WDCN,0x21E,RWT,EWT,WTRF,WDIF,WD0,WD1,EWDI,POR)

  // module 13
  __SFR_W(__io,IV,0x344)


/*-------------------------------------------------------------------------
 *   Special Function Registers
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 *   MODULE 0
 *-------------------------------------------------------------------------*/

__no_init volatile __io unsigned char PO0 @ _M(0,0);   // Port 0 Output Register

// Serial Port 0 Control Register
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
} @ _M(0,1);

__no_init volatile __io unsigned char SBUF0 @ _M(0,2);   // Serial 0 Data Buffer

// Timer 0 Control Register
__no_init volatile __io union
{
  unsigned char T0CN;
  struct
  {
    unsigned char M0             : 1;
    unsigned char M1             : 1;
    unsigned char CT             : 1;
    unsigned char GATE           : 1;
    unsigned char TR0            : 1;
    unsigned char TF0            : 1;
    unsigned char T0M            : 1;
    unsigned char ET0            : 1;
  } T0CN_bit;
} @ _M(0,3);

__no_init volatile __io unsigned char T0L @ _M(0,4);   // Timer 0 Least Significant Byte


// Timer 1 Control Register
__no_init volatile __io union
{
  unsigned char T1CN;
  struct
  {
    unsigned char CPRL1          : 1;
    unsigned char CT1            : 1;
    unsigned char TR1            : 1;
    unsigned char EXEN1          : 1;
    unsigned char DCEN           : 1;
    unsigned char T1OE           : 1;
    unsigned char EXF1           : 1;
    unsigned char TF1            : 1;
  } T1CN_bit;
} @ _M(0,5);

// Timer 1 Mode Register
__no_init volatile __io union
{
  unsigned char T1MD;
  struct
  {
    unsigned char T1M            : 1;
    unsigned char ET1            : 1;
  } T1MD_bit;
} @ _M(0,6);

// External Interrupt Flag Register (Port 0)
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
} @ _M(0,7);

__no_init volatile __io unsigned char PI0 @ _M(0,8);   // Port 0 Input Register

// Serial Port 0 Mode Register
__no_init volatile __io union
{
  unsigned char SMD0;
  struct
  {
    unsigned char FEDE0          : 1;
    unsigned char SMOD0          : 1;
    unsigned char ESI0           : 1;
    unsigned char DUMMY          : 3;
    unsigned char OFS            : 1;
    unsigned char EPWM           : 1;
  } SMD0_bit;
} @ _M(0,9);

__no_init volatile __io unsigned short PR0 @ _M(0,10);   // Phase Register 0

__no_init volatile __io unsigned char T0H @ _M(0,11);   // Timer 0 Most Significant Byte

__no_init volatile __io unsigned char T1L @ _M(0,12);   // Timer 1 Least Significant Byte

__no_init volatile __io unsigned char T1H @ _M(0,13);   // Timer 1 Most Significant Byte

__no_init volatile __io unsigned char T1CL @ _M(0,14);   // Timer 1 Capture Least Significant Byte

__no_init volatile __io unsigned char T1CH @ _M(0,15);   // Timer 1 Capture Most Significant Byte

__no_init volatile __io unsigned char PD0 @ _M(0,16);   // Port 0 Direction Register

// External Interrupt Enable Register
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
} @ _M(0,30);

// External Interrupt Edge Select Register (Port 3)
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
} @ _M(0,31);

/*-------------------------------------------------------------------------
 *   MODULE 1
 *-------------------------------------------------------------------------*/

__no_init volatile __io unsigned char PO1 @ _M(1,0);   // Port 1 Output Register

// Serial Port 1 Control Register
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
} @ _M(1,1);

__no_init volatile __io unsigned char SBUF1 @ _M(1,2);   // Serial 1 Data Buffer

// Timer 2 Control Register B
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
} @ _M(1,3);

__no_init volatile __io unsigned char T2H @ _M(1,4);   // Timer 2 Most Significant Byte Value

__no_init volatile __io unsigned char T2RH @ _M(1,5);   // Timer 2 Most Significant Byte Reload

__no_init volatile __io unsigned char T2CH @ _M(1,6);   // Timer 2 Most Significant Byte Capture/Compare

// External Interrupt Flag Register (Port 1)
__no_init volatile __io union
{
  unsigned char EIF1;
  struct
  {
    unsigned char IE8            : 1;
    unsigned char IE9            : 1;
    unsigned char IE10           : 1;
    unsigned char IE11           : 1;
    unsigned char DUMMY          : 4;
  } EIF1_bit;
} @ _M(1,7);

__no_init volatile __io unsigned char PI1 @ _M(1,8);   // Port 1 Input Register

// Serial Port 1 Mode Register
__no_init volatile __io union
{
  unsigned char SMD1;
  struct
  {
    unsigned char FEDE1          : 1;
    unsigned char SMOD1          : 1;
    unsigned char ESI1           : 1;
    unsigned char DUMMY          : 5;
  } SMD1_bit;
} @ _M(1,9);

__no_init volatile __io unsigned short PR1 @ _M(1,10);   // Phase Register 1

// Timer 2 Control Register A
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
} @ _M(1,11);

// Timer 2 Configuration Register
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
} @ _M(1,12);

__no_init volatile __io unsigned short T2V @ _M(1,13);   // Timer 2 Value Register

__no_init volatile __io unsigned short T2C @ _M(1,14);   // Timer 2 Capture/Compare Register

// Infrared Control Register
__no_init volatile __io union
{
  unsigned char IRCN;
  struct
  {
    unsigned char IRBB           : 1;
    unsigned char IRTX           : 1;
    unsigned char IREN           : 1;
  } IRCN_bit;
} @ _M(1,15);

__no_init volatile __io unsigned char PD1 @ _M(1,16);   // Port 1 Direction Register 

__no_init volatile __io unsigned short T2R @ _M(1,17);   // Timer 2 Reload Register

// External Interrupt Enable Register 1
__no_init volatile __io union
{
  unsigned char EIE1;
  struct
  {
    unsigned char EX8            : 1;
    unsigned char EX9            : 1;
    unsigned char EX10           : 1;
    unsigned char EX11           : 1;
    unsigned char DUMMY          : 4;
  } EIE1_bit;
} @ _M(1,30);

// External Interrupt Edge Select Register 1
__no_init volatile __io union
{
  unsigned char EIES1;
  struct
  {
    unsigned char IT8            : 1;
    unsigned char IT9            : 1;
    unsigned char IT10           : 1;
    unsigned char IT11           : 1;
    unsigned char DUMMY          : 4;
  } EIES1_bit;
} @ _M(1,31);

/*-------------------------------------------------------------------------
 *   MODULE 2
 *-------------------------------------------------------------------------*/

__no_init volatile __io unsigned char PO2 @ _M(2,0);   // Port 2 Output Register

// LCD Configuration
__no_init volatile __io union
{
  unsigned char LCFG;
  struct
  {
    unsigned char DPE            : 1;
    unsigned char OPM            : 1;
    unsigned char SMO            : 1;
    unsigned char DUMMY          : 1;
    unsigned char PCF            : 4;
  } LCFG_bit;
} @ _M(2,1);

// LCD Register Adjust
__no_init volatile __io union
{
  unsigned short LCRA;
  struct
  {
    unsigned short LRA            : 5;
    unsigned short LRIG           : 1;
    unsigned short LCCS           : 1;
    unsigned short FRM            : 4;
    unsigned short DUTY           : 2;
  } LCRA_bit;
} @ _M(2,2);

__no_init volatile __io unsigned short LCD0 @ _M(2,3);   // LCD Display Register 0

__no_init volatile __io unsigned short LCD1 @ _M(2,4);   // LCD Display Register 1

__no_init volatile __io unsigned short LCD2 @ _M(2,5);   // LCD Display Register 2

__no_init volatile __io unsigned short LCD3 @ _M(2,6);   // LCD Display Register 3

__no_init volatile __io unsigned short LCD4 @ _M(2,7);   // LCD Display Register 4

__no_init volatile __io unsigned char PI2 @ _M(2,8);   // Port 2 Input Register

__no_init volatile __io unsigned short LCD5 @ _M(2,9);   // LCD Display Register 5

__no_init volatile __io unsigned short LCD6 @ _M(2,10);   // LCD Display Register 6

__no_init volatile __io unsigned short LCD7 @ _M(2,11);   // LCD Display Register 7

__no_init volatile __io unsigned short LCD8 @ _M(2,12);   // LCD Display Register 8

__no_init volatile __io unsigned short LCD9 @ _M(2,13);   // LCD Display Register 9

__no_init volatile __io unsigned short LCD10 @ _M(2,14);   // LCD Display Register 10

__no_init volatile __io unsigned short LCD11 @ _M(2,15);   // LCD Display Register 11

__no_init volatile __io unsigned char PD2 @ _M(2,16);   // Port 2 Direction Register

__no_init volatile __io unsigned short LCD12 @ _M(2,17);   // LCD Display Register 12

__no_init volatile __io unsigned short LCD13 @ _M(2,18);   // LCD Display Register 13

__no_init volatile __io unsigned short LCD14 @ _M(2,19);   // LCD Display Register 13

__no_init volatile __io unsigned short LCD15 @ _M(2,20);   // LCD Display Register 13

__no_init volatile __io unsigned short LCD16 @ _M(2,21);   // LCD Display Register 13

__no_init volatile __io unsigned short LCD17 @ _M(2,22);   // LCD Display Register 13

__no_init volatile __io unsigned short LCD18 @ _M(2,23);   // LCD Display Register 13

__no_init volatile __io unsigned short LCD19 @ _M(2,24);   // LCD Display Register 13

/*-------------------------------------------------------------------------
 *   MODULE 3
 *-------------------------------------------------------------------------*/

__no_init volatile __io unsigned char PO3 @ _M(3,0);   // Port 3 Output Register

// RTRM:  Real-Time Clock Trim Register
__no_init volatile __io union
{
  unsigned char RTRM;
  struct
  {
    unsigned char TRM            : 7;
    unsigned char TSGN           : 1;
  } RTRM_bit;
} @ _M(3,1);

// RCNT:  Real-Time Clock Control Register
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
    unsigned short DUMMY          : 5;
    unsigned short WE             : 1;
  } RCNT_bit;
} @ _M(3,2);

// CCN0: Comparator Control Register 0
__no_init volatile __io union
{
    unsigned char CCN0;
    struct
    {
      unsigned char DUMMY0        : 1;
      unsigned char CMPOL         : 1;
      unsigned char CMO           : 1;
      unsigned char DUMMY1        : 1;
      unsigned char CMM           : 1;
      unsigned char CMF           : 1;
      unsigned char CMIE          : 1;
      unsigned char CMON          : 1;
    } CCN0_bit;
} @ _M(3,3);

// CCN1: Comparator Control Register 0
__no_init volatile __io union
{
    unsigned char CCN1;
    struct
    {
      unsigned char DUMMY0        : 1;
      unsigned char CMPOL         : 1;
      unsigned char CMO           : 1;
      unsigned char DUMMY1        : 1;
      unsigned char CMM           : 1;
      unsigned char CMF           : 1;
      unsigned char CMIE          : 1;
      unsigned char CMON          : 1;
    } CCN1_bit;
} @ _M(3,4);

__no_init volatile __io unsigned char TEMPR @ _M(3,6);   // Temperature Register

// TPCFG: Temperature Sensor Configuration
__no_init volatile __io union
{
    unsigned char TPCFG;
    struct
    {
      unsigned char START         : 1;
      unsigned char RES           : 2;
      unsigned char DUMMY         : 3;
      unsigned char TPIE          : 1;
      unsigned char TPIF          : 1;
    } TPCFG_bit;
} @ _M(3,7);

__no_init volatile __io unsigned char PI3 @ _M(3,8);   // Port 3 Input Register

__no_init volatile __io unsigned char RTSS @ _M(3,9); // Real-Time Clock Subsecond Register

__no_init volatile __io unsigned int RTSH @ _M(3,10); // Real-Time Clock High-Order Counter

__no_init volatile __io unsigned int RTSL @ _M(3,11); // Real-Time Clock Low-Order Counter

__no_init volatile __io unsigned int RSSA @ _M(3,12); // Real-Time Clock Subsecond Alarm Register

__no_init volatile __io unsigned int RASH @ _M(3,13); // Real-Time Clock High-Order Alarm Register

__no_init volatile __io unsigned int RASL @ _M(3,14); // Real-Time Clock Low-Order Alarm Regiser

// PWCN: Power-Down Control Register
__no_init volatile __io union
{
    unsigned char PWCN;
    struct
    {
      unsigned char BOD           : 1;
      unsigned char DUMMY         : 7;
    } PWCN_bit;
} @ _M(3,15);

__no_init volatile __io unsigned char PD3 @ _M(3,16);   // Port 3 Direction Register 


#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */


#endif /* __IOMAXQ */


