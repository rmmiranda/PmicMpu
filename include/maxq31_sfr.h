/*****************************************************************************
 * Filename:    maxq31_sfr.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 20, 2010
 *
 * Description: Definitions for MAXQ31 special function registers (SFR).
 ****************************************************************************/
#ifndef __MAXQ31_SFR_H__
#define __MAXQ31_SFR_H__

/* make sure this is always included first */
#include "registers.h"

#undef WriteRegByte
#undef WriteRegByteOr
#undef WriteRegByteAnd
#undef ReadRegByte

#define WriteRegByte( REG, VALUE )      ((REG)=(BYTE)((VALUE)&0xFF))
#define WriteRegByteOr( REG, VALUE )    ((REG)|=(BYTE)((VALUE)&0xFF))
#define WriteRegByteAnd( REG, VALUE )   ((REG)&=(BYTE)((VALUE)&0xFF))
#define ReadRegByte( REG ) 				((BYTE)((REG)&0xFF))

#undef WriteRegWord
#undef WriteRegWordOr
#undef WriteRegWordAnd
#undef ReadRegWord

#define WriteRegWord( REG, VALUE )      ((REG)=(WORD)((VALUE)&0xFFFF))
#define WriteRegWordOr( REG, VALUE )    ((REG)|=(WORD)((VALUE)&0xFFFF))
#define WriteRegWordAnd( REG, VALUE )   ((REG)&=(WORD)((VALUE)&0xFFFF))
#define ReadRegWord( REG ) 				((WORD)((REG)&0xFFFF))

/***********************/
/***** GPIO module *****/
/***********************/

__no_init volatile __io BYTE PO0_REG8 @ 0x0000;
__no_init volatile __io BYTE PI0_REG8 @ 0x0010;
__no_init volatile __io BYTE PD0_REG8 @ 0x0020;
__no_init volatile __io BYTE PE0_REG8 @ 0x0022;

__no_init volatile __io BYTE PO1_REG8 @ 0x0040;
__no_init volatile __io BYTE PI1_REG8 @ 0x0050;
__no_init volatile __io BYTE PD1_REG8 @ 0x0060;
__no_init volatile __io BYTE PE1_REG8 @ 0x0062;

__no_init volatile __io BYTE EIF0_REG8 @ 0x000E;
__no_init volatile __io BYTE EIE0_REG8 @ 0x001E;
__no_init volatile __io BYTE EIES0_REG8 @ 0x0024;

/***********************************/
/***** Interrupt Vector module *****/
/***********************************/

__no_init volatile __io WORD IV_REG16 @ 0x0344;

/**********************/
/***** SPI module *****/
/**********************/

__no_init volatile __io WORD SPICN0_REG16 @ 0x0002;
__no_init volatile __io WORD SPIST0_REG16 @ 0x0004;
__no_init volatile __io WORD SPIB0_REG16 @ 0x0006;
__no_init volatile __io WORD SPICF0_REG16 @ 0x0012;
__no_init volatile __io BYTE SPICK0_REG8 @ 0x0014;

__no_init volatile __io WORD SPICN1_REG16 @ 0x0042;
__no_init volatile __io WORD SPIST1_REG8 @ 0x0044;
__no_init volatile __io WORD SPIB1_REG16 @ 0x0046;
__no_init volatile __io WORD SPICF1_REG8 @ 0x0052;
__no_init volatile __io BYTE SPICK1_REG8 @ 0x0054;

__no_init volatile __io WORD SPICN2_REG16 @ 0x0082;
__no_init volatile __io WORD SPIST2_REG16 @ 0x0084;
__no_init volatile __io WORD SPIB2_REG16 @ 0x0086;
__no_init volatile __io WORD SPICF2_REG16 @ 0x0092;
__no_init volatile __io BYTE SPICK2_REG8 @ 0x0094;

#define SPICN_SPISTOPIE				( 1u << 13 )
#define SPICN_SPITXOVRIE			( 1u << 12 )
#define SPICN_SPITXEIE				( 1u << 11 )
#define SPICN_SPITXIE				( 1u << 10 )
#define SPICN_SPIRXOVRIE			( 1u << 9 )
#define SPICN_SPIRXIE				( 1u << 8 )
#define SPICN_SPIBUSY				( 1u << 7 )
#define SPICN_SPITXRST				( 1u << 5 )
#define SPICN_SPIRXRST				( 1u << 4 )
#define SPICN_MODF					( 1u << 3 )
#define SPICN_MODFE					( 1u << 2 )
#define SPICN_MSTM					( 1u << 1 )
#define SPICN_SPIEN					( 1u << 0 )

#define SPIST_SPITXFI				( 1u << 11 )
#define SPIST_SPIRXFI				( 1u << 10 )
#define SPIST_SPIRXHFI				( 1u << 9 )
#define SPIST_SPISTOPI				( 1u << 5 )
#define SPIST_SPITXOVRI				( 1u << 4 )
#define SPIST_SPITXEI				( 1u << 3 )
#define SPIST_SPITXI				( 1u << 2 )
#define SPIST_SPIRXOVRI				( 1u << 1 )
#define SPIST_SPIRXI				( 1u << 0 )

#define SPICF_SPITXFIE				( 1u << 10 )
#define SPICF_SPIRXFIE				( 1u << 9 )
#define SPICF_SPIRXHFIE				( 1u << 8 )
#define SPICF_SAS					( 1u << 6 )
#define SPICF_MODFIE				( 1u << 3 )
#define SPICF_CHR					( 1u << 2 )
#define SPICF_CKPHA					( 1u << 1 )
#define SPICF_CKPOL					( 1u << 0 )

/************************/
/***** Timer module *****/
/************************/

__no_init volatile __io WORD TBCN0_REG16 @ 0x004E;
__no_init volatile __io WORD TBV0_REG16 @ 0x005E;
__no_init volatile __io WORD TBR0_REG16 @ 0x0064;
__no_init volatile __io WORD TBC0_REG16 @ 0x0066;

__no_init volatile __io WORD TBCN1_REG16 @ 0x008E;
__no_init volatile __io WORD TBV1_REG16 @ 0x009E;
__no_init volatile __io WORD TBR1_REG16 @ 0x00A4;
__no_init volatile __io WORD TBC1_REG16 @ 0x00A6;

#define TBCN_CP_RLBN			( 1u << 0 )
#define TBCN_ETB				( 1u << 1 )
#define TBCN_TRB				( 1u << 2 )
#define TBCN_EXENB				( 1u << 3 )
#define TBCN_DCEN				( 1u << 4 )
#define TBCN_TBOE				( 1u << 5 )
#define TBCN_EXFB				( 1u << 6 )
#define TBCN_TFB				( 1u << 7 )
#define TBCN_TBPS_1				( 0u << 8 )
#define TBCN_TBPS_4				( 1u << 8 )
#define TBCN_TBPS_16			( 2u << 8 )
#define TBCN_TBPS_64			( 3u << 8 )
#define TBCN_TBPS_256			( 4u << 8 )
#define TBCN_TBPS_1024			( 5u << 8 )
#define TBCN_TBPS_MASK			( 7u << 8 )
#define TBCN_TBCR				( 1u << 11 )
#define TBCN_TBCS				( 1u << 12 )
#define TBCN_C_TBN				( 1u << 15 )

/************************************/
/***** Interrupt control module *****/
/************************************/

__no_init volatile __io BYTE INTCN_REG8 @ 0x080;

#define INTCN_IRQ		( 1u << 3 )
#define INTCN_FIQ		( 1u << 2 )
#define INTCN_IRQIE		( 1u << 1 )
#define INTCN_FIQIE		( 1u << 0 )

/********************************/
/***** Power control module *****/
/********************************/

__no_init volatile __io WORD PWCN_REG16 @ 0x090;

#define PWCN_DIS_OSC_PD	( 1u << 15 )
#define PWCN_SPMIRPD	( 1u << 14 )
#define PWCN_RTCOUTTHIE	( 1u << 13 )
#define PWCN_VRTCIE		( 1u << 12 )
#define PWCN_VGPIO3IE	( 1u << 11 )
#define PWCN_COMMIN1IE	( 1u << 10 )
#define PWCN_COMMIN0IE	( 1u << 9 )
#define PWCN_AVDDIE		( 1u << 8 )
#define PWCN_DISIBSO	( 1u << 7 )
#define PWCN_VRTCOK		( 1u << 6 )
#define PWCN_RTCOUTTHI	( 1u << 5 )
#define PWCN_RTCI		( 1u << 4 )
#define PWCN_VGPIO3I	( 1u << 3 )
#define PWCN_COMMIN1I	( 1u << 2 )
#define PWCN_COMMIN0I	( 1u << 1 )
#define PWCN_AVDDI		( 1u << 0 )

/*********************************/
/***** Shutdown timer module *****/
/*********************************/

__no_init volatile __io WORD SHDNCN_REG16 @ 0x148;

#define SHDNCN_SHDNI	( 1u << 2 )

/**********************/
/***** I2C module *****/
/**********************/

__no_init volatile __io WORD I2CCN0_REG16 @ 0x0008;
__no_init volatile __io WORD I2CST0_REG16 @ 0x000A;
__no_init volatile __io WORD I2CBUF0_REG16 @ 0x000C;
__no_init volatile __io WORD I2CIE0_REG16 @ 0x0016;
__no_init volatile __io WORD I2CSLA0_REG16 @ 0x0018;
__no_init volatile __io WORD I2CCK0_REG16 @ 0x001A;
__no_init volatile __io BYTE I2CTO0_REG16 @ 0x001C;

__no_init volatile __io WORD I2CCN1_REG16 @ 0x0048;
__no_init volatile __io WORD I2CST1_REG16 @ 0x004A;
__no_init volatile __io WORD I2CBUF1_REG16 @ 0x004C;
__no_init volatile __io WORD I2CIE1_REG16 @ 0x0056;
__no_init volatile __io WORD I2CSLA1_REG16 @ 0x0058;
__no_init volatile __io WORD I2CCK1_REG16 @ 0x005A;
__no_init volatile __io BYTE I2CTO1_REG16 @ 0x005C;

__no_init volatile __io WORD I2CCN2_REG16 @ 0x0088;
__no_init volatile __io WORD I2CST2_REG16 @ 0x008A;
__no_init volatile __io WORD I2CBUF2_REG16 @ 0x008C;
__no_init volatile __io WORD I2CIE2_REG16 @ 0x0096;
__no_init volatile __io WORD I2CSLA2_REG16 @ 0x0098;
__no_init volatile __io WORD I2CCK2_REG16 @ 0x009A;
__no_init volatile __io BYTE I2CTO2_REG16 @ 0x009C;

#define I2CCN_I2CRST	( 1u << 15 )
#define I2CCN_I2CSTREN	( 1u << 9 )
#define I2CCN_I2CGCEN	( 1u << 8 )
#define I2CCN_I2CSTOP	( 1u << 7 )
#define I2CCN_I2CSTART	( 1u << 6 )
#define I2CCN_I2CACK	( 1u << 5 )
#define I2CCN_I2CSTRS	( 1u << 4 )
#define I2CCN_I2CEA		( 1u << 3 )
#define I2CCN_I2CMODE	( 1u << 2 )
#define I2CCN_I2CMST	( 1u << 1 )
#define I2CCN_I2CEN		( 1u << 0 )

#define I2CST_I2CBUS	( 1u << 15 )
#define I2CST_I2CBUSY	( 1u << 14 )
#define I2CST_I2CBITCLR	( 1u << 12 )
#define I2CST_I2CBITSET	( 2u << 12 )
#define I2CST_I2CBITMSK	( 3u << 12 )
#define I2CST_I2CSPI	( 1u << 11 )
#define I2CST_I2CSCL	( 1u << 10 )
#define I2CST_I2CRFI	( 1u << 9 )
#define I2CST_I2CGCI	( 1u << 8 )
#define I2CST_I2CNACKI	( 1u << 7 )
#define I2CST_I2CALI	( 1u << 6 )
#define I2CST_I2CAMI	( 1u << 5 )
#define I2CST_I2CTOI	( 1u << 4 )
#define I2CST_I2CSTRI	( 1u << 3 )
#define I2CST_I2CRXI	( 1u << 2 )
#define I2CST_I2CTXI	( 1u << 1 )
#define I2CST_I2CSRI	( 1u << 0 )

#define I2CIE_I2CSPIE	( 1u << 11 )
#define I2CIE_I2CRFIE	( 1u << 9 )
#define I2CIE_I2CGCIE	( 1u << 8 )
#define I2CIE_I2CNACKIE	( 1u << 7 )
#define I2CIE_I2CALIE	( 1u << 6 )
#define I2CIE_I2CAMIE	( 1u << 5 )
#define I2CIE_I2CTOIE	( 1u << 4 )
#define I2CIE_I2CSTRIE	( 1u << 3 )
#define I2CIE_I2CRXIE	( 1u << 2 )
#define I2CIE_I2CTXIE	( 1u << 1 )
#define I2CIE_I2CSRIE	( 1u << 0 )

/****************************/
/***** 32K Timer module *****/
/****************************/

__no_init volatile __io BYTE T32KCN_REG8 @ 0x0140;
__no_init volatile __io WORD T32KINIT_REG16 @ 0x0142;
__no_init volatile __io WORD T32KV_REG16 @ 0x0144;
__no_init volatile __io WORD T32KC_REG16 @ 0x0146;

#define T32KCN_T32KI	( 1u << 3 )
#define T32KCN_T32KIE	( 1u << 2 )
#define T32KCN_T32KSTOP	( 1u << 1 )
#define T32KCN_T32KEN	( 1u << 0 )

/**********************/
/***** RTC module *****/
/**********************/

__no_init volatile __io WORD RCNT_REG16 @ 0x0150;
__no_init volatile __io WORD RTSH_REG16 @ 0x0152;
__no_init volatile __io WORD RTSL_REG16 @ 0x0154;
__no_init volatile __io WORD RTSS_REG16 @ 0x0156;
__no_init volatile __io WORD RASH_REG16 @ 0x0158;
__no_init volatile __io WORD RASL_REG16 @ 0x015A;
__no_init volatile __io WORD RSSA_REG16 @ 0x015C;
__no_init volatile __io WORD RTCCFG_REG16 @ 0x014A;

#define RCNT_WE			( 1u << 15 )
#define RCNT_ALSF		( 1u << 7 )
#define RCNT_ALDF		( 1u << 6 )
#define RCNT_RDYE		( 1u << 5 )
#define RCNT_RDY		( 1u << 4 )
#define RCNT_BUSY		( 1u << 3 )
#define RCNT_ASE		( 1u << 2 )
#define RCNT_ADE		( 1u << 1 )
#define RCNT_RTCE		( 1u << 0 )

#define RTCCFG_OUTCLKMD	( 1u << 15 )
#define RTCCFG_OUTFORCE	( 1u << 13 )
#define RTCCFG_VRTCTH	( 1u << 10 )
#define RTCCFG_VRTCPOR	( 1u << 9 )
#define RTCCFG_OUTCLK	( 1u << 8 )
#define RTCCFG_OUTTHEN0	( 1u << 4 )
#define RTCCFG_OUTTHEN1	( 2u << 4 )
#define RTCCFG_OUTTHEN2	( 4u << 4 )
#define RTCCFG_OUTTHEN3	( 8u << 4 )
#define RTCCFG_OUTEN0	( 1u << 0 )
#define RTCCFG_OUTEN1	( 2u << 0 )
#define RTCCFG_OUTEN2	( 4u << 0 )
#define RTCCFG_OUTEN3	( 8u << 0 )
#define RTCCFG_OUTENMSK	( 15u << 0 )

/**********************/
/***** ADC module *****/
/**********************/

__no_init volatile __io WORD ADCN_REG16 @ 0x0100;
__no_init volatile __io WORD ADSEL0_REG16 @ 0x0102;
__no_init volatile __io BYTE ADSEL1_REG8 @ 0x0104;
__no_init volatile __io WORD ADTRG_REG16 @ 0x0106;
__no_init volatile __io WORD ADST_REG16 @ 0x0108;
__no_init volatile __io BYTE ADCH_REG8 @ 0x010A;
__no_init volatile __io WORD ADTRGDLY_REG16 @ 0x010C;
__no_init volatile __io WORD ADDATA_REG16 @ 0x0110;
__no_init volatile __io BYTE ADDLY_REG8 @ 0x0112;

#define ADCN_ADAVG_1		( 0u << 10 )
#define ADCN_ADAVG_8		( 1u << 10 )
#define ADCN_ADAVG_16		( 2u << 10 )
#define ADCN_ADAVG_32		( 3u << 10 )
#define ADCN_ADAVG_MASK		( 3u << 10 )
#define ADCN_ADACQ_2		( 0u << 8 )
#define ADCN_ADACQ_8		( 1u << 8 )
#define ADCN_ADACQ_32		( 2u << 8 )
#define ADCN_ADACQ_128		( 3u << 8 )
#define ADCN_ADACQ_MASK		( 3u << 8 )
#define ADCN_ADALGN			( 1u << 3 )
#define ADCN_ADDAINV		( 1u << 2 )
#define ADCN_ADDAIE			( 1u << 1 )
#define ADCN_ADEN			( 1u << 0 )

#define ADTRG_ADTRGAVG_1	( 0u << 10 )
#define ADTRG_ADTRGAVG_8	( 1u << 10 )
#define ADTRG_ADTRGAVG_16	( 2u << 10 )
#define ADTRG_ADTRGAVG_32	( 3u << 10 )
#define ADTRG_ADTRGAVG_MASK	( 4u << 10 )
#define ADTRG_ADTRGACQ_2	( 0u << 8 )
#define ADTRG_ADTRGACQ_8	( 1u << 8 )
#define ADTRG_ADTRGACQ_32	( 2u << 8 )
#define ADTRG_ADTRGACQ_128	( 3u << 8 )
#define ADTRG_ADTRGACQ_MASK	( 4u << 8 )
#define ADTRG_ADTRGIE		( 1u << 3 )
#define ADTRG_ADTRGCH		( 1u << 2 )
#define ADTRG_ADTRGSEL		( 1u << 1 )
#define ADTRG_ADTRGEN		( 1u << 0 )

#define ADST_ADDACH(x)		( ((x)&0x1F) << 8 )
#define ADST_ADDACH_MASK	( 0x1Fu << 8 )
#define ADST_ADBUSY			( 1u << 6 )
#define ADST_ADTRGI			( 1u << 2 )
#define ADST_ADDAI			( 1u << 1 )
#define ADST_ADCONV			( 1u << 0 )

/****************************/
/***** System registers *****/
/****************************/

__no_init volatile __io BYTE PRIV_REG8 @ 0x0204;
__no_init volatile __io BYTE IC_REG8 @ 0x020A;
__no_init volatile __io WORD SC_REG16 @ 0x0210;
__no_init volatile __io WORD IPR0_REG16 @ 0x0212;
__no_init volatile __io WORD IPR1_REG16 @ 0x0214;
__no_init volatile __io BYTE PRIVF_REG8 @ 0x0216;
__no_init volatile __io WORD ULDR_REG16 @ 0x0218;
__no_init volatile __io WORD UAPP_REG16 @ 0x021A;
__no_init volatile __io BYTE CKCN_REG8 @ 0x021C;
__no_init volatile __io BYTE WDCN_REG8 @ 0x021E;

#define PRIV_PSYW			( 1u << 3 )
#define PRIV_PSYR			( 1u << 2 )
#define PRIV_PULW			( 1u << 1 )
#define PRIV_PULR			( 1u << 0 )

#define IC_IGE				( 1u << 0 )
#define IC_IPS_HIGH			( 0u << 1 )
#define IC_IPS_MEDIUM		( 1u << 1 )
#define IC_IPS_LOW			( 2u << 1 )
#define IC_IPS_NONE			( 3u << 1 )
#define IC_IPS_MASK			( 3u << 1 )

#define SC_RBPDS			( 1u << 14 )
#define SC_DPFRDS			( 1u << 13 )
#define SC_MPIDS			( 1u << 12 )
#define SC_DBGLCK			( 1u << 11 )
#define SC_MPE				( 1u << 10 )
#define SC_PWLL				( 1u << 9 )
#define SC_PWLS				( 1u << 8 )
#define SC_TAP				( 1u << 7 )
#define SC_CPA				( 1u << 6 )
#define SC_CDA_P0			( 0u << 4 )
#define SC_CDA_P1			( 1u << 4 )
#define SC_CDA_P2			( 2u << 4 )
#define SC_CDA_P3			( 3u << 4 )
#define SC_CDA_MASK			( 3u << 4 )
#define SC_UPA				( 1u << 3 )
#define SC_ROD				( 1u << 2 )
#define SC_PWL				( 1u << 1 )

#define PRIVF_PSYWF			( 1u << 7 )
#define PRIVF_PSYRF			( 1u << 6 )
#define PRIVF_PULWF			( 1u << 5 )
#define PRIVF_PULRF			( 1u << 4 )

#define CKCN_XT_RC			( 1u << 7 )
#define CKCN_RGSL			( 1u << 6 )
#define CKCN_RGMD			( 1u << 5 )
#define CKCN_STOP			( 1u << 4 )
#define CKCN_SWB			( 1u << 3 )
#define CKCN_PMME			( 1u << 2 )
#define CKCN_CD_1			( 0u << 0 )
#define CKCN_CD_2			( 1u << 0 )
#define CKCN_CD_4			( 2u << 0 )
#define CKCN_CD_8			( 3u << 0 )
#define CKCN_CD_MASK		( 3u << 0 )

#define WDCN_POR			( 1u << 7 )
#define WDCN_EWDI			( 1u << 6 )
#define WDCN_WD_2_15		( 0u << 4 )
#define WDCN_WD_2_18		( 1u << 4 )
#define WDCN_WD_2_21		( 2u << 4 )
#define WDCN_WD_2_24		( 3u << 4 )
#define WDCN_WD_MASK		( 3u << 4 )
#define WDCN_WDIF	 		( 1u << 3 )
#define WDCN_WTRF			( 1u << 2 )
#define WDCN_EWT	 		( 1u << 1 )
#define WDCN_RWT			( 1u << 0 )

#endif // __MAXQ31_SFR_H__
