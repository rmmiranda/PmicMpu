/******************************************************************************
 * rocket_pmu430.cmd
 * 
 * Command file which describes where/how the loader is located in memory
 ******************************************************************************/

/* PMU430 Memory Map */
MEMORY
{

    PERIPHERALS             : origin = 0x0000, length = 0x1000
    BSL                     : origin = 0x1000, length = 0x0800
    INFO                    : origin = 0x1800, length = 0x0200
    RAM_PRESERVED(RW)       : origin = 0x1C00, length = 0x0200      /* First 512Byte of ram are not touched by bootrom and preserved across an initiated reset*/
    RAM(RW)                 : origin = 0x1E00, length = 0x0DC0      /* Stack and globals are below the RAM interrupt vectors */
    RAMVECTORS(RW)          : origin = 0x2BC0, length = 0x0040      /* RAM interrupt vectors are at the top of RAM */

    BOOT_FLASH              : origin = 0x8000, length = 0x7800      /* Boot and App Section */
    
    LOADER_FLASH            : origin = 0xF800, length = 0x07C0      /* Bootloader Section   */
    LOADER_VECTORS(R)       : origin = 0xFFC0, length = 0x003E
    LOADER_RESET            : origin = 0xFFFE, length = 0x0002      /* HW Reset Vector to enter loader */

}

/* Section Assignments */
SECTIONS
{
#if defined( ROCKET_LOADER )

    /* .bss  .stack  .ramvectors
     * Sections containing all variables for LOADER code
     */
    .bss        : load = RAM
    .stack      : load = RAM (HIGH)
    .ramvectors : load = RAMVECTORS (HIGH)

    .boot_sec   : load = BOOT_FLASH, START(_start_of_boot_sec)
    {
        bootloader.o (.boot_sec)
    }

    /* Contains a run time flag (CRC of image to copy) written by BOOT updater
     * to flag the BOOTLOADER to copy new image over old
     */
    .info_sec   : load = INFO
    {
        metrics.o (.loader_msg)
    }

    /* .text  .cinit  .const
     * sections containing all LOADER code
     */
    .text       : load = LOADER_FLASH, START(_end_of_boot_sec)
    {
        /* loader_metrics needs to be located at the START of bootloader section */
        metrics.o (.loader_metrics)
        * (.text)
    }
    .cinit      : load = LOADER_FLASH
    .const      : load = LOADER_FLASH

    /* .boot_reset 
     * Location of pointer to boot code entrypoint used by LOADER to jump to BOOT
     */
    /*.boot_reset : load = BOOT_FLASH, START(_BootResetVector)*/

    /* .reset
     * Reset location of __c_init for LOADER code (msp430 reset vector)
     */
    .reset      : load = LOADER_RESET

#else
    /* .bss  .stack  .ramvectors
     * Sections containing all variables for BOOT/APP code
     */
    .preserved  : load = RAM_PRESERVED
    .bss        : load = RAM
    .stack      : load = RAM (HIGH)
    .ramvectors : load = RAMVECTORS (HIGH)

    /* Contains a run time flag (CRC of image to copy) written by BOOT updater
     * to flag the BOOTLOADER to copy new image over old
     */
    .info_sec   : load = INFO
    {
#if defined( DEVID_UPDATER_SUPPORT )
        metrics.o (.loader_msg)
#endif
    }

    /* .text .const
     * sections containing all BOOT code
     */
    .text_boot  : load = BOOT_FLASH, PALIGN(2), START(_start_of_boot_image), START(_start_of_boot_sec)
    {
#if defined( DEVID_UPDATER_SUPPORT )
        metrics.o (.boot_info)
#endif
        init.o (.text .const)
        rocket_gpio.o (.text .const)
        rocket_misc.o (.text .const)
        rocket_timer.o (.text .const)
        rocket_api.o (.text .const)
        timer_mux.o (.text .const)
        rocket_i2c.o (.text .const)
        rocket_slowclock.o (.text .const)
        eventlog.o (.text .const)
        msgque.o (.text .const)
        rocket_spi.o (.text .const)
        rocket_intvector.o (.text .const)
        rocket_adc_hw.o (.text .const)
#if defined( DEVID_UPDATER_SUPPORT )
        rocket_updater.o (.text .const)
#endif
#if defined( HOSTPROCESSOR_TAVOR_MG )
        hostctrl_common.o (.text .const)
        hostctrl_tavormg.o (.text .const)
#else
#if defined( HOSTPROCESSOR_ESHEL )
        hostctrl_common.o (.text .const)
        hostctrl_eshel.o (.text .const)
#else
#if defined( HOSTPROCESSOR_OMAP4430 )
        hostctrl_common.o (.text .const)
        hostctrl_omap4430.o (.text .const)
#endif // HOSTPROCESSOR_OMAP4430
#endif // HOSTPROCESSOR_ESHEL
#endif // HOSTPROCESSOR_TAVOR_MG
#if !defined( CC4_PROJECT )
//        msp430_hwdelay.o (.text .const)
        msp430_flash.o (.text .const)
        rocket_pmic_intctrl.o (.text .const)
        rocket_crc.o (.text .const)
#endif

#if defined( DEVID_UPDATER_SUPPORT )
        rts430.lib (.text .const .reset )
#else
        rts430.lib (.text .const )
#endif
        msp430_inout.o (.text)

#if defined( RIM_MAX8649 ) || defined( RIM_TPS62361 )
        pmic_max8649.o (.text .const )
#endif
        metrics.o (.text .const)

    }

#if !defined( DEVID_UPDATER_SUPPORT )
    .reset      : load = LOADER_RESET
    {
        rts430.lib (.reset)
    }
#endif

    /* .cinit
     * section for all BOOT/APP cinit tables. must be included in BOOT image above
     */
    .cinit : load = BOOT_FLASH, END(_end_of_boot_image)


    /* .text .const
     * sections containing all APP code
     */
    .text_app   : load = BOOT_FLASH (HIGH), PALIGN(512), START(_start_of_app_image), END(_end_of_app_image) 
    {
#if defined( DEVID_UPDATER_SUPPORT )
        metrics.o (.app_info)
#endif
        * (.text .const)
    }

    .loader_sec : load = LOADER_FLASH, START(_end_of_boot_sec)

    /* .reset
     * Reset location of __c_init for BOOT/APP code
     */
    /*.reset      : load = BOOT_RESET*/
#endif
}
