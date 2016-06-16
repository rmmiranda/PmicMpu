/*****************************************************************************
 * Filename:    bqhuge_pmic.c
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bryon Hummel
 *
 * Created:     March 23, 2009
 *
 * Description: Rocket bqhuge debug/support functions
 ****************************************************************************/
#include "basetype.h"
#include "critical.h"
#include "registers.h"
#include "gpio.h"
#include "i2c.h"
#include "bqhuge_pmic.h"

typedef struct {
    BYTE addr;
    BYTE val;       // Value to be written.  Bits not to be written must be set to zero.
    BYTE valMask;   // Indicates which bits of val are written, i.e. 0xFF = all bits
} BqHugeRegData;

typedef struct {
    BYTE startAddr;
    BYTE endAddr;   // inclusive
    BYTE val;       // default value for entire range
} BqHugeRegRangeData;

// TODO: ignore the mess of comments to the right.
static BqHugeRegData defaultSingleRegs[] =
{
    { BQHUGE_SUPPLYENA,     0x03, 0xFF },  // LDO2:OFF SM0:ON  SM1:ON   // 0x03
    { BQHUGE_SUPPLYENB,     0x03, 0xFF },  // LDO2:OFF SM0:ON  SM1:ON   // 0x00
    { BQHUGE_SUPPLYENC,     0x88, 0xFF },  // SM2:ON   LDO8:OFF LDO7:OFF LDO6:OFF LDO4:ON LDO3:OFF LDO1:OFF LDO0:OFF    // 0xD8
    { BQHUGE_SUPPLYEND,     0x88, 0xFF },  // SM2:ON   LDO8:OFF LDO7:OFF LDO6:OFF LDO4:ON LDO3:OFF LDO1:OFF LDO0:OFF    // 0xD8
    { BQHUGE_SUPPLYENE,     0x00, 0xFF },  // LDO9:OFF LDO5:OFF ?? TODO: do we need to set bit 3? (ENABLE NORMAL mode)  // 0x20
    { BQHUGE_VCC1,          0x00, 0xFF },  // 0x02
    { BQHUGE_VCC2,          0x00, 0xFF },  // 0x00
    { BQHUGE_SM1V1,         0x0B, 0xFF },  // SM1:1.0V  // 0x13
    { BQHUGE_SM1V2,         0x0B, 0xFF },  // SM1:1.0V  // 0x15
    { BQHUGE_SM1SL,         0x07, 0xFF },  // 0x07
//    { BQHUGE_SM0V1,         0x08, 0xFF },  // should be 1.8V  // 0x07
//    { BQHUGE_SM0V2,         0x08, 0xFF },  // should be 1.8V  // 0x07
//    { BQHUGE_SM0SL,         0x07, 0xFF },  // 0x07
    { BQHUGE_LDO2AV1,       0x15, 0xFF },  // LDO2:1.25V  // 0x15
    { BQHUGE_LDO2AV2,       0x15, 0xFF },  // LDO2:1.25V  // 0x15
//    { BQHUGE_LDO2BV1,       0x15, 0xFF },  // ?  // 0x15
//    { BQHUGE_LDO2BV2,       0x15, 0xFF },  // ?  // 0x15
    { BQHUGE_LDO4V1,        0x04, 0xFF },  // LDO4:1.8V   // 0x04
    { BQHUGE_LDO4V2,        0x04, 0xFF },  // LDO4:1.8V   // 0x04
    { BQHUGE_SUPPLYV1,      0x5D, 0xFF },  // LDO0:1.8V  LDO1:2.5V   // 0x4D
    { BQHUGE_SUPPLYV2,      0xA5, 0xFF },  // SM2:3.25V  LDO8:2.85V  // 0xA5
    { BQHUGE_SUPPLYV3,      0x2E, 0xFF },  // LDO6:3.1V  LDO7:2.85V  // 0x2E
    { BQHUGE_SUPPLYV4,      0x02, 0x07 },  // LDORTC:Fixed  LDO3: 1.8V  // 0xF3
    { BQHUGE_SUPPLYV5,      0x00, 0xFF },  // 0x00
    { BQHUGE_SUPPLYV6,      0x2D, 0xFF },  // LDO5:2.85V   LDO9:2.85V  // 0x2D
    { BQHUGE_SMODE1,        0x00, 0xFF },  // 0x40
//    { BQHUGE_SMODE2,        0x00, 0xFF },  // ?  // 0x08

//    { BQHUGE_CHG1,          0x2C, 0xFF },  // ?  // 0x2C
//    { BQHUGE_CHG2,          0xF8, 0xFF },  // ?  // 0xF8
//    { BQHUGE_CHG3,          0x61, 0xFF },  // ?  // 0x61
    { BQHUGE_PPATH1,        0x61, 0xFF },  // 0x69
    { BQHUGE_PGFLTMASK1,    0xFF, 0xFF },  // 0xFF
    { BQHUGE_PGFLTMASK2,    0xFC, 0xFF },  // 0xFC
    { BQHUGE_RGB1_FLASH,    0x00, 0xFF },  // 0x00
    { BQHUGE_RGB1_RED,      0x20, 0xFF },  // 0x00

    //{ BQHUGE_RGB2_RED,      0x3F, 0xFF },  // 0x3F   This was read on whitebluff... no other reason
};

static BqHugeRegRangeData defaultRangeRegs[] =
{
    { BQHUGE_RGB1_GREEN, BQHUGE_GPIO_SET2, 0x00 },
};

BYTE BqHugeRead(BYTE reg)
{
    BYTE result;
    BYTE retries = BQHUGE_RETRIES;

    while( retries > 0 ) {
        if( I2cWriteRead( BQI2C_MODULE, 0x68, reg, &result ) != 0 ) {
            // i2c write failed
            retries--;
        } else {
            // success
            return result;
        }
    }

    // BqHuge didn't respond after 3 tries: reset
    RimInitiateReset();
    return 0;
}

void BqHugeWrite(BYTE reg, BYTE data)
{
    BYTE retries = BQHUGE_RETRIES;

    while( retries > 0 ) {
        if( I2cWriteWrite( BQI2C_MODULE, 0x68, reg, data ) != 0 ) {
            // i2c write failed
            retries--;
        } else {
            // success
            return;
        }
    }

    // BqHuge didn't respond after 3 tries: reset
    RimInitiateReset();
}

// Shut down/disable everything that isn't required
void BqHugeWriteDefaults( void )
{
    BYTE i, reg, mask;

    // write single register values first
    for( i = 0; i < sizeof(defaultSingleRegs)/sizeof(BqHugeRegData); i++ ){
        reg = defaultSingleRegs[i].addr;
        mask = defaultSingleRegs[i].valMask;
        if( mask != 0xFF ){
            // read, mask, write
            BqHugeWrite( reg, (BqHugeRead( reg ) & ~mask) | (defaultSingleRegs[i].val & mask) );
        } else {
            // no need to read
            BqHugeWrite( reg, defaultSingleRegs[i].val );
        }
    }

    // write ranges
    for( i = 0; i < sizeof(defaultRangeRegs)/sizeof(BqHugeRegRangeData); i++ ){
        for( reg = defaultRangeRegs[i].startAddr;
             reg <= defaultRangeRegs[i].endAddr;
             reg++ ){
            BqHugeWrite( reg, defaultRangeRegs[i].val );
        }
    }
}
