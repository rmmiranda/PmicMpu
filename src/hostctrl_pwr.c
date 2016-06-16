/*****************************************************************************
 * Filename:    hostctrl_pwr.c
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     June 1, 2010
 *
 * Description: Rocket power request routines
 *
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "timer_mux.h"
#include "rocket_pmic.h"
#include "config_pwrseq.h"
#include "spi.h"
#include "eventlog.h"
#include "hostctrl.h"
#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
#include "max8649.h"
#endif
#if defined( RIM_SM0_MAIN_VCC ) && defined( CLK_ON )
#include "hostctrl_pwri2c.h"
#endif

#define KEYVOL_DEBOUNCE_TICKS       6553        // 200mS
#define KEYVOL_TIMEOUT_TICKS        288358      // 8.8 seconds

#define AUD_PROG_TIMEOUT_TICKS      1310        // 40ms

extern BYTE SysFlags0;

#if defined( RIM_SM0_MAIN_VCC )
#define NUM_SVCVLVLS        6
extern BYTE SvcVLvls[ NUM_SVCVLVLS ];
#endif // RIM_SM0_MAIN_VCC

#if defined( GPIO_PWR_SEQUENCE )
// must match order in SUPPLY_CTRL array
typedef enum {
    #if defined( AUD_PROG_EN ) && defined( AUD_PROG_EN_SUPPLY )
    CTRL_AUD_PROG_EN,
    #endif

    #if defined( BWF_26M_EN ) && defined( BWF_26M_EN_SUPPLY )
    CTRL_BWF_26M_EN,
    #endif

    #if defined( GPS_TCXO_EN ) && defined( GPS_TCXO_EN_SUPPLY )
    CTRL_GPS_TCXO_EN,
    #endif

    #if defined( CLK_ON ) && defined( CLK_ON_SUPPLY )
    CTRL_CLK_ON_EN,
    #endif

    #if defined( CP_26M_REQ_EN ) && defined( CP_26M_REQ_EN_SUPPLY )
    CTRL_CP_26M_REQ_EN,
    #endif

    #if defined( GPS_WBF_TCXO_EN ) && defined( GPS_WBF_TCXO_EN_SUPPLY )
    CTRL_GPS_WBF_TCXO_EN,
    #endif

    // these 2 inputs control the FPWM enable on SM2
    #if defined( TX_ACTIVITY ) && defined( TX_ACTIVITY_EN_SUPPLY )
    CTRL_TX_ACTIVITY_FPWM_EN,
    #endif

    #if defined( RX_ACTIVITY ) && defined( RX_ACTIVITY_EN_SUPPLY )
    CTRL_RX_ACTIVITY_FPWM_EN,
    #endif

} SupplyCtrlIndex;

static const struct {
    const BYTE    enReg;
    const BYTE    enMask;
    const Line    ioLine;
} SUPPLY_CTRL[] = {
    #if defined( AUD_PROG_EN ) && defined( AUD_PROG_EN_SUPPLY )
    AUD_PROG_EN_SUPPLY,
    #endif

    #if defined( BWF_26M_EN ) && defined( BWF_26M_EN_SUPPLY )
    BWF_26M_EN_SUPPLY,
    #endif

    #if defined( GPS_TCXO_EN ) && defined( GPS_TCXO_EN_SUPPLY )
    GPS_TCXO_EN_SUPPLY,
    #endif

    #if defined( CLK_ON ) && defined( CLK_ON_SUPPLY )
    CLK_ON_SUPPLY,
    #endif

    #if defined( CP_26M_REQ_EN ) && defined( CP_26M_REQ_EN_SUPPLY )
    CP_26M_REQ_EN_SUPPLY,
    #endif

    #if defined( GPS_WBF_TCXO_EN ) && defined( GPS_WBF_TCXO_EN_SUPPLY )
    GPS_WBF_TCXO_EN_SUPPLY,
    #endif

    // these 2 inputs control the FPWM enable on SM2
    #if defined( TX_ACTIVITY ) && defined( TX_ACTIVITY_EN_SUPPLY )
    TX_ACTIVITY_EN_SUPPLY,
    #endif

    #if defined( RX_ACTIVITY ) && defined( RX_ACTIVITY_EN_SUPPLY )
    RX_ACTIVITY_EN_SUPPLY,
    #endif
};
#endif   // GPIO_PWR_SEQUENCE

#if defined( RIM_SM0_MAIN_VCC )
// variable used to save the V level before going into disable
// so that we can re-enable at the previous level.
static BYTE sm0_vlvl_b4_dis = 0;
#endif // RIM_SM0_MAIN_VCC


#if defined( RIM_KEYVOL_RESET )

enum {
    KEY_DISABLED,
    KEY_WAIT_FOR_EDGE,
    KEY_DEBOUNCE,
    KEY_WAIT_DELAY
};
static BYTE keyVolState;

#if defined( KEYVOL_UP_DN )
/******************************************************************************
 * KeyVolIsr( )
 *
 * Handle KEYVOL_UP_DN IO level change
 *****************************************************************************/
void KeyVolToggle( WORD tmrExpired )
{
    BOOL io = PadRead( KEYVOL_UP_DN );

    switch( keyVolState ) {
        case KEY_DISABLED:
            // somehow got in here when we were disabled, disable the interrupt
            // and turn off the timer
            PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_DISABLE );
            StopTimer( KEYVOL_UP_DN_TIMER );
            break;
        case KEY_WAIT_FOR_EDGE:
            // debounce rising edge
            PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_DISABLE );
            keyVolState = KEY_DEBOUNCE;
            SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_DEBOUNCE_TICKS );
            break;

        case KEY_DEBOUNCE:
        keyDebounceIo:
            // if IO is still low lets re-enable interrutps and see if it
            // stays low
            if( !io ) {
                PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_CLEAR );
                PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_RISING );
                // make sure we didn't miss an edge between the io getting
                // set and setting up the interrupt
                if( PadRead( KEYVOL_UP_DN ) ) {
                    // if we did setup the DEBOUNCE timer again
                    PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_DISABLE );
                    SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_DEBOUNCE_TICKS );
                } else {
                    // wait 8seconds and then trigger the reset unless
                    // the user releases the keys
                    keyVolState = KEY_WAIT_DELAY;
                    SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_TIMEOUT_TICKS );
                }
            } else {
                // if the timer expired and the IO is low, go back to waiting
                // for the edge
                PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_CLEAR );
                PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_FALLING );
                // make sure we didn't miss an edge
                if( !PadRead( KEYVOL_UP_DN ) ) {
                    // if we did, setup the DEBOUNCE timer again
                    PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_DISABLE );
                    SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_DEBOUNCE_TICKS );
                } else {
                    // if we are still low, we are back at the idle state waiting
                    // for an edge
                    keyVolState = KEY_WAIT_FOR_EDGE;
                }
            }
            break;

        case KEY_WAIT_DELAY:
            // one last check, if the 8sec timer expired without being
            // interrupted and we are still LOW reset the device
            if( tmrExpired && !io ) {
                EventLogAdd( EVENTLOG_RESET_KEYVOL );
                // disable all the supplies on rocket
                HostCtrlDisableSupplies( 0 );
                //HostCtrlInitiateReset( RESET_COOKIE_INITIATED_KEYVOL, RESET_PMU430_CORE | RESET_LCD_IO );
                RimInitiateReset( RESET_COOKIE_INITIATED_KEYVOL );
            } else {
                keyVolState = KEY_DEBOUNCE;
                goto keyDebounceIo;
            }
            break;
    }
}
#else
void KeyVolToggle( WORD tmrExpired )
{
    BOOL up = PadRead( KEYVOL_UP );
    BOOL dn = PadRead( KEYVOL_DN );

    switch( keyVolState ) {
        case KEY_DISABLED:
            // somehow got in here when we were disabled, disable the interrupt
            // and turn off the timer
            PadConfig( KEYVOL_UP, PAD_INTERRUPT_DISABLE );
            PadConfig( KEYVOL_DN, PAD_INTERRUPT_DISABLE );
            StopTimer( KEYVOL_UP_DN_TIMER );
            break;
        case KEY_WAIT_FOR_EDGE:
            // debounce rising edge
            PadConfig( KEYVOL_UP, PAD_INTERRUPT_DISABLE );
            PadConfig( KEYVOL_DN, PAD_INTERRUPT_DISABLE );
            keyVolState = KEY_DEBOUNCE;
            SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_DEBOUNCE_TICKS );
            break;

        case KEY_DEBOUNCE:
        keyDebounceIo:
            // if IO is still low lets re-enable interrutps and see if it
            // stays low
            if( !up && !dn ) {
                PadConfig( KEYVOL_UP, PAD_INTERRUPT_CLEAR );
                PadConfig( KEYVOL_UP, PAD_INTERRUPT_RISING );
                PadConfig( KEYVOL_DN, PAD_INTERRUPT_CLEAR );
                PadConfig( KEYVOL_DN, PAD_INTERRUPT_RISING );
                // make sure we didn't miss an edge between the io getting
                // set and setting up the interrupt
                if( PadRead( KEYVOL_UP ) || PadRead( KEYVOL_DN ) ) {
                    // if we did setup the DEBOUNCE timer again
                    PadConfig( KEYVOL_UP, PAD_INTERRUPT_DISABLE );
                    PadConfig( KEYVOL_DN, PAD_INTERRUPT_DISABLE );
                    SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_DEBOUNCE_TICKS );
                } else {
                    // wait 8seconds and then trigger the reset unless
                    // the user releases the keys
                    keyVolState = KEY_WAIT_DELAY;
                    SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_TIMEOUT_TICKS );
                }
            } else {
                // if the timer expired and the IO is low, go back to waiting
                // for the edge
                PadConfig( KEYVOL_UP, PAD_INTERRUPT_CLEAR );
                PadConfig( KEYVOL_UP, PAD_INTERRUPT_FALLING );
                PadConfig( KEYVOL_DN, PAD_INTERRUPT_CLEAR );
                PadConfig( KEYVOL_DN, PAD_INTERRUPT_FALLING );
                // make sure we didn't miss an edge
                if( !PadRead( KEYVOL_UP ) || !PadRead( KEYVOL_DN ) ) {
                    // if we did, setup the DEBOUNCE timer again
                    PadConfig( KEYVOL_UP, PAD_INTERRUPT_DISABLE );
                    PadConfig( KEYVOL_DN, PAD_INTERRUPT_DISABLE );
                    SetTimer( KEYVOL_UP_DN_TIMER, KEYVOL_DEBOUNCE_TICKS );
                } else {
                    // if we are still low, we are back at the idle state waiting
                    // for an edge
                    keyVolState = KEY_WAIT_FOR_EDGE;
                }
            }
            break;

        case KEY_WAIT_DELAY:
            // one last check, if the 8sec timer expired without being
            // interrupted and we are still LOW reset the device
            if( tmrExpired && !up && !dn ) {
                EventLogAdd( EVENTLOG_RESET_KEYVOL );
                // disable all the supplies on rocket
                HostCtrlDisableSupplies( 0 );
                //HostCtrlInitiateReset( RESET_COOKIE_INITIATED_KEYVOL, RESET_PMU430_CORE | RESET_LCD_IO );
                RimInitiateReset( RESET_COOKIE_INITIATED_KEYVOL );
            } else {
                keyVolState = KEY_DEBOUNCE;
                goto keyDebounceIo;
            }
            break;
    }
    return;
}
#endif // KEYVOL_UP_DN
#endif

#if defined( GPIO_PWR_SEQUENCE )
/******************************************************************************
 * PwrSupplyEnable( idx )
 *
 * Enables the supply associated with the specified CTRL_GPIO index. Used as
 * a timer handler for the GPIO's which require a delay before the specific
 * supply is enabled
 *****************************************************************************/
void PwrSupplyEnable( WORD idx )
{
    EventLogAddN( EVENTLOG_DEBUG_EVENT_A,idx );
    switch( idx ) {
#if defined( CLK_ON_SUPPLY )
        case CTRL_CLK_ON_EN:
            // ECO mode enable logic is reversed (eco mode disabled when CLK_ON high)
            PmicClear( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );

            #if defined( RIM_SM0_MAIN_VCC )
            // sanity boundary check on the V levels
            if( (sm0_vlvl_b4_dis <= SvcVLvls[PWRI2C_REG_SVCVLVLD2 - 1]) ||
                (sm0_vlvl_b4_dis > SvcVLvls[PWRI2C_REG_SVCVLVL4 - 1]) ) {
                sm0_vlvl_b4_dis = SvcVLvls[PWRI2C_REG_SVCVLVL2 -1];
            }

			PmicWrite( PMIC_VSM0, sm0_vlvl_b4_dis );
            #endif

            #if defined( CLK_ON_TOGGLES_LDO11 )
            PmicSet( PMIC_EN_CMD2, CMD2_EN_LDO11 );
            #endif // CLK_ON_TOGGLES_LDO11

            #if defined( BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11 )
            PmicSet( PMIC_EN_CMD2, CMD2_EN_LDO11 );
            #if defined( TCXO_VCC_EN_SW )
            PadConfig( TCXO_VCC_EN_SW, PAD_OUTPUT_HIGH );
            #endif //TCXO_VCC_EN_SW
            #endif // BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11

            #if defined( BONSAI_PWR_EN_REG ) && defined( BONSAI_PWR_EN_MASK )
            // if bonsai is active, don't touch its supply; otherwise turn it back on
            if( (SysFlags0 & ROCKET_SYSFLAGS0_BONSAI_ACTIVE) == 0 ) {
                PmicSet( BONSAI_PWR_EN_REG, BONSAI_PWR_EN_MASK );
            }
            #endif
            break;
#endif // CLK_ON_SUPPLY

#if defined( AUD_PROG_EN_SUPPLY )
        case CTRL_AUD_PROG_EN:
            if( SysFlags0 & ROCKET_SYSFLAGS0_ALLOW_AUD_PROG ) {
                PmicSet( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );
                SetTimer( AUD_PROG_EN_TIMER, AUD_PROG_TIMEOUT_TICKS );
                SysFlags0 &= ~ROCKET_SYSFLAGS0_ALLOW_AUD_PROG;
            }
            break;
#endif // AUD_PROG_EN_SUPPLY

#if defined( BWF_26M_EN_SUPPLY )
        case CTRL_BWF_26M_EN:
            #if defined( BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11 )
            PmicSet( PMIC_EN_CMD2, CMD2_EN_LDO11 );
            #if defined( TCXO_VCC_EN_SW )
            PadConfig( TCXO_VCC_EN_SW, PAD_OUTPUT_HIGH );
            #endif //TCXO_VCC_EN_SW
            #endif // BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11
        break;
#endif //BWF_26M_EN_SUPPLY
        default:
            PmicSet( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );
            break;
    }
}

/******************************************************************************
 * PwrSupplyDisable( idx )
 *
 * Disables the supply associated with the specified CTRL_GPIO index.
 *****************************************************************************/
void PwrSupplyDisable( WORD idx )
{
    EventLogAddN( EVENTLOG_DEBUG_EVENT_B,idx );
    switch( idx ) {
#if defined( CLK_ON_SUPPLY )
        case CTRL_CLK_ON_EN:
            
            #if defined( RIM_SM0_MAIN_VCC )
            sm0_vlvl_b4_dis = PmicRead( PMIC_VSM0 );
            PmicWrite( PMIC_VSM0, SvcVLvls[PWRI2C_REG_SVCVLVLD2 - 1] );
            #endif

            // if bonsai isn't active, allow eco mode
            if( (SysFlags0 & ROCKET_SYSFLAGS0_BONSAI_ACTIVE) == 0 ) {
                // ECO mode enable logic is reversed (eco mode enabled when CLK_ON low)
                PmicSet( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );
            }
            #if defined( CLK_ON_TOGGLES_LDO11 )
            PmicClear( PMIC_EN_CMD2, CMD2_EN_LDO11 );
            #endif // CLK_ON_TOGGLES_LDO11

            #if defined( BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11 )
            // If BWF_EN is low as well, disable the LDO and lower out GPIO
            if(PadRead(BWF_26M_EN) == FALSE) {
            PmicClear( PMIC_EN_CMD2, CMD2_EN_LDO11 );

            #if defined( TCXO_VCC_EN_SW )
            PadConfig( TCXO_VCC_EN_SW, PAD_OUTPUT_LOW );
            #endif //TCXO_VCC_EN_SW
		    }
            #endif // BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11

            #if defined( BONSAI_PWR_EN_REG ) && defined( BONSAI_PWR_EN_MASK )
            // if bonsai is active, don't touch its supply; otherwise turn it off
            if( (SysFlags0 & ROCKET_SYSFLAGS0_BONSAI_ACTIVE) == 0 ) {
                PmicClear( BONSAI_PWR_EN_REG, BONSAI_PWR_EN_MASK );
            }
            #endif
            break;
#endif // CLK_ON_SUPPLY

#if defined( AUD_PROG_EN_SUPPLY )
        case CTRL_AUD_PROG_EN:
                StopTimer( AUD_PROG_EN_TIMER );
                PmicClear( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );
            break;
#endif // AUD_PROG_EN_SUPPLY
#if defined( TX_ACTIVITY_EN_SUPPLY ) && defined( RX_ACTIVITY_EN_SUPPLY )
        case CTRL_TX_ACTIVITY_FPWM_EN:
        case CTRL_RX_ACTIVITY_FPWM_EN:
            // if both signals are low, disable FPWM mode
            if( (PadRead( TX_ACTIVITY ) | PadRead( RX_ACTIVITY )) == 0 ) {
                PmicClear( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );
            }
            break;
#endif // TX_ACTIVITY_EN_SUPPLY && RX_ACTIVITY_EN_SUPPLY
#if defined( BWF_26M_EN_SUPPLY )
        case CTRL_BWF_26M_EN:
            #if defined( BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11 )
            // If CLK_ON is low as well, disable the LDO and lower out GPIO
            if(PadRead(CLK_ON) == FALSE) {
            PmicClear( PMIC_EN_CMD2, CMD2_EN_LDO11 );

            #if defined( TCXO_VCC_EN_SW )
            PadConfig( TCXO_VCC_EN_SW, PAD_OUTPUT_LOW );
            #endif //TCXO_VCC_EN_SW
		    }
            #endif // BWF_EN_SW_OR_CLK_ON_TOGGLES_LDO11
        break;
#endif //BWF_26M_EN_SUPPLY

        default:
            PmicClear( SUPPLY_CTRL[idx].enReg, SUPPLY_CTRL[idx].enMask );
            break;
    }
}

#if defined( AUD_PROG_EN_SUPPLY )
void AudProgEnTimeout( WORD unused )
{
    SysFlags0 &= ~ROCKET_SYSFLAGS0_ALLOW_AUD_PROG;
    PmicClear( SUPPLY_CTRL[CTRL_AUD_PROG_EN].enReg, SUPPLY_CTRL[CTRL_AUD_PROG_EN].enMask );
}
#endif

/******************************************************************************
 * PwrSupplyToggle( idx )
 *
 * Generic handler for all GPIO triggers associated with specific supply rails.
 * Also clears/sets up the next interrupt for the specific IO
 *****************************************************************************/
BOOL PwrSupplyToggle( SupplyCtrlIndex idx )
{
    Line ioLine = SUPPLY_CTRL[idx].ioLine;
    BOOL state;

    do {
        state = PadRead( ioLine );
        PadConfig( ioLine, PAD_INTERRUPT_CLEAR );

        if( state ) {
            PadConfig( ioLine, PAD_INTERRUPT_FALLING );
        } else {
            PadConfig( ioLine, PAD_INTERRUPT_RISING );
        }

    } while( state != PadRead( ioLine ) );

    if( state ) {
        PwrSupplyEnable( idx );
    } else {
        PwrSupplyDisable( idx );
    }

    return( state );
}

/******************************************************************************
 * PwrSupplyToggleRegister( idx, handler )
 *
 * Handle all the init/registering of the IO control for the specific supply.
 *****************************************************************************/
void PwrSupplyToggleRegister( SupplyCtrlIndex idx, FUNCTION handler )
{
    Line ioLine = SUPPLY_CTRL[idx].ioLine;

    // Register interrupt handler for IO
    PadRegisterIntHandler( ioLine, handler );

    // Enable IO interrupts and handle if we missed the first rising edge
    PadConfig( ioLine, PAD_INPUT_PULLDOWN );
    PadConfig( ioLine, PAD_INTERRUPT_RISING );
    if( PadRead( ioLine ) ) {
        handler();
    }
}
#endif   // GPIO_PWR_SEQUENCE

#if defined( PLATFORM_RIMKELOWNA )
void BtWifiGpsHandler( void )
{
    BOOL gpsState;
    BOOL bwfState;

    // configure next edge interrupt and make sure we don't miss a transition
    do {
        gpsState = PadRead( GPS_TCXO_EN );
        bwfState = PadRead( BWF_26M_EN );

        PadConfig( GPS_TCXO_EN, PAD_INTERRUPT_CLEAR );
        PadConfig( BWF_26M_EN, PAD_INTERRUPT_CLEAR );

        if( gpsState ) {
            PadConfig( GPS_TCXO_EN, PAD_INTERRUPT_FALLING );
        } else {
            PadConfig( GPS_TCXO_EN, PAD_INTERRUPT_RISING );
        }

        if( bwfState ) {
            PadConfig( BWF_26M_EN, PAD_INTERRUPT_FALLING );
        } else {
            PadConfig( BWF_26M_EN, PAD_INTERRUPT_RISING );
        }
    } while( (gpsState != PadRead( GPS_TCXO_EN )) || (bwfState != PadRead( BWF_26M_EN ) ) );

    if( gpsState || bwfState ) {
        // if BT/WIFI or GPS are on, we need to enable the GPS supply to power
        // the shared TCXO
        PmicSet( GPS_PWR_EN_REG, GPS_PWR_EN_MASK );
    } else if( (gpsState || bwfState) == 0 ) {
        // if both are off, we can shut down the shared TCX0
        PmicClear( GPS_PWR_EN_REG, GPS_PWR_EN_MASK );
    }

    if( bwfState ) {
        PmicSet( BTWIFI_PWR_EN_REG, BTWIFI_PWR_EN_MASK );
    } else {
        PmicClear( BTWIFI_PWR_EN_REG, BTWIFI_PWR_EN_MASK );
    }
}
#endif

#if defined( AUD_PROG_EN ) && defined( AUD_PROG_EN_SUPPLY )
/******************************************************************************
 * AudProgEnHandler( )
 *
 * Handle AUD_PROG_EN IO level change
 *****************************************************************************/
void AudProgEnHandler( void )
{
    PwrSupplyToggle( CTRL_AUD_PROG_EN );
}
#endif

#if defined( BWF_26M_EN ) && defined( BWF_26M_EN_SUPPLY )
/******************************************************************************
 * BwfEnHandler( )
 *
 * Handle BWF_26M_EN IO level change
 *****************************************************************************/
void BwfEnHandler( void )
{
    PwrSupplyToggle( CTRL_BWF_26M_EN );
}
#endif

#if defined( GPS_TCXO_EN ) && defined( GPS_TCXO_EN_SUPPLY )
/******************************************************************************
 * GpsTcxoEnHandler( )
 *
 * Handle GPS_TCXO_EN IO level change
 *****************************************************************************/
void GpsTcxoEnHandler( void )
{
    PwrSupplyToggle( CTRL_GPS_TCXO_EN );
}
#endif

#if defined( CLK_ON ) && defined( CLK_ON_SUPPLY )
/******************************************************************************
 * EcoModeEnHandler( )
 *
 * Handle CLK_ON IO level change to support ECO mode control
 *****************************************************************************/
void EcoModeEnHandler( void )
{
    PwrSupplyToggle( CTRL_CLK_ON_EN );
}
#endif

#if defined( GPS_WBF_TCXO_EN ) && defined( GPS_WBF_TCXO_EN_SUPPLY )
/******************************************************************************
 * EcoModeEnHandler( )
 *
 * Handle GPS_WBF_TCXO_EN IO level change
 *****************************************************************************/
void GpsWbfTcxoEnHandler( void )
{
    PwrSupplyToggle( CTRL_GPS_WBF_TCXO_EN );
}
#endif

#if defined( TX_ACTIVITY ) && defined( TX_ACTIVITY_EN_SUPPLY )
/******************************************************************************
 * TxActivityFpwmEnHandler( )
 *
 * Handle TX_ACTIVITY IO level change
 *****************************************************************************/
void TxActivityFpwmEnHandler( void )
{
    PwrSupplyToggle( CTRL_TX_ACTIVITY_FPWM_EN );
}
#endif

#if defined( RX_ACTIVITY ) && defined( RX_ACTIVITY_EN_SUPPLY )
/******************************************************************************
 * RxActivityFpwmEnHandler( )
 *
 * Handle RX_ACTIVITY IO level change
 *****************************************************************************/
void RxActivityFpwmEnHandler( void )
{
    PwrSupplyToggle( CTRL_RX_ACTIVITY_FPWM_EN );
}
#endif


#if defined( CP_26M_REQ_EN ) && defined( CP_26M_REQ_EN_SUPPLY )
/******************************************************************************
 * Cp26MReqEnHandler( )
 *
 * Handle CP_26M_REQ_EN IO level change
 *****************************************************************************/
void Cp26MReqEnHandler( void )
{
    PwrSupplyToggle( CTRL_CP_26M_REQ_EN );
}
#endif

#if defined( RIM_KEYVOL_RESET )
/******************************************************************************
 * KeyVolHandler( )
 *
 * Handle KEYVOL_UP_DN IO level change
 *****************************************************************************/
void KeyVolHandler( void )
{
    KeyVolToggle( 0 );
}

/******************************************************************************
 * IsKeyVolumeResetEnabled( )
 *
 * Returns current state of the KEYVOL handler
 *****************************************************************************/
BOOL IsKeyVolumeResetEnabled( void )
{
    return( keyVolState != KEY_DISABLED );
}

/******************************************************************************
 * KeyVolumeResetEnable( enable )
 *
 * Enables or disables the volume key "hold-to-reset" functionality
 *****************************************************************************/
void KeyVolumeResetEnable( BOOL enable )
{
    // only enable if we were previously disabled
    if( enable && (keyVolState == KEY_DISABLED) ) {
        keyVolState = KEY_WAIT_FOR_EDGE;

        // Enable IO interrupts and handle if we missed the first rising edge
#if defined( KEYVOL_UP_DN )
        PadConfig( KEYVOL_UP_DN, PAD_INPUT_ENABLE );
        PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_FALLING );
        if( !PadRead( KEYVOL_UP_DN ) ) {
            KeyVolHandler();
        }
#else
        PadConfig( KEYVOL_UP, PAD_INPUT_ENABLE );
        PadConfig( KEYVOL_UP, PAD_INTERRUPT_FALLING );
        PadConfig( KEYVOL_DN, PAD_INPUT_ENABLE );
        PadConfig( KEYVOL_DN, PAD_INTERRUPT_FALLING );
        if( !PadRead( KEYVOL_UP ) || !PadRead( KEYVOL_DN ) ) {
            KeyVolHandler();
        }
#endif
    } else if( !enable ) {
        keyVolState = KEY_DISABLED;
        StopTimer( KEYVOL_UP_DN_TIMER );
#if defined( KEYVOL_UP_DN )
        PadConfig( KEYVOL_UP_DN, PAD_INTERRUPT_DISABLE );
#else
        PadConfig( KEYVOL_UP, PAD_INTERRUPT_DISABLE );
        PadConfig( KEYVOL_DN, PAD_INTERRUPT_DISABLE );
#endif
    }
}
#endif

#if defined( DIGRF_CLK_EN )
/******************************************************************************
 * DigRFClkEnTimerHandler( )
 *
 * Handle DIGRF_CLK_EN_TIMER timer expiry to reduce VSM0 to 1.2V from 1.25V
 *****************************************************************************/
void DigRFClkEnTimerHandler( WORD unused )
{
#if defined( HOSTPROCESSOR_TAVOR_MG )
#if defined( RIM_LDO1_SUPPLIES_RF )
      PmicWrite(PMIC_VLDO1, LDOSM_DCHG_EN|LVLDO_1V200); //Set LDO1 to 1.2 volts
#else
      PmicWrite( PMIC_VSM0, 0x30 ); // VSM0 = 1.2V
#endif
#else
    // TODO: need to adjust the 1.2dig supply here. on OMAP/Tedeschi rev1 this is an external
    // switcher
#warn DIGRF_CLK_EN handler does not exist

#endif
    return;
}

/******************************************************************************
 * DigRFClkEnHandler( )
 *
 * Handle DIGRF_CLK_EN IO level change
 *****************************************************************************/
void DigRFClkEnHandler( void )
{
    BOOL state;
    static BOOL triggeredOnce = FALSE;

    do {
        state = PadRead( DIGRF_CLK_EN );
        PadConfig( DIGRF_CLK_EN, PAD_INTERRUPT_CLEAR );

        if( state ) {
            PadConfig( DIGRF_CLK_EN, PAD_INTERRUPT_FALLING );
        } else {
            PadConfig( DIGRF_CLK_EN, PAD_INTERRUPT_RISING );
        }

    } while( state != PadRead( DIGRF_CLK_EN ) );

    // on first assertion of DIGRF_CLK_EN we need to wait 10ms and then drop VSM0 to 1.2V
    if( state & !triggeredOnce) {
        SetTimer( DIGRF_CLK_EN_TIMER, 328 );
        triggeredOnce = TRUE;
    }

#if defined( HOSTPROCESSOR_TAVOR_MG ) || defined( HOSTPROCESSOR_ESHEL )
    if( state ) {
        PmicSet( PMIC_BUCKCTRL, PMIC_BUCKCTRL_SM3_FPWM | PMIC_BUCKCTRL_SM2_FPWM );
    } else {
        PmicClear( PMIC_BUCKCTRL, PMIC_BUCKCTRL_SM3_FPWM | PMIC_BUCKCTRL_SM2_FPWM );
    }
#elif defined( HOSTPROCESSOR_OMAP4430 )
    if( state ) {
        PadConfig( V1_8DIG_PWM, PAD_OUTPUT_HIGH );
        PmicSet( PMIC_BUCKCTRL, PMIC_BUCKCTRL_SM3_FPWM );
    } else {
        PadConfig( V1_8DIG_PWM, PAD_OUTPUT_LOW );
        PmicClear( PMIC_BUCKCTRL, PMIC_BUCKCTRL_SM3_FPWM );
    }
#endif

    return;
}
#endif

/******************************************************************************
 * HostCtrlPwrInit( )
 *
 * Init all Power control funcationality (called from APP init)
 *****************************************************************************/
void HostCtrlPwrInit( void )
{
    WORD wason;

    // Configure each GPIO interrupt; should already be in INPUT mode
    // from GPIO init
    #if defined( AUD_PROG_EN ) && defined( AUD_PROG_EN_SUPPLY )
    RegisterTimerExpiry( AUD_PROG_EN_TIMER, AudProgEnTimeout, 0 );
    PwrSupplyToggleRegister( CTRL_AUD_PROG_EN, AudProgEnHandler );
    #endif

    #if defined( BWF_26M_EN ) && defined( BWF_26M_EN_SUPPLY )
    PwrSupplyToggleRegister( CTRL_BWF_26M_EN, BwfEnHandler );
    #endif

    #if defined( GPS_TCXO_EN ) && defined( GPS_TCXO_EN_SUPPLY )
    PwrSupplyToggleRegister( CTRL_GPS_TCXO_EN, GpsTcxoEnHandler );
    #endif

    #if defined( CLK_ON ) && defined( CLK_ON_SUPPLY )
    PwrSupplyToggleRegister( CTRL_CLK_ON_EN, EcoModeEnHandler );
    #endif

    #if defined( CP_26M_REQ_EN ) && defined( CP_26M_REQ_EN_SUPPLY )
    PwrSupplyToggleRegister( CTRL_CP_26M_REQ_EN, Cp26MReqEnHandler );
    #endif

    #if defined( GPS_WBF_TCXO_EN ) && defined( GPS_WBF_TCXO_EN_SUPPLY )
    PwrSupplyToggleRegister( CTRL_GPS_WBF_TCXO_EN, GpsWbfTcxoEnHandler );
    #endif

    #if defined( TX_ACTIVITY ) && defined( TX_ACTIVITY_EN_SUPPLY )
    PwrSupplyToggleRegister( CTRL_TX_ACTIVITY_FPWM_EN, TxActivityFpwmEnHandler );
    #endif

    #if defined( RX_ACTIVITY ) && defined( RX_ACTIVITY_EN_SUPPLY )
    PwrSupplyToggleRegister( CTRL_RX_ACTIVITY_FPWM_EN, RxActivityFpwmEnHandler );
    #endif

    #if defined( DIGRF_CLK_EN )
     // Register interrupt handler for IO
    PadRegisterIntHandler( DIGRF_CLK_EN, DigRFClkEnHandler );
    RegisterTimerExpiry( DIGRF_CLK_EN_TIMER, DigRFClkEnTimerHandler, 0 );
    // Enable IO interrupts and handle if we missed the first rising edge
    PadConfig( DIGRF_CLK_EN, PAD_INPUT_PULLDOWN );
    PadConfig( DIGRF_CLK_EN, PAD_INTERRUPT_RISING );
    if( PadRead( DIGRF_CLK_EN ) ) {
        DigRFClkEnHandler();
    }
    #endif

    #if defined( PLATFORM_RIMKELOWNA )
    // Register interrupt handler for IO
    PadRegisterIntHandler( GPS_TCXO_EN, BtWifiGpsHandler );
    PadRegisterIntHandler( BWF_26M_EN, BtWifiGpsHandler );

    // Enable IO interrupts and handle if we missed the first rising edge
    PadConfig( GPS_TCXO_EN, PAD_INPUT_PULLDOWN );
    PadConfig( BWF_26M_EN, PAD_INPUT_PULLDOWN );

    // call the handler to initialize its state and configure the initial
    // levels of the supplies
    wason = CoreIntDisable();
    BtWifiGpsHandler();
    CoreIntEnable( wason );
    #endif



#if defined( RIM_KEYVOL_RESET )
    keyVolState = KEY_DISABLED;
#if defined( KEYVOL_UP_DN )
    PadRegisterIntHandler( KEYVOL_UP_DN, KeyVolHandler );
#else
    PadRegisterIntHandler( KEYVOL_UP, KeyVolHandler );
    PadRegisterIntHandler( KEYVOL_DN, KeyVolHandler );
#endif

    // Register delay enable timer expiry handler for speicic CTRL index
    RegisterTimerExpiry( KEYVOL_UP_DN_TIMER, KeyVolToggle, 1 );

#if defined( RIM_KEYVOL_ENABLE )
    KeyVolumeResetEnable( TRUE );
#endif // RIM_KEYVOL_ENABLE

#endif

}

/******************************************************************************
 * HostCtrlPwrUninit( )
 *
 * Uninit all Power control funcationality (called from APP uninit)
 *****************************************************************************/
void HostCtrlPwrUninit( void )
{

    #if defined( AUD_PROG_EN ) && defined( AUD_PROG_EN_SUPPLY )
    PadConfig( AUD_PROG_EN, PAD_INTERRUPT_DISABLE );
    #endif

    #if defined( BWF_26M_EN ) && defined( BWF_26M_EN_SUPPLY )
    PadConfig( BWF_26M_EN, PAD_INTERRUPT_DISABLE );
    #endif

    #if defined( GPS_TCXO_EN ) && defined( GPS_TCXO_EN_SUPPLY )
    PadConfig( GPS_TCXO_EN, PAD_INTERRUPT_DISABLE );
    #endif

    #if defined( CLK_ON ) && defined( CLK_ON_SUPPLY )
    PadConfig( CLK_ON, PAD_INTERRUPT_DISABLE );
    #endif

    #if defined( TX_ACTIVITY ) && defined( TX_ACTIVITY_EN_SUPPLY )
    PadConfig( TX_ACTIVITY, PAD_INTERRUPT_DISABLE );
    #endif

    #if defined( RX_ACTIVITY ) && defined( RX_ACTIVITY_EN_SUPPLY )
    PadConfig( RX_ACTIVITY, PAD_INTERRUPT_DISABLE );
    #endif

    #if defined( DIGRF_CLK_EN )
    PadConfig( DIGRF_CLK_EN, PAD_INTERRUPT_DISABLE );
    StopTimer( DIGRF_CLK_EN_TIMER );
    #endif

    #if defined( PLATFORM_RIMKELOWNA )
    PadConfig( GPS_TCXO_EN, PAD_INTERRUPT_DISABLE );
    PadConfig( BWF_26M_EN, PAD_INTERRUPT_DISABLE );
    #endif

#if defined( RIM_KEYVOL_RESET )
    KeyVolumeResetEnable( FALSE );
#endif

}
