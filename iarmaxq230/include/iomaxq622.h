/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ622.
 *
 * This file supports assembler and C development for MAXQ622
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 1541 $
 *
 ********************************************************************/


#ifndef __IOMAXQ622
#define __IOMAXQ622

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

/* Wake-Up Timer Control Register */
__no_init volatile __io union
{
  unsigned char WUTC;
  struct
  {
    unsigned char WTE    : 1;
    unsigned char WTF    : 1;
  } WUTC_bit;
} @ _M(1,4);

/* Wake-Up Timer Register */
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
} @ _M(1,8);

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
} @ _M(1,9);

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
  } PI9_bit;
} @ _M(1,0xA);

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
    unsigned short IRTXOE       :1;
    unsigned short IRTXOUT      :1;
    unsigned short IRRXWP       :1;
    unsigned short PFRST        :1;
    unsigned short PFRCK        :2;
    unsigned short FRCVDD       :1;
    unsigned short CTMS         :1;
    unsigned short CTM          :1;
    unsigned short CKRY         :1;
    unsigned short PFWARNCN     :2;
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

/*-------------------------------*
 *           Module 2            *
 *-------------------------------*/

/* Timer B 0 Capture/Reload Value Register */
__no_init volatile __io unsigned short TB0R @ _M(2,0);

/* Timer B 0 Control Register */
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

/* Timer B 1 Capture/Reload Value Register */
__no_init volatile __io unsigned short TB1R @ _M(2,2);

/* Timer B 1 Control Register */
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

/* Infrared Control Register */
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

/* IR Carrier Register */
__no_init volatile __io union
{
  unsigned short IRCA;
  struct
  {
    unsigned short IRCAL        : 8;
    unsigned short IRCAH        : 8;
  } IRCA_bit;
} @ _M(2,5);

/* IR Modulator Time */
__no_init volatile __io unsigned short IRMT @ _M(2,6);

/* Infrared Control Register B */
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

/* Timer B 0 Compare Register */
__no_init volatile __io unsigned short TB0C @ _M(2,8);

/* Timer B 0 Value Register */
__no_init volatile __io unsigned short TB0V @ _M(2,9);

/* Timer B 1 Compare Register */
__no_init volatile __io unsigned short TB1C @ _M(2,10);

/* Timer B 1 Value Register */
__no_init volatile __io unsigned short TB1V @ _M(2,11);

/* IR Value Register */
__no_init volatile __io unsigned short IRV @ _M(2,12);

/*-------------------------------*
 *           Module 3            *
 *-------------------------------*/

/* Serial Port 0 Control Register */
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

/* Serial Port 0 Data Buffer */
__no_init volatile __io unsigned char SBUF0 @ _M(3,1);   

/* Serial Port 1 Control Register */
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

/* Serial Port 1 Data Buffer */
__no_init volatile __io unsigned char SBUF1 @ _M(3,3); 

/* SPI 0 Data Buffer Register */
__no_init volatile __io unsigned short SPIB0 @ _M(3,4);

/* SPI 0 Control Register */
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
} @ _M(3,5);

/* SPI 1 Data Buffer Register */
__no_init volatile __io unsigned short SPIB1 @ _M(3,6);

/* SPI 1 Control Register */
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
} @ _M(3,7);

/* Serial Port 0 Phase Register */
__no_init volatile __io unsigned short PR0 @ _M(3,8);

/* Serial Port 0 Mode Register */
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

/* Serial Port 1 Phase Register */
__no_init volatile __io unsigned short PR1 @ _M(3,0xA);

/* Serial Port 1 Mode Register */
__no_init volatile __io union
{
  unsigned char SMD1;
  struct
  {
    unsigned char FEDE           : 1;
    unsigned char SMOD           : 1;
    unsigned char ESI            : 1;
  } SMD1_bit;
} @ _M(3,0xB);

/* SPI 0 Configuration Register */
__no_init volatile __io union
{
  unsigned char SPICF0;
  struct
  {
    unsigned char CKPOL          : 1;
    unsigned char CKPHA          : 1;
    unsigned char CHR            : 1;
    unsigned char                : 3;
    unsigned char SAS            : 1;
    unsigned char ESPII          : 1;
  } SPICF0_bit;
} @ _M(3,0xC);

/* SPI 0 Clock Register */
__no_init volatile __io unsigned char SPICK0 @ _M(3,0xD);

/* SPI 1 Configuration Register */
__no_init volatile __io union
{
  unsigned char SPICF1;
  struct
  {
    unsigned char CKPOL          : 1;
    unsigned char CKPHA          : 1;
    unsigned char CHR            : 1;
    unsigned char                : 3;
    unsigned char SAS            : 1;
    unsigned char ESPII          : 1;
  } SPICF1_bit;
} @ _M(3,0xE);

/* SPI 1 Clock Register */
__no_init volatile __io unsigned char SPICK1 @ _M(3,0xF);


/*-------------------------------*
 *           Module 4            *
 *-------------------------------*/

/* I2C Control Register */
__no_init volatile __io union
{
  unsigned short I2CCN;
  struct
  {
    unsigned short I2CEN     : 1;
    unsigned short I2CMST    : 1;
    unsigned short I2CMODE   : 1;
    unsigned short           : 1;
    unsigned short I2CSTRS   : 1;
    unsigned short I2CACK    : 1;
    unsigned short I2CSTART  : 1;
    unsigned short I2CSTOP   : 1;
    unsigned short I2CGCEN   : 1;
    unsigned short I2CSTREN  : 1;
    unsigned short           : 5;
    unsigned short I2CRST    : 1;
  } I2CCN_bit;
} @ _M(4,0);

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
} @ _M(4,1);

/* I2C Data Buffer Register */
__no_init volatile __io unsigned char I2CBUF @ _M(4,2);  

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
} @ _M(4,3);

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
} @ _M(4,4);

/* USB Data Register */
__no_init volatile __io unsigned char UDATA @ _M(4,5);   

/* I2C Clock Control Register */
__no_init volatile __io union
{
  unsigned short I2CCK;
  struct
  {
    unsigned short I2CCKL    : 8;
    unsigned short I2CCKH    : 8;
  } I2CCK_bit;
} @ _M(4,8);

/* I2C Timeout Register */
__no_init volatile __io unsigned char I2CTO @ _M(4,9);

/* I2C Slave Address Register */
__no_init volatile __io unsigned short I2CSLA @ _M(4,0xA);


#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ622 */


