/*****************************************************************************
 * Filename:    rocket_adc_hw.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     Sept. 14, 2009
 *
 * Description: Definitions of ADC module registers
 ****************************************************************************/
#ifndef __ROCKET_ADC_HW_H__
#define __ROCKET_ADC_HW_H__

#if defined( PROCESSOR_PMU430 )

#define REFCTL0_REG16               ( 0x1B0 )

#define REFCTL0_BGMODE              ( 1<<11 )
#define REFCTL0_REFGENBUSY          ( 1<<10 )
#define REFCTL0_REFBGACT            ( 1<<9 )
#define REFCTL0_REFGENACT           ( 1<<8 )
#define REFCTL0_REFMSTR             ( 1<<7 )
#define REFCTL0_REFVSEL_MASK        ( 3<<4 )
#define REFCTL0_REFVSEL_1V5         ( 0<<4 )
#define REFCTL0_REFVSEL_2V0         ( 1<<4 )
#define REFCTL0_REFVSEL_2V5         ( 2<<4 )
#define REFCTL0_REFTCOFF            ( 1<<3 )
#define REFCTL0_REFOUT              ( 1<<1 )
#define REFCTL0_REFON               ( 1<<0 )

#define ADC12A_BASE                 ( 0x700 )

#define ADC12_MAX_SEQUENCE          ( 16 )

#define ADC12MEM_N_REG16(x)         ( ((x)<<1) + ADC12A_BASE + 0x20 )
#define ADC12MCTL_N_REG8(x)         ( ((x)<<0) + ADC12A_BASE + 0x10 )
#define ADC12IV_REG16               ( ADC12A_BASE + 0x0E )
#define ADC12IE_REG16               ( ADC12A_BASE + 0x0C )
#define ADC12IFG_REG16              ( ADC12A_BASE + 0x0A )
#define ADC12CTL2_REG16             ( ADC12A_BASE + 0x04 )
#define ADC12CTL1_REG16             ( ADC12A_BASE + 0x02 )
#define ADC12CTL0_REG16             ( ADC12A_BASE + 0x00 )

#define ADC12CTL0_SHT1_MASK         ( 0xF<<12 )
#define ADC12CTL0_SHT0_MASK         ( 0xF<<8 )
#define ADC12CTL0_MSC               ( 1<<7 )
#define ADC12CTL0_REF2V5            ( 1<<6 )
#define ADC12CTL0_REFON             ( 1<<5 )
#define ADC12CTL0_ON                ( 1<<4 )
#define ADC12CTL0_OVIE              ( 1<<3 )
#define ADC12CTL0_TOVIE             ( 1<<2 )
#define ADC12CTL0_ENC               ( 1<<1 )
#define ADC12CTL0_SC                ( 1<<0 )

#define ADC12CTL1_CSTARTADD_MASK    ( 0xF<<12 )
#define ADC12CTL1_SHS_MASK          ( 0x3<<10 )
#define ADC12CTL1_SHP               ( 1<<9 )
#define ADC12CTL1_ISSH              ( 1<<8 )
#define ADC12CTL1_DIV_MASK          ( 0x3<<5 )
#define ADC12CTL1_SSEL_MODCLK       ( 0<<3 )
#define ADC12CTL1_SSEL_ACLK         ( 1<<3 )
#define ADC12CTL1_SSEL_MCLK         ( 0x2<<3 )
#define ADC12CTL1_SSEL_SMCLK        ( 0x3<<3 )
#define ADC12CTL1_CONSEQ_MASK       ( 0x3<<1 )
#define ADC12CTL1_CONSEQ_SCH_SC     ( 0x0<<1 )
#define ADC12CTL1_CONSEQ_SEQCH      ( 0x1<<1 )
#define ADC12CTL1_CONSEQ_RSCH       ( 0x2<<1 )
#define ADC12CTL1_CONSEQ_RSEQCH     ( 0x3<<1 )


#define ADC12CTL1_BUSY              ( 1<<0 )

#define ADC12CTL2_PDIV              ( 1<<8 )
#define ADC12CTL2_TCOFF             ( 1<<7 )
#define ADC12CTL2_RES_MASK          ( 3<<4 )
#define ADC12CTL2_RES_12BIT         ( 2<<4 )
#define ADC12CTL2_RES_10BIT         ( 1<<4 )
#define ADC12CTL2_RES_8BIT          ( 0<<4 )
#define ADC12CTL2_DF                ( 1<<3 )
#define ADC12CTL2_SR                ( 1<<2 )
#define ADC12CTL2_REFOUT            ( 1<<1 )
#define ADC12CTL2_REFBURST          ( 1<<0 )

#define ADC12MCTL_EOS               ( 1<<7 )
#define ADC12MCTL_INCH_MASK         ( 0xF<<0 )

#if defined( PROCESSOR_PMU430 )
// Current source enable on Port3
#define ADCP3CE_REG8                ( 0x228 )
#define ADCP3CE_EN(x)               ( 1<<((x) & 0x7) )
#endif

// Enum corresponds to ADC12INCHx field of ADC12MCTLx registers
typedef enum {
    CH_MUX,
    CH_A1,
    CH_A2,
    CH_A3,
    CH_A4,
    CH_A5,
    CH_A6,
    CH_A7,
    CH_VEREF_P,
    CH_VEREF_N,
    CH_TEMP_DIODE,
    CH_HALF_AVCC,
    CH_A12,
    CH_A13,
    CH_A14,
    CH_A15,
    NUM_ADC_CHANNELS
} AdcChannel;

typedef enum {
    CH_MUXCHANNEL_DISABLED,
    CH_MUXCHANNEL_VUSB,
    CH_MUXCHANNEL_VAC,
    CH_MUXCHANNEL_VPWR,
    CH_MUXCHANNEL_VBAT,
    CH_MUXCHANNEL_VSYS,
    CH_MUXCHANNEL_VTS,
    CH_MUXCHANNEL_VCHGR_TEMP,
    CH_MUXCHANNEL_VRED,
    CH_MUXCHANNEL_VGRN,
    CH_MUXCHANNEL_VBLU,
    CH_MUXCHANNEL_VFLASHSRC,
    CH_MUXCHANNEL_VLEDM_COL0,
    CH_MUXCHANNEL_VLEDM_COL1,
    CH_MUXCHANNEL_VLEDM_COL2,
    CH_MUXCHANNEL_VLEDM_COL3,
    CH_MUXCHANNEL_ISOURCE_3uA,
    CH_MUXCHANNEL_ISOURCE_10uA,
    CH_MUXCHANNEL_ISOURCE_50uA,
} AdcMuxChannel;

typedef enum {
    CHREF_AVCC_AVSS,
    CHREF_VREFP_AVSS,
    CHREF_VEREFP_AVSS,
    CHREF_VEREFP_AVSS_2,
    CHREF_AVCC_VEREFN,
    CHREF_VREFP_VEREFN,
    CHREF_VEREFP_VEREFN,
    CHREF_VEREFP_VEREFN_2
} AdcChannelRef;


void AdcInitHw( void );

void AdcConvertHw( AdcChannel ch, AdcMuxChannel chMux, AdcChannelRef cr, FUNCTION1 ccCallback );

WORD AdcReadSampleHw( void );

void AdcHalt( void );

#else
 #error Unknown ADC register definitions
#endif

#endif /* __ROCKET_ADC_HW_H__ */

