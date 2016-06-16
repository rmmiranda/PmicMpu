/****************************************************************************
 * Filename:    eventlog_events.h
 *
 * Copyright 2011, Research In Motion Ltd
 *
 * Author:      Ronald Miranda
 *
 * Created:     March 8, 2011
 *
 * Description: Eventlog event numbers shared between Nessus and rocket
 ****************************************************************************/
#ifndef __EVENTLOG_EVENTS_H__
#define __EVENTLOG_EVENTS_H__

#define BUGBUILD_ENUM "EvtLogEvent"

#define EVENTLOG_NOEVENT                0x0000
#define EVENTLOG_INIT                   0x0001
#define EVENTLOG_RTC_INIT               0x0002
#define EVENTLOG_TIMER_INIT             0x0003
#define EVENTLOG_ADC_HW_INIT            0x0004
#define EVENTLOG_I2C_INIT               0x0005
#define EVENTLOG_SPI_INIT               0x0006
#define EVENTLOG_HOST_POWERUP_COMPLETE  0x0007
#define EVENTLOG_PMIC_INTCTRL_INIT      0x0008
#define EVENTLOG_ADC_ENGINE_INIT        0x0009
#define EVENTLOG_FG_INIT                0x000A
#define EVENTLOG_EXT_XTAL               0x000B
#define EVENTLOG_I2C_ROM_INIT           0x000C     //AS+ Reserve an eventlog for OMAP4 ROM mode 
#define EVENTLOG_I2C_ROM_ERROR          0x000D     //AS+ Reserve an eventlog for OMAP4 ROM error 
#define EVENTLOG_I2C_ROM_FINISH         0x000E     //AS+ Reserve an eventlog for OMAP4 ROM finished 
#define EVENTLOG_TIMESTAMP              0x000F

#define EVENTLOG_MSGQUE_INIT            0x0010
#define EVENTLOG_MSGQUE_FULL            0x0011
#define EVENTLOG_MSGQUE_POST            0x0012
#define EVENTLOG_MSGQUE_GET             0x0013

#define EVENTLOG_INITIATE_RESET         0x0020
#define EVENTLOG_SUPERCAP_LPM_RESET     0x0021
#define EVENTLOG_WATCHDOG_RESET         0x0022
#define EVENTLOG_WATCHDOG_HIT_ISR       0x0023
#define EVENTLOG_WATCHDOG_HIT_MANUAL    0x0024
#define EVENTLOG_RESET_REASON           0x0025
#define EVENTLOG_RESET_EXTRA            0x0026
#define EVENTLOG_RESET_COOKIE           0x0027
#define EVENTLOG_RESET_KEYVOL           0x0028
#define EVENTLOG_RESET_PMICFAULT        0x0029
#define EVENTLOG_RESET_UNEXPECETED      0x002A
#define EVENTLOG_RESET_FASTRST          0x002B

#define EVENTLOG_ENTER_SCSLEEP          0x0030
#define EVENTLOG_EXIT_SCSLEEP           0x0031

#define EVENTLOG_ADC_START_ENGINE       0x0040
#define EVENTLOG_ADC_SAMPLE_SCHEDULED   0x0041
#define EVENTLOG_ADC_TIMER_EXPIRED      0x0042
#define EVENTLOG_ADC_SAMPLE_STARTED     0x0043
#define EVENTLOG_ADC_SAMPLE_COMPLETE    0x0044
#define EVENTLOG_ADC_INTERRUPT_HOST     0x0045
#define EVENTLOG_ADC_INHIBIT_TESTGPIO   0x0046
#define EVENTLOG_ADC_SAMPLE_DISCARDED   0x0047
#define EVENTLOG_ADC_CONFIG_INHIBIT     0x0048
#define EVENTLOG_ADC_SAMPLE_CANT_BE_SCHEDULED 0x0049
#define EVENTLOG_ADC_SAMPLE_NOT_ALLOWED 0x004A
#define EVENTLOG_ADC_SAMPLE_TRIGGERED   0x004B
#define EVENTLOG_ADC_SAMPLE_MISSED      0x004C
#define EVENTLOG_ADC_SAMPLE_ALLOWED     0x004D
#define EVENTLOG_ADC_POLLING            0x004E
#define EVENTLOG_ADC_SAMPLE_NOT_EXPECTED 0x004F
#define EVENTLOG_ADC_ENGINE_DONE        0x0050
#define EVENTLOG_ADC_CHNGSTATE          0x0051
#define EVENTLOG_ADC_SMENTRY            0x0052
#define EVENTLOG_ADC_WAIT_HW_START      0x0053

#define EVENTLOG_PMIC_INTCTRL_INT               0x0060
#define EVENTLOG_PMIC_INTCTRL_MSP430HANDLER     0x0061
#define EVENTLOG_PMIC_HPD               0x0062

#define EVENTLOG_API_HOSTINT            0x0080

#define EVENTLOG_UPDATER_INIT           0x0090
#define EVENTLOG_UPDATER_WRITE_BOOT     0x0091
#define EVENTLOG_UPDATER_PREP_BOOT      0x0092
#define EVENTLOG_UPDATER_VERIFY_BOOT    0x0093
#define EVENTLOG_UPDATER_CHECK_UPDATE   0x0094
#define EVENTLOG_UPDATER_WRITE_APP      0x0095
#define EVENTLOG_UPDATER_PREP_APP       0x0096
#define EVENTLOG_UPDATER_VERIFY_APP     0x0097
#define EVENTLOG_UPDATER_WRITE_MSG      0x0098
#define EVENTLOG_UPDATER_BLOW_JTAG_FUSE 0x0099
#define EVENTLOG_UPDATER_ERASE_SEG      0x009A

#define EVENTLOG_HOSTCTRL_HARD_RESET    0x0100

#define EVENTLOG_ENTER_SUPERCAP_LPM     0x0110
#define EVENTLOG_EXIT_SUPERCAP_LPM      0x0111
#define EVENTLOG_ENTER_BRICK_LPM        0x0112
#define EVENTLOG_EXIT_BRICK_LPM         0x0113
#define EVENTLOG_NMI_ERROR              0x0114

#define EVENTLOG_I2C_START              0x0200
#define EVENTLOG_I2C_RX_DEVID           0x0201
#define EVENTLOG_I2C_RX_REGADDR         0x0202
#define EVENTLOG_I2C_RX_DATA            0x0203
#define EVENTLOG_I2C_RESTART            0x0204
#define EVENTLOG_I2C_TX_DATA            0x0205
#define EVENTLOG_I2C_STOP               0x0206
#define EVENTLOG_I2C_ERROR              0x0207
#define EVENTLOG_I2C_NACKED             0x0208
#define EVENTLOG_I2C_ARBLOST            0x0209
#define EVENTLOG_I2C_STRETCH_REQUIRED   0x020A
#define EVENTLOG_I2C_STRETCH_ERROR      0x020B
#define EVENTLOG_I2C_WRITE_ERROR        0x020C
#define EVENTLOG_I2C_READ_ERROR         0x020D
#define EVENTLOG_I2C_STAT0              0x020E
#define EVENTLOG_I2C_CTL0               0x020F
#define EVENTLOG_I2C_CTL1               0x0210
#define EVENTLOG_I2C_IE                 0x0211
#define EVENTLOG_I2C_IFG                0x0212

#define EVENTLOG_I2CRLY_PWRISR          0x0280

#define EVENTLOG_TIMER_EXPIRY           0x0300
#define EVENTLOG_TIMER_SCHEDULED        0x0301
#define EVENTLOG_TIMER_NOW              0x0302
#define EVENTLOG_TIMER_NUM              0x0303
#define EVENTLOG_TIMER_STOP             0x0304
#define EVENTLOG_TIMER_DUMP             0x0305
#define EVENTLOG_TIMER_SET              0x0306
#define EVENTLOG_TIMER_TRY_EXPIRE       0x0307
#define EVENTLOG_TIMER_HWMATCH          0x0308
#define EVENTLOG_TIMER_DEBUG1           0x0309
#define EVENTLOG_TIMER_DEBUG2           0x030A
#define EVENTLOG_TIMER_DEBUG3           0x030B
#define EVENTLOG_TIMER_DEBUG4           0x030C

#define EVENTLOG_FG_INT                 0x0400

#define EVENTLOG_PMU430_UNHANDLED_INT   0x0500
#define EVENTLOG_PMU430_PORT1GPIO_INT   0x0501
#define EVENTLOG_PMU430_PORT2GPIO_INT   0x0502
#define EVENTLOG_PMU430_ADC_INT         0x0503
#define EVENTLOG_PMU430_USCIB0_INT      0x0504
#define EVENTLOG_PMU430_USCIB1_INT      0x0505
#define EVENTLOG_PMU430_USCIB2_INT      0x0506
#define EVENTLOG_PMU430_USRNMI_INT      0x0507
#define EVENTLOG_PMU430_SYSNMI_INT      0x0508
#define EVENTLOG_PMU430_USCIA0_INT      0x0509
#define EVENTLOG_PMU430_USCIA1_INT      0x050A
#define EVENTLOG_PMU430_USCIA2_INT      0x050B
#define EVENTLOG_PMU430_USCIA3_INT      0x050C
#define EVENTLOG_PMU430_TIMERA0_INT     0x050D
#define EVENTLOG_PMU430_TIMERA1_INT     0x050E
#define EVENTLOG_PMU430_RTC_INT         0x050F

#define EVENTLOG_HOST_RESET_N_INT       0x0510

#define EVENTLOG_STATE_EXTSW_LASTVAL    0x0600
#define EVENTLOG_STATE_MSGQUE_LASTCALL  0x0601

// Special flags for decoding/printing routine.
// Signals that the next eventlog entry is data, not an even enum
#define EVENTLOG_PRINTN                 0xFFF0
// Generic eventS to allow printing data/seeing order of events while debugging
#define EVENTLOG_DEBUG_DATA             0xFFF1
#define EVENTLOG_DEBUG_EVENT_A          0xFFF2
#define EVENTLOG_DEBUG_EVENT_B          0xFFF3
#define EVENTLOG_DEBUG_EVENT_C          0xFFF4
#define EVENTLOG_DEBUG_EVENT_D          0xFFF5
#define EVENTLOG_DEBUG_EVENT_E          0xFFF6
#define EVENTLOG_DEBUG_EVENT_F          0xFFF7

#undef BUGBUILD_ENUM // "EvtLogEvent"

#endif
