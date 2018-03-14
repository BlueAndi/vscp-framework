# The MIT License (MIT)
# 
# Copyright (c) 2014 - 2018, Andreas Merkle
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
################################################################################

################################################################################
# Configuration
################################################################################

# Software version
VERSION= 0.1.0

# Binary file name
BIN= vscp_test

# C Unit Testing Framework version
CUNITVERSION= 2.1-3

# Includes
INCLUDES= -I. \
		-I.. \
		-I../events \
		-ICUnit-$(CUNITVERSION)/CUnit/Headers \
		-IvscpUser

# Sources
SOURCES= main.c \
		vscp_test.c \
		vscpUser/vscp_action.c \
		vscpUser/vscp_app_reg.c \
		vscpUser/vscp_portable.c \
		vscpUser/vscp_ps_access.c \
		vscpUser/vscp_tp_adapter.c \
		vscpUser/vscp_timer.c \
		../vscp_core.c \
		../vscp_dev_data.c \
		../vscp_dm.c \
		../vscp_dm_ng.c \
		../vscp_logger.c \
		../vscp_ps.c \
		../vscp_transport.c \
		../vscp_util.c \
		../events/vscp_log.c

# Include all source pathes here like ../src1:../src2
VPATH= ..:vscpUser:../events

# Library search path
LIBPATH= -LCUnit-$(CUNITVERSION)/CUnit/Sources/.libs

# Libraries
LIBRARIES= -lcunit

# Preprocessor defines
PREPROC= -DVERSION=\"$(VERSION)\" \
		-DCUNIT_TESTS

# General compiler flags independent of debug or release build
# All warnings: -Wall
CFLAGS= -Wall

# General linker flags independent of debug or release build
# All warnings: -Wall
# Create map file: -Wl,-Map,foo.map
LDFLAGS= -Wall -Wl,-Map,$(BINDIR)/$(BIN).map

ifeq ($(MAKECMDGOALS),debug)

# Additional compiler flags for debug build
# Minimal: -g1
# Default: -g
# Maximum: -g3
# Optimization: -O1
# Generate additional coverage information: -ftest-coverage
# Generate profile output: -fprofile-arcs
CFLAGS+= -g -O1 -ftest-coverage -fprofile-arcs

# Additional linker flags
# Enable coverage: -ftest-coverage
# Enable profiling: -fprofile-arcs
LDFLAGS+= -ftest-coverage -fprofile-arcs

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

# Remove path from .c files, rename to .o files and add object directory
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:%.c=%.o)))

# All dependency files
DEPFILES= $(OBJECTS:%.o=%.d)

# C Unit Testing Framework directory
CUNITDIR= CUnit-$(CUNITVERSION)

# C Unit Testing Framework library
CUNITLIB= $(CUNITDIR)/CUnit/Sources/.libs/libcunit.a

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
	@echo "cunit   - Build cunit testing framework"
	@echo ""
	
debug: $(BINDIR)/$(BIN).exe
	@echo "Finished."

release: $(BINDIR)/$(BIN).exe
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

cunit: $(CUNITLIB)
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

$(CUNITLIB):
	@cp makefile_cunit $(CUNITDIR)
	@$(MAKE) -f makefile_cunit -C $(CUNITDIR) all

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
