/*****************************************************************************
 * Filename:    rocket_gpio.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     November 25, 2008
 *
 * Description: Rocket gpio configuration routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "intvector.h"
#include "hostctrl.h"
#include "i2c.h"
#include "eventlog.h"

#if !defined( ROCKET_LOADER )
// interrupt handlers
void Port1IntHandler( void );
void Port2IntHandler( void );

// Only Pads on Port1 and Port2 support interrupts
// Each port has its own vector which service all
// 8 pads on each.
static FUNCTION PortHandler[2][8];
#endif  // !ROCKET_LOADER

void InternalUsciReset( BOOL inReset );

// utility function to get port address
WORD getPadBase( Line line )
{
    WORD ret;

    ret = GET_PORT_BASE( line ) + GET_PORT_OFFSET( line );

    return ret;
}

void PadConfig( Line line, PadFunction pf )
{
    BYTE padMask;
    WORD padBase;
    WORD wason = CoreIntDisable();

    padMask = 1<<(line & 0x7);
    padBase = getPadBase( line );

    switch( pf ) {
        case PAD_OUTPUT_HIGH:
            // level HIGH
            WriteRegByteOr( POUT_REG8(padBase), padMask );
            // direction OUT
            WriteRegByteOr( PDIR_REG8(padBase), padMask );
            break;
        case PAD_OUTPUT_LOW:
            // level LOW
            WriteRegByteAnd( POUT_REG8(padBase), ~padMask );
            // direction OUT
            WriteRegByteOr( PDIR_REG8(padBase), padMask );
            break;
        case PAD_INPUT_ENABLE:
            // pull resistor DISABLED
            WriteRegByteAnd( PREN_REG8(padBase), ~padMask );
            // direction IN
            WriteRegByteAnd( PDIR_REG8(padBase), ~padMask );
            break;
        case PAD_INPUT_PULLUP:
            // direction IN
            WriteRegByteAnd( PDIR_REG8(padBase), ~padMask );
            // pull direction UP
            WriteRegByteOr( POUT_REG8(padBase), padMask );
            // pull resistor ENABLED
            WriteRegByteOr( PREN_REG8(padBase), padMask );
            break;
        case PAD_INPUT_PULLDOWN:
            // direction IN
            WriteRegByteAnd( PDIR_REG8(padBase), ~padMask );
            // pull direction DOWN
            WriteRegByteAnd( POUT_REG8(padBase), ~padMask );
            // pull resistor ENABLED
            WriteRegByteOr( PREN_REG8(padBase), padMask );
            break;
#if !defined( ROCKET_LOADER )
        case PAD_INTERRUPT_RISING:
            WriteRegByteAnd( PIES_REG8(padBase), ~padMask );
            // Clear any pending flags ( Writing to PIES can result in
            // the PIFG reg being set
            WriteRegByteAnd( PIFG_REG8(padBase), ~padMask );
            // Enable interrupt for pad
            WriteRegByteOr( PIE_REG8(padBase), padMask );
            break;
        case PAD_INTERRUPT_FALLING:
            WriteRegByteOr( PIES_REG8(padBase), padMask );
            // Clear any pending flags ( Writing to PIES can result in
            // the PIFG reg being set
            WriteRegByteAnd( PIFG_REG8(padBase), ~padMask );
            // Enable interrupt for pad
            WriteRegByteOr( PIE_REG8(padBase), padMask );
            break;
        case PAD_INTERRUPT_DISABLE:
            WriteRegByteAnd( PIE_REG8(padBase), ~padMask );
            break;
        case PAD_INTERRUPT_CLEAR:
            WriteRegByteAnd( PIFG_REG8(padBase), ~padMask );
            break;
#endif // ROCKET_LOADER
        case PAD_ALT_MODE:
            WriteRegByteOr( PSEL_REG8(padBase), padMask );
            break;
        case PAD_STRENGTH_REDUCED:
            WriteRegByteAnd( PDS_REG8(padBase), ~padMask );
            break;
        case PAD_STRENGTH_FULL:
            WriteRegByteOr( PDS_REG8(padBase), padMask );
            break;
        default:
            break;

    }

    // if we arn't using ALT mode make sure we are in GPIO mode
    if( pf != PAD_ALT_MODE ) {
        WriteRegByteAnd( PSEL_REG8(padBase), ~padMask );
    }

    CoreIntEnable( wason );
}


// converts ALL signals to input, and enable their pulldowns to 
// reduce current consumption; with the exception of the 2 
// interrupt lines from the FG and the PowerDie. Also, dont touch
// the Supercap enable LDO or we will kill power to ourselves.
void PadSupercapMode( void )
{
    // hard coded since the driver was only designed to handle a
    // single signal change at a time
    //
    // disable all interrupts on IOs
    WriteRegByte( PIE_REG8(PORT1_BASE), 0 );
    WriteRegByte( PIE_REG8(PORT2_BASE), 0 );
    WriteRegByte( PIE_REG8(PORT3_BASE), 0 );
    WriteRegByte( PIE_REG8(PORT4_BASE), 0 );
    WriteRegByte( PIE_REG8(PORT5_BASE), 0 );
    WriteRegByte( PIE_REG8(PORTJ_BASE), 0 );

    // set direction to INPUT
    WriteRegByte( PDIR_REG8(PORT1_BASE), 0 );
    WriteRegByte( PDIR_REG8(PORT2_BASE), 0 );
    WriteRegByte( PDIR_REG8(PORT3_BASE), 0 );
    WriteRegByte( PDIR_REG8(PORT4_BASE), 0 );
    WriteRegByteAnd( PDIR_REG8(PORT5_BASE), ~0xBF );    // don't touch 5P6 since that is the supercap switch
    WriteRegByte( PDIR_REG8(PORTJ_BASE), 0 );

    // make sure the function selected is IO
    WriteRegByte( PSEL_REG8(PORT1_BASE), 0 );
    WriteRegByte( PSEL_REG8(PORT2_BASE), 0 );
    WriteRegByte( PSEL_REG8(PORT3_BASE), 0 );
    WriteRegByte( PSEL_REG8(PORT4_BASE), 0 );
    WriteRegByteAnd( PSEL_REG8(PORT5_BASE), 0x3 ); // don't reconfigure the SPI pins
    WriteRegByte( PSEL_REG8(PORTJ_BASE), 0 );

    // configure all IOs for pulldown resistors
    WriteRegByte( POUT_REG8(PORT1_BASE), 0 );
    WriteRegByte( POUT_REG8(PORT2_BASE), 0 );
    WriteRegByte( POUT_REG8(PORT3_BASE), 0 );
    WriteRegByte( POUT_REG8(PORT4_BASE), 0 );
    WriteRegByteAnd( POUT_REG8(PORT5_BASE), ~0xBF ); // don't touch 5P6 since thats the supercap switch
    WriteRegByte( POUT_REG8(PORTJ_BASE), 0 );

    // enable the pulldowns
    WriteRegByte( PREN_REG8(PORT1_BASE), 0xFF );
    WriteRegByte( PREN_REG8(PORT2_BASE), 0x3F );    // don't enable the pulldown on the internal PMIC_NPWRINT and FG_SOC_INT signals
    WriteRegByte( PREN_REG8(PORT3_BASE), 0xFF );
    WriteRegByte( PREN_REG8(PORT4_BASE), 0xFF );
    WriteRegByte( PREN_REG8(PORT5_BASE), 0xBF );    // dont touch 5P6
    WriteRegByte( PREN_REG8(PORTJ_BASE), 0x0F );

    return;
}

BOOL PadRead( Line line )
{
    BYTE reg;
    BYTE padMask;
    WORD padBase;
    WORD wason = CoreIntDisable();

    padMask = 1<<(line & 0x7);
    padBase = getPadBase( line );

    reg = ReadRegByte( PIN_REG8(padBase) ) & padMask;

    CoreIntEnable( wason );

    return( (reg != 0 ) );
}

#if !defined( ROCKET_LOADER )

void PadRegisterIntHandler( Line line, FUNCTION function )
{
    BYTE portNum = GET_PORT_NUMBER( line );
    WORD index;
    WORD wason = CoreIntDisable();

    index = line & 0x7;

    // store handler function in array for appropriate port/index
    PortHandler[ portNum ][ index ] = function;

    CoreIntEnable( wason );
}

static void GpioInterruptFailure( void )
{
    // TODO: some error handling
}

static void InitializeGpioInterrupts( void )
{
    WORD i;
    // initialize any handlers that haven't been touched yet
    for( i=0 ; i<8 ; i++ ) {
        PortHandler[0][i] = GpioInterruptFailure;
        PortHandler[1][i] = GpioInterruptFailure;
    }
}

#if defined( I2C_RCKT_SDA ) && \
    defined( I2C_RCKT_SCL ) && \
    defined( FG_I2C_SDA ) &&   \
    defined( FG_I2C_SCL )
void ConfigureI2cPortMapping( BOOL external )
{
    static BOOL portMappedExternal = FALSE;
    WORD wason = CoreIntDisable();

    if( external != portMappedExternal ) {

        portMappedExternal = external;

        InternalUsciReset( TRUE );

        if( external ) {

            // make sure we arn't in the alt mode before we unconfigure it
            PadConfig( FG_I2C_SCL, PAD_INPUT_ENABLE );
            PadConfig( FG_I2C_SDA, PAD_INPUT_ENABLE );

            // UNLOCK access to the port mapping registers
            WriteRegWord( PMAPPWD_REG16, PMAPPWD_PASSWD );

            // map the external sensor i2c functionality
            WriteRegByte( PMAP_REG8( FG_I2C_SCL ), PM_NONE );
            WriteRegByte( PMAP_REG8( FG_I2C_SDA ), PM_NONE );
            WriteRegByte( PMAP_REG8( I2C_RCKT_SDA ), PM_UCB2SDA );
            WriteRegByte( PMAP_REG8( I2C_RCKT_SCL ), PM_UCB2SCL );

            // LOCK access to the port mapping registers
            WriteRegWord( PMAPPWD_REG16, 0x00 );

            PadConfig( I2C_RCKT_SCL, PAD_ALT_MODE );
            PadConfig( I2C_RCKT_SDA, PAD_ALT_MODE );

        } else {

            // make sure we arn't in the alt mode before we unconfigure it
            PadConfig( I2C_RCKT_SCL, PAD_INPUT_ENABLE );
            PadConfig( I2C_RCKT_SDA, PAD_INPUT_ENABLE );

            // UNLOCK access to the port mapping registers
            WriteRegWord( PMAPPWD_REG16, PMAPPWD_PASSWD );

            // unmap the external sensor i2c functionality and remap it to Fuel Gauge
            WriteRegByte( PMAP_REG8( I2C_RCKT_SCL ), PM_NONE );
            WriteRegByte( PMAP_REG8( I2C_RCKT_SDA ), PM_NONE );
            WriteRegByte( PMAP_REG8( FG_I2C_SDA ), PM_UCB2SDA );
            WriteRegByte( PMAP_REG8( FG_I2C_SCL ), PM_UCB2SCL );

            // LOCK access to the port mapping registers
            WriteRegWord( PMAPPWD_REG16, 0x00 );

            PadConfig( FG_I2C_SDA, PAD_ALT_MODE );
            PadConfig( FG_I2C_SCL, PAD_ALT_MODE );
        }

        InternalUsciReset( FALSE );
    }
    CoreIntEnable( wason );
}
#endif

static void ConfigurePortMapping( void )
{
    WORD wason;

    wason = CoreIntDisable();

    // make sure we arn't in the alt mode before we unconfigure it
    PadConfig( I2C_RCKT_SCL, PAD_INPUT_ENABLE );
    PadConfig( I2C_RCKT_SDA, PAD_INPUT_ENABLE );
    PadConfig( FG_I2C_SCL, PAD_INPUT_ENABLE );
    PadConfig( FG_I2C_SDA, PAD_INPUT_ENABLE );

    // UNLOCK access to the port mapping registers
    WriteRegWord( PMAPPWD_REG16, PMAPPWD_PASSWD );
    // Allow port mapping to be re-configured at run time
    // NOTE: MUST immediatly follow PASSWD write
    WriteRegWord( PMAPCTL_REG16, PMAPCTL_PMAPRECFG );

    // unmap the external sensor i2c functionality and remap it to Fuel Gauge
    WriteRegByte( PMAP_REG8( I2C_RCKT_SCL ), PM_NONE );
    WriteRegByte( PMAP_REG8( I2C_RCKT_SDA ), PM_NONE );
    WriteRegByte( PMAP_REG8( FG_I2C_SDA ), PM_UCB2SDA );
    WriteRegByte( PMAP_REG8( FG_I2C_SCL ), PM_UCB2SCL );

#if defined( ACLK_32KOUT_2 )
    WriteRegByte( PMAP_REG8( ACLK_32KOUT_2 ), PM_SMCLK );
#endif
#if defined( EXT_32K )
    WriteRegByte( PMAP_REG8( EXT_32K ), PM_ACLK );
#endif
#if defined( PMIC_32K_CLK )
    WriteRegByte( PMAP_REG8( PMIC_32K_CLK), PM_ACLK );
#endif
#if defined( SYS_32K_CLK0 )
    WriteRegByte( PMAP_REG8( SYS_32K_CLK0 ), PM_ACLK );
#endif
#if defined( SYS_32K_CLK1 )
    WriteRegByte( PMAP_REG8( SYS_32K_CLK1 ), PM_ACLK );
#endif

#if defined( RIM_PWM )
    WriteRegByte( PMAP_REG8( PAD_PORT_X_X ), PM_TA1CCR1A );
#endif

    // LOCK access to the port mapping registers
    WriteRegWord( PMAPPWD_REG16, 0x00 );

    PadConfig( FG_I2C_SDA, PAD_ALT_MODE );
    PadConfig( FG_I2C_SCL, PAD_ALT_MODE );

    CoreIntEnable( wason );
}


void GpioInit( void )
{
    ConfigurePortMapping();

    RegisterIntHandler( PORT1_VECTOR, Port1IntHandler );
    RegisterIntHandler( PORT2_VECTOR, Port2IntHandler );

    InitializeGpioInterrupts();

    // ---- Internal Rocket pin configurations ----

    // Configure internal PMU430-to-PMIC SPI pin modes
    PadConfig( PMIC_SPI_CS, PAD_OUTPUT_LOW );
    PadConfig( PMIC_SPI_SIMO, PAD_ALT_MODE );
    PadConfig( PMIC_SPI_SOMI, PAD_ALT_MODE );
    PadConfig( PMIC_SPI_CLK, PAD_ALT_MODE );

    // ---- External Rocket pin configurations ----

    PadConfig( HOST_I2C_SDA, PAD_ALT_MODE );
    PadConfig( HOST_I2C_SCL, PAD_ALT_MODE );

    PadConfig( PWR_I2C_SDA, PAD_ALT_MODE );
    PadConfig( PWR_I2C_SCL, PAD_ALT_MODE );

}

void Port1IntHandler( void )
{
    // Reads and clears highest priority pending interrupt
    WORD intVec = ReadRegWord( PIV1_REG8 );

    EventLogAddN( EVENTLOG_PMU430_PORT1GPIO_INT, intVec );

    // make sure we have something to service
    if( intVec == 0 ) {
        return;
    }

    // convert int vector into index into handler function array
    intVec = intVec >> 1;
    intVec--;

    // run the appropriate handler
    (PortHandler[0][ intVec ])();
}

void Port2IntHandler( void )
{
    // Reads and clears highest priority pending interrupt
    WORD intVec = ReadRegWord( PIV2_REG8 );

    EventLogAddN( EVENTLOG_PMU430_PORT2GPIO_INT, intVec );

    // make sure we have something to service
    if( intVec == 0 ) {
        return;
    }

    // convert int vector into index into handler function array
    intVec = intVec >> 1;
    intVec--;

    // run the appropriate handler
    (PortHandler[1][ intVec ])();
}
#endif   // !ROCKET_LOADER

