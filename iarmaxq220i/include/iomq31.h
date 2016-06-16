/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MQ31.
 *
 * This file supports assembler and C development for MQ31
 *
 * Copyright 2003-2010 IAR Systems. All rights reserved.
 *
 * $Revision: 1344 $
 *
 ********************************************************************/


#ifndef __IOMQ31
#define __IOMQ31

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

__no_init volatile __io union
{
  unsigned char PO0;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PO0_bit;
} @ _M(0,0);

__no_init volatile __io union
{
  unsigned short SPICN0;
  struct
  {
    unsigned short SPIEN : 1;
    unsigned short MSTM : 1;
    unsigned short MODFE : 1;
    unsigned short MODF : 1;
    unsigned short SPIRXRST : 1;
    unsigned short SPITXRST : 1;
    unsigned short  : 1;
    unsigned short SPIBUSY : 1;
    unsigned short SPIRXIE : 1;
    unsigned short SPIRXOVRIE : 1;
    unsigned short SPITXIE : 1;
    unsigned short SPITXEIE : 1;
    unsigned short SPITXOVRIE : 1;
    unsigned short SPISTOPIE : 1;
  } SPICN0_bit;
} @ _M(0,1);

__no_init volatile __io union
{
  unsigned short SPIST0;
  struct
  {
    unsigned short SPIRXI : 1;
    unsigned short SPIRXOVRI : 1;
    unsigned short SPITXI : 1;
    unsigned short SPITXEI : 1;
    unsigned short SPITXOVRI : 1;
    unsigned short SPISTOPI : 1;
    unsigned short  : 2;
    unsigned short SPIRXHFI : 1;
    unsigned short SPIRXFI : 1;
    unsigned short SPITXFI : 1;
  } SPIST0_bit;
} @ _M(0,2);

__no_init volatile __io unsigned short SPIB0 @ _M(0,3);

__no_init volatile __io union
{
  unsigned short I2CCN0;
  struct
  {
    unsigned short I2CEN : 1;
    unsigned short I2CMST : 1;
    unsigned short I2CMODE : 1;
    unsigned short I2CEA : 1;
    unsigned short I2CSTRS : 1;
    unsigned short I2CACK : 1;
    unsigned short I2CSTART : 1;
    unsigned short I2CSTOP : 1;
    unsigned short I2CGCEN : 1;
    unsigned short I2CSTREN : 1;
    unsigned short  : 5;
    unsigned short I2CRST : 1;
  } I2CCN0_bit;
} @ _M(0,4);

__no_init volatile __io union
{
  unsigned short I2CST0;
  struct
  {
    unsigned short I2CSRI : 1;
    unsigned short I2CTXI : 1;
    unsigned short I2CRXI : 1;
    unsigned short I2CSTRI : 1;
    unsigned short I2CTOI : 1;
    unsigned short I2CAMI : 1;
    unsigned short I2CALI : 1;
    unsigned short I2CNACKI : 1;
    unsigned short I2CGCI : 1;
    unsigned short I2CRXFI : 1;
    unsigned short I2CSCL : 1;
    unsigned short I2CSPI : 1;
    unsigned short  : 2;
    unsigned short I2CBUSY : 1;
    unsigned short I2CBUS : 1;
  } I2CST0_bit;
} @ _M(0,5);

__no_init volatile __io unsigned short I2CBUF0 @ _M(0,6);

__no_init volatile __io union
{
  unsigned char EIF0;
  struct
  {
    unsigned char IE0 : 1;
    unsigned char IE1 : 1;
    unsigned char IE2 : 1;
    unsigned char IE3 : 1;
    unsigned char IE4 : 1;
    unsigned char IE5 : 1;
    unsigned char IE6 : 1;
    unsigned char IE7 : 1;
  } EIF0_bit;
} @ _M(0,7);

__no_init volatile __io union
{
  unsigned char PI0;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PI0_bit;
} @ _M(0,8);

__no_init volatile __io union
{
  unsigned short SPICF0;
  struct
  {
    unsigned short CKPOL : 1;
    unsigned short CKPHA : 1;
    unsigned short CHR : 1;
    unsigned short MODFIE : 1;
    unsigned short  : 2;
    unsigned short SAS : 1;
    unsigned short  : 1;
    unsigned short SPIRXHFIE : 1;
    unsigned short SPIRXFIE : 1;
    unsigned short SPITXFIE : 1;
  } SPICF0_bit;
} @ _M(0,9);

__no_init volatile __io unsigned char SPICK0 @ _M(0,10);

__no_init volatile __io union
{
  unsigned short I2CIE0;
  struct
  {
    unsigned short I2CSRIE : 1;
    unsigned short I2CTXIE : 1;
    unsigned short I2CRXIE : 1;
    unsigned short I2CSTRIE : 1;
    unsigned short I2CTOIE : 1;
    unsigned short I2CAMIE : 1;
    unsigned short I2CALIE : 1;
    unsigned short I2CNACKIE : 1;
    unsigned short I2CGCIE : 1;
    unsigned short I2CRXFIE : 1;
    unsigned short  : 1;
    unsigned short I2CSPIE : 1;
  } I2CIE0_bit;
} @ _M(0,11);

__no_init volatile __io unsigned short I2CSLA0 @ _M(0,12);

__no_init volatile __io union
{
  unsigned short I2CCK0;
  struct
  {
    unsigned short I2CCKL : 8;
    unsigned short I2CCKH : 8;
  } I2CCK0_bit;
} @ _M(0,13);

__no_init volatile __io unsigned char I2CTO0 @ _M(0,14);

__no_init volatile __io union
{
  unsigned char EIE0;
  struct
  {
    unsigned char EX0 : 1;
    unsigned char EX1 : 1;
    unsigned char EX2 : 1;
    unsigned char EX3 : 1;
    unsigned char EX4 : 1;
    unsigned char EX5 : 1;
    unsigned char EX6 : 1;
    unsigned char EX7 : 1;
  } EIE0_bit;
} @ _M(0,15);

__no_init volatile __io union
{
  unsigned char PD0;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PD0_bit;
} @ _M(0,16);

__no_init volatile __io union
{
  unsigned char PE0;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PE0_bit;
} @ _M(0,17);

__no_init volatile __io union
{
  unsigned char EIES0;
  struct
  {
    unsigned char IT0 : 1;
    unsigned char IT1 : 1;
    unsigned char IT2 : 1;
    unsigned char IT3 : 1;
    unsigned char IT4 : 1;
    unsigned char IT5 : 1;
    unsigned char IT6 : 1;
    unsigned char IT7 : 1;
  } EIES0_bit;
} @ _M(0,18);

__no_init volatile __io union
{
  unsigned char PO1;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PO1_bit;
} @ _M(1,0);

__no_init volatile __io union
{
  unsigned short SPICN1;
  struct
  {
    unsigned short SPIEN : 1;
    unsigned short MSTM : 1;
    unsigned short MODFE : 1;
    unsigned short MODF : 1;
    unsigned short SPIRXRST : 1;
    unsigned short SPITXRST : 1;
    unsigned short  : 1;
    unsigned short SPIBUSY : 1;
    unsigned short SPIRXIE : 1;
    unsigned short SPIRXOVRIE : 1;
    unsigned short SPITXIE : 1;
    unsigned short SPITXEIE : 1;
    unsigned short SPITXOVRIE : 1;
    unsigned short SPISTOPIE : 1;
  } SPICN1_bit;
} @ _M(1,1);

__no_init volatile __io union
{
  unsigned short SPIST1;
  struct
  {
    unsigned short SPIRXI : 1;
    unsigned short SPIRXOVRI : 1;
    unsigned short SPITXI : 1;
    unsigned short SPITXEI : 1;
    unsigned short SPITXOVRI : 1;
    unsigned short SPISTOPI : 1;
    unsigned short  : 2;
    unsigned short SPIRXHFI : 1;
    unsigned short SPIRXFI : 1;
    unsigned short SPITXFI : 1;
  } SPIST1_bit;
} @ _M(1,2);

__no_init volatile __io unsigned short SPIB1 @ _M(1,3);

__no_init volatile __io union
{
  unsigned short I2CCN1;
  struct
  {
    unsigned short I2CEN : 1;
    unsigned short I2CMST : 1;
    unsigned short I2CMODE : 1;
    unsigned short I2CEA : 1;
    unsigned short I2CSTRS : 1;
    unsigned short I2CACK : 1;
    unsigned short I2CSTART : 1;
    unsigned short I2CSTOP : 1;
    unsigned short I2CGCEN : 1;
    unsigned short I2CSTREN : 1;
    unsigned short  : 5;
    unsigned short I2CRST : 1;
  } I2CCN1_bit;
} @ _M(1,4);

__no_init volatile __io union
{
  unsigned short I2CST1;
  struct
  {
    unsigned short I2CSRI : 1;
    unsigned short I2CTXI : 1;
    unsigned short I2CRXI : 1;
    unsigned short I2CSTRI : 1;
    unsigned short I2CTOI : 1;
    unsigned short I2CAMI : 1;
    unsigned short I2CALI : 1;
    unsigned short I2CNACKI : 1;
    unsigned short I2CGCI : 1;
    unsigned short I2CRXFI : 1;
    unsigned short I2CSCL : 1;
    unsigned short I2CSPI : 1;
    unsigned short  : 2;
    unsigned short I2CBUSY : 1;
    unsigned short I2CBUS : 1;
  } I2CST1_bit;
} @ _M(1,5);

__no_init volatile __io unsigned short I2CBUF1 @ _M(1,6);

__no_init volatile __io union
{
  unsigned short TB0CN;
  struct
  {
    unsigned short CP_RLB : 1;
    unsigned short ETB : 1;
    unsigned short TRB : 1;
    unsigned short EXENB : 1;
    unsigned short DCEN : 1;
    unsigned short TBOE : 1;
    unsigned short EXFB : 1;
    unsigned short TFB : 1;
    unsigned short TBPS : 3;
    unsigned short TBCR : 1;
    unsigned short TBCS : 1;
    unsigned short  : 2;
    unsigned short C_TB : 1;
  } TB0CN_bit;
} @ _M(1,7);

__no_init volatile __io union
{
  unsigned char PI1;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PI1_bit;
} @ _M(1,8);

__no_init volatile __io union
{
  unsigned char SPICF1;
  struct
  {
    unsigned char CKPOL : 1;
    unsigned char CKPHA : 1;
    unsigned char CHR : 1;
    unsigned char MODFIE : 1;
    unsigned char  : 2;
    unsigned char SAS : 1;
    unsigned char  : 1;
    unsigned char SPIRXHFIE : 1;
    unsigned char SPIRXFIE : 1;
    unsigned char SPITXFIE : 1;
  } SPICF1_bit;
} @ _M(1,9);

__no_init volatile __io unsigned char SPICK1 @ _M(1,10);

__no_init volatile __io union
{
  unsigned short I2CIE1;
  struct
  {
    unsigned short I2CSRIE : 1;
    unsigned short I2CTXIE : 1;
    unsigned short I2CRXIE : 1;
    unsigned short I2CSTRIE : 1;
    unsigned short I2CTOIE : 1;
    unsigned short I2CAMIE : 1;
    unsigned short I2CALIE : 1;
    unsigned short I2CNACKIE : 1;
    unsigned short I2CGCIE : 1;
    unsigned short I2CRXFIE : 1;
    unsigned short  : 1;
    unsigned short I2CSPIE : 1;
  } I2CIE1_bit;
} @ _M(1,11);

__no_init volatile __io unsigned short I2CSLA1 @ _M(1,12);

__no_init volatile __io union
{
  unsigned short I2CCK1;
  struct
  {
    unsigned short I2CCKL : 8;
    unsigned short I2CCKH : 8;
  } I2CCK1_bit;
} @ _M(1,13);

__no_init volatile __io unsigned char I2CTO1 @ _M(1,14);

__no_init volatile __io unsigned short TB0V @ _M(1,15);

__no_init volatile __io union
{
  unsigned char PD1;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PD1_bit;
} @ _M(1,16);

__no_init volatile __io union
{
  unsigned char PE1;
  struct
  {
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
  } PE1_bit;
} @ _M(1,17);

__no_init volatile __io unsigned short TB0R @ _M(1,18);

__no_init volatile __io unsigned short TB0C @ _M(1,19);

__no_init volatile __io unsigned char INTCN @ _M(2,0);

__no_init volatile __io union
{
  unsigned short SPICN2;
  struct
  {
    unsigned short SPIEN : 1;
    unsigned short MSTM : 1;
    unsigned short MODFE : 1;
    unsigned short MODF : 1;
    unsigned short SPIRXRST : 1;
    unsigned short SPITXRST : 1;
    unsigned short  : 1;
    unsigned short SPIBUSY : 1;
    unsigned short SPIRXIE : 1;
    unsigned short SPIRXOVRIE : 1;
    unsigned short SPITXIE : 1;
    unsigned short SPITXEIE : 1;
    unsigned short SPITXOVRIE : 1;
    unsigned short SPISTOPIE : 1;
  } SPICN2_bit;
} @ _M(2,1);

__no_init volatile __io union
{
  unsigned short SPIST2;
  struct
  {
    unsigned short SPIRXI : 1;
    unsigned short SPIRXOVRI : 1;
    unsigned short SPITXI : 1;
    unsigned short SPITXEI : 1;
    unsigned short SPITXOVRI : 1;
    unsigned short SPISTOPI : 1;
    unsigned short  : 2;
    unsigned short SPIRXHFI : 1;
    unsigned short SPIRXFI : 1;
    unsigned short SPITXFI : 1;
  } SPIST2_bit;
} @ _M(2,2);

__no_init volatile __io unsigned short SPIB2 @ _M(2,3);

__no_init volatile __io union
{
  unsigned short I2CCN2;
  struct
  {
    unsigned short I2CEN : 1;
    unsigned short I2CMST : 1;
    unsigned short I2CMODE : 1;
    unsigned short I2CEA : 1;
    unsigned short I2CSTRS : 1;
    unsigned short I2CACK : 1;
    unsigned short I2CSTART : 1;
    unsigned short I2CSTOP : 1;
    unsigned short I2CGCEN : 1;
    unsigned short I2CSTREN : 1;
    unsigned short  : 5;
    unsigned short I2CRST : 1;
  } I2CCN2_bit;
} @ _M(2,4);

__no_init volatile __io union
{
  unsigned short I2CST2;
  struct
  {
    unsigned short I2CSRI : 1;
    unsigned short I2CTXI : 1;
    unsigned short I2CRXI : 1;
    unsigned short I2CSTRI : 1;
    unsigned short I2CTOI : 1;
    unsigned short I2CAMI : 1;
    unsigned short I2CALI : 1;
    unsigned short I2CNACKI : 1;
    unsigned short I2CGCI : 1;
    unsigned short I2CRXFI : 1;
    unsigned short I2CSCL : 1;
    unsigned short I2CSPI : 1;
    unsigned short  : 2;
    unsigned short I2CBUSY : 1;
    unsigned short I2CBUS : 1;
  } I2CST2_bit;
} @ _M(2,5);

__no_init volatile __io unsigned short I2CBUF2 @ _M(2,6);

__no_init volatile __io union
{
  unsigned short TB1CN;
  struct
  {
    unsigned short CP_RLB : 1;
    unsigned short ETB : 1;
    unsigned short TRB : 1;
    unsigned short EXENB : 1;
    unsigned short DCEN : 1;
    unsigned short TBOE : 1;
    unsigned short EXFB : 1;
    unsigned short TFB : 1;
    unsigned short TBPS : 3;
    unsigned short TBCR : 1;
    unsigned short TBCS : 1;
    unsigned short  : 2;
    unsigned short C_TB : 1;
  } TB1CN_bit;
} @ _M(2,7);

__no_init volatile __io union
{
  unsigned short PWCN;
  struct
  {
    unsigned short AVDDI : 1;
    unsigned short COMMIN01 : 1;
    unsigned short COMMIN1I : 1;
    unsigned short VGPIO3I : 1;
    unsigned short VRTCI : 1;
    unsigned short  : 2;
    unsigned short DISIBSO : 1;
    unsigned short AVDDIE : 1;
    unsigned short COMMIN0IE : 1;
    unsigned short COMMIN1IE : 1;
    unsigned short VGPIO3IE : 1;
    unsigned short VRTCIE : 1;
  } PWCN_bit;
} @ _M(2,8);

__no_init volatile __io union
{
  unsigned char SPICF2;
  struct
  {
    unsigned char CKPOL : 1;
    unsigned char CKPHA : 1;
    unsigned char CHR : 1;
    unsigned char MODFIE : 1;
    unsigned char  : 2;
    unsigned char SAS : 1;
    unsigned char  : 1;
    unsigned char SPIRXHFIE : 1;
    unsigned char SPIRXFIE : 1;
    unsigned char SPITXFIE : 1;
  } SPICF2_bit;
} @ _M(2,9);

__no_init volatile __io unsigned char SPICK2 @ _M(2,10);

__no_init volatile __io union
{
  unsigned short I2CIE2;
  struct
  {
    unsigned short I2CSRIE : 1;
    unsigned short I2CTXIE : 1;
    unsigned short I2CRXIE : 1;
    unsigned short I2CSTRIE : 1;
    unsigned short I2CTOIE : 1;
    unsigned short I2CAMIE : 1;
    unsigned short I2CALIE : 1;
    unsigned short I2CNACKIE : 1;
    unsigned short I2CGCIE : 1;
    unsigned short I2CRXFIE : 1;
    unsigned short  : 1;
    unsigned short I2CSPIE : 1;
  } I2CIE2_bit;
} @ _M(2,11);

__no_init volatile __io unsigned short I2CSLA2 @ _M(2,12);

__no_init volatile __io union
{
  unsigned short I2CCK2;
  struct
  {
    unsigned short I2CCKL : 8;
    unsigned short I2CCKH : 8;
  } I2CCK2_bit;
} @ _M(2,13);

__no_init volatile __io unsigned char I2CTO2 @ _M(2,14);

__no_init volatile __io unsigned short TB1V @ _M(2,15);

__no_init volatile __io unsigned short TB1R @ _M(2,18);

__no_init volatile __io unsigned short TB1C @ _M(2,19);

__no_init volatile __io union
{
  unsigned short SPMICN;
  struct
  {
    unsigned short SPMIEN : 1;
    unsigned short SPMIMASTER : 1;
    unsigned short SPMITXRDY : 1;
    unsigned short SPMIDESCRDY : 1;
    unsigned short SPMILCKEN : 1;
    unsigned short SPMIRXRST : 1;
    unsigned short SPMICMDRST : 1;
    unsigned short  : 7;
    unsigned short SPMIBUSY : 1;
    unsigned short SPMIIDLE : 1;
  } SPMICN_bit;
} @ _M(3,0);

__no_init volatile __io union
{
  unsigned short SPMIMCFG;
  struct
  {
    unsigned short SPMIMID        : 2;
    unsigned short SPMICONN       : 1;
    unsigned short SPMIDISC       : 1;
    unsigned short  : 3;
    unsigned short SPMIMREQ       : 1;
    unsigned short SPMIMPL        : 2;
    unsigned short  : 4;
    unsigned short SPMIMIDEN      : 1;
    unsigned short SPMIARBEN      : 1;
  } SPMIMCFG_bit;
} @ _M(3,1);

__no_init volatile __io union
{
  unsigned short SPMISCFG;
  struct
  {
    unsigned short SPMISIDI : 1;
    unsigned short SPMIGSIDI : 1;
    unsigned short  : 4;
    unsigned short SPMISRHRST : 1;
    unsigned short SPMISREQ : 1;
    unsigned short SPMISIDIE : 1;
    unsigned short SPMIGSIDIE : 1;
  } SPMISCFG_bit;
} @ _M(3,2);

__no_init volatile __io union
{
  unsigned short SPMICLKCN;
  struct
  {
    unsigned short SPMIRADDR : 8;
    unsigned short SPMIRLEN : 4;
    unsigned short  : 3;
    unsigned short SPMIRSEL : 1;
  } SPMICLKCN_bit;
} @ _M(3,3);

__no_init volatile __io union
{
  unsigned short SPMIMST;
  struct
  {
    unsigned short SPMIMST : 2;
    unsigned short  : 2;
    unsigned short SPMISTOPI : 1;
    unsigned short SPMICONNI : 1;
    unsigned short SPMIDISCI : 1;
    unsigned short SPMIMIDI : 1;
    unsigned short SPMIMCODE : 2;
    unsigned short  : 2;
    unsigned short SPMISTOPIE : 1;
    unsigned short SPMICONNIE : 1;
    unsigned short SPMIDISCIE : 1;
    unsigned short SPMIMIDIE : 1;
  } SPMIMST_bit;
} @ _M(3,4);

__no_init volatile __io unsigned char SPMITX @ _M(3,5);

__no_init volatile __io union
{
  unsigned short SPMIST0;
  struct
  {
    unsigned short SPMICMDI : 1;
    unsigned short SPMICMDFI : 1;
    unsigned short SPMIRXFI : 1;
    unsigned short SPMIRXOVRI : 1;
    unsigned short SPMITXI : 1;
    unsigned short SPMILCKI : 1;
    unsigned short  : 2;
    unsigned short SPMICMDIE : 1;
    unsigned short SPMICMDFIE : 1;
    unsigned short SPMIRXIE : 1;
    unsigned short SPMIRXOVRIE : 1;
    unsigned short SPMITXIE : 1;
    unsigned short SPMILCKIE : 1;
  } SPMIST0_bit;
} @ _M(3,6);

__no_init volatile __io union
{
  unsigned short SPMIST1;
  struct
  {
    unsigned short SPMIDNAVI       : 1;
    unsigned short SPMICPERRI      : 1;
    unsigned short SPMIDPERRI      : 1;
    unsigned short SPMINRSPI       : 1;
    unsigned short SPMIBALI        : 1;
    unsigned short SPMITXERRI      : 1;
    unsigned short  : 2;
    unsigned short SPMIDNAVIE      : 1;
    unsigned short SPMICPERRIE     : 1;
    unsigned short SPMIDPERRIE     : 1;
    unsigned short SPMINRSPIE      : 1;
    unsigned short SPMIBALIE       : 1;
    unsigned short SPMITXERRIE     : 1;
  } SPMIST1_bit;
} @ _M(3,7);

__no_init volatile __io union
{
  unsigned short SPMICMDSEQ;
  struct
  {
    unsigned short SPMICMD : 8;
    unsigned short SPMIADDR : 8;
  } SPMICMDSEQ_bit;
} @ _M(3,8);

__no_init volatile __io union
{
  unsigned short SPMIDNAVCMD;
  struct
  {
    unsigned short SPMIDNAVCMD : 8;
    unsigned short SPMIDNAVADDR : 8;
  } SPMIDNAVCMD_bit;
} @ _M(3,9);

__no_init volatile __io unsigned char SPMIRX @ _M(3,10);

__no_init volatile __io union
{
  unsigned short SPMISID;
  struct
  {
    unsigned short SPMISID : 4;
    unsigned short SPMIGSID : 4;
    unsigned short  : 4;
    unsigned short SPMISIDEN : 1;
    unsigned short SPMIGSIDEN : 1;
  } SPMISID_bit;
} @ _M(3,11);

__no_init volatile __io union
{
  unsigned short SPMICK;
  struct
  {
    unsigned short SPMICK : 8;
    unsigned short SPMIRETRY : 5;
    unsigned short  : 1;
    unsigned short SPMICKPS : 2;
  } SPMICK_bit;
} @ _M(3,12);

__no_init volatile __io unsigned short SPMITO @ _M(3,13);

__no_init volatile __io unsigned char SPMIDESC @ _M(3,16);

__no_init volatile __io union
{
  unsigned short ADCN;
  struct
  {
    unsigned short ADEN : 1;
    unsigned short ADDAIE : 1;
    unsigned short ADDAINV : 1;
    unsigned short ADALGN : 1;
    unsigned short  : 4;
    unsigned short ADACQ : 2;
    unsigned short ADAVG : 2;
  } ADCN_bit;
} @ _M(4,0);

__no_init volatile __io unsigned short ADSEL0 @ _M(4,1);

__no_init volatile __io unsigned char ADSEL1 @ _M(4,2);

__no_init volatile __io union
{
  unsigned short ADTRG;
  struct
  {
    unsigned short ADTRGEN : 1;
    unsigned short ADTRGSEL : 1;
    unsigned short ADTRGCH : 1;
    unsigned short ADTRGIE : 1;
    unsigned short  : 4;
    unsigned short ADTRGACQ : 2;
    unsigned short ADTRGAVG : 2;
  } ADTRG_bit;
} @ _M(4,3);

__no_init volatile __io union
{
  unsigned short ADST;
  struct
  {
    unsigned short ADCONV : 1;
    unsigned short ADDAI : 1;
    unsigned short ADTRGI : 1;
    unsigned short  : 3;
    unsigned short ADBUSY : 1;
    unsigned short  : 1;
    unsigned short ADDACH : 5;
  } ADST_bit;
} @ _M(4,4);

__no_init volatile __io unsigned char ADCH @ _M(4,5);

__no_init volatile __io unsigned short ADTRGDLY @ _M(4,6);

__no_init volatile __io unsigned short ADDATA @ _M(4,8);

__no_init volatile __io unsigned char ADDLY @ _M(4,9);

__no_init volatile __io union
{
  unsigned char T32KCN;
  struct
  {
    unsigned char T32KEN : 1;
    unsigned char T32KSTOP : 1;
    unsigned char T32KIE : 1;
    unsigned char T32KI : 1;
  } T32KCN_bit;
} @ _M(5,0);

__no_init volatile __io unsigned short T32KINIT @ _M(5,1);

__no_init volatile __io unsigned short T32KV @ _M(5,2);

__no_init volatile __io unsigned short T32KC @ _M(5,3);

__no_init volatile __io union
{
  unsigned short SHDNCN;
  struct
  {
    unsigned short SHDNEN : 1;
    unsigned short SHDNRST : 1;
    unsigned short SHDNI : 1;
    unsigned short  : 5;
    unsigned short SHDNV : 6;
  } SHDNCN_bit;
} @ _M(5,4);

__no_init volatile __io union
{
  unsigned short RTCCFG;
  struct
  {
    unsigned short RTCOUTEN : 3;
    unsigned short RTCOUTTHEN : 3;
    unsigned short RTCOUTCLK : 1;
    unsigned short VRTCPOR : 1;
    unsigned short VRTCTH : 3;
    unsigned short RTCOUTFORCE : 1;
    unsigned short  : 3;
    unsigned short RTCOUTCLKMD : 1;
  } RTCCFG_bit;
} @ _M(5,5);

__no_init volatile __io union
{
  unsigned short RCNT;
  struct
  {
    unsigned short RTCE : 1;
    unsigned short ADE : 1;
    unsigned short ASE : 1;
    unsigned short BUSY : 1;
    unsigned short RDY : 1;
    unsigned short RDYE : 1;
    unsigned short ALDF : 1;
    unsigned short ALSF : 1;
    unsigned short  : 7;
    unsigned short WE : 1;
  } RCNT_bit;
} @ _M(5,8);

__no_init volatile __io unsigned short RTSH @ _M(5,9);

__no_init volatile __io unsigned short RTSL @ _M(5,10);

__no_init volatile __io unsigned char RTSS @ _M(5,11);

__no_init volatile __io unsigned short RASH @ _M(5,12);

__no_init volatile __io unsigned short RASL @ _M(5,13);

#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAXQ61x */


