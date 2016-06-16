  // *******************************************************************************
  // *
  // *  Copyright (C) 2006 Dallas/Maxim Semiconductor Corporation. 
  // *  All rights Reserved. Printed in U.S.A.
  // *
  // *  This software is protected by copyright laws of the United States and of 
  // *  foreign countries.  This material may also be protected by patent laws of
  // *  the United States and of foreign countries.  This software is furnished 
  // *  under a license agreement and/or a nondisclosure agreement and may only 
  // *  be used or copied in accordance with the terms of those agreements.  The 
  // *  mere transfer of this software does not imply any licenses of trade 
  // *  secrets, proprietary technology, copyrights, patents, trademarks, 
  // *  maskwork rights, or any other form of intellectual property whatsoever. 
  // *  Dallas/Maxim Semiconductor retains all ownership rights.
  // *
  // *
  // *     Description: Register and Bit definitions for MAXQ7667 (Internal Die Name = UC34)
  // *																		
  // *        Filename: iomaxq7667.h
  // *
  // *          Author: David Voutour
  // *
  // *        Compiler: IAR
  // *
  // *         Version: 1.13D
  // *
  // *         Created: 4/03/06 from iomaxq765x.h file
  // *												
  // *   Modifications: 5/08/06 Extensive modifications from UC34 register specification	
  // *														 
  // *	                8/28/06 Updated EIF0, EIES0 and EIE0 to add Bits 7:6									
  // *
  // *                  8/28/06 Removed DPMG bit in FCNTL register
  // *
  // *                  8/28/06 Added STDIV label to SCNT register
  // *
  // *                  8/31/06 Added structure for Bit Addressing for PS0, PS1, PR0, PR1, 
  // *                          BPH, BTRN, SARC,RCVC, PLLF
  // *
  // *			        9/11/06 Added structures for LPFC and CMPC registers
  // *
  // *			        9/11/06	Changed structure bit types from char to short for register
  // *				            SCNT, ASR, OSCC, APE, ATST
  // *
  // *                  9/11/06 Changed SCNT structure label "RCNT_bit" to SCNT
  // *
  // *                  11/21/06 Fixed LPFC FFDP and FFIL register bits as the Register Specification was incorrect
  // *
  // *   
  // *                  Feb 21, 2007- Faisal Tariq
  // *                           Fixed SCNT
  // *                           Changed #ifndef in the beginning to __IOMAXQ7667
  // *                           Removed BIASTRM, BGTRM, MONTRM, SP0TRM, SP1TRM, SARLTRM, SAROTRM
  // *                           Removed ICDT0, ICDT1, ICDC, ICDF, ICDB, ICDA, ICDD, TM, ATST
  // *                           
  // *   
  // *    
  // *******************************************************************************

#ifndef __IOMAXQ7667   /* FT Change */
#define __IOMAXQ7667   /* FT Change */

#if (((__TID__ >> 8) & 0x7F) != 0x42)     /* 0x42 = 66 dec */
#error iomaxqxx.h file for use with ICCMAXQ/AMAXQ only
#endif

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended

#include "iomacro.h"


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
  unsigned char Bit0        :1;
  unsigned char Bit1        :1;
  unsigned char Bit2        :1;
  unsigned char Bit3        :1;
  unsigned char Bit4        :1;
  unsigned char Bit5        :1;
  unsigned char Bit6        :1;
  unsigned char Bit7        :1;
  } PO0_bit;
} @ _M(0,0);


/* Port 1 Output Register */
__no_init volatile __io union
{
  unsigned char PO1;
  struct
  {
  unsigned char Bit0        :1;
  unsigned char Bit1        :1;
  unsigned char Bit2        :1;
  unsigned char Bit3        :1;
  unsigned char Bit4        :1;
  unsigned char Bit5        :1;
  unsigned char Bit6        :1;
  unsigned char Bit7        :1;
  } PO1_bit;
} @ _M(0,1);


/* External Interrupt Flag 0 Register */
__no_init volatile __io union
{
  unsigned char EIF0;
  struct
  {
    unsigned char IE0             : 1;
    unsigned char IE1             : 1;
    unsigned char IE2             : 1;
    unsigned char IE3             : 1;
    unsigned char IE4             : 1;
    unsigned char IE5             : 1;
    unsigned char IE6             : 1;
    unsigned char IE7             : 1;
  } EIF0_bit;
} @ _M(0,3);


/* External Interrupt Flag 1 Register */
__no_init volatile __io union
{
  unsigned char EIF1;
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
  } EIF1_bit;
} @ _M(0,4);


/* Port 0 Input Register */
__no_init volatile __io union
{
  unsigned char PI0;
  struct
  {
  unsigned char Bit0        :1;
  unsigned char Bit1        :1;
  unsigned char Bit2        :1;
  unsigned char Bit3        :1;
  unsigned char Bit4        :1;
  unsigned char Bit5        :1;
  unsigned char Bit6        :1;
  unsigned char Bit7        :1;
  } PI0_bit;
} @ _M(0,0x8);


/* Port 1 Input Register */
__no_init volatile __io union
{
  unsigned char PI1;
  struct
  {
  unsigned char Bit0        :1;
  unsigned char Bit1        :1;
  unsigned char Bit2        :1;
  unsigned char Bit3        :1;
  unsigned char Bit4        :1;
  unsigned char Bit5        :1;
  unsigned char Bit6        :1;
  unsigned char Bit7        :1;
  } PI1_bit;
} @ _M(0,0x9);


/* External Interrupt Enable 0 Register */
__no_init volatile __io union
{
  unsigned char EIE0;
  struct
  {
    unsigned char EX0             : 1;
    unsigned char EX1             : 1;
    unsigned char EX2             : 1;
    unsigned char EX3             : 1;
    unsigned char EX4             : 1;
    unsigned char EX5             : 1;
    unsigned char EX6             : 1;
    unsigned char EX7             : 1;

   } EIE0_bit;
} @ _M(0, 0xB);


/* External Interrupt Enable 1 Register  */
__no_init volatile __io union
{
  unsigned char EIE1;
  struct
  {
    unsigned char EX0             : 1;
    unsigned char EX1             : 1;
    unsigned char EX2             : 1;
    unsigned char EX3             : 1;
    unsigned char EX4             : 1;
    unsigned char EX5             : 1;
    unsigned char EX6             : 1;
    unsigned char EX7             : 1;
  } EIE1_bit;
} @ _M(0, 0xC);


/* Port 0 Direction Register */
__no_init volatile __io union
{
  unsigned char PD0;
  struct
  {
  unsigned char Bit0        :1;
  unsigned char Bit1        :1;
  unsigned char Bit2        :1;
  unsigned char Bit3        :1;
  unsigned char Bit4        :1;
  unsigned char Bit5        :1;
  unsigned char Bit6        :1;
  unsigned char Bit7        :1;
  } PD0_bit;
} @ _M(0,0x10);


/* Port 1 Direction Register */
__no_init volatile __io union
{
  unsigned char PD1;
  struct
  {
  unsigned char Bit0        :1;
  unsigned char Bit1        :1;
  unsigned char Bit2        :1;
  unsigned char Bit3        :1;
  unsigned char Bit4        :1;
  unsigned char Bit5        :1;
  unsigned char Bit6        :1;
  unsigned char Bit7        :1;
  } PD1_bit;
} @ _M(0,0x11);


/* External Interrupt Edge Select 0 Register */
__no_init volatile __io union
{
  unsigned char EIES0;
  struct
  {
    unsigned char IT0             : 1;
    unsigned char IT1             : 1;
    unsigned char IT2             : 1;
    unsigned char IT3             : 1;
    unsigned char IT4             : 1;
    unsigned char IT5             : 1;
    unsigned char IT6             : 1;
    unsigned char IT7             : 1;
  } EIES0_bit;
} @ _M(0,0x13);


/* External Interrupt Edge Select 1 Register */
__no_init volatile __io union
{
  unsigned char EIES1;
  struct
  {
    unsigned char IT0             : 1;
    unsigned char IT1             : 1;
    unsigned char IT2             : 1;
    unsigned char IT3             : 1;
    unsigned char IT4             : 1;
    unsigned char IT5             : 1;
    unsigned char IT6             : 1;
    unsigned char IT7             : 1;
  } EIES1_bit;
} @ _M(0,0x14);


/* Pad Drive Strength Port 0 Register */
__no_init volatile __io union 
{
  unsigned char PS0;
  struct
  {
    unsigned char Bit0        :1;
    unsigned char Bit1        :1;
    unsigned char Bit2        :1;
    unsigned char Bit3        :1;
    unsigned char Bit4        :1;
    unsigned char Bit5        :1;
    unsigned char Bit6        :1;
    unsigned char Bit7        :1;
  } PS0_bit;
} @ _M(0,0x18); 


/* Pad Drive Strength Port 1 Register */
__no_init volatile __io union
{
  unsigned char PS1;
  struct
  {
    unsigned char Bit0        :1;
    unsigned char Bit1        :1;
    unsigned char Bit2        :1;
    unsigned char Bit3        :1;
    unsigned char Bit4        :1;
    unsigned char Bit5        :1;
    unsigned char Bit6        :1;
    unsigned char Bit7        :1;
  } PS1_bit;
} @ _M(0,0x19); 


/* Pad Resistive Pull Direction Port 0 Register */
__no_init volatile __io union
{
  unsigned char PR0;
  struct
  {
    unsigned char Bit0        :1;
    unsigned char Bit1        :1;
    unsigned char Bit2        :1;
    unsigned char Bit3        :1;
    unsigned char Bit4        :1;
    unsigned char Bit5        :1;
    unsigned char Bit6        :1;
    unsigned char Bit7        :1;
  } PR0_bit;
} @ _M(0,0x1B); 


/* Pad Resistive Pull Direction Port 1 Register */
__no_init volatile __io union
{
  unsigned char PR1;
  struct
  {
    unsigned char Bit0        :1;
    unsigned char Bit1        :1;
    unsigned char Bit2        :1;
    unsigned char Bit3        :1;
    unsigned char Bit4        :1;
    unsigned char Bit5        :1;
    unsigned char Bit6        :1;
    unsigned char Bit7        :1;
  } PR1_bit;
} @ _M(0,0x1C);


/*-------------------------------*
 *           Module 1            *
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
} @ _M(1,0x0);


__no_init volatile __io unsigned short MA  @ _M(1,1); /* Multiplier Operand A Register */

__no_init volatile __io unsigned short MB  @ _M(1,2); /* Multiplier Operand B Register */

__no_init volatile __io unsigned short MC2 @ _M(1,3); /* Multiplier Accumulate Register 2 */

__no_init volatile __io unsigned short MC1 @ _M(1,4); /* Multiplier Accumulate Register 1 */

__no_init volatile __io unsigned short MC0 @ _M(1,5); /* Multiplier Accumulate Register 0 */


/* SPI Data Buffer Register */
__no_init volatile __io unsigned short SPIB @ _M(1,6);


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
} @ _M(1,0x07);


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
} @ _M(1,0x08);


__no_init volatile __io unsigned char SPICK @ _M(1,0x09);   /* SPI Clock Register */


/* Flash Control Register */
__no_init volatile __io union
{
  unsigned char FCNTL;
  struct
  {
    unsigned char FC0             : 1;
    unsigned char FC1             : 1;
    unsigned char FC2             : 1;
    unsigned char                 : 1;
    unsigned char                 : 1;
    unsigned char                 : 1;
    unsigned char                 : 1;
    unsigned char FBUSY           : 1;
  } FCNTL_bit;
} @ _M(1,0x0A);


__no_init volatile __io unsigned short FDATA @ _M(1,0x0B);  /* Flash Data Register */

__no_init volatile __io unsigned short MC1R @ _M(1,0x0C);   /* Multiplier Read Register 1 */

__no_init volatile __io unsigned short MC0R @ _M(1,0x0D);   /* Multiplier Read Register 2 */


/* System Timer Control Register */
__no_init volatile __io union
{
  unsigned short SCNT;
  struct
  {
    unsigned short STIME          : 1;
    unsigned short SALME          : 1;
    unsigned short                : 1;
    unsigned short                : 1;
    unsigned short                : 1;
    unsigned short                : 1;
    unsigned short                : 1;    /* FT delete */
	unsigned short SALMF          : 1;    /* FT Add    */
    unsigned short SSYNC_EN       : 1;
    unsigned short STDIV          : 3;
    unsigned short                : 1;
  } SCNT_bit;
} @ _M(1,0x0E);


__no_init volatile __io unsigned short  STIM @ _M(1,0x0F);        /* System Timer Register */

__no_init volatile __io unsigned short  SALM @ _M(1,0x10);        /* System Alarm Register */

__no_init volatile __io unsigned short  RCTRM @ _M(1,0x17);       /* RC Oscillator Trim Register */

__no_init volatile __io unsigned short  ID0 @ _M(1,0x1C);         /* Identity Register 0 */

__no_init volatile __io unsigned short  ID1 @ _M(1,0x1D);         /* Identity Register 1 */

__no_init volatile __io unsigned short  ID2 @ _M(1,0x1E);         /* Identity Register 2 */

__no_init volatile __io unsigned short  ID3 @ _M(1,0x1F);         /* Identity Register 3 */


/*-------------------------------*
 *           Module 2            *
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
} @ _M(2,0);


__no_init volatile __io unsigned char T2H0 @ _M(2,1);   /* Timer 0: Type 2 Most Significant Byte Value */

__no_init volatile __io unsigned char T2RH0 @ _M(2,2);   /* Timer 0: Type 2 Most Significant Byte Reload */

__no_init volatile __io unsigned char T2CH0 @ _M(2,3);   /* Timer 0: Type 2 Most Significant Byte Capture/Compare */


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
} @ _M(2,4);


__no_init volatile __io unsigned char T2H1 @ _M(2,5);   /* Timer 1: Type 2 Most Significant Byte Value */

__no_init volatile __io unsigned char T2RH1 @ _M(2,6);   /* Timer 1: Type 2 Most Significant Byte Reload */

__no_init volatile __io unsigned char T2CH1 @ _M(2,7);   /* Timer 1: Type 2 Most Significant Byte Capture/Compare */


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
} @ _M(2,0x08);


__no_init volatile __io unsigned short T2V0 @ _M(2,0x09);   /* Timer 0: Type 2 Value Register */

__no_init volatile __io unsigned short T2R0 @ _M(2,0x0A);   /* Timer 0: Type 2 Reload Register */

__no_init volatile __io unsigned short T2C0 @ _M(2,0x0B);   /* Timer 0: Type 2 Capture/Compare Register */



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
} @ _M(2,0x0C);



__no_init volatile __io unsigned short T2V1 @ _M(2,0x0D);   /* Timer 1: Type 2 Value Register */

__no_init volatile __io unsigned short T2R1 @ _M(2,0x0E); /* Timer 1: Type 2 Reload Register */

__no_init volatile __io unsigned short T2C1 @ _M(2,0x0F); /* Timer 1: Type 2 Capture/Compare Register */


/* Timer 0: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG0;
  struct
  {
    unsigned char CT                : 1;
    unsigned char CCF               : 2;
    unsigned char T2MD              : 1;
    unsigned char T2DIV             : 3;
    unsigned char T2CI              : 1;
  } T2CFG0_bit;
} @ _M(2,0x10);


/* Timer 1: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG1;
  struct
  {
    unsigned char CT                : 1;
    unsigned char CCF               : 2;
    unsigned char T2MD              : 1;
    unsigned char T2DIV             : 3;
    unsigned char T2CI              : 1;
  } T2CFG1_bit;
} @ _M(2,0x11);


/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

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
} @ _M(3,0);


__no_init volatile __io unsigned char T2H2  @ _M(3,1);   /* Timer 2: Type 2 Most Significant Byte Value */

__no_init volatile __io unsigned char T2RH2 @ _M(3,2);  /* Timer 2: Type 2 Most Significant Byte Reload */

__no_init volatile __io unsigned char T2CH2 @ _M(3,3);  /* Timer 2: Type 2 Most Significant Byte Capture/Compare */


/*  Control Register 1 */
__no_init volatile __io union
{
  unsigned char CNT1;
  struct
  {
    unsigned char FL              : 6;
    unsigned char CK              : 1;
    unsigned char RTN             : 1;
  } CNT1_bit;
} @ _M(3,5);


/*  Serial Control Register  */
__no_init volatile __io union
{
  unsigned char SCON;
  struct
  {
    unsigned char RI              : 1;
    unsigned char TI              : 1;
    unsigned char RB8             : 1;
    unsigned char TB8             : 1;
    unsigned char REN             : 1;
    unsigned char SM2             : 1;
    unsigned char SM1             : 1;
    unsigned char SM0             : 1;
  } SCON_bit;
} @ _M(3,6);


__no_init volatile __io unsigned char SBUF @ _M(3,7);  /* Serial Port Buffer Register */


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
} @ _M(3,0x08);

__no_init volatile __io unsigned short T2V2 @ _M(3,0x09); /* Timer 2: Type 2 Value Register */

__no_init volatile __io unsigned short T2R2 @ _M(3,0x0A); /* Timer 2: Type 2 Reload Register */

__no_init volatile __io unsigned short T2C2 @ _M(3,0x0B); /* Timer 2: Type 2 Capture/Compare Register */


/* FIFO Status Register  */
__no_init volatile __io union
{
  unsigned char FSTAT;
  struct
  {
    unsigned char RFE     : 1;
    unsigned char RFAF    : 1;
    unsigned char RFF     : 1;
    unsigned char TFE     : 1;
    unsigned char TFAE    : 1;
    unsigned char TFF     : 1;
    unsigned char         : 1;
    unsigned char         : 1;
  } FSTAT_bit;
} @ _M(3,0x0C);


/* Error Register  */
__no_init volatile __io union
{
  unsigned char ERRR;
  struct
  {
    unsigned char P0E     : 1;
    unsigned char P0      : 1;
    unsigned char PIE     : 1;
    unsigned char P1      : 1;
    unsigned char CKE     : 1;
    unsigned char DME     : 1;
    unsigned char OTE     : 1;
    unsigned char         : 1;
  } ERRR_bit;
} @ _M(3,0x0D);


__no_init volatile __io unsigned short CHKSUM @ _M(3,0x0E); /* Checksum Register */

__no_init volatile __io unsigned short ISVEC @ _M(3,0x0F); /* Interrupt State Vector Register */


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
} @ _M(3,0x010);


/* Status  Register 0 */
__no_init volatile __io union
{
  unsigned char STA0;
  struct
  {
    unsigned char BUSY            : 1;
    unsigned char INP             : 1;
    unsigned char                 : 6;
  } STA0_bit;
} @ _M(3,0x011);


/* Serial Mode Register  */
__no_init volatile __io union
{
  unsigned char SMD;
  struct
  {
    unsigned char FEDE            : 1;
    unsigned char SMOD            : 1;
    unsigned char IE              : 1;
    unsigned char                 : 1;
    unsigned char                 : 1;
    unsigned char                 : 1;
    unsigned char OFS             : 1;
    unsigned char EIR             : 1;  
  } SMD_bit;
} @ _M(3,0x012);


/* FIFO Control Register  */
__no_init volatile __io union
{
  unsigned char FCON;
  struct
  {
    unsigned char FEN             : 1;
    unsigned char OE              : 1;
    unsigned char RXFT            : 2;
    unsigned char TXFT            : 2;
    unsigned char FRF             : 1;
    unsigned char FTF             : 1;
  } FCON_bit;
} @ _M(3,0x013);


/* Control Register 0  */
__no_init volatile __io union
{
  unsigned char CNT0;
  struct
  {
    unsigned char LUN             : 2;
    unsigned char INIT            : 1;
    unsigned char AUT             : 1;
    unsigned char INE             : 1;
    unsigned char FP              : 2;
    unsigned char WU              : 1;
  } CNT0_bit;
} @ _M(3,0x014);


/* Control Register 2  */
__no_init volatile __io union
{
  unsigned char CNT2;
  struct
  {
    unsigned char BTH             : 1;
    unsigned char FBS             : 1;
    unsigned char HDO             : 1;
    unsigned char PM              : 1;
    unsigned char DMIS            : 1;
    unsigned char                 : 3;
  } CNT2_bit;
} @ _M(3,0x015);


__no_init volatile __io unsigned short IDFB @ _M(3,0x016);    /* Identifier Boundary Register */

__no_init volatile __io unsigned char SADDR @ _M(3,0x017);   /* Serial Address Register */

__no_init volatile __io unsigned char SADEN @ _M(3,0x018);   /* Serial Address Mask Register */

__no_init volatile __io unsigned short BT @ _M(3,0x019);      /* Bit Timing Register */

__no_init volatile __io unsigned short TMR @ _M(3,0x01A);     /* Timing Register */


/*-------------------------------*
 *           Module 4 - Empty    *
 *-------------------------------*/



/*-------------------------------*
 *           Module 5            *
 *-------------------------------*/




/* Burst Pulse High Width Control Register */
__no_init volatile __io union
{
  unsigned short BPH;
  struct
  {
    unsigned short  BPH           :10;
    unsigned short                :4;
    unsigned short  BDS           :1;
    unsigned short  BSTT          :1;
  } BPH_bit;
} @ _M(5,0x00);      


/* Burst Transmit Control Register */
__no_init volatile __io union
{
  unsigned short BTRN;
  struct
  {
    unsigned short  BCNT          :8;
    unsigned short  BGT           :1;
    unsigned short  BTRI          :1;
    unsigned short  BCKS          :1;
    unsigned short  BPOL          :1;
    unsigned short  BDIV          :4;
  } BTRN_bit;
} @ _M(5,0x01);     


/* SAR ADC Control Register */
__no_init volatile __io union
{
  unsigned short SARC;
  struct
  {
    unsigned short  SARS          :3;
    unsigned short  SARBY         :1;
    unsigned short  SARASD        :1;
    unsigned short  SARRSEL       :1;
    unsigned short  SARDUL        :1;
    unsigned short  SARBIP        :1;
    unsigned short  SARDIF        :1;
    unsigned short  SARMX         :3;
    unsigned short                :4;
  } SARC_bit;
} @ _M(5,0x02);     


/* Echo Receive Path Control Register */
__no_init volatile __io union
{
  unsigned short RCVC;
  struct
  {
    unsigned short  RCVGN         :5;
    unsigned short                :1;
    unsigned short  LNAISEL       :2;
    unsigned short  LNAOSEL       :1;
    unsigned short                :7;
  } RCVC_bit;
} @ _M(5,0x03);     


/* PLL Based Programmable Oscillator Control Register */
__no_init volatile __io union
{
  unsigned short PLLF;
  struct
  {
    unsigned short  PLLF          :9;
    unsigned short  PLLC          :2;
    unsigned short                :5;
  } PLLF_bit;
} @ _M(5,0x04);     


/* Analog Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned char AIE;
  struct
  {
    unsigned char SARIE             : 1;
    unsigned char LPFIE             : 1;
    unsigned char LFLIE             : 1;
    unsigned char CMPIE             : 1;
    unsigned char VABIE             : 1;
    unsigned char VDBIE             : 1;
    unsigned char VIBIE             : 1;
    unsigned char XTIE              : 1;
  } AIE_bit;
} @ _M(5,0x05);


/* Echo Envelope Comparator Control Register */
__no_init volatile __io union
{
  unsigned short CMPC;
  struct
  { 
    unsigned short CMPH             : 15;
    unsigned short CMPP             : 1;
  } CMPC_bit;
} @ _M(5,0x06);     


__no_init volatile __io unsigned short CMPT @ _M(5,0x07);     /* Echo Envelope Comparator Threshold Register */


/* Analog Status Register */
__no_init volatile __io union
{
  unsigned short ASR;
  struct
  {
    unsigned short SARRDY           : 1;
    unsigned short LPFRDY           : 1;
    unsigned short LPFFL            : 1;
    unsigned short CMPI             : 1;
    unsigned short VABI             : 1;
    unsigned short VDBI             : 1;
    unsigned short VIBI             : 1;
    unsigned short XTI              : 1;
    unsigned short XTRDY            : 1;
    unsigned short                  : 1;
    unsigned short                  : 1;
    unsigned short                  : 1;
    unsigned short CMPLVL           : 1;
    unsigned short AVLVL            : 1;
    unsigned short DVLVL            : 1;
    unsigned short VIOLVL           : 1;
  } ASR_bit;
} @ _M(5,0x08);


__no_init volatile __io unsigned short SARD @ _M(5,0x09);     /* SAR ADC Output Data Register */


/* Echo Envelope Low Pass Filter Control Register */
__no_init volatile __io union
{
  unsigned short LPFC; 
  struct
  {
    unsigned short  FFLS            : 3;  
    unsigned short  FFLD            : 1;
    unsigned short                  : 3;
    unsigned short  FFOV            : 1;
    unsigned short  FFDP            : 4;
    unsigned short  FFIL            : 4;
  } LPFC_bit;  
}   @ _M(5,0x0A);     


/* Oscillator Control Register */
__no_init volatile __io union
{
  unsigned short OSCC;
  struct
  {
    unsigned short RCE               : 1;
    unsigned short XTE               : 1;
    unsigned short SARCD             : 2;
//    unsigned char                   : 12;
  } OSCC_bit;
} @ _M(5,0x0B);


__no_init volatile __io unsigned short BPFI @ _M(5,0x0C);     /* Echo Envelope Band Pass Filter Input Data Register */

__no_init volatile __io unsigned short BPFO @ _M(5,0x0D);     /* Echo Envelope Band Pass Filter Output Data Register */

__no_init volatile __io unsigned short LPFD @ _M(5,0x0E);     /* Echo Envelope Low Pass Filter Output Data Register */

__no_init volatile __io unsigned short LPFF @ _M(5,0x0F);     /* Echo Envelope Low Pass Filter FIFO Output Data Register */


/* Analog Power Enable Register */
__no_init volatile __io union
{
  unsigned short APE;
  struct
  {
    unsigned short BIASE           : 1;
    unsigned short LNAE            : 1;
    unsigned short MDE             : 1;
    unsigned short PLLE            : 1;
    unsigned short SARE            : 1;
    unsigned short VABE            : 1;
    unsigned short VDBE            : 1;
    unsigned short VDPE            : 1;
    unsigned short VIBE            : 1;
    unsigned short LRAPD           : 1;
    unsigned short LRDPD           : 1;
    unsigned short LRIOPD          : 1;
    unsigned short BGE             : 1;
    unsigned short RSARE           : 1;
    unsigned short RECHOE          : 1;
    unsigned short                 : 1;
  } APE_bit;
} @ _M(5,0x010);




__no_init volatile __io unsigned short FGAIN  @ _M(5,0x012);      /* Filter Gain Register */

__no_init volatile __io unsigned short B1COEF @ _M(5,0x013);      /* B1 Coeffecient Register */

__no_init volatile __io unsigned short B2COEF @ _M(5,0x014);      /* B2 Coeffecient Register */

__no_init volatile __io unsigned short B3COEF @ _M(5,0x015);      /* B3 Coeffecient Register */

__no_init volatile __io unsigned short A2A    @ _M(5,0x016);      /* A2A Coeffecient Register */

__no_init volatile __io unsigned short A2B    @ _M(5,0x017);      /* A2B Coeffecient Register */

__no_init volatile __io unsigned short A2C    @ _M(5,0x018);      /* A2C Coeffecient Register */

__no_init volatile __io unsigned short A2D    @ _M(5,0x019);      /* A2D Coeffecient Register */

__no_init volatile __io unsigned short A2E    @ _M(5,0x01A);      /* A2E Coeffecient Register */

__no_init volatile __io unsigned short A3A    @ _M(5,0x01B);      /* A3A Coeffecient Register */

__no_init volatile __io unsigned short A3B    @ _M(5,0x01C);      /* A3B Coeffecient Register */

__no_init volatile __io unsigned short A3C    @ _M(5,0x01D);      /* A3C Coeffecient Register */

__no_init volatile __io unsigned short A3D    @ _M(5,0x01E);      /* A3D Coeffecient Register */

__no_init volatile __io unsigned short A3E    @ _M(5,0x01F);      /* A3E Coeffecient Register */


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
    unsigned char RCMD           : 1;
    unsigned char                : 1;
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
    unsigned char WD             : 2;
    unsigned char EWDI           : 1;
    unsigned char POR            : 1;
  } WDCN_bit;
} @ _M(8,0x0F);

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ7667 */

