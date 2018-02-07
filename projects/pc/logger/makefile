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
# Makefile for VSCP PC logger
# Author: Andreas Merkle, http://www.blue-andi.de
#
################################################################################

################################################################################
# Configuration
################################################################################

# Software version
VERSION= 0.1.0

# Binary file name
BIN= vscpLoggerPc

# General include directories
INCLUDES= -I. \
		-I../../../common/pc \
		-I../../../common/pc/vscphelper

# Sources
SOURCES= main.c \
		../../../common/pc/cmdLineParser.c \
		../../../common/pc/log.c \
		../../../common/pc/platform.c

# Include all source pathes here like ../src1:../src2
VPATH= ../../../common/pc
		
# Library search path (-L...)
LIBPATH= -L../../../common/pc/vscphelper/lib/linux

# Libraries (-l...)
LIBRARIES= -lpthread -lvscphelper

# Windows specific part
ifeq ($(OS),Windows_NT)

LIBPATH= -L../../../common/pc/vscphelper/lib/win32

LIBRARIES+= -lws2_32

endif

# Preprocessor defines
PREPROC= -D_REENTRANT -DVERSION=\"$(VERSION)\"

# General compiler flags independent of debug or release build
# -Wall                : Enable all warnings
# -ffunction-sections -fdata-sections : Remove unused functions
# -fshort-enums        : Allocate to an enum type only as many bytes as it needs for the declared range of possible values.
# -std=gnu99           : Define the C standard
# -funsigned-bitfields : Bitfields always unsigned
CFLAGS= -Wall -ffunction-sections -fdata-sections -fshort-enums -std=gnu99 -funsigned-bitfields

# General linker flags independent of debug or release build
# -Wall             : Enable all warnings
# -Wl,-Map,foo.map  : Create map file
# -Wl,--cref        : Add cross reference to map file
# -Wl,--gc-sections : Enable garbage collection of unused input sections
# -Wl,-rpath='$$ORIGIN/../lib/linux' : Set shared object search path
LDFLAGS= -Wall -Wl,-Map,$(BINDIR)/$(BIN).map -Wl,--cref -Wl,--gc-sections -Wl,-rpath='$$ORIGIN/../../../../common/pc/vscphelper/lib/linux'

ifeq ($(MAKECMDGOALS),debug)

# Additional C compiler flags for debug build
# -g1             : Minimal debug information
# -g              : Default debug information
# -g3             : Maximum debug information
# -O1             : Optimization level
# -ftest-coverage : Generate additional coverage information
# -fprofile-arcs  : Generate profile output
CFLAGS+= -g -O1

# Additional linker flags for debug build
# -ftest-coverage : Enable coverage
# -fprofile-arcs  : Enable profiling
LDFLAGS+=

endif

ifeq ($(MAKECMDGOALS),release)

# Additional C compiler flags for release build
# -Os             : Optimization level
# -ftest-coverage : Generate additional coverage information
# -fprofile-arcs  : Generate profile output
CFLAGS+= -Os

# Additional linker flags for release build
# -ftest-coverage : Enable coverage
# -fprofile-arcs  : Enable profiling
LDFLAGS+=

endif

# Object directory
OBJDIR= obj

# Binary directory
BINDIR= bin

# Library directory
LIBDIR= ../../../common/pc/vscphelper/lib/linux

ifeq ($(OS),Windows_NT)

LIBDIR= ../../../common/pc/vscphelper/lib/win32

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
	@$(LD) $(OBJECTS) $(LDFLAGS) $(LIBPATH) $(LIBRARIES) -o $(BINDIR)/$(BIN)

# Copy all necessary windows DLLs to the binary folder
$(BINDIR)/%.dll: $(LIBDIR)/%.dll
	@echo "Copy" $< "to ./"$(BINDIR)"/"
	@$(COPY) $< ./$(BINDIR)

# Copy all necessary shared objects
$(BINDIR)/%.so: $(LIBDIR)/%.so
	@echo "Copy" $< "to ./"$(BINDIR)"/"
	@$(COPY) $< ./$(BINDIR)/

