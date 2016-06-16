/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAXQ1004
 *
 * This file supports assembler and C development for MAXQ1004
 *
 * Copyright 2003-2009 IAR Systems. All rights reserved.
 *
 * $Revision: 1536 $
 *
 ********************************************************************/

#ifndef __IOMAXQ1004
#define __IOMAXQ1004

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
} @ _M(0,1);

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
} @ _M(0x0, 0x2);

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
} @ _M(0,3);

/* SPI Data Buffer Register */
__no_init volatile __io unsigned short SPIB @ _M(0,4);

/* Timer0 Control Register */
__no_init volatile __io union
{
  unsigned char T0CN;
  struct
  {
    unsigned char M          : 2;
    unsigned char C_T        : 1;
    unsigned char GATE       : 1;
    unsigned char TR0        : 1;
    unsigned char TF0        : 1;
    unsigned char T0M        : 1;
    unsigned char ET0        : 1;
  } T0CN_bit;
} @ _M(0,5);

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
} @ _M(0,0x9);

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
} @ _M(0,0xA);

/* SPI Clock Register */
__no_init volatile __io unsigned char SPICK @ _M(0,0xB);   

/* Timer 0 Least Significant Byte Register */
__no_init volatile __io unsigned char T0L @ _M(0,0xC);   

/* Timer 0 Most Significant Byte Register */
__no_init volatile __io unsigned char T0H @ _M(0,0xD);   

/* Flash Memory Control Register */
__no_init volatile __io union
{
  unsigned char FCNTL;
  struct
  {
    unsigned char FC          	: 3;
    unsigned char FCNTL_Reserv  : 4;
    unsigned char FBUSY         : 1;
  } FCNTL_bit;
} @ _M(0,0x0E);

/* FDATA Register */
__no_init volatile __io unsigned int FDATA @ _M(0,0xF);   

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

/*-------------------------------*
 *           Module 1            *
 *-------------------------------*/

/* 1-Wire Control Register */
__no_init volatile __io union
{
  unsigned short OWCNT;
  struct
  {
    unsigned char OWEN          : 1;
    unsigned char OWIE          : 1;
    unsigned char OWIF          : 1;
    unsigned char OWSTOP        : 1;
    unsigned char OWCBBZ        : 1;
    unsigned char OWER          : 1;
    unsigned char OWBUSY        : 1;
    unsigned char OWIS          : 1;
    unsigned char OWCMD         : 8;
  } OWCNT_bit;
} @ _M(1,0);

/* 1-Wire Byte Length Register */
__no_init volatile __io unsigned short OWBL @ _M(1,1);

/* 1-Wire Micro Status Register */
__no_init volatile __io union
{
  unsigned char OWMS;
  struct
  {
    unsigned char OWRSP         : 6;
    unsigned char OWPOR         : 1;
    unsigned char OWIOST        : 1;
  } OWMS_bit;
} @ _M(1,2);

/* 1-Wire Input/Output Register */
__no_init volatile __io unsigned char OWIO @ _M(1,3);

/* AES Engine Control Register */
__no_init volatile __io union
{
  unsigned char AESC;
  struct
  {
    unsigned char AESST         : 1;
    unsigned char AESMD         : 1;
    unsigned char AESSEK        : 1;
    unsigned char AESKS         : 2;
    unsigned char AESIE         : 1;
    unsigned char AESIF         : 1;
    unsigned char AESER         : 1;
  } AESC_bit;
} @ _M(1,4);

/* Memory Control Register */
__no_init volatile __io union
{
  unsigned char MEMCN;
  struct
  {
    unsigned char MEMFIE        : 1;
    unsigned char MEMFI         : 1;
  } MEMCN_bit;
} @ _M(1,5);

/* 1-Wire Status Register */
__no_init volatile __io union
{
  unsigned char OWUS;
  struct
  {
    unsigned char OWSTAT    : 4;
    unsigned char OWOPRE    : 1;
    unsigned char OWOPRF    : 1;
    unsigned char OWINRE    : 1;
    unsigned char OWINRF    : 1;
  } OWUS_bit;
} @ _M(1,8);

/* 1-Wire ROM Identification Register */
__no_init volatile __io unsigned char OWRD @ _M(1,9);

/* Random Number Register */
__no_init volatile __io unsigned short RNR @ _M(1,0xA);

/* Random Number Generator Entropy Register */
__no_init volatile __io unsigned short RNGE @ _M(1,0xB);

/*-------------------------------*
 *           Module 2            *
 *-------------------------------*/
/* Wake-Up Timer Control Register */
__no_init volatile __io union
{
  unsigned char WUTC;
  struct
  {
    unsigned char WTE    : 1;
    unsigned char WTF    : 1;
    unsigned char WTCLK  : 1;
  } WUTC_bit;
} @ _M(2,0);

/* ADC Control Register */
__no_init volatile __io union
{
  unsigned short ADCN;
  struct
  {
    unsigned short ADEN     : 1;
    unsigned short ADDAIE   : 1;
    unsigned short ADALGN   : 1;
    unsigned short ADCH     : 1;
    unsigned short ADSCL    : 1;
    unsigned short SATURATE : 1;
    unsigned short ADREF    : 1;
    unsigned short IREFEN   : 1;
    unsigned short ADCCLK   : 2;
  } ADCN_bit;
} @ _M(2,1);

/* ADC Status Register */
__no_init volatile __io union
{
  unsigned char ADST;
  struct
  {
    unsigned char ADCONV : 1;
    unsigned char ADDAI  : 1;
    unsigned char ADOVF  : 1;
  } ADST_bit;
} @ _M(2,2);

/* Temperature Sensor Control Register */
__no_init volatile __io union
{
  unsigned char TSCN;
  struct
  {
    unsigned char TSCN : 1;
  } TSCN_bit;
} @ _M(2,3);

/* Stop Mode Interrupt Flag Register */
__no_init volatile __io unsigned short STOPIF @ _M(2,4);

/* Port 1 Output Register */
__no_init volatile __io unsigned char PO1 @ _M(2,5);

/* Wake-Up Timer Register */
__no_init volatile __io unsigned short WUT @ _M(2,8);

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
    unsigned short PWRLOSS      :1;
    unsigned short PFRCK        :2;
  } PWCN_bit;
} @ _M(2,9);

/* ADC Data Register */
__no_init volatile __io unsigned short ADDATA @ _M(2,0xA);

/* Port 1 Input Register */
__no_init volatile __io unsigned char PI1 @ _M(2,0xB);

/* Port 1 Direction Register */
__no_init volatile __io unsigned char PD1 @ _M(2,0x10);

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ1004 */
