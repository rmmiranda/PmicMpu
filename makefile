######################################################################
##                                                                  ##
## This makefile will build the pmic microcontroller code.          ##
##                                                                  ##
######################################################################

#
# Define flags for the build environment
#
ifeq ($(BOOTLOADER), true)

 ifeq ($(NULL), obj/$(PLATFORM)/loader_$(PLATFORM)/product.mak)
  $(error Specify target platform using MAKE.BAT)
 else
  include obj/$(PLATFORM)/loader_$(PLATFORM)/product.mak
 endif

else

 ifeq ($(NULL), obj/$(PLATFORM)/product.mak)
  $(error Specify target platform using MAKE.BAT)
 else
  include obj/$(PLATFORM)/product.mak
 endif

endif

ifdef ROCKET_LOADER
OBJ_DIR = obj/$(PLATFORM)/loader_$(PLATFORM)
else
OBJ_DIR = obj/$(PLATFORM)
endif

REL_DIR = obj/$(PLATFORM)/nessus_release

PLATFORM_DIR = config/$(PLATFORM)

ifeq "$(OS)" "Windows_NT"
 NULL    =
else
 NULL    = nul
endif

.SUFFIXES:
.SUFFIXES: .gsp .h .c .dep .o .s .def .elf .bin .egs

#
# Processor types
#

ifdef PROCESSOR_PMU430

PMIC = rocket
TOOLSET = msp430ti321

else
ifdef PROCESSOR_MQ31

PMIC = lagavulin
TOOLSET = iarmaxq230

endif
endif

#
# Build types
#

ifdef ROCKET_LOADER
TARGET      = $(PMIC)_$(PLATFORM)_loader.ahx
TARGET_OUT  = $(OBJ_DIR)/$(PMIC)_$(PLATFORM)_loader.out
TARGET_MAP  = $(OBJ_DIR)/$(PMIC)_$(PLATFORM)_loader.map
else
TARGET      = $(PMIC)_$(PLATFORM).bin
TARGET_H    = $(PMIC)_$(PLATFORM).h
TARGET_OUT  = $(OBJ_DIR)/$(PMIC).out
TARGET_MAP  = $(OBJ_DIR)/$(PMIC).map
TARGET_MAP_PARTIAL = $(OBJ_DIR)/$(PMIC)_partial.map
TARGET_OUT_PARTIAL = $(OBJ_DIR)/$(PMIC)_partial.out
endif

#
# Tool definitions
#

ARMOS_ROOT_DIR = ..

TOOLSDIR = ../common/tools
include $(TOOLSDIR)/tools.mak

DEFINESFILE_C = $(OBJ_DIR)/defines_c.txt
DEFINESFILE_ASM = $(OBJ_DIR)/defines_asm.txt
DEFINESFILE_CMD = $(OBJ_DIR)/defines_cmd.txt

INC = -Iinclude

ifdef ROCKET_LOADER
INC += -I$(REL_DIR)
endif

INC += -I$(OBJ_DIR)
INC += -I$(PLATFORM_DIR)
INC += -I.

include $(TOOLSET).mak

# Main Target -------------------------------------------------------------
default: all

# File lists ---------------------------------------------------------------

OBJS =
APPOBJS =
STUBOBJS =
LIBS =

ifdef ROCKET_LOADER

ifdef PROCESSOR_PMU430
OBJS += $(OBJ_DIR)/bootloader.o
OBJS += $(OBJ_DIR)/rocket_crc.o
OBJS += $(OBJ_DIR)/msp430_flash.o
OBJS += $(OBJ_DIR)/metrics.o
else
ifdef PROCESSOR_MQ31
OBJS += $(OBJ_DIR)/mq31_bootmanager.o
endif
endif

else

# BOOT Section Code
# ** WARNING **  Any addition to this section requires a change to the 
# ** WARNING **  scatter file as well to actually allocate the code in the 
# ** WARNING **  correct location.
OBJS += $(OBJ_DIR)/init.o
OBJS += $(OBJ_DIR)/timer_mux.o
OBJS += $(OBJ_DIR)/eventlog.o
OBJS += $(OBJ_DIR)/msgque.o
ifdef HOSTPROCESSOR_TAVOR_MG
OBJS += $(OBJ_DIR)/hostctrl_common.o
OBJS += $(OBJ_DIR)/hostctrl_tavormg.o
else
ifdef HOSTPROCESSOR_ESHEL
OBJS += $(OBJ_DIR)/hostctrl_common.o
OBJS += $(OBJ_DIR)/hostctrl_eshel.o
else
ifdef HOSTPROCESSOR_OMAP4430
OBJS += $(OBJ_DIR)/hostctrl_common.o
OBJS += $(OBJ_DIR)/hostctrl_omap4430.o
endif #HOSTPROCESSOR_OMAP4430
endif #HOSTPROCESSOR_ESHEL
endif #HOSTPROCESSOR_TAVOR_MG
ifdef RIM_MAX8649
OBJS += $(OBJ_DIR)/pmic_max8649.o
endif #RIM_MAX8649
ifdef RIM_TPS62361
OBJS += $(OBJ_DIR)/pmic_max8649.o
endif #RIM_TPS62361
ifdef PROCESSOR_PMU430
OBJS += $(OBJ_DIR)/metrics.o
OBJS += $(OBJ_DIR)/rocket_gpio.o
OBJS += $(OBJ_DIR)/rocket_misc.o
OBJS += $(OBJ_DIR)/rocket_timer.o
OBJS += $(OBJ_DIR)/rocket_api.o
OBJS += $(OBJ_DIR)/rocket_i2c.o
OBJS += $(OBJ_DIR)/rocket_slowclock.o
OBJS += $(OBJ_DIR)/rocket_spi.o
OBJS += $(OBJ_DIR)/msp430_flash.o
OBJS += $(OBJ_DIR)/rocket_pmic_intctrl.o
OBJS += $(OBJ_DIR)/rocket_intvector.o
ifdef DEVID_UPDATER_SUPPORT
OBJS += $(OBJ_DIR)/rocket_updater.o
endif #DEVID_UPDATER_SUPPORT
OBJS += $(OBJ_DIR)/rocket_crc.o
OBJS += $(OBJ_DIR)/rocket_adc_hw.o
OBJS += $(OBJ_DIR)/msp430_inout.o
else
ifdef PROCESSOR_MQ31
OBJS += $(OBJ_DIR)/delay.o
OBJS += $(OBJ_DIR)/lagavulin_metrics.o
OBJS += $(OBJ_DIR)/lagavulin_gpio.o
OBJS += $(OBJ_DIR)/lagavulin_misc.o
OBJS += $(OBJ_DIR)/lagavulin_timer.o
OBJS += $(OBJ_DIR)/lagavulin_api.o
OBJS += $(OBJ_DIR)/lagavulin_i2c.o
OBJS += $(OBJ_DIR)/lagavulin_slowclock.o
OBJS += $(OBJ_DIR)/lagavulin_spi.o
OBJS += $(OBJ_DIR)/mq31_flash.o
OBJS += $(OBJ_DIR)/lagavulin_pmic_intctrl.o
OBJS += $(OBJ_DIR)/lagavulin_intvector.o
ifdef DEVID_UPDATER_SUPPORT
OBJS += $(OBJ_DIR)/lagavulin_updater.o
endif #DEVID_UPDATER_SUPPORT
OBJS += $(OBJ_DIR)/lagavulin_crc.o
OBJS += $(OBJ_DIR)/lagavulin_adc_hw.o
OBJS += $(OBJ_DIR)/mq31_inout.o
endif #PROCESSOR_PMU430
endif #PROCESSOR_MQ31


# APP Section Code
APPOBJS += $(OBJ_DIR)/app_init.o
APPOBJS += $(OBJ_DIR)/adc.o
APPOBJS += $(OBJ_DIR)/led.o
APPOBJS += $(OBJ_DIR)/gpio_relay.o
APPOBJS += $(OBJ_DIR)/hostctrl_paslump.o
ifdef PROCESSOR_PMU430
APPOBJS += $(OBJ_DIR)/rocket_led_hw.o
APPOBJS += $(OBJ_DIR)/hostctrl_pwr.o
APPOBJS += $(OBJ_DIR)/rocket_fuelgauge.o
APPOBJS += $(OBJ_DIR)/rocket_i2c_relay.o
APPOBJS += $(OBJ_DIR)/rocket_utc.o
ifdef RIM_BRICK_MODE
APPOBJS += $(OBJ_DIR)/rocket_ft.o
endif #RIM_BRICK_MODE
ifdef HOSTPROCESSOR_TAVOR_MG
APPOBJS += $(OBJ_DIR)/hostctrl_pwri2c.o
else 
ifdef HOSTPROCESSOR_ESHEL
APPOBJS += $(OBJ_DIR)/hostctrl_pwri2c.o
endif #HOSTPROCESSOR_ESHEL
endif #HOSTPROCESSOR_TAVOR_MG
ifdef RIM_PWM
APPOBJS += $(OBJ_DIR)/rocket_pwm.o
endif #RIM_PWM
else
ifdef PROCESSOR_MQ31
APPOBJS += $(OBJ_DIR)/lagavulin_led_hw.o
APPOBJS += $(OBJ_DIR)/lagavulin_utc.o
ifdef RIM_PWM
APPOBJS += $(OBJ_DIR)/lagavulin_pwm.o
endif #RIM_PWM
endif #PROCESSOR_PMU430
endif #PROCESSOR_MQ31


# APP Stubs (APP code safe to be referenced by BOOT code)
STUBOBJS += $(OBJ_DIR)/app_stubs.o


# Library Section Code
ifdef PROCESSOR_PMU430
LIBS += --library=rts430.lib
else
ifdef PROCESSOR_MQ31
LIBS += cstartup_usr.r66
LIBS += mgLib.r66
LIBS += clib/clmaxq68mllc.r66
endif #PROCESSOR_PMU430
endif #PROCESSOR_MQ31


endif

# -------------------------------------------------------------------------
# Dependencies
#
# Now that we have all the -D defines we need, use them to generate
# dependency rules for each source file.
# -------------------------------------------------------------------------

SOURCEPATHS = src

#
TARGDEP = $(OBJ_DIR)/makefile.dep
HEADERDEPDIRS = $(filter-out -I$(OBJ_DIR), $(INC) )
HEADERDEP = $(filter-out %/*.h %/builder.h , $(wildcard $(patsubst -I%,%/*.h, $(HEADERDEPDIRS))))
SOURCEDEP = $(filter-out %/*.c %/*.cpp, $(wildcard $(addsuffix /*.c,$(SOURCEPATHS)) $(addsuffix /*.cpp,$(SOURCEPATHS))))

$(TARGDEP) : $(OBJ_DIR) makefile $(SOURCEDEP) $(HEADERDEP)
	$(PRINT) Building dependency file $@ ...
	-$(RM) -f $@
	$(PRINT). > $@
	$(MAKEDEP) -q $(INC) -f$@ -p$(OBJ_DIR)/ -o.o $(SOURCEDEP)
	$(RM) $@.bak

-include $(TARGDEP)
# Rules---------------------------------------------------------------------
### Process the ELF file and generate a BIN file with an appended LDR section ###

### Create the hex output file for the jtag loader in manufacturing to load.
ifdef ROCKET_LOADER

$(TARGET): $(TARGET_OUT)
	$(PRINT) "TRANSLATING        : $(TARGET_OUT) to $(TARGET)"
	$(PRINT) ""	
ifdef PROCESSOR_PMU430
	$(HEX) -a -memwidth=8 $(TARGET_OUT) -o $(OBJ_DIR)/$(TARGET)
else
ifdef PROCESSOR_MQ31
	$(COPY) $(TARGET_OUT) $(OBJ_DIR)/$(TARGET)
endif
endif
	$(COPY) $(TARGET_OUT) release/$(PLATFORM)/$(PMIC)_$(PLATFORM)_loader.out
	$(COPY) $(TARGET_MAP) release/$(PLATFORM)/$(PMIC)_$(PLATFORM)_loader.map
	$(COPY) $(OBJ_DIR)/$(TARGET) release/$(PLATFORM)/$(TARGET)

else 
$(TARGET_H): $(TARGET_OUT_PARTIAL) $(TARGET_OUT) 
	$(PRINT) "TRANSLATING        : $(PMIC).out to $(TARGET_H)"
	$(PRINT) ""	
ifdef PROCESSOR_PMU430
	$(PYTHON) RocketBuildCmd.py -m$(TARGET_MAP) -o$(OBJ_DIR)/rocket_sections.cmd
	$(HEX) -a -image -order=MS -zero -memwidth=16 $(OBJ_DIR)/rocket_sections.cmd $(TARGET_OUT) -o $(OBJ_DIR)/$(TARGET)_boot -o $(OBJ_DIR)/$(TARGET)_app
	$(PYTHON) RocketFirmwareBuilder.py $(OBJ_DIR)/$(TARGET)_boot $(OBJ_DIR)/$(TARGET)_app $(REL_DIR)/$(TARGET_H) $(REL_DIR)/$(PMIC)_image.h
else
ifdef PROCESSOR_MQ31
	$(PYTHON) LagavulinFirmwareBuilder.py $(TARGET_OUT) $(REL_DIR)/$(TARGET_H) $(REL_DIR)/$(PMIC)_image.h
endif
endif
	$(COPY) $(REL_DIR)/$(TARGET_H) release/$(PLATFORM)/$(TARGET_H)
	$(COPY) $(TARGET_OUT) release/$(PLATFORM)/$(PMIC).out
	$(COPY) $(TARGET_MAP) release/$(PLATFORM)/$(PMIC).map
endif

$(OBJ_DIR):
	if not exist $(OBJ_DIR)/$(NULL) mkdir $(OBJ_DIR)

# Special rule to force the constant rebuild of metrics.c
$(OBJ_DIR)/metrics.o: builder

$(OBJS): $(INC_FILES) $(H_FILES)

$(APPOBJS) : $(INC_FILES) $(H_FILES)

$(STUBOBJS) : $(INC_FILES) $(H_FILES)

$(LIST_OBJS): $(INC_FILES) $(H_FILES)

$(OBJ_DIR)/builder.h:
	$(PRINT) Generating $@
	$(HTOMAK2) /b $(PLATFORM_DIR)/product.def $@

builder: $(OBJ_DIR)/builder.h

$(OBJ_DIR)/%.o: src/%.s
	$(PRINT) $<
	$(ASM) $< $(AAOPT_CPU) $(INC) 

$(OBJ_DIR)/%.o: src/%.c
	$(PRINT) $<
	$(CC) $< $(CCOPT_CPU) $(INC)


# Build executable binary------------------------------------------------
$(TARGET_OUT_PARTIAL) : $(STUBOBJS) $(OBJS) 
	$(PRINT) ""
	$(PRINT) "LINKING            : $(TARGET_OUT_PARTIAL)"
	$(LINK) $(LLOPT_CPU) $(OBJS) $(STUBOBJS) $(LIBS) $(LLOPT_CPU_OUT_PARTIAL)
	$(PRINT) ""

$(TARGET_OUT) : $(OBJS) $(APPOBJS)
	$(PRINT) ""
	$(PRINT) "LINKING            : $(TARGET_OUT)"
	$(LINK) $(LLOPT_CPU) $(OBJS) $(APPOBJS) $(LIBS) $(LLOPT_CPU_OUT)
	$(PRINT) ""

# -------------------------------------------------------------------------
# Build info (used by osreview, in particular)
# -------------------------------------------------------------------------
buildinfo:
	$(PRINT) SourcePaths: $(SOURCEPATHS) > $(OBJ_DIR)/buildinfo.txt
	$(PRINT) Modules: $(notdir $(OBJS:.o=)) >> $(OBJ_DIR)/buildinfo.txt
	$(PRINT) CFlags: $(INC) $(CFLAGS) >> $(OBJ_DIR)/buildinfo.txt
	$(PRINT) Libraries: $(LIBS) >> $(OBJ_DIR)/buildinfo.txt
	$(PRINT) Scatter: scatter/$(SCATTER) >> $(OBJ_DIR)/buildinfo.txt

ifdef ROCKET_LOADER
all: builder $(OBJ_DIR) $(TARGET) buildinfo
	$(PRINT) "**************************************************************************************"		
	$(PRINT) '** Bootloader Build Complete:   $(TARGET)' 
	$(PRINT) "**************************************************************************************"		
else
all: builder $(OBJ_DIR) $(TARGET_H) buildinfo
	$(PRINT) "**************************************************************************************"		
	$(PRINT) '** Boot/App Image Build Complete:   $(TARGET_H)'	
	$(PRINT) "**************************************************************************************"		
endif	

list: builder $(OBJ_DIR) $(LIST_OBJS) buildinfo
	$(PRINT) 'done'
