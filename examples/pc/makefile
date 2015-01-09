# The MIT License (MIT)
# 
# Copyright (c) 2014 - 2015, Andreas Merkle
# http://www.blue-andi.de
# vscp@blue-andi.de
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

################################################################################
# Makefile for VSCP tests
# Author: Andreas Merkle, http://www.blue-andi.de
#
# $Author: amerkle $
# $Rev: 449 $
# $Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
################################################################################

################################################################################
# Configuration
################################################################################

# Software version
VERSION= 0.1.0

# Binary file name
BIN= vscpExamplePc

# Includes
INCLUDES= -I. \
		-Iinclude \
		-IvscpUser \
		-I../../vscp

# Sources
SOURCES= main.c \
		eeprom.c \
		log.c \
		platform.c \
		vscpUser/vscp_action.c \
		vscpUser/vscp_app_reg.c \
		vscpUser/vscp_portable.c \
		vscpUser/vscp_ps_access.c \
		vscpUser/vscp_tp_adapter.c \
		vscpUser/vscp_timer.c \
		../../vscp/vscp_core.c \
		../../vscp/vscp_dev_data.c \
		../../vscp/vscp_dm.c \
		../../vscp/vscp_dm_ng.c \
		../../vscp/vscp_ps.c \
		../../vscp/vscp_transport.c \
		../../vscp/vscp_util.c \
		../../vscp/events/vscp_alarm.c \
		../../vscp/events/vscp_control.c \
		../../vscp/events/vscp_information.c \
		../../vscp/events/vscp_security.c \
		../../vscp/events/vscp_weather.c \
		../../vscp/events/vscp_weather_forecast.c

# Include all source pathes here like ../src1:../src2
VPATH= ../../vscp:../../vscp/events:vscpUser
		
# Library search path (-L...)
LIBPATH= -Llib/linux

# Windows specific library path:
ifeq ($(OS),Windows_NT)

LIBPATH= -Llib/win32

endif

# Libraries (-l...)
LIBRARIES= -lpthread -lvscphelper

# Windows specific libraries:
ifeq ($(OS),Windows_NT)

LIBRARIES+= -lws2_32

endif

# Preprocessor defines
PREPROC= -D_REENTRANT -DVERSION=\"$(VERSION)\"

# General compiler flags independent of debug or release build
# All warnings: -Wall
CFLAGS= -Wall

# General linker flags independent of debug or release build
# All warnings: -Wall
# Create map file: -Wl,-Map,foo.map
LDFLAGS= -Wall -Wl,-Map,$(BINDIR)/$(BIN).map -Wl,-rpath='$$ORIGIN/../lib/linux'

ifeq ($(MAKECMDGOALS),debug)

# Additional compiler flags for debug build
# Minimal: -g1
# Default: -g
# Maximum: -g3
# Optimization: -O1
# Generate additional coverage information: -ftest-coverage
# Generate profile output: -fprofile-arcs
CFLAGS+= -g -O1

# Additional linker flags
# Enable coverage: -ftest-coverage
# Enable profiling: -fprofile-arcs
LDFLAGS+=

endif

ifeq ($(MAKECMDGOALS),release)

# Additional compiler flags for release build
# Optimization: -Os
# Generate additional coverage information: -ftest-coverage
# Generate profile output: -fprofile-arcs
CFLAGS+= -Os

# Additional linker flags
LDFLAGS+= 

endif

# Object directory
OBJDIR= obj

# Binary directory
BINDIR= bin

# Library directory
LIBDIR= lib/linux

ifeq ($(OS),Windows_NT)

LIBDIR= lib/win32

endif

# Remove path from .c files, rename to .o files and add object directory
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:%.c=%.o)))

# All dependency files
DEPFILES= $(OBJECTS:%.o=%.d)

# Shared objects
SHARED_OBJS= $(BINDIR)/libvscphelper.so

# DLLS
DLLS=

ifeq ($(OS),Windows_NT)

# Shared objects
SHARED_OBJS=

# DLLs
DLLS= $(BINDIR)/vscphelper.dll \
	$(BINDIR)/ssleay32.dll \
	$(BINDIR)/libeay32.dll

endif

################################################################################
# Tools
################################################################################

# Compiler
CC= gcc

# Linker
LD= gcc

# Remove file(s)
REMOVE= rm

# Make directory
MKDIR= mkdir

# Copy file(s)
COPY= cp

################################################################################
# Targets
################################################################################

help:
	@echo "********"
	@echo "* HELP *"
	@echo "********"
	@echo ""
	@echo "Targets:"
	@echo "debug   - Build binary (debug)"
	@echo "release - Build binary (release)"
	@echo "depend  - Generate dependency files"
	@echo "clean   - Remove object and binary files"
	@echo "objsize - Show object sizes"
	@echo "help    - This help is shown"
	@echo ""
	
debug: $(BINDIR)/$(BIN).exe $(SHARED_OBJS) $(DLLS)
	@echo "Finished."

release: $(BINDIR)/$(BIN).exe $(SHARED_OBJS) $(DLLS)
	@echo "Finished."

clean:
	@echo "Cleaning files ..."
	@$(REMOVE) -Rf $(BINDIR) $(OBJDIR) 2> /dev/null
	@echo "Finished."
	
objsize:
	@echo ".text       : normal program code"
	@echo ".bootloader : bootloader program code"
	@echo ".data       : initialized data"
	@echo ".bss        : data initialized by 0"
	@echo ".noinit     : not initialized data"
	@echo "Flash  = .text + .bootloader + .data"
	@echo "SRAM   = .data + .bss + .noinit"
	@echo "EEPROM = .eeprom"
	@size -d $(OBJDIR)/*.o
	
depend: $(DEPFILES)
	@echo "Finished."

.PHONY: help clean

# Include dependency files only in case of target 'debug' or 'release'
ifeq ($(MAKECMDGOALS),debug)
-include $(DEPFILES)
endif
ifeq ($(MAKECMDGOALS),release)
-include $(DEPFILES)
endif

################################################################################
# Rules
################################################################################
	
# Common rule to create object files from source files
$(OBJDIR)/%.o: %.c
	@echo "Compiling" $<
	@$(CC) -c $(CFLAGS) $(PREPROC) $(INCLUDES) $< -o $(OBJDIR)/$(@F) 2>&1
	
# Common rule to create dependency files from source files
$(OBJDIR)/%.d: %.c
	@echo "Create dependencies of" $<
	@$(CC) $(PREPROC) $(INCLUDES) -MM $< | sed 's#^\(.*:\)#./$(OBJDIR)/\1#' > $(OBJDIR)/$(@F)

$(DEPFILES): | $(OBJDIR)

$(OBJECTS): | $(OBJDIR) $(BINDIR)

# Create directory for object and dependency files
$(OBJDIR):
	@$(MKDIR) -p $(OBJDIR)

# Create directory for binary
$(BINDIR):
	@$(MKDIR) -p $(BINDIR)

# Link all objects together and build the binary
$(BINDIR)/$(BIN).exe: $(DEPFILES) $(OBJECTS)
	@echo "Linking ..."
	@$(CC) $(OBJECTS) $(LDFLAGS) $(LIBPATH) $(LIBRARIES) -o $(BINDIR)/$(BIN)

# Copy all necessary windows DLLs to the binary folder
$(BINDIR)/%.dll: $(LIBDIR)/%.dll
	@echo "Copy" $< "to ./"$(BINDIR)"/"
	@$(COPY) $< ./$(BINDIR)

# Copy all necessary shared objects
$(BINDIR)/%.so: $(LIBDIR)/%.so
	@echo "Copy" $< "to ./"$(BINDIR)"/"
	@$(COPY) $< ./$(BINDIR)/

