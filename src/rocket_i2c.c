/*****************************************************************************
 * Filename:    rocket_i2c.c
 *
 * Copyright 2008, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     December 11, 2008
 *
 * Modified:    February 25, 2011 by AS (TI)
 *
 * Description: Rocket I2C Routines
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "intvector.h"
#include "gpio.h"
#include "rocket_usci.h"
#include "timer_mux.h"
#include "i2c.h"
#include "delay.h"
#include "api.h"
#include "msgque.h"
#include "rocket_registers.h"
#include "rocket_pmic.h"
#include "watchdog.h"
#include "hostctrl.h"

#define VERBOSE_LEVEL 2
#include "eventlog.h"

#define I2C_MASTER_DIV          10    // 4MHz/10  = 400kHz
#define I2C_MASTER_FG_DIV       4000  // 4MHz/4000 = 1Hz
#define I2C_MASTER_FGROM_DIV    40    // 4MHz/40  = 100kHz

#define I2C_MASTER_FG_BOOST_DELAY   25 // 25/32768 = 750us


// interrupt handlers
static void usciB0RxTxIsr( void );
static void usciB1RxTxIsr( void );
static void usciB2RxTxIsr( void );

#if defined( HOSTPROCESSOR_OMAP4430 )
const BYTE ADR_SEQ_USB  [17] = {0x4A<<1, 0x4A<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x49<<1, 0x49<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x7F<<1}; //AS+ Address sequence for ROM commands in USB Debug mode (code download to eMMC when P1.0 is high)

const BYTE ADR_SEQ_MMC1 [12] = {0x4A<<1, 0x4A<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x48<<1, 0x49<<1, 0x49<<1, 0x48<<1, 0x48<<1, 0x7F<<1};       //AS+ Address sequence for ROM commands in MMC1 Debug mode (Boot from uSD card for debugging when P1.0 is high)
const BYTE ADR_SEQ_MMC2 [3] = {0x4A<<1, 0x4A<<1, 0x7F<<1};                                                 //AS+ Address sequence for ROM commands in MMC2 Application mode (normal boot from eMMC)
UINT ADR_CNT = 0;                                                                                  //AS+ Pointer for ADR_SEQ_USB, ADR_SEQ_MMC1 and ADR_SEQ_MMC2
BYTE ROM_MODE = 1;                                                                                 //AS+ Define global variable which selects between ROM-Emulation and Rocket-Application mode 
#endif // HOSTPROCESSOR_OMAP4430

// internal functions
static void i2cMasterSpeedBoost( WORD moduleBase );

// I2C States for slave mode
//   HOST refers to I2C Module connected to host side I2C Bus
//   DEV refers to I2C device peripheral within rocket (IE BqHuge or the FG)
typedef enum {
    HOST_RECEIVED_START,
    // Slave related states
    HOST_I2C_ERROR,
    HOST_RECEIVED_DEVID,
    HOST_RECEIVED_REGADDR,
    HOST_HAS_DEVID_REGADDR,
    HOST_RECEIVED_STOP,
    HOST_REQUESTING_BYTE
} I2C_State;

static void (*slaveExtIsr)( WORD moduleBase ) = NULL;

// Next state for relay on event
static WORD nextState[ I2C_NUM_MODULES ];

// keep track of how many bytes have been sent/received in a transaction
// NOT counting SlaveAddr, SubDevID, or RegAddr
static WORD accessCount[ I2C_NUM_MODULES ];

// index into this array must match with the I2cModuleId value
const WORD i2cBusMapping[] = {
    USCI_HOSTI2C_BUS,
    USCI_PWRI2C_BUS,
    USCI_FGI2C_BUS
};

static I2cModule module[ I2C_NUM_MODULES ];

// TODO: only one slave can be defined right now, they all use these state variables.
// TODO: test if more than one slave can be created now and remove above comment
static BYTE devId[ I2C_NUM_MODULES ];   /*= 0xFF; */           // 0xFF represents no devId has been specified yet
static BYTE regAddr[ I2C_NUM_MODULES ]; /*= 0xFF; */           // 0xFF represents no regAddr has been specified yet

extern BYTE SysFlags0;

// does not return until a clock stretch is detected; allows us to force a clock stretch
// to workaround a bug where reading the RXBuffer on bit7 of the next byte being received
// corrupts the IFG bits
// If a restart is possible to occur, also watch for the bus direction change and break out
// on that event too
#if defined( SYS_EN )
#define HOST_RESET_N     SYS_EN
#elif defined( NRESWARM )
#define HOST_RESET_N     NRESWARM
#endif
void UsciForceClockStretch( WORD moduleBase, BOOL RestartPossible, Line SclGpio, BOOL highSpeed ) {
    BYTE count;
    BOOL clockStretchRequired = FALSE;

    // first, lets check to see if another byte is coming in... if its not, then don't
    // try to clock stretch

    count = 14;
    do {
        HwDelay( 250 );
        // if the SCL line ever toggles, we need to force a clock stretch
        // since this means another byte is coming in
        if( !PadRead( SclGpio ) ) {
            clockStretchRequired = TRUE;
            break;
        }
    } while( count-- );

    V1( EventLogAddN( EVENTLOG_I2C_STRETCH_REQUIRED, clockStretchRequired ) );
    count = 10;
    if( clockStretchRequired ) {
        do {
            HwDelay( 500 );
            #if defined( HOST_RESET_N )
            // if for some reason host requested a reset mid transaction, catch it and reset rocket
            if( !PadRead( HOST_RESET_N ) ) {
                RimInitiateReset( RESET_COOKIE_INITIATED_HOST );
            }
            #endif
            if( RestartPossible && (ReadRegByte( UCBCTL1_REG8( moduleBase ) ) & UCBCTL1_UCTR) ) {
                // RE-start scenario... break out and continue
                break;
            }

            if( !PadRead( SclGpio ) ) {
                count--;
            } else {
                count = 10;
            }


        } while( count );
// comment this out; trying to remove as much latency between the PadRead above and when we read
// the RxBuffer incase we are hitting this scenario where the host is clock stretching us, and not
// the other way around.
        if( count == 0 ) {

            // TODO: maybe try toggling the LED on here if we hit this case and its not 0x50?

            // if the USCI module doesn't think we are clock stretching... then we can still
            // run into issues where we *may* read the RX buffer on the 7th bit of the next byte
            if( ReadRegByte( UCBSTAT_REG8( moduleBase ) != 0x50 ) ) {
                V1( EventLogAddN( EVENTLOG_I2C_STRETCH_ERROR, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
            }
        }
    }
}

// puts the internal USCI module in reset/out of reset during port map changes
void InternalUsciReset( BOOL inReset )
{
    if( inReset ) {
        WriteRegByteOr( UCBCTL1_REG8( USCI_FGI2C_BUS ), UCBCTL1_UCSWRST );
    } else {
        WriteRegByteAnd( UCBCTL1_REG8( USCI_FGI2C_BUS ), ~UCBCTL1_UCSWRST );
    }
}

static void usciInitI2c( I2cModuleId i2cModule, I2cAddress addr, WORD mode )
{
    WORD moduleBase = i2cBusMapping[ i2cModule ];
    I2cModule *m = &module[ i2cModule ];

    // Put module in reset while configuring it
    // Stops i2c comm, SDA, SCL in HiZ
    WriteRegByteOr( UCBCTL1_REG8( moduleBase ), UCBCTL1_UCSWRST );

    // set these flags to their default
    devId[ i2cModule ] = 0xFF;
    regAddr[ i2cModule ] = 0xFF;

    m->mode = mode;
    m->inUse = MODULE_AVAILABLE;

    switch( mode ) {
        case I2C_MASTER:
            // Configure I2C for master mode, Synchronous, Single master, 7-Bit addressing mode
            WriteRegByte( UCBCTL0_REG8( moduleBase ), UCBCTL0_UCMODE_I2C | UCBCTL0_UCSYNC | UCBCTL0_UCMST );

            // configure master clock source for SMCLK (4MHz) & transmit mode by default
            WriteRegByte( UCBCTL1_REG8( moduleBase ), UCBCTL1_UCSSEL_SMCLK | UCBCTL1_UCSWRST | UCBCTL1_UCTR );

            // Bit Clock prescaler
            WriteRegWord( UCBBR_REG16( moduleBase ), I2C_MASTER_DIV );
            break;
        case I2C_SLAVE_DUMMY:
            // todo: when cleaning up omap version of this call... can probably remove this qwerky logic
        case I2C_SLAVE_EXT:
            if( slaveExtIsr ) {
                m->mode = I2C_SLAVE_EXT;
            }
            //-lint,fallthrough
        case I2C_SLAVE:
#if defined( HOSTPROCESSOR_OMAP4430 )
        case I2C_SLAVE_ROM:
#endif // HOSTPROCESSOR_OMAP4430
            // Configure I2C for Slave mode, Synchronous, Single master, 7-Bit addressing mode
            WriteRegByte( UCBCTL0_REG8( moduleBase ),  UCBCTL0_UCMODE_I2C | UCBCTL0_UCSYNC );
            break;
        default:
            // uh oh
            break;
    }

    // Configure slave address & disable general call capability
    WriteRegWord( UCBI2COA_REG16( moduleBase ), addr>>1 );

    // Take module out of reset now that we are done
    WriteRegByteAnd( UCBCTL1_REG8( moduleBase ), ~UCBCTL1_UCSWRST );

    // Enable module interrupts
#if defined( HOSTPROCESSOR_OMAP4430 )
	if( m->mode == I2C_SLAVE || m->mode == I2C_SLAVE_ROM || m->mode == I2C_SLAVE_EXT ) {
#else
    if( m->mode == I2C_SLAVE || m->mode == I2C_SLAVE_EXT ) {
#endif // HOSTPROCESSOR_OMAP4430
        nextState[ i2cModule ] = HOST_RECEIVED_START;
        WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE );
    } else if( m->mode != I2C_MASTER ) {
        WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCNACKIE | UCBIE_UCALIE | UCBIE_UCRXIE | UCBIE_UCTXIE | UCBIE_UCSTTIE | UCBIE_UCSTPIE );
    }
}

void I2cRegisterExtCb( I2cModuleId mId, void (*isr)( WORD ) )
{
    slaveExtIsr = isr;
    module[ mId ].mode = isr ? I2C_SLAVE_EXT : I2C_SLAVE_DUMMY;

    usciInitI2c( PWRI2C_MODULE, PWR_SLAVE_ADDR, module[ mId ].mode );
}


/**
 * I2C Slave ISR used to ack Pwr I2C Commands and throw them away.
 *
 */
__inline static void i2cSlaveDummyIsr( I2cModuleId i2cModule )
{
    WORD moduleBase = i2cBusMapping[ i2cModule ];

    // read the received value (if any)
    ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
    // read and clear the highest priority pending interrupt
    WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~ReadRegWord( UCBIV_REG16( moduleBase ) ) );
}
#if defined( HOSTPROCESSOR_OMAP4430 ) && defined( RIM_SMART_REFLEX )
static void i2cSlaveSrIsr( WORD moduleBase )
{
    WORD data [2];
    WORD flags;
    WORD recvd = 0;

    // read and clear the highest priority pending interrupt
    flags = ReadRegWord( UCBIFG_REG8( moduleBase ) );

    // Clear the START IFG
    WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCSTTIFG );

    V1( EventLogAdd( EVENTLOG_I2CRLY_PWRISR ) );

    while( 1 ) {
        if( flags & UCBIFG_UCRXIFG ) {
            if( recvd == 0 ) {
                // force a clock stretch to occur on first byte we receive
                // so we can guaruntee we don't read the RXbuffer on bit7 of the
                // next byte coming in (workaround for RXBuffer read bug).
                // don't need to do this for all bytes because we will be polling
                // the emainder of the ISR and grabbing the bytes quickly
                UsciForceClockStretch( moduleBase, FALSE, PWR_I2C_SCL, TRUE );
            }
            // read the received value
            data[recvd] = ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
            recvd++;
            if( recvd == 2 ) {
                PmicWrite( data[0], data[1] );
                break;
            }
        }
        flags = ReadRegByte( UCBIFG_REG8( moduleBase ) );
    }
}



#endif

__inline static void i2cSlaveIsr( I2cModuleId i2cModule )
{
    BYTE tempByte;
    BYTE rxByte;
    WORD moduleBase = i2cBusMapping[ i2cModule ];


#if defined( HOSTPROCESSOR_OMAP4430 )
    if( ROM_MODE > 0 )                                                //AS+ ROM interrupt handling to emulate TWL6030
    {                                                                 //AS+ Only STTIFG is active to call this sequence once per ROM command	      
		++ ADR_CNT;                                                   //AS+ Prepare address change, Increase rom_adr_counter which points to address array rom_adr_sequence [] 
		WriteRegByte (UCBIFG_REG8( USCI_HOSTI2C_BUS ), UCBIFG_UCSTTIFG);    //AS+ delete all interrupt flags except STT

		if ( ReadRegByte( UCBCTL1_REG8( USCI_HOSTI2C_BUS )) & UCBCTL1_UCTR )                                    //AS+ This sequence handles the WRITE commands
		{                                                                           //AS+ (with while instead of RX interrupt routine)
			switch ( ReadRegByte( UCBI2COA_REG16( USCI_HOSTI2C_BUS )) )                   //AS+ Transmit data will be handled here with no time limitation (now clock stretching active)
			{                                                                       //AS+
				case 0x4A:  
#if defined( OMAP_SYSBOOT_USB_UART_MMC1_MMC2 )
                            ROM_MODE = 1;                                           //AS+ Use USB/MMC1/MMC2 boot order
#else
                            ROM_MODE = 3;                                           //AS+ Use MMC1/USB 
#endif
							WriteRegByte( UCBTXBUF_REG8( USCI_HOSTI2C_BUS ), 0x01 );      //AS+ send the expected answer for P1 to emulate TWL6030

							break;                                                  //AS+
				case 0x48:  WriteRegByte( UCBTXBUF_REG8( USCI_HOSTI2C_BUS ), 0x00 );      //AS+ send the expected answer for P2 to emulate TWL6030
							break;                                                  //AS+
				case 0x49:
				 	 	    if ( PmicRead( PMIC_REG_STAT_PPCHG1 ) & PMIC_STAT_PPCHG1_USB_PRESENT )  //AS+ Read VBUS flag from PMIC STAT_PPCHG1 register
				 	 	    {                                                       //AS+ when USB_PRESENT = 1
				 	 	        ROM_MODE = 0x01;                                    //AS+ Stay in USB address sequence
				 	 	        WriteRegByte( UCBTXBUF_REG8( USCI_HOSTI2C_BUS ), 0x84 );  //AS+ Tell OMAP that he shall boot from USB via P8 response = 0x84
							}                                                       //AS+
				 	 	    else                                                    //AS+
				 	 	    {                                                       //AS+ when USB_PRESENT = 0
				 	 	        ROM_MODE = 0x02;                                    //AS+ Switch to MMC1 address sequence
				 	 	         WriteRegByte( UCBTXBUF_REG8( USCI_HOSTI2C_BUS ), 0x80 ); //AS+ Tell OMAP that he shall boot from MMC1 via P8 response = 0x80
							}                                                       //AS+
				 	 	    break;			                                        //AS+      
				default:    V1( EventLogAdd( EVENTLOG_I2C_ROM_ERROR ) );            //AS+ Add a log entry
						    break;                                                  //AS+ if command is unknown signalize with dummy execution delay			 	 	           
			}   				                                                    //AS+	
        }						                                                    //AS+	
					
		else                                                                        //AS+ This sequence handles the READ commands
			{                                                                                                 //AS+ (with while instead of TX interrupt routine)
			while( !(ReadRegByte( UCBIFG_REG8( USCI_HOSTI2C_BUS ) ) & UCBIFG_UCRXIFG) );                        //AS+ Wait until first byte is received
			rxByte = ReadRegByte( UCBRXBUF_REG8( USCI_HOSTI2C_BUS ) );	                                      //AS+ Store received byte (= REGISTER ADDRESS)
			//WriteRegByteAnd (UCBIFG_REG8( moduleBase ), ~UCBIFG_UCRXIFG);                                 //AS+ Delete RX flag
			//while( !(ReadRegByte( UCBIFG_REG8( moduleBase ) ) & UCBIFG_UCRXIFG) |  !(ReadRegByte( UCBIFG_REG8( moduleBase ) ) & UCBIFG_UCSTPIFG) );  //AS+ Wait until second byte or STOP is received
			//ReadRegByte( UCBRXBUF_REG8( moduleBase ) );	                                              //AS+ Dummy read of received byte (= DATA)
		}                                                                                                 //AS+
						
        
		while( !(ReadRegByte( UCBIFG_REG8( USCI_HOSTI2C_BUS ) ) & UCBIFG_UCSTPIFG) );        //AS+ Now the address change starts (must be finished in 30 us)
		//WriteRegByteOr( UCBCTL1_REG8( USCI_HOSTI2C_BUS ), UCBCTL1_UCSWRST );                 //AS+ Activate reset for USCI UCB0CTL1 
		switch ( ROM_MODE )                                                            //AS+ Select the ROM sequence state machine 
	    {                                                                              //AS+
	        case 1: usciInitI2c( HOSTI2C_MODULE, ADR_SEQ_USB [ADR_CNT] , I2C_SLAVE_ROM );  //AS+ Configure for USB sequence 
	                break;                                                             //AS+
	        case 2: usciInitI2c( HOSTI2C_MODULE, ADR_SEQ_MMC1 [ADR_CNT] , I2C_SLAVE_ROM ); //AS+ Configure for MMC1 sequence 
	                break;                                                             //AS+
	        case 3: usciInitI2c( HOSTI2C_MODULE, ADR_SEQ_MMC2 [ADR_CNT] , I2C_SLAVE_ROM ); //AS+ Configure for MMC2 sequence
	                break;	                                                           //AS+
	        default:V1( EventLogAdd( EVENTLOG_I2C_ROM_ERROR ) );                       //AS+ Add a log entry 
			        break;                                                             //AS+
	    }             		                                                           //AS+          
		//WriteRegByteAnd( UCBCTL1_REG8( USCI_HOSTI2C_BUS ), ~UCBCTL1_UCSWRST );               //AS+ Disable reset for USCI 
		//WriteRegByte( UCBIE_REG8( USCI_HOSTI2C_BUS ), UCBIE_UCSTTIE );                       //AS+ Enable the interrupt, deleted by reset 

	    if( (ReadRegWord( UCBI2COA_REG16( USCI_HOSTI2C_BUS )) & 0x7F) == 0x7F )    //AS+ Detects when ROM execution is finished 
		{                                                          //AS+ (signalized by address 0xFF)       	    
	        ADR_CNT = 0x00;                                        //AS+ Reset ROM mode counter to be prepared for a OMAP reset
			ROM_MODE = 0x00;                                       //AS+ Switch to APPLICATION mode which disables this part of the interrupt routine
			V1( EventLogAdd( EVENTLOG_I2C_ROM_FINISH ) );          //AS+ Add a log entry that ROM execution is finished
			I2cInit();                                             //AS+ Call RIM's normal init routine here for I2C 
	    }                                                          //AS+
    }                                                              //AS+                                             //AS+ 
    else                                                                                //AS+ 
    {                                                                                   //AS+ 
#endif // HOSTPROCESSOR_OMAP4430
    switch( nextState[ i2cModule ] ) {
        case HOST_RECEIVED_START:
            if( ReadRegByte( UCBCTL1_REG8( moduleBase )) & UCBCTL1_UCTR ) {
                // host should not be requesting anything from us at this point!
                nextState[ i2cModule ] = HOST_I2C_ERROR;
            } else {
                WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCSTTIFG );
                // host is sending us something... when we recieve it, trigger next event
                WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCRXIE );
                #if defined( PLATFORM_RIMEVALBOARD )
                // on the eval board assume DEVID = DEVID_PMIC and move on to register
                nextState[ i2cModule ] = HOST_RECEIVED_REGADDR;
                devId[ i2cModule ] = DEVID_PMIC;
                #else
                nextState[ i2cModule ] = HOST_RECEIVED_DEVID;
                #endif // PLATFORM_RIMEVALBOARD
                V2( EventLogAdd( EVENTLOG_I2C_START ) );
            }
            break;

        case HOST_RECEIVED_DEVID:
            // get the received byte
            UsciForceClockStretch( moduleBase, FALSE, HOST_I2C_SCL, FALSE );
            rxByte = ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIFG_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL0_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIE_REG8( moduleBase ) ) ) );

            devId[ i2cModule ] = rxByte;

            V2( EventLogAddN( EVENTLOG_I2C_RX_DEVID, rxByte ) );

            // let the device know it just received a START
            ApiStartReceived( rxByte );
            nextState[ i2cModule ] = HOST_RECEIVED_REGADDR;
            break;

        case HOST_RECEIVED_REGADDR:
            // get the received byte
            UsciForceClockStretch( moduleBase, TRUE, HOST_I2C_SCL, FALSE );
            rxByte = ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIFG_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL0_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIE_REG8( moduleBase ) ) ) );
            regAddr[ i2cModule ] = rxByte;

            V2( EventLogAddN( EVENTLOG_I2C_RX_REGADDR, rxByte ) );

            // we have the devid and the register now, can either have a restart/TX, or receive another byte
            nextState[ i2cModule ] = HOST_HAS_DEVID_REGADDR;
            WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCRXIE | UCBIE_UCSTTIE );

            accessCount[ i2cModule ] = 0;
            break;

        case HOST_HAS_DEVID_REGADDR:
            tempByte = ReadRegByte( UCBIFG_REG8( moduleBase ) );

            // REPEAT START
            if( (tempByte & UCBIFG_UCSTTIFG) && (ReadRegByte( UCBCTL1_REG8( moduleBase ) ) & UCBCTL1_UCTR ) && (accessCount[i2cModule] == 0) ) {
                // if we haven't sent/received any data the START is a RepeatStart to change
                // data directions
                if( accessCount[ i2cModule ] == 0 ) {
                    nextState[ i2cModule ] = HOST_HAS_DEVID_REGADDR;
                    WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCTXIE | UCBIE_UCRXIE );

                    WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCSTTIFG );

                    V2( EventLogAdd( EVENTLOG_I2C_RESTART ) );
                }

            // host is writing to the device register
            } else if( (tempByte & UCBIFG_UCRXIFG) ) {
                BOOL stopReceived;
                stopReceived = FALSE;

                // give 30us to ensure we see the STOP bit too if the byte we just recieved was the last.
                // This is an ugly workaround to avoid a scenario where we miss the STOP bit and get
                // out of sync.
                HwDelay(30000);
                // re-read the IFG to see if we got a STOP too
                tempByte = ReadRegByte( UCBIFG_REG8( moduleBase ) );

                if( tempByte & (UCBIFG_UCSTPIFG | UCBIFG_UCSTTIFG ) ) {
                    stopReceived = TRUE;
                    V2( EventLogAddN( EVENTLOG_I2C_STOP, 3 ) );
                }

GOT_ANOTHER_BYTE:
                // if stop hasn't been received, wait for the clock stretch to start before we allow
                // the byte to be read out. this avoids the case where we read the RXBuffer on the 7th
                // bit of the next RX byte thats coming (which seems to corrupt the IFG bits and USCI state machine)
                if( !stopReceived ) {
                    UsciForceClockStretch( moduleBase, FALSE, HOST_I2C_SCL, FALSE );
                }
                rxByte = ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIFG_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL0_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
//                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIE_REG8( moduleBase ) ) ) );
                accessCount[ i2cModule ]++;

                // if autoincrement isn't being overridden, increment the reg adder
                // this will be overriden by special registers and access to to the first register being accessed in a transaction
                // 1st byte in a transaction doesn't get auto incremented
                if( !ApiAutoIncrementOverried( devId[ i2cModule ], regAddr[ i2cModule ] ) ) {
                    regAddr[ i2cModule ]++;
                }
                ApiRegWrite( devId[ i2cModule ], regAddr[ i2cModule ], rxByte );

                V2( EventLogAddN( EVENTLOG_I2C_RX_DATA, rxByte ) );
                if( !stopReceived ) {
                    tempByte = ReadRegByte( UCBIFG_REG8( moduleBase ) );
                    // got another byte and stop
                    if( (tempByte & (UCBIFG_UCSTPIFG | UCBIFG_UCRXIFG)) == (UCBIFG_UCSTPIFG | UCBIFG_UCRXIFG) ) {
                        stopReceived = TRUE;
                        V2( EventLogAddN( EVENTLOG_I2C_STOP, 4 ) );
                        // ugly, but go handle the new byte we just got since we are falling behind and things are about
                        // to go off the rails
                        goto GOT_ANOTHER_BYTE;
                    } else if( tempByte & UCBIFG_UCSTPIFG ) {
                        V2( EventLogAddN( EVENTLOG_I2C_STOP, 5 ) );
                        //TODO: for some reason in the middle of a multi byte transaction (write) to rocket, we are detecting a STOP
                        //condition here (even though the host is still sending many more bytes)

                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIFG_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL0_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
                        V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIE_REG8( moduleBase ) ) ) );
                        RimInitiateReset( RESET_COOKIE_INITIATED_ERROR );
                        // just handle the stop
                        stopReceived = TRUE;
                    } else if( tempByte & UCBIFG_UCSTTIFG ) {
                        V2( EventLogAddN( EVENTLOG_I2C_STOP, 6 ) );
                        // got a start, and missed the stop. 
                        stopReceived = TRUE;
                    }
                }

                if( stopReceived ) {
                    ApiStopReceived( devId[ i2cModule ] );
                    WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE );
                    nextState[ i2cModule ] = HOST_RECEIVED_START;
                    V2( EventLogAddN( EVENTLOG_I2C_STOP, 0 ) );
                } else {
                    // trigger on a STOP or a START (incase host sends START right after stop, which clears our STOP IFG without us knowing
                    // also, trigger on RXing another byte
                    WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE | UCBIE_UCSTPIE | UCBIE_UCRXIE );
                }
                // host is reading from the device register
            } else if( (tempByte & UCBIFG_UCTXIFG) && (ReadRegByte( UCBCTL1_REG8( moduleBase ) ) & UCBCTL1_UCTR ) ) {

                BYTE txData;
                // if autoincrement isn't being overridden, increment the reg adder
                // this will be overriden by special registers and access to to the first register being accessed in a transaction
//                if( !ApiAutoIncrementOverried( devId[ i2cModule ], regAddr[ i2cModule ] ) ) {
//                    regAddr[ i2cModule ]++;
//                }

                txData = ApiRegRead( devId[ i2cModule ], regAddr[ i2cModule ] );

                WriteRegByte( UCBTXBUF_REG8( moduleBase ), txData );
                accessCount[ i2cModule ]++;

                V2( EventLogAddN( EVENTLOG_I2C_TX_DATA , txData ) );

                nextState[ i2cModule ] = HOST_RECEIVED_STOP;
                // trigger on a STOP or a START (incase host sends START right after stop, which clears our STOP IFG without us knowing
                // NOTE: for now, we cannot respond with multiple bytes in one i2c transaction due to pre-loading of TX reg and auto increment registers(like the eventlog)
                WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE | UCBIE_UCSTPIE  );

            } else if( tempByte & (UCBIFG_UCSTPIFG | UCBIFG_UCSTTIFG) ) {
                ApiStopReceived( devId[ i2cModule ] );
                WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE );
                nextState[ i2cModule ] = HOST_RECEIVED_START;
                V2( EventLogAddN( EVENTLOG_I2C_STOP, 1 ) );
            }
            break;

        case HOST_RECEIVED_STOP:
            ApiStopReceived( devId[ i2cModule ] );
            WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE );
            nextState[ i2cModule ] = HOST_RECEIVED_START;
            V2( EventLogAddN( EVENTLOG_I2C_STOP, 2 ) );
            break;

        case HOST_I2C_ERROR:
        default:
            WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCSTTIE );
            nextState[ i2cModule ] = HOST_RECEIVED_START;
            V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIFG_REG8( moduleBase ) ) ) );
            V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL0_REG8( moduleBase ) ) ) );
            V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
            V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
            V2( EventLogAddN( EVENTLOG_I2C_ERROR, ReadRegByte( UCBIE_REG8( moduleBase ) ) ) );

            RimInitiateReset( RESET_COOKIE_INITIATED_ERROR );
            break;
    }
#if defined( HOSTPROCESSOR_OMAP4430 )
  }                                                                                                //AS+ End of "rom_mode" else case
#endif // HOSTPROCESSOR_OMAP4430
}

// fuel gauge must start at <12kHz, and after a time we can boost it up to 400kHz
static void i2cMasterSpeedBoost( WORD moduleBase )
{
    WriteRegWord( UCBBR_REG16( moduleBase ), I2C_MASTER_FGROM_DIV );
}

/**
 * Performs a START or REPEATED_START transaction on the I2C bus
 *
 * @param address 7-bit device address on the I2C bus.
 * @param rw A bit indicating whether this is a write (0) or read (1) transaction
 *
 * @return 1 on success, 0 on any failure
 */
static BOOL i2cDoStart( I2cModuleId i2cModule, I2cAddress address, BOOL tx )
{
    BYTE ctl1Reg;
    I2cModule *m = &module[ i2cModule ];
    // get the register base address of the module we are using
    WORD moduleBase = i2cBusMapping[i2cModule];
    // lower the speed for the fuel gauge, and set a timer to raise it again
    if( address == 0xAA ) {
        WriteRegWord( UCBBR_REG16( moduleBase ), I2C_MASTER_FG_DIV );
    } else if( address == 0x16 ) {
        if( SysFlags0 & ROCKET_SYSFLAGS0_FGROM_400K_EN ) {
            WriteRegWord( UCBBR_REG16( moduleBase ), I2C_MASTER_DIV );
        } else {
            WriteRegWord( UCBBR_REG16( moduleBase ), I2C_MASTER_FGROM_DIV );
        }
    } else {
        WriteRegWord( UCBBR_REG16( moduleBase ), I2C_MASTER_DIV );
    }

    ctl1Reg = ReadRegByte( UCBCTL1_REG8( moduleBase ) );

    // Write address of device we will be talking to
    WriteRegWord( UCBI2CSA_REG16( moduleBase ), (WORD)(address>>1) );

    // configure direction of transaction for NEXT operation being performed
    if( !tx ) {
        // read mode
        ctl1Reg &= ~UCBCTL1_UCTR;
    } else {
        // write mode
        ctl1Reg |= UCBCTL1_UCTR;
    }

    // incrment to next operation
    m->index++;
    m->bufferIndex = 0;

    WriteRegByte( UCBIFG_REG8( moduleBase ), 0x00 );

    // send start condition & address the slave
    WriteRegByte( UCBCTL1_REG8( moduleBase ), ctl1Reg | UCBCTL1_UCTXSTT );

    // setup the wakeup clock speed change timer for BOOST_DELAY time after the START is sent
    if( address == 0xAA ) {
        SetTimer( I2C_FG_SPEED_TIMER, I2C_MASTER_FG_BOOST_DELAY );
    }

    // Enable module interrupts
    WriteRegByte( UCBIE_REG8( moduleBase ), UCBIE_UCNACKIE | UCBIE_UCALIE | UCBIE_UCRXIE | UCBIE_UCTXIE );

    return( TRUE );
}

__inline static void i2cMasterIsr( I2cModuleId i2cModule )
{
    WORD ucbiv;
    I2cModule *m = &module[ i2cModule ];
    WORD moduleBase = i2cBusMapping[ i2cModule ];       // Base address of I2C module being used
    BYTE ctl1Reg;

    // read highest pending interrupt source
    ucbiv = ReadRegWord( UCBIV_REG16( moduleBase ) );

    switch( ucbiv ) {
        // ARBITRATION Error
        case UCBIV_UCALIFG:
            m->inUse = MODULE_REQ_ERRSTOP;
            WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCALIFG );
            V1( EventLogAdd( EVENTLOG_I2C_ARBLOST ) );
            break;
        // NACK Error
        case UCBIV_UCNACKIFG:
            m->inUse = MODULE_REQ_ERRSTOP;
            WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCNACKIFG );
            V1( EventLogAdd( EVENTLOG_I2C_NACKED ) );
            break;

        // RX buffer contains new data
        case UCBIV_UCRXIFG:
            // if the byte we just about to read out was our last byte, we are done
            if( m->bufferIndex == m->desc[m->index].len-1 ) {
                m->inUse = MODULE_COMPLETE;
                WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCRXIFG );
            }

            // then read RXd byte so we send the proper NACK/STOP to the slave
            m->desc[m->index].data[ m->bufferIndex ] = ReadRegByte( UCBRXBUF_REG8( moduleBase ) );
            m->bufferIndex++;
            m->bytesRead++;


            // next byte we get is our last, so send stop with it
            if( m->bufferIndex == (m->desc[m->index].len-1) ) {
                WriteRegByteOr( UCBCTL1_REG8( moduleBase ), UCBCTL1_UCTXSTP );
            }
            break;
        // TX buffer ready for data
        case UCBIV_UCTXIFG:


            WriteRegByte( UCBTXBUF_REG8( moduleBase ), m->desc[m->index].data[ m->bufferIndex ] );
            m->bufferIndex++;
            V1( EventLogAdd( EVENTLOG_I2C_TX_DATA ) );
            // Any we out of bytes in this descriptor?
            if( m->bufferIndex == m->desc[m->index].len ) {
                m->index++;
                V1( EventLogAddN( EVENTLOG_I2C_STAT0, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
                V1( EventLogAddN( EVENTLOG_I2C_CTL0, ReadRegByte( UCBCTL0_REG8( moduleBase ) ) ) );
                V1( EventLogAddN( EVENTLOG_I2C_CTL1, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
                // hack?
                // wait for TXIFG be set so we know we can send the STOP or RESTART with it
                while( !(ReadRegByte( UCBIFG_REG8( moduleBase ) ) & UCBIFG_UCTXIFG) ) {
                    if( ReadRegByte( UCBIFG_REG8( moduleBase ) ) & (UCBIFG_UCNACKIFG | UCBIFG_UCALIFG) ) {
                        m->inUse = MODULE_REQ_ERRSTOP;
                        V1( EventLogAddN( EVENTLOG_I2C_NACKED, ReadRegByte( UCBIFG_REG8( moduleBase ) ) ) );
                        V1( EventLogAddN( EVENTLOG_I2C_STAT0, ReadRegByte( UCBSTAT_REG8( moduleBase ) ) ) );
                        V1( EventLogAddN( EVENTLOG_I2C_CTL1, ReadRegByte( UCBCTL1_REG8( moduleBase ) ) ) );
                        WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~(UCBIFG_UCNACKIFG | UCBIFG_UCALIFG) );
                        break;
                    }
                }

                if( m->inUse == MODULE_REQ_ERRSTOP ) {
                    // don't continue processing, return the error
                    break;
                }

                // we are out of descriptors to run; we are done, send STOP
                if(  m->index == m->descSize ) {
                    // we have finished TXing, send stop
                    m->inUse = MODULE_REQ_STOP;
                    break;
                }
                // we expect a RESTART here... otherwise return an error
                if( m->desc[m->index].op != I2C_OP_RESTART ) {
                    m->inUse = MODULE_REQ_ERRSTOP;
                    break;
                }
                m->index++;
                m->bufferIndex = 0;

                ctl1Reg = ReadRegByte( UCBCTL1_REG8( moduleBase ) );

                // configure direction of transaction for NEXT operation being performed
                if( m->desc[m->index].op == I2C_OP_READ ) {
                    // read mode
                    ctl1Reg &= ~UCBCTL1_UCTR;
                    // disable TX interrupt since we arn't TX'ing
                } else {
                    // write mode
                    ctl1Reg |= UCBCTL1_UCTR;
                }
                // send repeat start
                WriteRegByte( UCBCTL1_REG8( moduleBase ), ctl1Reg | UCBCTL1_UCTXSTT );
                WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCTXIFG );

                // if this next byte transaction is going to be our last, we need to send a stop with it in the
                // RX case
                if( (m->desc[m->index].len <= 1) && !( ctl1Reg & UCBCTL1_UCTR ) ) {
                    V1( EventLogAdd( EVENTLOG_I2C_RESTART ) );
                    // wait for repeat start to be sent
                    while( ReadRegByte( UCBCTL1_REG8( moduleBase ) ) & UCBCTL1_UCTXSTT ) {
                        if( ReadRegByte( UCBIFG_REG8( moduleBase ) ) & UCBIFG_UCNACKIFG ) {
                            // slave NACKed the repeat start address
                            m->inUse = MODULE_REQ_ERRSTOP;
                            WriteRegByteAnd( UCBIFG_REG8( moduleBase ), ~UCBIFG_UCNACKIFG );
                            V1( EventLogAdd( EVENTLOG_I2C_NACKED ) );
                            break;
                        }
                    }
                    // then trigger the stop to go with next read
                    WriteRegByteOr( UCBCTL1_REG8( moduleBase ), UCBCTL1_UCTXSTP );
                }
            }
            break;
        default:
            m->inUse = MODULE_REQ_ERRSTOP;
            break;
    }

    if( m->inUse == MODULE_REQ_STOP ) {
        WriteRegByteOr( UCBCTL1_REG8( moduleBase ), UCBCTL1_UCTXSTP );
        m->inUse = MODULE_COMPLETE;
    } else if( m->inUse == MODULE_REQ_ERRSTOP ) {
        WriteRegByteOr( UCBCTL1_REG8( moduleBase ), UCBCTL1_UCTXSTP );
        m->inUse = MODULE_ERROR;
    }

    // if we are done, or there was an error free up the module and do the callback
    // if required
    if( (m->inUse >= MODULE_COMPLETE) /*&& (m->callback != NULL)*/ ) {
        // disable the interrupt sources now that we are done
        WriteRegByte( UCBIE_REG8( moduleBase ), 0 );
        // nothing for CB to read back, so make module available right away
        // incase CB wants to setup another I2C transaction.
        // If we arn't using a callback, don't flag it as available yet.
        if( m->bytesRead == 0 && (m->callback != NULL) ) {
            m->inUse = MODULE_AVAILABLE;
        }
        if( m->callback != NULL ) {
            (m->callback)();
            m->inUse = MODULE_AVAILABLE;
        }
    }
}

/**
 * Performs an I2C write write operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
WORD I2cWriteWrite( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer1, BYTE wSize1, BYTE *wBuffer2, BYTE wSize2 )

{
    I2cModule *m = &module[ i2cModule ];
    I2cDescriptor d[3];
    WORD wason = CoreIntDisable();
    // interrupts were already off, can't run
    if( !wason ) {
        return( 1 );
    } else if( m->inUse != MODULE_AVAILABLE) {
        CoreIntEnable( wason );
        // module already in use
        return( 1 );
    } else {
        m->inUse = MODULE_INUSE;
    }
    CoreIntEnable( wason );

    // make sure we don't try to callback anything
    m->callback = NULL;

    m->index = 0;
    m->bufferIndex = 0;
    m->bytesRead = 0;

    m->desc = d;

    m->desc[0].op   = I2C_OP_START;
    m->desc[1].op   = I2C_OP_WRITE;
    m->desc[1].data = wBuffer1;
    m->desc[1].len  = wSize1;
    m->desc[2].op   = I2C_OP_WRITE;
    m->desc[2].data = wBuffer2;
    m->desc[2].len  = wSize2;
    m->descSize = 3;

    i2cDoStart( i2cModule, address, TRUE );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE );

    // wait for the module to be inactive (last byte could/is still be being sent out)
    while( ReadRegByte( UCBSTAT_REG8( i2cBusMapping[ i2cModule ] ) ) & UCBSTAT_UCBBUSY );

    if( m->inUse == MODULE_ERROR ) {
        m->inUse = MODULE_AVAILABLE;
        return( 1 );
    }

    m->inUse = MODULE_AVAILABLE;
    return( 0 );
}

/**
 * Performs an I2C write write operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
WORD I2cWrite( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer1, BYTE wSize1 )
{
    I2cModule *m = &module[ i2cModule ];
    I2cDescriptor d[2];
    WORD wason = CoreIntDisable();
    // interrupts were already off, can't run
    if( !wason ) {
        return( 1 );
    } else if( m->inUse != MODULE_AVAILABLE) {
        CoreIntEnable( wason );
        // module already in use
        return( 1 );
    } else {
        m->inUse = MODULE_INUSE;
    }
    CoreIntEnable( wason );
    // make sure we don't try to callback anything
    m->callback = NULL;

    m->index = 0;
    m->bufferIndex = 0;
    m->bytesRead = 0;

    m->desc = d;

    m->desc[0].op   = I2C_OP_START;
    m->desc[1].op   = I2C_OP_WRITE;
    m->desc[1].data = wBuffer1;
    m->desc[1].len  = wSize1;
    m->descSize = 2;

    i2cDoStart( i2cModule, address, TRUE );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE );

    // wait for the module to be inactive (last byte could/is still be being sent out)
    while( ReadRegByte( UCBSTAT_REG8( i2cBusMapping[ i2cModule ] ) ) & UCBSTAT_UCBBUSY );

    if( m->inUse == MODULE_ERROR ) {
        m->inUse = MODULE_AVAILABLE;
        return( 1 );
    }

    m->inUse = MODULE_AVAILABLE;
    return( 0 );
}

/**
 * Performs an I2C write read operation and blocks until it completes
 * This cannot be called with interrupts disabled
 *
 * Returns 0 on success, none zero on an error
 */
WORD I2cWriteRead( I2cModuleId i2cModule, I2cAddress address, BYTE *wBuffer, BYTE wSize, BYTE *rBuffer, BYTE rSize )
{
    I2cModule *m = &module[ i2cModule ];
    I2cDescriptor d[4];

    WORD wason = CoreIntDisable();

    // interrupts were already off, can't run
    if( !wason ) {
        return( 1 );
    } else if( m->inUse != MODULE_AVAILABLE) {
        CoreIntEnable( wason );
        // module already in use
        return( 1 );
    } else {
        m->inUse = MODULE_INUSE;
    }
    CoreIntEnable( wason );

    // make sure we don't try to callback anything
    m->callback = NULL;

    m->index = 0;
    m->bufferIndex = 0;
    m->bytesRead = 0;

    m->desc = d;

    m->desc[0].op   = I2C_OP_START;
    m->desc[1].op   = I2C_OP_WRITE;
    m->desc[1].data = wBuffer;
    m->desc[1].len  = wSize;
    m->desc[2].op   = I2C_OP_RESTART;
    m->desc[3].op   = I2C_OP_READ;
    m->desc[3].data = rBuffer;
    m->desc[3].len  = rSize;
    m->descSize = 4;

    i2cDoStart( i2cModule, address, TRUE );

    // wait for COMPLETE or ERROR flags
    while( m->inUse < MODULE_COMPLETE );

    if( m->inUse == MODULE_ERROR ) {
        m->inUse = MODULE_AVAILABLE;
        return( 1 );
    }

    m->inUse = MODULE_AVAILABLE;
    return( 0 );
}

void I2cInitInternalBus( void )
{

    RegisterIntHandler( USCIB0_RXTX_VECTOR, usciB0RxTxIsr );
    RegisterIntHandler( USCIB1_RXTX_VECTOR, usciB1RxTxIsr );
    RegisterIntHandler( USCIB2_RXTX_VECTOR, usciB2RxTxIsr );

    usciInitI2c( FGI2C_MODULE, 0x00 , I2C_MASTER );
}

#if defined( HOSTPROCESSOR_OMAP4430 )
void I2cRomInit( void )                                                                //AS+ Initialisation of ROM mode 
{                                                                                      //AS+ to emulate TWL6030
    WORD wason = CoreIntDisable();                                                     //AS+ Disable interrupts 
    V1( EventLogAdd( EVENTLOG_I2C_ROM_INIT ) );                                        //AS+ Add a log entry 
    ADR_CNT = 0;                                                                       //AS+ Set pointer to first address of ROM sequence
    switch ( ROM_MODE )                                                                //AS+ Select the ROM sequence state machine
	{                                                                                  //AS+
	case 1:  usciInitI2c( HOSTI2C_MODULE, ADR_SEQ_USB [ADR_CNT] , I2C_SLAVE_ROM );         //AS+ Configure for USB sequence
	         break;                                                                    //AS+
	case 2:  usciInitI2c( HOSTI2C_MODULE, ADR_SEQ_MMC1 [ADR_CNT] , I2C_SLAVE_ROM );        //AS+ Configure for MMC1 sequence
	         break;                                                                    //AS+
	case 3:  usciInitI2c( HOSTI2C_MODULE, ADR_SEQ_MMC2 [ADR_CNT] , I2C_SLAVE_ROM );        //AS+ Configure for MMC2 sequence
	         break;	                                                                   //AS+
	default: V1( EventLogAdd( EVENTLOG_I2C_ROM_ERROR ) );                              //AS+ Add a log entry 
	         break;                                                                    //AS+
	}             		                                                               //AS+
	CoreIntEnable( wason );                                                            //AS+ Enable interrupts 
}                                                                                      //AS+
#endif // HOSTPROCESSOR_OMAP4430

void I2cInit( void )
{
    WORD wason = CoreIntDisable();
    V1( EventLogAdd( EVENTLOG_I2C_INIT ) );

    usciInitI2c( HOSTI2C_MODULE, ROCKET_SLAVE_ADDR, I2C_SLAVE );
#if defined( HOSTPROCESSOR_OMAP4430 ) && defined( RIM_SMART_REFLEX )
    I2cRegisterExtCb( PWRI2C_MODULE, i2cSlaveSrIsr );
#endif
    RegisterTimerExpiry( I2C_FG_SPEED_TIMER, i2cMasterSpeedBoost, i2cBusMapping[ FGI2C_MODULE ] );


    CoreIntEnable( wason );
}

void I2cUninit( void )
{
    // Put module in reset while configuring it
    // Stops i2c comm, SDA, SCL in HiZ
    WriteRegByteOr( UCBCTL1_REG8( i2cBusMapping[ HOSTI2C_MODULE ] ), UCBCTL1_UCSWRST );
    WriteRegByteOr( UCBCTL1_REG8( i2cBusMapping[ PWRI2C_MODULE ] ), UCBCTL1_UCSWRST );
}

static void i2cIsrHandler( I2cModuleId mId )
{
    switch( module[ mId ].mode ) {
        case I2C_MASTER:
            i2cMasterIsr( mId );
            break;
        case I2C_SLAVE:
#if defined( HOSTPROCESSOR_OMAP4430 )
        case I2C_SLAVE_ROM:
#endif // HOSTPROCESSOR_OMAP4430
            i2cSlaveIsr( mId );
            break;
        case I2C_SLAVE_DUMMY:
            i2cSlaveDummyIsr( mId );
            break;
        case I2C_SLAVE_EXT:
            slaveExtIsr( i2cBusMapping[ mId ] );
            break;
        default:
            break;
    }
}

static void usciB0RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIB0_INT );
    i2cIsrHandler( HOSTI2C_MODULE );
}

static void usciB1RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIB1_INT );
    i2cIsrHandler( PWRI2C_MODULE );
}

static void usciB2RxTxIsr( void )
{
    EventLogAdd( EVENTLOG_PMU430_USCIB2_INT );
    i2cIsrHandler( FGI2C_MODULE );
}


