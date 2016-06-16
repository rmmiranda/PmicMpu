#############################################################################
# Filename:    iarmaxq220i.mak
#
# Copyright 2011, Research In Motion Ltd
#
# Author:      Ronald Miranda
#
# Created:     April 21, 2011
#
# Description: Build options for the MQ31, using the IAR's tools, MAXQ 2.20I
#############################################################################

TOOLSET = iarmaxq220i


export MAXQ_TOOLS := iarmaxq220i

# Tools

CC				= $(MAXQ_TOOLS)/bin/iccmaxq.exe
ASM				= $(MAXQ_TOOLS)/bin/amaxq.exe
LINK			= $(MAXQ_TOOLS)/bin/xlink.exe

STDCHEADERDIR	= $(MAXQ_TOOLS)/include
STDCLIBDIR      = $(MAXQ_TOOLS)/lib

INC 			+= -I$(STDCHEADERDIR)
INC 			+= -I$(STDCHEADERDIR)/clib

CCOPT_CPU		+= -e
CCOPT_CPU		+= -z3
CCOPT_CPU		+= --no_cse
CCOPT_CPU		+= --no_unroll
CCOPT_CPU		+= --no_inline
CCOPT_CPU		+= --no_code_motion
CCOPT_CPU		+= --no_tbaa
CCOPT_CPU		+= --debug
CCOPT_CPU		+= --silent
CCOPT_CPU		+= --accumulators=16
CCOPT_CPU		+= --core=maxq68
CCOPT_CPU		+= --place_const_in_code
CCOPT_CPU		+= --data_model=l
CCOPT_CPU		+= --code_model=l 
CCOPT_CPU		+= -f $(DEFINESFILE_C) 
CCOPT_CPU		+= -o $@ 

AAOPT_CPU		+= -r
AAOPT_CPU		+= -M'<>'
AAOPT_CPU		+= --accumulators=16
AAOPT_CPU		+= --core=maxq68 
AAOPT_CPU		+= --silent
AAOPT_CPU		+= -f $(DEFINESFILE_ASM) 
AAOPT_CPU		+= -o $@ 

LLOPT_CPU		+= -S
LLOPT_CPU		+= -f scatter/$(SCATTER)
LLOPT_CPU		+= -I$(STDCLIBDIR)

LLOPT_CPU_OUT	+= -l $(TARGET_MAP) -xms
LLOPT_CPU_OUT	+= -Ointel-extended=$@
LLOPT_CPU_OUT	+= -rt

LLOPT_CPU_OUT_PARTIAL+= -l $(TARGET_MAP_PARTIAL) -xms
LLOPT_CPU_OUT_PARTIAL+= -Ointel-extended=$@
LLOPT_CPU_OUT_PARTIAL+= -rt
