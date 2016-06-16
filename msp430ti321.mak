#############################################################################
# Filename:    msp430ti321.mak
#
# Copyright 2009, Research In Motion Ltd
#
# Author:      Bryon Hummel
#
# Created:     Sept 18, 2009
#
# Description: Build options for the MSP430, using the TI's tools, 3.2.1
#############################################################################

TOOLSET = msp430ti321


export MSP430_TOOLS := msp430ti321

# Tools

CC				= $(MSP430_TOOLS)/bin/cl430.exe
ASM				= $(MSP430_TOOLS)/bin/cl430.exe
LINK			= $(MSP430_TOOLS)/bin/cl430.exe
HEX             = $(MSP430_TOOLS)/bin/hex430.exe

STDCHEADERDIR	= $(MSP430_TOOLS)/include
STDCLIBDIR      = $(MSP430_TOOLS)/lib

INC 			+= -I$(STDCHEADERDIR)

CCOPT_CPU		+= -O3
CCOPT_CPU		+= --auto_inline=0
CCOPT_CPU		+= --single_inline
CCOPT_CPU		+= --diag_suppress=230    # "trailing comma is nonstandard"
CCOPT_CPU		+= --diag_error=994       # "missing return statement at end of non-void function"
CCOPT_CPU		+= -@=$(DEFINESFILE_C) 
CCOPT_CPU		+= -fr=$(OBJ_DIR) -eo=.o

AAOPT_CPU		+= -@=$(DEFINESFILE_ASM)
AAOPT_CPU		+= -fr=$(OBJ_DIR) -eo=.o

LLOPT_CPU		+= --run_linker
LLOPT_CPU		+= --rom_model
LLOPT_CPU		+= -i$(STDCLIBDIR)
LLOPT_CPU		+= $(DEFINESFILE_CMD)

LLOPT_CPU_OUT	+= -m $(TARGET_MAP)
LLOPT_CPU_OUT	+= scatter/$(SCATTER)
LLOPT_CPU_OUT	+= -o $@

LLOPT_CPU_OUT_PARTIAL += -m $(TARGET_MAP_PARTIAL)
LLOPT_CPU_OUT_PARTIAL += scatter/$(SCATTER)
LLOPT_CPU_OUT_PARTIAL += -o $@
