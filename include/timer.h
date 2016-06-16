/*****************************************************************************
 * Filename:    timer.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     December 08, 2010
 *
 * Description: Timer Module Defines
 ****************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#if defined( PROCESSOR_PMU430 )

#define TIMERA0_BASE            ( 0x340 )
#define TIMERA1_BASE            ( 0x380 )

// Timer A0

#define TAR_REG16               ( TIMERA0_BASE + 0x10 )

#define TACCR_REG16(x)          ( TIMERA0_BASE + 0x12 + ((x)*2))
#define TACCR0_REG16            ( TACCR_REG16(0) )
#define TACCR1_REG16            ( TACCR_REG16(1) )
#define TACCR2_REG16            ( TACCR_REG16(2) )

#define TACTL_REG16             ( TIMERA0_BASE )

#define TACCTL_REG16(x)         ( TIMERA0_BASE + 0x02 + ((x)*2))
#define TACCTL0_REG16           ( TACCTL_REG16(0) )
#define TACCTL1_REG16           ( TACCTL_REG16(1) )
#define TACCTL2_REG16           ( TACCTL_REG16(2) )

#define TAIV_REG16              ( TIMERA0_BASE + 0x2E )

// Timer A1

#define TA1R_REG16              ( TIMERA1_BASE + 0x10 )

#define TA1CCR_REG16(x)         ( TIMERA1_BASE + 0x12 + ((x)*2))
#define TA1CCR0_REG16           ( TA1CCR_REG16(0) )
#define TA1CCR1_REG16           ( TA1CCR_REG16(1) )
#define TA1CCR2_REG16           ( TA1CCR_REG16(2) )

#define TA1CTL_REG16            ( TIMERA1_BASE )

#define TA1CCTL_REG16(x)        ( TIMERA1_BASE + 0x02 + ((x)*2))
#define TA1CCTL0_REG16          ( TA1CCTL_REG16(0) )
#define TA1CCTL1_REG16          ( TA1CCTL_REG16(1) )
#define TA1CCTL2_REG16          ( TA1CCTL_REG16(2) )

#define TA1IV_REG16             ( TIMERA1_BASE + 0x2E )



#define TACTL_TAIFG             ( 1<<0 )
#define TACTL_TAIE              ( 1<<1 )
#define TACTL_TACLR             ( 1<<2 )
//#define TACTL_UNUSED            ( 1<<3 )
#define TACLT_MC_MASK           ( 3<<4 )
#define TACTL_MC_STOP           ( 0<<4 )
#define TACTL_MC_UP             ( 1<<4 )
#define TACTL_MC_CONTINUOUS     ( 2<<4 )
#define TACTL_MC_UPDOWN         ( 3<<4 )
#define TACTL_ID_MASK           ( 3<<6 )
#define TACTL_ID_1              ( 0<<6 )
#define TACTL_ID_2              ( 1<<6 )
#define TACTL_ID_4              ( 2<<6 )
#define TACTL_ID_8              ( 3<<6 )
#define TACTL_TASSEL_MASK       ( 3<<8 )
#define TACTL_TASSEL_TACLK      ( 0<<8 )
#define TACTL_TASSEL_ACLK       ( 1<<8 )
#define TACTL_TASSEL_SMCLK      ( 2<<8 )
#define TACTL_TASSEL_ITACLK     ( 3<<8 )

#define TACCTL_CCIFG            ( 1<<0 )
#define TACCTL_COV              ( 1<<1 )
#define TACCTL_OUT              ( 1<<2 )
#define TACCTL_CCI              ( 1<<3 )
#define TACCTL_CCIE             ( 1<<4 )
#define TACCTL_OUTMOD_MASK      ( 7<<5 )
#define TACCTL_CAP              ( 1<<8 )
//#define TACCTL_UNUSED           ( 1<<9 )
#define TACCTL_SCCI             ( 1<<10 )
#define TACCTL_SCS              ( 1<<11 )
#define TACCTL_CCIS_MASK        ( 3<<12 )
#define TACCTL_CM_MASK          ( 3<<14 )

#elif defined( PROCESSOR_MQ31 )

#include "maxq31_sfr.h"

#else

#error Timer module not defined for this processor

#endif


typedef enum {
    MUX_TIMER = 0,
    //  TIMER_A_1 = 1,
    //  TIMER_A_2 = 2,
    NUM_HW_TIMERS
} TimerType;


void  TimerHWInit( void );
void  TimerHWHalt( void );
void  RegisterHWTimerExpiry( TimerType timer, FUNCTION func );
void  SetHWTimer( TimerType timer, WORD ticksFromNow );
void  StopHWTimer( TimerType timer );
WORD  GetHWTimerCountWord( void );
DWORD GetHWTimerCountDword( void );


#endif /*_TIMER_H_*/
