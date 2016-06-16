
/********************************************************************
 *
 * Standard register and bit definitions for the Dallas MAXQ.
 *
 * CUSTOMIZED FOR THE MAXQ3120 by BAS
 *
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1106 $
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

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended
#include "iomacro.h"


  // module 8
  __SFR_BITS(__io,PSF,0x208,,,OV,GPF0,GPF1,,,)
  __SFR_BITS(__io,IC,0x20A,IGE,INS,,,,CGDS,,)
  __SFR_BITS(__io,IMR,0x20C,IM0,IM1,IM2,IM3,IM4,IM5,,IMS)
  __SFR_BITS(__io,SC,0x210,,PWL,ROD,UPA,CDA0,CDA1,,TAPD)
  __SFR(__io,IIR,0x216)
  __SFR_BITS(__io,CKCN,0x21C,CD0,CD1,PMME,SWB,STOP,RGMD,RGSL,XTRG)
  __SFR_BITS(__io,WDCN,0x21E,RWT,EWT,WTRF,WDIF,WD0,WD1,EWDI,POR)

  // module 13
  __SFR_W(__io,IV,0x344)


/*-------------------------------------------------------------------------
 *   Special Function Registers
 *-------------------------------------------------------------------------*/

__no_init volatile __io unsigned char PO0 @ _M(0,0);   // Port 0 Output Register

__no_init volatile __io unsigned char PO1 @ _M(0,1);   // Port 1 Output Register

__no_init volatile __io unsigned char PO2 @ _M(0,2);   // Port 2 Output Register

__no_init volatile __io unsigned char PO3 @ _M(0,3);   // Port 3 Output Register


// External Interrupt Flag Register (Port 0)
__no_init volatile __io union
{
  unsigned char EIF0;
  struct
  {
    unsigned char IE0            : 1;
    unsigned char IE1            : 1;
    unsigned char IE2            : 1;
    unsigned char DUMMY          : 5;
  } EIF0_bit;
} @ _M(0,6);


// External Interrupt Enable Register
__no_init volatile __io union
{
  unsigned char EIE0;
  struct
  {
    unsigned char EX0            : 1;
    unsigned char EX1            : 1;
    unsigned char EX2            : 1;
    unsigned char DUMMY          : 5;
  } EIE0_bit;
} @ _M(0,7);


__no_init volatile __io unsigned char PI0 @ _M(0,8);   // Port 0 Input Register

__no_init volatile __io unsigned char PI1 @ _M(0,9);   // Port 1 Input Register

__no_init volatile __io unsigned char PI2 @ _M(0,10);   // Port 2 Input Register

__no_init volatile __io unsigned char PI3 @ _M(0,11);   // Port 3 Input Register


// External Interrupt Edge Select Register (Port 3)
__no_init volatile __io union
{
  unsigned char EIES0;
  struct
  {
    unsigned char IT0            : 1;
    unsigned char IT1            : 1;
    unsigned char IT2            : 1;
    unsigned char DUMMY          : 5;
  } EIES0_bit;
} @ _M(0,12);

// Flash Control Register
__no_init volatile __io union
{
  unsigned char FCNTL;
  struct
  {
    unsigned char DUMMY          : 1;
    unsigned char FC1            : 1;
    unsigned char FC2            : 1;
    unsigned char DQ5            : 1;
    unsigned char FBYP           : 1;
    unsigned char FINE           : 1;
    unsigned char FERR           : 1;
    unsigned char FBUSY          : 1;
  } FCNTL_bit;
} @ _M(0,14);

__no_init volatile __io unsigned char FDATA @ _M(0,15);   // Flash memory data

__no_init volatile __io unsigned char PD0 @ _M(0,16);   // Port 0 Direction Register

__no_init volatile __io unsigned char PD1 @ _M(0,17);   // Port 1 Direction Register 

__no_init volatile __io unsigned char PD2 @ _M(0,18);   // Port 2 Direction Register

__no_init volatile __io unsigned char PD3 @ _M(0,19);   // Port 3 Direction Register 

// Real time clock registers

// RTRM:  Real-Time Clock Trim Register
__no_init volatile __io union
{
  unsigned char RTRM;
  struct
  {
    unsigned char TRM            : 5;
    unsigned char TSGN           : 1;
    unsigned char DUMMY          : 2;
  } RTRM_bit;
} @ _M(0,24);

// RCNT:  Real-Time Clock Control Register
/*__no_init volatile __io union
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
} @ _M(0,25);
*/
__SFR_W_BITS (__io, RCNT, _M(0,25) ,RTCE, ADE, ASE, BUSY, RDY, RDYE, ALDF, ALSF, SQE, FT, DUMMY1, DUMMY2, DUMMY3, DUMMY4, DUMMY5, WE)
__no_init volatile __io unsigned char RTSS @ _M(0,26); // Real-Time Clock Subsecond Register

__no_init volatile __io unsigned int RTSH @ _M(0,27); // Real-Time Clock High-Order Counter

__no_init volatile __io unsigned int RTSL @ _M(0,28); // Real-Time Clock Low-Order Counter

__no_init volatile __io unsigned int RSSA @ _M(0,29); // Real-Time Clock Subsecond Alarm Register

__no_init volatile __io unsigned int RASH @ _M(0,30); // Real-Time Clock High-Order Alarm Register

__no_init volatile __io unsigned int RASL @ _M(0,31); // Real-Time Clock Low-Order Alarm Regiser

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
} @ _M(1,0);

__no_init volatile __io unsigned char T0L @ _M(1,1);   // Timer 0 Least Significant Byte

__no_init volatile __io unsigned char T0H @ _M(1,2);   // Timer 0 Most Significant Byte


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
} @ _M(1,3);

__no_init volatile __io unsigned char SBUF0 @ _M(1,4);   // Serial 0 Data Buffer

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
} @ _M(1,5);

__no_init volatile __io unsigned char SBUF1 @ _M(1,6);   // Serial 1 Data Buffer

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
} @ _M(1,9);

__no_init volatile __io unsigned short PR0 @ _M(1,10);   // Phase Register 0

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
} @ _M(1,11);

__no_init volatile __io unsigned short PR1 @ _M(1,12);   // Phase Register 1

__no_init volatile __io unsigned short ICDT0 @ _M(1,24);   // In-circuit Debug Temp 0 Register

__no_init volatile __io unsigned short ICDT1 @ _M(1,25);   // In-circuit Debug Temp 1 Register


// In-circuit Debug Control Register
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
} @ _M(1,26);


// In-circuit Debug Flag Register
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
} @ _M(1,27);

__no_init volatile __io unsigned char ICDB @ _M(1,28);   // In-circuit Debug Buffer Register

__no_init volatile __io unsigned char ICDA @ _M(1,29);   // In-circuit Debug Address Register

__no_init volatile __io unsigned char ICDD @ _M(1,30);   // In-circuit Debug Data Register


// Test Mode Register
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
} @ _M(1,31);

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
} @ _M(2,0);

__no_init volatile __io unsigned char T1L @ _M(2,1);   // Timer 1 Least Significant Byte

__no_init volatile __io unsigned char T1H @ _M(2,2);   // Timer 1 Most Significant Byte

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
} @ _M(2,3);

__no_init volatile __io unsigned char T2H @ _M(2,4);   // Timer 2 Most Significant Byte Value

__no_init volatile __io unsigned char T2RH @ _M(2,5);   // Timer 2 Most Significant Byte Reload

__no_init volatile __io unsigned char T2CH @ _M(2,6);   // Timer 2 Most Significant Byte Capture/Compare

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
} @ _M(2,10);

__no_init volatile __io unsigned char T1CL @ _M(2,8);   // Timer 1 Capture Least Significant Byte

__no_init volatile __io unsigned char T1CH @ _M(2,9);   // Timer 1 Capture Most Significant Byte

// Timer 1 Mode Register
__no_init volatile __io union
{
  unsigned char T1MD;
  struct
  {
    unsigned char T1M            : 1;
    unsigned char ET1            : 1;
  } T1MD_bit;
} @ _M(2,10);

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
} @ _M(2,11);

__no_init volatile __io unsigned short T2V @ _M(2,12);   // Timer 2 Value Register

__no_init volatile __io unsigned short T2R @ _M(2,13);   // Timer 2 Reload Register

__no_init volatile __io unsigned short T2C @ _M(2,14);   // Timer 2 Capture/Compare Register

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
} @ _M(2,15);

// Multiplier Control Register
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
} @ _M(3,0);

__no_init volatile __io unsigned short MA @ _M(3,1);   // Multiplier Operand A Register

__no_init volatile __io unsigned short MB @ _M(3,2);   // Multiplier Operand B Register

__no_init volatile __io unsigned short MC2 @ _M(3,3);   // Multiplier Accumulate Register 2

__no_init volatile __io unsigned short MC1 @ _M(3,4);   // Multiplier Accumulate Register 1

__no_init volatile __io unsigned short MC0 @ _M(3,5);   // Multiplier Accumulate Register 0

__no_init volatile __io unsigned short MC1R @ _M(3,6);   // Multiplier Read Register 1

__no_init volatile __io unsigned short MC0R @ _M(3,7);   // Multiplier Read Register 0

// A/D Converter Control Register
__no_init volatile __io union
{
  unsigned short ADCN;
  struct
  {
    unsigned short ABF0           : 1;
    unsigned short ABF1           : 1;
    unsigned short FOV0           : 1;
    unsigned short FOV1           : 1;
    unsigned short FLU0           : 1;
    unsigned short FLU1           : 1;
    unsigned short EDBI           : 1;
    unsigned short UFF            : 1;
    unsigned short ADP0           : 1;
    unsigned short ADP1           : 1;
    unsigned short ADP2           : 1;
    unsigned short                : 1;
    unsigned short G0             : 2;
    unsigned short G1             : 2;
  } ADCN_bit;
} @ _M(3,8);

// A/D Converter Phase Correction Register
__no_init volatile __io union
{
  unsigned int PHC;
  struct
  {
    unsigned char ZPS            : 1;
    unsigned char                : 6;
    unsigned char CHANNEL        : 1;
    unsigned char DELTA;
  } PHC_bit;
} @ _M(3,9);

__no_init volatile __io unsigned short AD0 @ _M(3,10);   // A/D Converter 0 Output Register

__no_init volatile __io unsigned short AD1 @ _M(3,11);   // A/D Converter 1 Output Register

__no_init volatile __io short ATRM @ _M(3,12);   // A/D Converter Trim Register

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
} @ _M(3,13);

// LCD Configuration
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
} @ _M(3,14);

__no_init volatile __io unsigned short LCD0 @ _M(3,16);   // LCD Display Register 0

__no_init volatile __io unsigned short LCD1 @ _M(3,17);   // LCD Display Register 1

__no_init volatile __io unsigned short LCD2 @ _M(3,18);   // LCD Display Register 2

__no_init volatile __io unsigned short LCD3 @ _M(3,19);   // LCD Display Register 3

__no_init volatile __io unsigned short LCD4 @ _M(3,20);   // LCD Display Register 4

__no_init volatile __io unsigned short LCD5 @ _M(3,21);   // LCD Display Register 5

__no_init volatile __io unsigned short LCD6 @ _M(3,22);   // LCD Display Register 6

__no_init volatile __io unsigned short LCD7 @ _M(3,23);   // LCD Display Register 7

__no_init volatile __io unsigned short LCD8 @ _M(3,24);   // LCD Display Register 8

__no_init volatile __io unsigned short LCD9 @ _M(3,25);   // LCD Display Register 9

__no_init volatile __io unsigned short LCD10 @ _M(3,26);   // LCD Display Register 10

__no_init volatile __io unsigned short LCD11 @ _M(3,27);   // LCD Display Register 11

__no_init volatile __io unsigned short LCD12 @ _M(3,28);   // LCD Display Register 12

__no_init volatile __io unsigned short LCD13 @ _M(3,29);   // LCD Display Register 13

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */


#endif /* __IOMAXQ */


