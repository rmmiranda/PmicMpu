/********************************************************************
 *
 *  Register and bit definitions for Dallas Semiconductor/Maxim
 *  MAXQ766x.
 *
 *  This file supports assembler and C development for MAXQ7666 (UC28)
 *
 *  Copyright 2003 IAR Systems. All rights reserved.
 *
 *  Author: David Voutour
 *
 *  Created: 12/01/05 from MAXQ765x.h file
 *
 *  Modifications: 12/05/05 Added MC1R and SBUF0 registers	
 *
 * Version 2.10a
 *      Modifications: 10/04/06 Added missing items, please see labels
 *            Faisal            with Faisal written infront.
 *                              
 *                              The following Registers have been removed
 *                              from the previous
 *                              FCNTL, ICDT0, ICDT1, ICDC, ICDF, ICDB, 
 *                              ICDA, ICDD, TM
 *
 *		 
 *		   
 *
 *                  
 *
 ********************************************************************/


#ifndef __IOMAXQ7666   /* Faisal */
#define __IOMAXQ7666   /* Faisal */

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
    unsigned char IE6             : 1;  /* Added by faisal */
    unsigned char IE7             : 1;  /* Added by faisal */
  } EIF0_bit;
} @ _M(0,3);


__no_init volatile __io unsigned char SBUF0 @ _M(0,7);   /* Serial Port 0 Data Buffer Register */


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
    unsigned char EX6             : 1;   /* Added by Faisal */
    unsigned char EX7             : 1;   /* Added by Faisal */
   } EIE0_bit;
} @ _M(0, 0xB);


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
    unsigned char IT6             : 1; /* Added by Faisal */
    unsigned char IT7             : 1; /* Added by Faisal */
  } EIES0_bit;
} @ _M(0,0x13);


/* Serial Port Control Register */
__no_init volatile __io union
{
  unsigned char SCON0;
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
  } SCON0_bit;
} @ _M(0,0x1D);


/* Serial Port Mode Register */
__no_init volatile __io union
{
  unsigned char SMD0;
  struct
  {
    unsigned char FEDE            : 1;
    unsigned char SMOD            : 1;
    unsigned char ESI             : 1;
  } SMD0_bit;
} @ _M(0,0x1E);


__no_init volatile __io unsigned short PR0 @ _M(0,0x1F); /* Phase Register 0 */



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

__no_init volatile __io unsigned short MC1R @ _M(1,0x0C);   /* Multiplier Read Register 1 */

__no_init volatile __io unsigned short MC0R @ _M(1,0x0D);   /* Multiplier Read Register 2 */


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
    unsigned char CT                : 1;  /* change to C_T2- Faisal */
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
    unsigned char CT                : 1;  /* change to C_T2 - Faisal */
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



__no_init volatile __io unsigned char T2H2 @ _M(3,1);   /* Timer 2: Type 2 Most Significant Byte Value */

__no_init volatile __io unsigned char T2RH2 @ _M(3,2);  /* Timer 2: Type 2 Most Significant Byte Reload */

__no_init volatile __io unsigned char T2CH2 @ _M(3,3);  /* Timer 2: Type 2 Most Significant Byte Capture/Compare */




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
    unsigned char                : 1;  /* Removed T2POL1 - Faisal */
    unsigned char                : 1;  /* Removed T2OE1  - Faisal */
    unsigned char ET2L           : 1;
  } T2CNB2_bit;
} @ _M(3,0x08);

__no_init volatile __io unsigned short T2V2 @ _M(3,0x09); /* Timer 2: Type 2 Value Register */

__no_init volatile __io unsigned short T2R2 @ _M(3,0x0A); /* Timer 2: Type 2 Reload Register */

__no_init volatile __io unsigned short T2C2 @ _M(3,0x0B); /* Timer 2: Type 2 Capture/Compare Register */



/* Timer 0: Type 2 Configuration Register */
__no_init volatile __io union
{
  unsigned char T2CFG2;
  struct
  {
    unsigned char CT             : 1;  /* change to C_T2- Faisal */
    unsigned char CCF            : 2;
    unsigned char T2MD           : 1;
    unsigned char T2DIV          : 3;
    unsigned char T2CI           : 1;
  } T2CFG2_bit;
} @ _M(3,0x010);




/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/



/* CAN Control Register */
__no_init volatile __io union
{
  unsigned char C0C;
  struct
  {
    unsigned char SWINT           : 1;
    unsigned char ERCS            : 1;
    unsigned char AUTOB           : 1;
    unsigned char CRST            : 1;
    unsigned char SIESTA          : 1;
    unsigned char PDE             : 1;
    unsigned char STIE            : 1;
    unsigned char ERIR            : 1;   /* change to ERIE - Faisal */
  } C0C_bit;
} @ _M(4,0x00);



/* CAN Status Register */
__no_init volatile __io union
{
  unsigned char C0S;
  struct
  {
    unsigned char ER0             : 1;
    unsigned char ER1             : 1;
    unsigned char ER2             : 1;
    unsigned char TXS             : 1;
    unsigned char RXS             : 1;
    unsigned char WKS             : 1;
    unsigned char EC96            : 1;  /* change to EC96_128 - Faisal */
    unsigned char BSS             : 1;
  } C0S_bit;
} @ _M(4,0x01);



/* CAN Interrupt Register */
__no_init volatile __io union
{
  unsigned char C0IR;
  struct
  {
    unsigned char INTIN0          : 1;
    unsigned char INTIN1          : 1;
    unsigned char INTIN2          : 1;
    unsigned char INTIN3          : 1;
    unsigned char INTIN4          : 1;
    unsigned char INTIN5          : 1;
    unsigned char INTIN6          : 1;
    unsigned char INTIN7          : 1;
  } C0IR_bit;
} @ _M(4,0x02);

                        /* changed short to char - Faisal */
__no_init volatile __io unsigned char C0TE @ _M(4,0x03); /* CAN Transmit Error Register */
                        /* changed short to char - Faisal */
__no_init volatile __io unsigned char C0RE @ _M(4,0x04); /* CAN Receive Error Register */


/* CAN Operation Control Register */
__no_init volatile __io union
{
  unsigned char COR;
  struct
  {
    unsigned char C0IE            : 1;
    unsigned char C0BIE           : 1;
    unsigned char                 : 1;
    unsigned char C0BPR6          : 1;
    unsigned char C0BPR7          : 1;
    unsigned char AID             : 1;
    unsigned char INDEC           : 1;
    unsigned char CAN0BA          : 1;
  } COR_bit;
} @ _M(4,0x05);


__no_init volatile __io unsigned short C0DP @ _M(4,0x06);  /* CAN Data Pointer Register */

__no_init volatile __io unsigned short C0DB @ _M(4,0x07);  /* CAN Data Buffer Register */

__no_init volatile __io unsigned short C0RMS @ _M(4,0x08); /* CAN Received Message Stored Register */

__no_init volatile __io unsigned short C0TMA @ _M(4,0x09); /* CAN Transmit Message Acknowledgment Register */


/* CAN Message 1 Control Register */
__no_init volatile __io union
{
  unsigned char C0M1C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M1C_bit;
} @ _M(4,0x011);


/* CAN Message 2 Control Register */
__no_init volatile __io union
{
  unsigned char C0M2C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M2C_bit;
} @ _M(4,0x012);


/* CAN Message 3 Control Register */
__no_init volatile __io union
{
  unsigned char C0M3C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M3C_bit;
} @ _M(4,0x013);


/* CAN Message 4 Control Register */
__no_init volatile __io union
{
  unsigned char C0M4C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M4C_bit;
} @ _M(4,0x014);


/* CAN Message 5 Control Register */
__no_init volatile __io union
{
  unsigned char C0M5C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M5C_bit;
} @ _M(4,0x015);


/* CAN Message 6 Control Register */
__no_init volatile __io union
{
  unsigned char C0M6C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M6C_bit;
} @ _M(4,0x016);


/* CAN Message 7 Control Register */
__no_init volatile __io union
{
  unsigned char C0M7C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M7C_bit;
} @ _M(4,0x017);


/* CAN Message 8 Control Register */
__no_init volatile __io union
{
  unsigned char C0M8C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M8C_bit;
} @ _M(4,0x018);


/* CAN Message 9 Control Register */
__no_init volatile __io union
{
  unsigned char C0M9C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M9C_bit;
} @ _M(4,0x019);


/* CAN Message 10 Control Register */
__no_init volatile __io union
{
  unsigned char C0M10C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M10C_bit;
} @ _M(4,0x01A);


/* CAN Message 11 Control Register */
__no_init volatile __io union
{
  unsigned char C0M11C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M11C_bit;
} @ _M(4,0x01B);


/* CAN Message 12 Control Register */
__no_init volatile __io union
{
  unsigned char C0M12C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M12C_bit;
} @ _M(4,0x01C);


/* CAN Message 13 Control Register */
__no_init volatile __io union
{
  unsigned char C0M13C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M13C_bit;
} @ _M(4,0x01D);


/* CAN Message 14 Control Register */
__no_init volatile __io union
{
  unsigned char C0M14C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M14C_bit;
} @ _M(4,0x01E);


/* CAN Message 15 Control Register */
__no_init volatile __io union
{
  unsigned char C0M15C;
  struct
  {
    unsigned char DTUP            : 1;
    unsigned char ROW_TIH         : 1;
    unsigned char MTRQ            : 1;
    unsigned char EXTRQ           : 1;
    unsigned char INTRQ           : 1;
    unsigned char ERI             : 1;
    unsigned char ETI             : 1;
    unsigned char MSRDY           : 1;
  } C0M15C_bit;
} @ _M(4,0x01F);



/*-------------------------------*
 *           Module 5            *
 *-------------------------------*/


/* Voltage Monitor Control Register */
__no_init volatile __io union
{
  unsigned char VMC;
  struct
  {
    unsigned char VDBR0             : 1;
    unsigned char VDBR1             : 1;
    unsigned char VDBI0             : 1;
    unsigned char VDBI1             : 1;
    unsigned char VIOBI0            : 1;
    unsigned char VIOBI1            : 1;
    unsigned char                   : 1; /* Removed VABI0 - Faisal */
    unsigned char                   : 1; /* Removed VABI1 - Faisal */
  } VMC_bit;
} @ _M(5,0x00);



/* Analog Power Enable Register */
__no_init volatile __io union
{
//  unsigned char APE;
  unsigned short APE;
  struct
  {
    unsigned short ADCE              : 1;
    unsigned short DAC1E             : 1;  /* change to DACE -Faisal */
    unsigned short                   : 1;
    unsigned short PGAE              : 1;
    unsigned short TSE               : 1;
    unsigned short PGG0              : 1;
    unsigned short PGG1              : 1;
    unsigned short PGG2              : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short VDPE              : 1;
    unsigned short VDBE              : 1;
    unsigned short VIBE              : 1;
  } APE_bit;
} @ _M(5,0x01);



/* ADC Control Register */
__no_init volatile __io union
{
  unsigned short ACNT;     /* changed char to short - Faisal */
  struct
  {
    unsigned short ADCS0             : 1;
    unsigned short ADCS1             : 1;
    unsigned short ADCS2             : 1;
    unsigned short ADCBY             : 1;
    unsigned short ADCASD            : 1;
    unsigned short                   : 1;
    unsigned short ADCDUL            : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short ADCBIP            : 1;
    unsigned short ADCDIF            : 1;
    unsigned short ADCMX0            : 1;
    unsigned short ADCMX1            : 1;
    unsigned short ADCMX2            : 1;
    unsigned short ADCMX3            : 1;
    unsigned short ADCMX4            : 1;
  } ACNT_bit;
} @ _M(5,0x02);



/* DAC Control Register */
__no_init volatile __io union
{
  unsigned char DCNT;
  struct
  {
    unsigned char                   : 1;
    unsigned char                   : 1;
    unsigned char                   : 1;
    unsigned char                   : 1;
    unsigned char DACLD0            : 1;
    unsigned char DACLD1            : 1;
    unsigned char DACLD2            : 1;
    unsigned char                   : 1;
  } DCNT_bit;
} @ _M(5,0x03);


__no_init volatile __io unsigned short DACI @ _M(5,0x04); /* DAC1 Input Register */

__no_init volatile __io unsigned short DACO @ _M(5,0x06); /* DAC1 Output Register */

__no_init volatile __io unsigned short ADCD @ _M(5,0x08);  /* ADC Data Register */

__no_init volatile __io unsigned short TSO @ _M(5,0x09);   /* Temperature Offset Register */



/* Analog Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned char AIE;
  struct
  {
    unsigned char                   : 1;
    unsigned char ADCIE             : 1;
    unsigned char AORIE             : 1;
    unsigned char                   : 1;
    unsigned char DVBIE             : 1;
    unsigned char VIOBIE            : 1;
    unsigned char HFFIE             : 1;
  } AIE_bit;
} @ _M(5,0x0A);


/* Analog Status Register */
__no_init volatile __io union
{
//  unsigned char ASR;
  unsigned short ASR;
  struct
  {
    unsigned short                   : 1;
    unsigned short ADCRY             : 1;
    unsigned short ADCOV             : 1;
    unsigned short                   : 1;
    unsigned short DVBI              : 1;
    unsigned short VIOBI             : 1;
    unsigned short HFFINT            : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short XHFRY             : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short DVLVL             : 1;
    unsigned short VIOLVL            : 1;
  } ASR_bit;
} @ _M(5,0x0B);



/* Oscillator Control Register */
__no_init volatile __io union
{
  unsigned short OSCC;
  struct
  {
    unsigned short HFE               : 1;
    unsigned short RCE               : 1;
    unsigned short EXTHF             : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short ADCCD0            : 1;
    unsigned short ADCCD1            : 1;
    unsigned short ADCCD2            : 1;
    unsigned short HFIC0             : 1;
    unsigned short HFIC1             : 1;
    unsigned short HFOC0             : 1;
    unsigned short HFOC1             : 1;
  } OSCC_bit;
} @ _M(5,0x0C);



/* One Time Programming Bit Register */
__no_init volatile __io union
{
  unsigned short OTP;
  struct
  {
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short OTP_AMDF          : 1;
    unsigned short ROMSZ0            : 1;
    unsigned short ROMSZ1            : 1;
    unsigned short                   : 9;   /* Faisal */
//    unsigned short                   : 1;
//    unsigned short                   : 1;
  } OTP_bit;
} @ _M(5,0x0D);



/* Analog Test Register */
__no_init volatile __io union
{
  unsigned short ATST;
  struct
  {
    unsigned short TSEL0             : 1;
    unsigned short TSEL1             : 1;
    unsigned short TSEL2             : 1;
    unsigned short TSEL3             : 1;
    unsigned short TSEL4             : 1;
    unsigned short TSEL5             : 1;
    unsigned short                   : 1;
    unsigned short                   : 1;
    unsigned short                   : 8;
  } ATST_bit;
} @ _M(5,0x0E);




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
    unsigned char                : 1;
    unsigned char XT             : 1;
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
#endif /* __IOMAXQ766X */

