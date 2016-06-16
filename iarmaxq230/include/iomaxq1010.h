/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ1010.
 *
 * This file supports assembler and C development for MAXQ1010
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1528 $
 *
 ********************************************************************/


#ifndef __IOMAXQ1010
#define __IOMAXQ1010

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


/* External Interrupt Enable 1 Register */
__no_init volatile __io union
{
  unsigned char EIE1;
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
  } EIE1_bit;
} @ _M(0x0, 0x5);

/* External Interrupt Flag 3 Register */
__no_init volatile __io union
{
  unsigned char EIF3;
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
  } EIF3_bit;
} @ _M(0,6);


/* External Interrupt Enable 3 Register  */
__no_init volatile __io union
{
  unsigned char EIE3;
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
  } EIE3_bit;
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

/* External Interrupt Edge Select 1 Register */
__no_init volatile __io union
{
  unsigned char EIES1;
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
  } EIES1_bit;
} @ _M(0,0xC);

/* External Interrupt Edge Select 3 Register */
__no_init volatile __io union
{
  unsigned char EIES3;
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
  } EIES3_bit;
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

/* Flash Programming Clock Divide Register */
__no_init volatile __io unsigned char FCKDV @ _M(0,0x14);

/* Software Breakpoint Register */
__no_init volatile __io union
{
  unsigned char BRKP;
  struct
  {
    unsigned char BREAK         : 1;
    unsigned char BPT           : 7;
  } BRKP_bit;
} @ _M(0,0x16);

/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

/* USB Register Address Register */
__no_init volatile __io union
{
    unsigned char UADDR;
    struct
    {
        unsigned char UADDR     : 5;
        unsigned char           : 1;
        unsigned char UBUSY     : 1;
        unsigned char USBRW     : 1;
    } UADDR_bit;
} @ _M(1,0);

/* USB Data Register */
__no_init volatile __io unsigned char UDATA @ _M(1,1);

/* Wake-up Timer Control Register */
__no_init volatile __io union
{
  unsigned char WUTC;
  struct
  {
    unsigned char WTE    : 1;
    unsigned char WTF    : 1;
  } WUTC_bit;
} @ _M(1,4);

/* Wake-up Timer Value Register */
__no_init volatile __io unsigned short WUT @ _M(1,5);

/* Self-destruct Register */
__no_init volatile __io union
{
  unsigned char SDI;
  struct
  {
    unsigned char SDIF          :1;
    unsigned char SDIC          :1;
    unsigned char SDIE          :1;
  } SDI_bit;
} @ _M(1,0xB);

/* Power Control Register */
__no_init volatile __io union
{
  unsigned short PWCN;
  struct
  {
    unsigned short PFD          :1;
    unsigned short PFIE         :1;
    unsigned short PFI          :1;
    unsigned short REGEN        :1;
    unsigned short              :1;
    unsigned short OWCKS        :1;
    unsigned short RNRF         :1;
    unsigned short PFRST        :1;
    unsigned short PFRCK        :2;
    unsigned short FRCVDD       :1;
    unsigned short CTMS         :1;
    unsigned short CTM          :1;
    unsigned short CKRY         :1;
    unsigned short PFWARNCN     :2;
  } PWCN_bit;
} @ _M(1,0xC);

/*-------------------------------*
 *           Module 2            *
 *-------------------------------*/

/* I2C Data Buffer Register */
__no_init volatile __io unsigned char I2CBUF @ _M(2,0);

/* I2C Control Register */
__no_init volatile __io union
{
  unsigned short I2CCN;
  struct
  {
    unsigned short I2CEN     : 1;
    unsigned short I2CMST    : 1;
    unsigned short I2CMODE   : 1;
    unsigned short I2CEA     : 1;
    unsigned short I2CSTRS   : 1;
    unsigned short I2CACK    : 1;
    unsigned short I2CSTART  : 1;
    unsigned short I2CSTOP   : 1;
    unsigned short I2CGCEN   : 1;
    unsigned short I2CSTREN  : 1;
    unsigned short           : 5;
    unsigned short I2CRST    : 1;
  } I2CCN_bit;
} @ _M(2,1);

/* I2C Clock Control Register */
__no_init volatile __io union
{
  unsigned short I2CCK;
  struct
  {
    unsigned short I2CCKL    : 8;
    unsigned short I2CCKH    : 8;
  } I2CCK_bit;
} @ _M(2,2);

/* I2C Timeout Register */
__no_init volatile __io unsigned char I2CTO @ _M(2,3);

/* I2C Slave Address Register */
__no_init volatile __io unsigned short I2CSLA @ _M(2,4);

/* I2C Status Register */
__no_init volatile __io union
{
  unsigned short I2CST;
  struct
  {
    unsigned short I2CSRI    : 1;
    unsigned short I2CTXI    : 1;
    unsigned short I2CRXI    : 1;
    unsigned short I2CSTRI   : 1;
    unsigned short I2CTOI    : 1;
    unsigned short I2CAMI    : 1;
    unsigned short I2CALI    : 1;
    unsigned short I2CNACKI  : 1;
    unsigned short I2CGCI    : 1;
    unsigned short I2CROI    : 1;
    unsigned short I2CSCL    : 1;
    unsigned short I2CSPI    : 1;
    unsigned short           : 2;
    unsigned short I2CBUSY   : 1;
    unsigned short I2CBUS    : 1;
  } I2CST_bit;
} @ _M(2,5);

/* I2C Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned short I2CIE;
  struct
  {
    unsigned short I2CSRIE   : 1;
    unsigned short I2CTXIE   : 1;
    unsigned short I2CRXIE   : 1;
    unsigned short I2CSTRIE  : 1;
    unsigned short I2CTOIE   : 1;
    unsigned short I2CAMIE   : 1;
    unsigned short I2CALIE   : 1;
    unsigned short I2CNACKIE : 1;
    unsigned short I2CGCIE   : 1;
    unsigned short I2CROIE   : 1;
    unsigned short           : 1;
    unsigned short I2CSPIE   : 1;
    unsigned short           : 4;
  } I2CIE_bit;
} @ _M(2,6);

/*-------------------------------*
 *           Module 3            *
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
} @ _M(3,0);

/* Serial Port Data Buffer */
__no_init volatile __io unsigned char SBUF @ _M(3,1);   

/* SPI Data Buffer Register */
__no_init volatile __io unsigned short SPIB @ _M(3,2);

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
} @ _M(3,3);

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
} @ _M(3,4);

/* ISO UART Data Buffer Register */
__no_init volatile __io unsigned short UTR @ _M(3,5);

/* ISO UART Status Register */
__no_init volatile __io union
{
  unsigned char UST;
  struct
  {
    unsigned char TBE_RBF        : 1;
    unsigned char FER            : 1;
    unsigned char PE             : 1;
    unsigned char FE             : 1;
    unsigned char BGT            : 1;
    unsigned char DISTBE_RBF     : 1;
    unsigned char EUI            : 1;
  } UST_bit;
} @ _M(3,6);

/* ISO UART Extra Guard Time Register */
__no_init volatile __io unsigned char UGT @ _M(3,7);

/* Serial Port Phase Register */
__no_init volatile __io unsigned short PR @ _M(3,8);

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
} @ _M(3,9);

/* ISO UART Control Register */
__no_init volatile __io union
{
  unsigned short UCNT;
  struct
  {
    unsigned short CONV           : 1;
    unsigned short SCS            : 1;
    unsigned short LCT            : 1;
    unsigned short TR             : 1;
    unsigned short PROT           : 1;
    unsigned short AUTOC          : 1;
    unsigned short FIP            : 1;
    unsigned short ISUEN          : 1;
    unsigned short FL             : 3;
    unsigned short FTE            : 1;
    unsigned short PEC            : 3;
  } UCNT_bit;
} @ _M(3,0xA);

/* ISO UART Clock Register */
__no_init volatile __io union
{
  unsigned short UCK;
  struct
  {
    unsigned short PD             : 8;
    unsigned short AC             : 3;
    unsigned short PSC            : 1;
  } UCK_bit;
} @ _M(3,0xB);

__no_init volatile __io unsigned char SPICK @ _M(3,0xC);

/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/

/* User DES Engine Data Register */
__no_init volatile __io unsigned short UDESD @ _M(4,0);

/* User DES Engine Control Register */
__no_init volatile __io union
{
  unsigned char UDESC;
  struct
  {
    unsigned char COS            : 2;
    unsigned char ROS            : 3;
    unsigned char                : 2;
    unsigned char PSE            : 1;
  } UDESC_bit;
} @ _M(4,1);

/* AES Engine Control Register */
__no_init volatile __io union
{
  unsigned char AESC; 
  struct
  {
    unsigned char AESST          : 1;
    unsigned char AESMD          : 1;
    unsigned char SEK            : 1;
    unsigned char KS             : 2;
    unsigned char AESIE          : 1;
    unsigned char AESIF          : 1;
    unsigned char AESER          : 1;
  } AESC_bit;
} @ _M(4,2);

/* Timer 1 Instance 0 Control Register */
__no_init volatile __io union
{
  unsigned char T1CN0;
  struct
  {
    unsigned char CP_RL1         : 1;
    unsigned char C_T1           : 1;
    unsigned char TR1            : 1;
    unsigned char EXEN1          : 1;
    unsigned char DCEN           : 1;
    unsigned char T1OE           : 1;
    unsigned char EXF1           : 1;
    unsigned char TF1            : 1;
  } T1CN0_bit;
} @ _M(4,4);

/* Timer 1 Instance 0 Value Register */
__no_init volatile __io unsigned short T1L0 @ _M(4,5);

/* Timer 1 Instance 1 Control Register */
__no_init volatile __io union
{
  unsigned char T1CN1;
  struct
  {
    unsigned char CP_RL1         : 1;
    unsigned char C_T1           : 1;
    unsigned char TR1            : 1;
    unsigned char EXEN1          : 1;
    unsigned char DCEN           : 1;
    unsigned char T1OE           : 1;
    unsigned char EXF1           : 1;
    unsigned char TF1            : 1;
  } T1CN1_bit;
} @ _M(4,6);

/* Timer 1 Instance 1 Value Register */
__no_init volatile __io unsigned short T1L1 @ _M(4,7);

/* Random Number Register */
__no_init volatile __io unsigned short RNR @ _M(4,8);

/* Timer 1 Instance 0 Capture Register */
__no_init volatile __io unsigned short T1CL0 @ _M(4,9);

/* Timer 1 Instance 0 Mode Register */
__no_init volatile __io union
{
  unsigned char T1MD0;
  struct
  {
    unsigned char T1M            : 1;
    unsigned char ET1            : 1;
  } T1MD0_bit;
} @ _M(4,0xA);

/* Timer 1 Instance 1 Capture Register */
__no_init volatile __io unsigned short T1CL1 @ _M(4,0xB);

/* Timer 1 Instance 1 Mode Register */
__no_init volatile __io union
{
  unsigned char T1MD1;
  struct
  {
    unsigned char T1M            : 1;
    unsigned char ET1            : 1;
  } T1MD1_bit;
} @ _M(4,0xC);

/* Random Number Generator Entropy Register */
__no_init volatile __io unsigned char RNGE @ _M(4,0x10);

/* RTC Trim Register */
__no_init volatile __io union
{
  unsigned char RTRM;
  struct
  {
    unsigned char TRIM           : 7;
    unsigned char TSGN           : 1;
  } RTRM_bit;
} @ _M(4,0x18);


/* RTC Control Register */
__no_init volatile __io union
{
  unsigned short RCNT;
  struct
  {
    unsigned short RTCE          : 1;
    unsigned short ADE           : 1;
    unsigned short ASE           : 1;
    unsigned short BUSY          : 1;
    unsigned short RDY           : 1;
    unsigned short RDYE          : 1;
    unsigned short ALDF          : 1;
    unsigned short ALSF          : 1;
    unsigned short SQE           : 1;
    unsigned short FT            : 1;
    unsigned short X32KMD        : 2;
    unsigned short BYP32K        : 1;
    unsigned short X32RDY        : 1;
    unsigned short X32D          : 1;
    unsigned short WE            : 1;

  } RCNT_bit;
} @ _M(4,0x19);

/* RTC Sub-Second Counter Register */
__no_init volatile __io union
{
  unsigned short RTSS;
  struct
  {
    unsigned short UIP           : 1;
    unsigned short COUNT         : 15;

  } RTSS_bit;
} @ _M(4,0x1A);


/* RTC Second Counter High Register */
__no_init volatile __io unsigned short RTSH @ _M(4,0x1B);

/* RTC Second Counter Low Register */
__no_init volatile __io unsigned short RTSL @ _M(4,0x1C);

/* RTC Sub-second Alarm Low Register */
__no_init volatile __io unsigned short RSSA @ _M(4,0x1D);

/* RTC Sub-second Alarm High Register */
__no_init volatile __io unsigned char RASH @ _M(4,0x1E);

/* RTC Alarm Time-of-Day Low Register */
__no_init volatile __io unsigned short RASL @ _M(4,0x1F);

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
#endif /* __IOMAXQ1010 */


