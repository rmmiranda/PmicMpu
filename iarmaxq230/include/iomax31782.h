/********************************************************************
 *
 * Register and bit definitions for Dallas Semiconductor/Maxim
 * MAX31782.
 *
 * This file supports assembler and C development for MAX31782
 *
 * Copyright 2003-2006 IAR Systems. All rights reserved.
 *
 *
 * $Revision: 1536 $  Added bit structures for all Port Input,
 *                   Output and Direction registers
 *
 * 11/07/08 - BJV - Convert to max31782
 *
 *  8/05/09 - NL - Finalized conversion after tape out.
 *
 ********************************************************************/


#ifndef __IOMAX31782
#define __IOMAX31782

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




/*-----------------------------------------------------------------------*
 *                               Module 0                                *
 *----------------------------------------------------------------------*/

/* Port 2 Output Register */
__no_init volatile __io union
{
  unsigned char PO2;  /* Yeah, that is an O followed by a ZERO */
  struct
  {
    unsigned char PO2_0         : 1;
    unsigned char PO2_1         : 1;
    unsigned char PO2_2         : 1;
    unsigned char PO2_3         : 1;
    unsigned char PO2_4         : 1;
    unsigned char PO2_5         : 1;
    unsigned char PO2_6         : 1;
    unsigned char PO2_7         : 1;
  } PO2_bit;
} @ _M(0,0);


/* Port 1 Output Register */
__no_init volatile __io union
{
  unsigned char PO1;
  struct
  {
    unsigned char PO1_0         : 1;
    unsigned char PO1_1         : 1;
    unsigned char PO1_2         : 1;
    unsigned char PO1_3         : 1;
    unsigned char PO1_4         : 1;
    unsigned char PO1_5         : 1;
    unsigned char PO1_6         : 1;
    unsigned char PO1_7         : 1;
  } PO1_bit;
} @ _M(0,1);

/* Port 0 Output Register */
__no_init volatile __io union
{
  unsigned char PO0;
  struct
  {
    unsigned char PO0_0         : 1;
    unsigned char PO0_1         : 1;
    unsigned char PO0_2         : 1;
    unsigned char PO0_3         : 1;
    unsigned char PO0_4         : 1;
    unsigned char PO0_5         : 1;
    unsigned char PO0_6         : 1;
    unsigned char PO0_7         : 1;
  } PO0_bit;
} @ _M(0,2);



/* Module0 Interrupt Register */
__no_init volatile __io union
{
  unsigned short MIIR0;
  struct
  {
    unsigned char TB0         	 : 1;
    unsigned char 	         : 7;
    unsigned char                : 8;
  } MIIR0_bit;
} @ _M(0,3);



/* External Interrupt Flag 0 Register */
//EVAL ONLY
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
//EVAL ONLY
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
} @ _M(0,5);



__no_init volatile __io unsigned short TB0C  @ _M(0,6);    /* Timer B 0 Capture Register  */
__no_init volatile __io unsigned short TB0R  @ _M(0,7);    /* Timer B 0 Reload Register   */



/* Port 2 Input Register */
__no_init volatile __io union
{
  unsigned char PI2;
  struct
  {
    unsigned char PI2_0         : 1;
    unsigned char PI2_1         : 1;
    unsigned char PI2_2         : 1;
    unsigned char PI2_3         : 1;
    unsigned char PI2_4         : 1;
    unsigned char PI2_5         : 1;
    unsigned char PI2_6         : 1;
    unsigned char PI2_7         : 1;
  } PI2_bit;
} @ _M(0,8);


/* Port 1 Input Register */
__no_init volatile __io union
{
  unsigned char PI1;
  struct
  {
    unsigned char PI1_0         : 1;
    unsigned char PI1_1         : 1;
    unsigned char PI1_2         : 1;
    unsigned char PI1_3         : 1;
    unsigned char PI1_4         : 1;
    unsigned char PI1_5         : 1;
    unsigned char PI1_6         : 1;
    unsigned char PI1_7         : 1;
  } PI1_bit;
} @ _M(0,9);



/* Port 0 Input Register */
__no_init volatile __io union
{
  unsigned char PI0;
  struct
  {
    unsigned char PI0_0         : 1;
    unsigned char PI0_1         : 1;
    unsigned char PI0_2         : 1;
    unsigned char PI0_3         : 1;
    unsigned char PI0_4         : 1;
    unsigned char PI0_5         : 1;
    unsigned char PI0_6         : 1;
    unsigned char PI0_7         : 1;
  } PI0_bit;
} @ _M(0,10);


__no_init volatile __io unsigned short TB0V  @ _M(0,11);    /* Timer B 0 Value Register   */


/* External Interrupt Edge Select 0 Register */
//EVAL ONLY
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
} @ _M(0,12);


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
} @ _M(0,13);


__no_init volatile __io unsigned short PWCN  @ _M(0,15);    /* Power Control Register */  //EVAL ONLY



/* Port 2 Direction Register */
__no_init volatile __io union
{
  unsigned char PD2;
  struct
  {
    unsigned char PD2_0         : 1;
    unsigned char PD2_1         : 1;
    unsigned char PD2_2         : 1;
    unsigned char PD2_3         : 1;
    unsigned char PD2_4         : 1;
    unsigned char PD2_5         : 1;
    unsigned char PD2_6         : 1;
    unsigned char PD2_7         : 1;
  } PD2_bit;
} @ _M(0,16);


/* Port 1 Direction Register */
__no_init volatile __io union
{
  unsigned char PD1;
  struct
  {
    unsigned char PD1_0         : 1;
    unsigned char PD1_1         : 1;
    unsigned char PD1_2         : 1;
    unsigned char PD1_3         : 1;
    unsigned char PD1_4         : 1;
    unsigned char PD1_5         : 1;
    unsigned char PD1_6         : 1;
    unsigned char PD1_7         : 1;
  } PD1_bit;
} @ _M(0,17);

/* Port 0 Direction Register */
__no_init volatile __io union
{
  unsigned char PD0;
  struct
  {
    unsigned char PD0_0         : 1;
    unsigned char PD0_1         : 1;
    unsigned char PD0_2         : 1;
    unsigned char PD0_3         : 1;
    unsigned char PD0_4         : 1;
    unsigned char PD0_5         : 1;
    unsigned char PD0_6         : 1;
    unsigned char PD0_7         : 1;
  } PD0_bit;
} @ _M(0,18);






/*-----------------------------------------------------------------------*
 *                               Module 1                               *
 *----------------------------------------------------------------------*/

__no_init volatile __io unsigned short I2CBUF_M @ _M(1,0);		/* I2C_M Data Buffer */


/* I2C_M Status Register */
__no_init volatile __io union
{
  unsigned short I2CST_M;
  struct
  {
    unsigned char I2CSRI         : 1;
    unsigned char I2CTXI         : 1;
    unsigned char I2CRXI         : 1;
    unsigned char I2CSTRI        : 1;
    unsigned char I2CTOI         : 1;
    unsigned char I2CAMI         : 1;		//EVAL ONLY
    unsigned char I2CALI         : 1;
    unsigned char I2CNACKI       : 1;
    unsigned char I2CGCI         : 1;		//EVAL ONLY
    unsigned char I2CROI         : 1;
    unsigned char I2CSCL         : 1;
    unsigned char I2CSPI         : 1;
    unsigned char                : 2;
    unsigned char I2CBUSY        : 1;
    unsigned char I2CBUS         : 1;
  } I2CST_M_bit;
} @ _M(1,1);


/* I2C_M Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned short I2CIE_M;
  struct
  {
    unsigned char I2CSRIE        : 1;
    unsigned char I2CTXIE        : 1;
    unsigned char I2CRXIE        : 1;
    unsigned char I2CSTRIE       : 1;
    unsigned char I2CTOIE        : 1;
    unsigned char I2CAMIE        : 1;		//EVAL ONLY
    unsigned char I2CALIE        : 1;
    unsigned char I2CNACKIE      : 1;
    unsigned char I2CGCIE        : 1;		//EVAL ONLY
    unsigned char I2CROIE        : 1;
    unsigned char I2CSTREN       : 1;		//EVAL ONLY
    unsigned char I2CSPIE        : 1;
    unsigned char 				 : 4;
  } I2CIE_M_bit;
} @ _M(1,2);


/* Port 6 Output Register */
__no_init volatile __io union
{
  unsigned char PO6;
  struct
  {
    unsigned char PO6_0          : 1;
    unsigned char PO6_1          : 1;
    unsigned char PO6_2          : 1;
    unsigned char PO6_3          : 1;
    unsigned char PO6_4          : 1;
    unsigned char PO6_5          : 1;
    unsigned char PO6_6          : 1;
    unsigned char PO6_7          : 1;
  } PO6_bit;
} @ _M(1,3);


/* Module1 Interrupt Register */
__no_init volatile __io union
{
  unsigned short MIIR1;
  struct
  {
    unsigned char P6_0         	 : 1;
    unsigned char P6_1         	 : 1;
    unsigned char P6_2         	 : 1;
    unsigned char P6_3         	 : 1;
    unsigned char P6_4         	 : 1;
    unsigned char SVM         	 : 1;
    unsigned char P6_6         	 : 1;
    unsigned char P6_7         	 : 1;
    unsigned char I2CM         	 : 1;
    unsigned char I2CM_WU      	 : 1;
    unsigned char 	             : 6;
  } MIIR1_bit;
} @ _M(1,4);


/* External Interrupt Flag 6 Register */
__no_init volatile __io union
{
  unsigned char EIF6;
  struct
  {
    unsigned char IFP6_0           : 1;
    unsigned char IFP6_1           : 1;
    unsigned char IFP6_2           : 1;
    unsigned char IFP6_3           : 1;
    unsigned char IFP6_4           : 1;
    unsigned char				   : 1;
    unsigned char IFP6_6           : 1;
    unsigned char IFP6_7           : 1;
  } EIF6_bit;
} @ _M(1,6);


/* External Interrupt Enable 6 Register  */
__no_init volatile __io union
{
  unsigned char EIE6;
  struct
  {
    unsigned char IEP6_0           : 1;
    unsigned char IEP6_1           : 1;
    unsigned char IEP6_2           : 1;
    unsigned char IEP6_3           : 1;
    unsigned char IEP6_4		   : 1;
    unsigned char            	   : 1;
    unsigned char IEP6_6           : 1;
    unsigned char IEP6_7           : 1;
  } EIE6_bit;
} @ _M(1,7);


/* Port 6 Input Register */
__no_init volatile __io union
{
  unsigned char PI6;
  struct
  {
    unsigned char PI6_0          : 1;
    unsigned char PI6_1          : 1;
    unsigned char PI6_2          : 1;
    unsigned char PI6_3          : 1;
    unsigned char PI6_4          : 1;
    unsigned char PI6_5          : 1;
    unsigned char PI6_6          : 1;
    unsigned char PI6_7          : 1;
  } PI6_bit;
} @ _M(1,8);


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
} @ _M(1,9);



/* Flash Memory Control Register */
__no_init volatile __io union
{
  unsigned char FCNTL;
  struct
  {
    unsigned char CMD            : 3;
    unsigned char 		         : 4;
    unsigned char FBUSY          : 1;
  } FCNTL_bit;
} @ _M(1,10);


__no_init volatile __io unsigned short FDATA @ _M(1,11);		/* Flash Memory Data */


/* I2C_M Control Register */
__no_init volatile __io union
{
  unsigned short I2CCN_M;
  struct
  {
    unsigned char I2CEN          : 1;
    unsigned char I2CMST         : 1;
    unsigned char I2CMODE        : 1;
    unsigned char I2CEA          : 1;		//EVAL ONLY
    unsigned char I2CSTRS        : 1;
    unsigned char I2CACK         : 1;
    unsigned char I2CSTART       : 1;
    unsigned char I2CSTOP        : 1;
   	unsigned char I2CGCEN        : 1;    	//EVAL ONLY
   	unsigned char I2CSTREN       : 1;
  	unsigned char 		         : 5;
   	unsigned char I2CRST         : 1;

  } I2CCN_M_bit;
} @ _M(1,12);


/* I2C_M Clock Register */
__no_init volatile __io union
{
  unsigned short I2CCK_M;
  struct
  {
    unsigned char I2CCKL         : 8;
    unsigned char I2CCKH         : 8;
  } I2CCK_M_bit;
} @ _M(1,13);


__no_init volatile __io unsigned short I2CTO_M  @ _M(1,14);   /* I2C_M Timeout Register       */
__no_init volatile __io unsigned char I2CSLA_M @ _M(1,15);   /* I2C_M Slave Address Register */	//EVAL ONLY


/* External Interrupt Edge Select 6 Register */
__no_init volatile __io union
{
  unsigned char EIES6;
  struct
  {
    unsigned char IESP6_0           : 1;
    unsigned char IESP6_1           : 1;
    unsigned char IESP6_2           : 1;
    unsigned char IESP6_3           : 1;
    unsigned char IESP6_4           : 1;
    unsigned char					: 1;
    unsigned char IESP6_6           : 1;
    unsigned char IESP6_7           : 1;
  } EIES6_bit;
} @ _M(1,16);



/* Port 6 Direction Register */
__no_init volatile __io union
{
  unsigned char PD6;
  struct
  {
    unsigned char PD6_0          : 1;
    unsigned char PD6_1          : 1;
    unsigned char PD6_2          : 1;
    unsigned char PD6_3          : 1;
    unsigned char PD6_4          : 1;
    unsigned char PD6_5          : 1;
    unsigned char PD6_6          : 1;
    unsigned char PD6_7          : 1;
  } PD6_bit;
} @ _M(1,18);


__no_init volatile __io unsigned short ERT  @ _M(1,21);   /* ADC Spare Register   */		//EVAL ONLY
__no_init volatile __io unsigned short ETS  @ _M(1,22);   /* ADC External Temperature Slope  */
__no_init volatile __io unsigned short ADCG1  @ _M(1,23);   /* ADC Scale Trim  */
__no_init volatile __io unsigned short ADCG5  @ _M(1,24);   /* ADC Scale Trim  */
__no_init volatile __io unsigned short ADVOFF  @ _M(1,25);   /* ADC Voltage Offset  */
__no_init volatile __io unsigned short TOEX  @ _M(1,26);   /* ADC External Temperature Offset  */










/*-----------------------------------------------------------------------*
 *                               Module 2                                *
 *----------------------------------------------------------------------*/

__no_init volatile __io unsigned short I2CBUF_S @ _M(2,0);		/* I2C_S Data Buffer */


/* I2C_S Status Register */
__no_init volatile __io union
{
  unsigned short I2CST_S;
  struct
  {
    unsigned char I2CSRI         : 1;
    unsigned char I2CTXI         : 1;
    unsigned char I2CRXI         : 1;
    unsigned char I2CSTRI        : 1;
    unsigned char I2CTOI         : 1;		//EVAL ONLY
    unsigned char I2CAMI         : 1;
    unsigned char I2CALI         : 1;		//EVAL ONLY
    unsigned char I2CNACKI       : 1;
    unsigned char I2CGCI         : 1;
    unsigned char I2CROI         : 1;
    unsigned char I2CSCL         : 1;
    unsigned char I2CSPI         : 1;
    unsigned char I2C_BYP        : 1;
    unsigned char EXIT_BYP       : 1;
    unsigned char I2CBUSY        : 1;
    unsigned char I2CBUS         : 1;
  } I2CST_S_bit;
} @ _M(2,1);


/* I2C_S Interrupt Enable Register */
__no_init volatile __io union
{
  unsigned short I2CIE_S;
  struct
  {
    unsigned char I2CSRIE        : 1;
    unsigned char I2CTXIE        : 1;
    unsigned char I2CRXIE        : 1;
    unsigned char I2CSTRIE       : 1;
    unsigned char I2CTOIE        : 1;		//EVAL ONLY
    unsigned char I2CAMIE        : 1;
    unsigned char I2CALIE        : 1;		//EVAL ONLY
    unsigned char I2CNACKIE      : 1;
    unsigned char I2CGCIE        : 1;
    unsigned char I2CROIE        : 1;
    unsigned char I2CSTREN       : 1;
    unsigned char I2CSPIE        : 1;
    unsigned char 				 : 4;
  } I2CIE_S_bit;
} @ _M(2,2);


/* Module2 Interrupt Register */
__no_init volatile __io union
{
  unsigned short MIIR2;
  struct
  {
    unsigned char ADC         	 : 1;
    unsigned char I2CS         	 : 1;
    unsigned char I2CS_WU     	 : 1;
    unsigned char 	         : 5;
    unsigned char                : 8;
  } MIIR2_bit;
} @ _M(2,3);



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
    unsigned char    	         : 1;
    unsigned char ADDAT          : 4;
    unsigned char                : 4;

  } ADST_bit;
} @ _M(2,6);


/* ADC Address Register */
__no_init volatile __io union
{
  unsigned short ADADDR;
  struct
  {
    unsigned char ADEND          : 3;
    unsigned char                : 1;
    unsigned char ADSTART        : 3;
    unsigned char                : 1;
    unsigned char ADBADD         : 4;
    unsigned char                : 4;
  } ADADDR_bit;
} @ _M(2,7);


/* ADC Control Register */
__no_init volatile __io union
{
  unsigned short ADCN;
  struct
  {
    unsigned char ADACQ          : 4;
    unsigned char 				 : 1;
    unsigned char ADDAIE         : 1;
    unsigned char ADCONT         : 1;
    unsigned char IREFEN         : 1;
    unsigned char 		         : 2;
    unsigned char ADDAINV        : 2;
    unsigned char ADCCLK         : 3;
    unsigned char 		         : 1;
  } ADCN_bit;
} @ _M(2,8);


/* ADC Data Register */
__no_init volatile __io union
{
  unsigned short ADDATA;
  struct
  {
    unsigned char ADCH           : 3;
    unsigned char ADDIFF         : 1;
    unsigned char ADALIGN        : 1;
    unsigned char ADACQEN        : 1;
    unsigned char ADGAIN         : 1;
    unsigned char EXTEMP         : 1;
    unsigned char         		 : 8;
  } ADDATA_bit;
} @ _M(2,9);



/* I2C_S Control Register */
__no_init volatile __io union
{
  unsigned short I2CCN_S;
  struct
  {
    unsigned char I2CEN          : 1;		//EVAL ONLY
    unsigned char I2CMST         : 1;		//EVAL ONLY
    unsigned char I2CMODE        : 1;		//EVAL ONLY
    unsigned char I2CEA          : 1;		//EVAL ONLY
    unsigned char I2CSTRS        : 1;
    unsigned char I2CACK         : 1;
    unsigned char I2CSTART       : 1;		//EVAL ONLY
    unsigned char I2CSTOP        : 1;		//EVAL ONLY
   	unsigned char I2CGCEN        : 1;
   	unsigned char I2CSTREN       : 1;
  	unsigned char 		         : 5;
   	unsigned char I2CRST         : 1;

  } I2CCN_S_bit;
} @ _M(2,12);


/* I2C_S Clock Register */		//EVAL ONLY
__no_init volatile __io union
{
  unsigned short I2CCK_S;
  struct
  {
    unsigned char I2CCKL         : 8;
    unsigned char I2CCKH         : 8;
  } I2CCK_S_bit;
} @ _M(2,13);


__no_init volatile __io unsigned short I2CTO_S  @ _M(2,14);   /* I2C_S Timeout Register       */	//EVAL ONLY
__no_init volatile __io unsigned char I2CSLA_S @ _M(2,15);   /* I2C_S Slave Address Register */


__no_init volatile __io unsigned short SMP0  @ _M(2,19);   		/* Temperature Sample 0 */	//EVAL ONLY
__no_init volatile __io unsigned short SMP1 @ _M(2,20);   		/* Temperature Sample 1 */	//EVAL ONLY
__no_init volatile __io unsigned char I2C_SPB  @ _M(2,21);   	/* I2C Bootload Enable  */
__no_init volatile __io unsigned char DEV_NUM @ _M(2,22);   	/* Device Number */
__no_init volatile __io unsigned short TM2  @ _M(2,23);   		/* TM2 */					//EVAL ONLY
__no_init volatile __io unsigned short ICDT0 @ _M(2,24);   		/* ICD Temp0 */
__no_init volatile __io unsigned short ICDT1  @ _M(2,25);   	/* ICD Temp1 */



/* ICD Control */
__no_init volatile __io union
{
  unsigned char ICDC;
  struct
  {
    unsigned char CMD          : 4;
    unsigned char              : 1;
    unsigned char REGE         : 1;
    unsigned char              : 1;
    unsigned char DME          : 1;
  } ICDC_bit;
} @ _M(2,26);


/* ICD Flags */
__no_init volatile __io union
{
  unsigned char ICDF;
  struct
  {
    unsigned char TXC           : 1;
    unsigned char JTAG_SPE      : 1;
    unsigned char PSS           : 2;
    unsigned char               : 4;
  } ICDF_bit;
} @ _M(2,27);


__no_init volatile __io unsigned char ICDB @ _M(2,28);   		/* ICD Buffer */
__no_init volatile __io unsigned short ICDA  @ _M(2,29);   		/* ICD Address */
__no_init volatile __io unsigned short ICDD @ _M(2,30);   		/* ICD Data */
__no_init volatile __io unsigned short TM  @ _M(2,31);   		/* TM */					//EVAL ONLY





/*-----------------------------------------------------------------------*
 *                               Module 3                                *
 *----------------------------------------------------------------------*/



__no_init volatile __io unsigned short PWMC0  @ _M(3,0);    	/* PWM0 Capture Register  */
__no_init volatile __io unsigned short PWMR0  @ _M(3,1);    	/* PWM0 Reload Register   */
__no_init volatile __io unsigned short PWMC1  @ _M(3,2);    	/* PWM1 Capture Register  */
__no_init volatile __io unsigned short PWMR1  @ _M(3,3);    	/* PWM1 Reload Register   */
//__no_init volatile __io unsigned short TACHC0  @ _M(3,4);    	/* TACH0 Capture Register  */	//EVAL ONLY
/* SMBUS Register*/
__no_init volatile __io union
{
  unsigned short SMBUS;
  struct
  {
    unsigned char SMB_MOD_M      : 1;
    unsigned char SMB_MOD_S      : 1;
    unsigned char RESET_M        : 1;
    unsigned char RESET_S        : 1;
    unsigned char            	 : 4;
    unsigned char                : 8;
  } SMBUS_bit;
} @ _M(3,4);

__no_init volatile __io unsigned short TACHR0  @ _M(3,5);    	/* TACH0 Reload Register   */
//__no_init volatile __io unsigned short TACHC1  @ _M(3,6);    	/* TACH1 Capture Register  */	//EVAL ONLY
__no_init volatile __io unsigned short TACHR1  @ _M(3,7);    	/* TACH1 Reload Register   */


__no_init volatile __io unsigned short PWMV0  @ _M(3,8);    	/* PWM0 Value Register   */


/* PWM0 Control Register*/
__no_init volatile __io union
{
  unsigned short PWMCN0;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char ETB            : 1;		//EVAL ONLY
    unsigned char PWMEN          : 1;
    unsigned char EXENB          : 1;		//EVAL ONLY
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char EXFB           : 1;		//EVAL ONLY
    unsigned char TFB            : 1;		//EVAL ONLY
    unsigned char PWMPS          : 3;
    unsigned char PWMCR          : 1;
    unsigned char PWMCS          : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } PWMCN0_bit;
} @ _M(3,9);


__no_init volatile __io unsigned short PWMV1  @ _M(3,10);    	/* PWM1 Value Register   */


/* PWM1 Control Register*/
__no_init volatile __io union
{
  unsigned short PWMCN1;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char ETB            : 1;		//EVAL ONLY
    unsigned char PWMEN          : 1;
    unsigned char EXENB          : 1;		//EVAL ONLY
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char EXFB           : 1;		//EVAL ONLY
    unsigned char TFB            : 1;		//EVAL ONLY
    unsigned char PWMPS          : 3;
    unsigned char PWMCR          : 1;
    unsigned char PWMCS          : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } PWMCN1_bit;
} @ _M(3,11);



__no_init volatile __io unsigned short TACHV0  @ _M(3,12);    	/* TACH0 Value Register   */


/* TACH0 Control Register*/
__no_init volatile __io union
{
  unsigned short TACHCN0;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char TACHIE         : 1;
    unsigned char TACHE          : 1;
    unsigned char TEXEN          : 1;
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char TEXF           : 1;
    unsigned char TF             : 1;
    unsigned char TPS            : 3;
    unsigned char TBCR           : 1;		//EVAL ONLY
    unsigned char TBCS           : 1;		//EVAL ONLY
    unsigned char TRPS           : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } TACHCN0_bit;
} @ _M(3,13);


__no_init volatile __io unsigned short TACHV1  @ _M(3,14);    	/* TACH1 Value Register   */


/* TACH1 Control Register*/
__no_init volatile __io union
{
  unsigned short TACHCN1;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char TACHIE         : 1;
    unsigned char TACHE          : 1;
    unsigned char TEXEN          : 1;
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char TEXF           : 1;
    unsigned char TF             : 1;
    unsigned char TPS            : 3;
    unsigned char TBCR           : 1;		//EVAL ONLY
    unsigned char TBCS           : 1;		//EVAL ONLY
    unsigned char TRPS           : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } TACHCN1_bit;
} @ _M(3,15);


/* Module3 Interrupt Register */
__no_init volatile __io union
{
  unsigned short MIIR3;
  struct
  {
    unsigned char TACH0       	 : 1;
    unsigned char TACH1       	 : 1;
    unsigned char 	         : 6;
    unsigned char 	         : 8;
  } MIIR3_bit;
} @ _M(3,16);








/*-----------------------------------------------------------------------*
 *                               Module 4                                *
 *----------------------------------------------------------------------*/


__no_init volatile __io unsigned short PWMC2  @ _M(4,0);    	/* PWM2 Capture Register  */
__no_init volatile __io unsigned short PWMR2  @ _M(4,1);    	/* PWM2 Reload Register   */
__no_init volatile __io unsigned short PWMC3  @ _M(4,2);    	/* PWM3 Capture Register  */
__no_init volatile __io unsigned short PWMR3  @ _M(4,3);    	/* PWM3 Reload Register   */
//__no_init volatile __io unsigned short TACHC2  @ _M(4,4);    	/* TACH2 Capture Register  */	//EVAL ONLY
__no_init volatile __io unsigned short TACHR2  @ _M(4,5);    	/* TACH2 Reload Register   */
//__no_init volatile __io unsigned short TACHC3  @ _M(4,6);    	/* TACH3 Capture Register  */	//EVAL ONLY
__no_init volatile __io unsigned short TACHR3  @ _M(4,7);    	/* TACH3 Reload Register   */


__no_init volatile __io unsigned short PWMV2  @ _M(4,8);    	/* PWM2 Value Register   */


/* PWM2 Control Register*/
__no_init volatile __io union
{
  unsigned short PWMCN2;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char ETB            : 1;		//EVAL ONLY
    unsigned char PWMEN          : 1;
    unsigned char EXENB          : 1;		//EVAL ONLY
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char EXFB           : 1;		//EVAL ONLY
    unsigned char TFB            : 1;		//EVAL ONLY
    unsigned char PWMPS          : 3;
    unsigned char PWMCR          : 1;
    unsigned char PWMCS          : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } PWMCN2_bit;
} @ _M(4,9);


__no_init volatile __io unsigned short PWMV3  @ _M(4,10);    	/* PWM3 Value Register   */


/* PWM3 Control Register*/
__no_init volatile __io union
{
  unsigned short PWMCN3;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char ETB            : 1;		//EVAL ONLY
    unsigned char PWMEN          : 1;
    unsigned char EXENB          : 1;		//EVAL ONLY
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char EXFB           : 1;		//EVAL ONLY
    unsigned char TFB            : 1;		//EVAL ONLY
    unsigned char PWMPS          : 3;
    unsigned char PWMCR          : 1;
    unsigned char PWMCS          : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } PWMCN3_bit;
} @ _M(4,11);



__no_init volatile __io unsigned short TACHV2  @ _M(4,12);    	/* TACH2 Value Register   */


/* TACH2 Control Register*/
__no_init volatile __io union
{
  unsigned short TACHCN2;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char TACHIE         : 1;
    unsigned char TACHE          : 1;
    unsigned char TEXEN          : 1;
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char TEXF           : 1;
    unsigned char TF             : 1;
    unsigned char TPS            : 3;
    unsigned char TBCR           : 1;		//EVAL ONLY
    unsigned char TBCS           : 1;		//EVAL ONLY
    unsigned char TRPS           : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } TACHCN2_bit;
} @ _M(4,13);


__no_init volatile __io unsigned short TACHV3  @ _M(4,14);    	/* TACH3 Value Register   */


/* TACH3 Control Register*/
__no_init volatile __io union
{
  unsigned short TACHCN3;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char TACHIE         : 1;
    unsigned char TACHE          : 1;
    unsigned char TEXEN          : 1;
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char TEXF           : 1;
    unsigned char TF             : 1;
    unsigned char TPS            : 3;
    unsigned char TBCR           : 1;		//EVAL ONLY
    unsigned char TBCS           : 1;		//EVAL ONLY
    unsigned char TRPS           : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } TACHCN3_bit;
} @ _M(4,15);


/* Module4 Interrupt Register */
__no_init volatile __io union
{
  unsigned short MIIR4;
  struct
  {
    unsigned char TACH2       	 : 1;
    unsigned char TACH3       	 : 1;
    unsigned char 	         : 6;
    unsigned char                : 8;
  } MIIR4_bit;
} @ _M(4,16);






/*-----------------------------------------------------------------------*
 *                               Module 5                                *
 *----------------------------------------------------------------------*/
/* Multiplier Control Register */
__no_init volatile __io union
{
  unsigned char MCNT;
  struct
  {
    unsigned char SUS            : 1;
    unsigned char MMAC           : 1;
    unsigned char MSUB		     : 1;
    unsigned char OPCS           : 1;
    unsigned char SQU            : 1;
    unsigned char CLD            : 1;
    unsigned char MCW            : 1;
    unsigned char OF             : 1;
  } MCNT_bit;
} @ _M(5,0);


__no_init volatile __io unsigned short MA  @ _M(5,1);   /* Multiplier Operand A Register    */
__no_init volatile __io unsigned short MB  @ _M(5,2);   /* Multiplier Operand B Register    */
__no_init volatile __io unsigned short MC2 @ _M(5,3);   /* Multiplier Accumulate Register 2 */
__no_init volatile __io unsigned short MC1 @ _M(5,4);   /* Multiplier Accumulate Register 1 */
__no_init volatile __io unsigned short MC0 @ _M(5,5);   /* Multiplier Accumulate Register 0 */
__no_init volatile __io unsigned short MC1R @ _M(5,6);   /* Multiplier Read Register 1 */
__no_init volatile __io unsigned short MC0R @ _M(5,7);   /* Multiplier Read Register 0 */




__no_init volatile __io unsigned short PWMV4  @ _M(5,8);    	/* PWM4 Value Register   */


/* PWM4 Control Register*/
__no_init volatile __io union
{
  unsigned short PWMCN4;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char ETB            : 1;		//EVAL ONLY
    unsigned char PWMEN          : 1;
    unsigned char EXENB          : 1;		//EVAL ONLY
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char EXFB           : 1;		//EVAL ONLY
    unsigned char TFB            : 1;		//EVAL ONLY
    unsigned char PWMPS          : 3;
    unsigned char PWMCR          : 1;
    unsigned char PWMCS          : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } PWMCN4_bit;
} @ _M(5,9);


__no_init volatile __io unsigned short PWMC4  @ _M(5,10);    	/* PWM4 Capture Register  */
__no_init volatile __io unsigned short PWMR4  @ _M(5,11);    	/* PWM4 Reload Register   */

__no_init volatile __io unsigned short TACHV4  @ _M(5,12);    	/* TACH4 Value Register   */


/* TACH4 Control Register*/
__no_init volatile __io union
{
  unsigned short TACHCN4;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char TACHIE         : 1;
    unsigned char TACHE          : 1;
    unsigned char TEXEN          : 1;
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char TEXF           : 1;
    unsigned char TF             : 1;
    unsigned char TPS            : 3;
    unsigned char TBCR           : 1;		//EVAL ONLY
    unsigned char TBCS           : 1;		//EVAL ONLY
    unsigned char TRPS           : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } TACHCN4_bit;
} @ _M(5,13);


//__no_init volatile __io unsigned short TACHC4  @ _M(5,14);    	/* TACH4 Capture Register  */	//EVAL ONLY
__no_init volatile __io unsigned short TACHR4  @ _M(5,15);    	/* TACH4 Reload Register   */
//__no_init volatile __io unsigned short TACHC5  @ _M(5,16);    	/* TACH5 Capture Register  */	//EVAL ONLY
__no_init volatile __io unsigned short TACHR5  @ _M(5,17);    	/* TACH5 Reload Register   */

__no_init volatile __io unsigned short TACHV5  @ _M(5,18);    	/* TACH5 Value Register   */


/* TACH5 Control Register*/
__no_init volatile __io union
{
  unsigned short TACHCN5;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char TACHIE         : 1;
    unsigned char TACHE          : 1;
    unsigned char TEXEN          : 1;
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char TEXF           : 1;
    unsigned char TF             : 1;
    unsigned char TPS            : 3;
    unsigned char TBCR           : 1;		//EVAL ONLY
    unsigned char TBCS           : 1;		//EVAL ONLY
    unsigned char TRPS           : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } TACHCN5_bit;
} @ _M(5,19);



__no_init volatile __io unsigned short PWMC5  @ _M(5,20);    	/* PWM5 Capture Register  */
__no_init volatile __io unsigned short PWMR5  @ _M(5,21);    	/* PWM5 Reload Register   */


__no_init volatile __io unsigned short PWMV5  @ _M(5,22);    	/* PWM5 Value Register   */


/* PWM5 Control Register*/
__no_init volatile __io union
{
  unsigned short PWMCN5;
  struct
  {
    unsigned char CPnRLB         : 1;		//EVAL ONLY
    unsigned char ETB            : 1;		//EVAL ONLY
    unsigned char PWMEN          : 1;
    unsigned char EXENB          : 1;		//EVAL ONLY
    unsigned char DCEN           : 1;		//EVAL ONLY
    unsigned char TBOE           : 1;		//EVAL ONLY
    unsigned char EXFB           : 1;		//EVAL ONLY
    unsigned char TFB            : 1;		//EVAL ONLY
    unsigned char PWMPS          : 3;
    unsigned char PWMCR          : 1;
    unsigned char PWMCS          : 1;
    unsigned char                : 2;
    unsigned char CnTB           : 1;		//EVAL ONLY
  } PWMCN5_bit;
} @ _M(5,23);



/* Module5 Interrupt Register */
__no_init volatile __io union
{
  unsigned short MIIR5;
  struct
  {
    unsigned char TACH4       	 : 1;
    unsigned char TACH5       	 : 1;
    unsigned char 	         : 6;
    unsigned char                : 8;
  } MIIR5_bit;
} @ _M(5,24);












 /*-----------------------------------------------------------------------*
  *                               Module 8                                *
 *----------------------------------------------------------------------*/

/* Only CKCN and WDCN are defined in this file, the rest of the */
/* registers in Module 8 can be found in iomaxq.h.              */

/* Clock Control register */
__no_init volatile __io union
{
  unsigned char CKCN;
  struct
  {
    unsigned char 	             : 4;
    unsigned char STOP           : 1;
    unsigned char                : 3;
  } CKCN_bit;
} @ _M(8,14);


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
} @ _M(8,15);


#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */
#endif /* __IOMAX31782 */


















