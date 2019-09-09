# Copyright (c) 2017-2019 The University of Manchester
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Makefile for SARK
ifeq ($(GNU),1)
    BUILD_DIR := ../build/gnu
else
    BUILD_DIR := ../build/arm
endif
override LIB := 1
include ../make/spinnaker_tools.mk

SARKOPT := $(OSPACE)
CFLAGS += -DSARK_API $(SARKOPT)
INSTALL ?= install

#-------------------------------------------------------------------------------
ifeq ($(GNU),1)
    LIBNAME := libsark
    AS_END := gas
else
    LIBNAME := sark
    AS_END := s
endif

SARKLIB := $(SPINN_LIB_DIR)/$(LIBNAME).a


#-------------------------------------------------------------------------------

# SARK objects

SARKOBJS := sark_alib.o sark_base.o sark_event.o sark_timer.o \
	sark_hw.o sark_isr.o sark_alloc.o sark_io.o sark_pkt.o sark_div0.o
SARK_ASM_OBJS := sark_alib.end spinnaker.end sark.end
SARKOBJ = $(SARKOBJS:%.o=$(BUILD_DIR)/%.o)
SARK_ASM_OBJ = $(SARK_ASM_OBJS:%.end=$(BUILD_DIR)/%.$(AS_END))

#-------------------------------------------------------------------------------

# SARK library (default target)
$(SARKLIB): $(SARKOBJ)
	$(RM) $@
	$(AR) $@ $^

%/sark.o: $(SARKOBJ)
	$(RM) $@
	$(LD) -o $@ $^

#-------------------------------------------------------------------------------

# SARK assembler sources

ifeq ($(GNU),1)

$(BUILD_DIR)/%.gas:	$(SPINN_INC_DIR)/%.h
	$(MKDIR) $(BUILD_DIR)
	$(SPINN_TOOLS_DIR)/h2asm $< | $(SPINN_TOOLS_DIR)/arm2gas > $@

$(BUILD_DIR)/%.gas: %.s
	$(MKDIR) $(BUILD_DIR)
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

$(BUILD_DIR)/sark_alib.o: $(BUILD_DIR)/sark_alib.gas $(BUILD_DIR)/spinnaker.gas $(BUILD_DIR)/sark.gas
	$(MKDIR) $(BUILD_DIR)
	$(AS) -I $(BUILD_DIR) -o $@ $<

else

$(BUILD_DIR)/%.s: $(SPINN_INC_DIR)/%.h
	$(MKDIR) $(BUILD_DIR)
	$(SPINN_TOOLS_DIR)/h2asm $< > $@

$(BUILD_DIR)/%.s: %.s
	$(MKDIR) $(BUILD_DIR)
	$(CP) $< $@

$(BUILD_DIR)/sark_alib.o: $(BUILD_DIR)/sark_alib.s $(BUILD_DIR)/spinnaker.s $(BUILD_DIR)/sark.s
	$(MKDIR) $(BUILD_DIR)
	$(AS) -o $@ $<

endif

#-------------------------------------------------------------------------------

# SARK C sources

$(BUILD_DIR)/%.o: %.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h $(SPINN_INC_DIR)/spin1_api.h
	$(CC_THUMB) $(CFLAGS) -o $@ $<
	
$(BUILD_DIR)/sark_isr.o: sark_isr.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h $(SPINN_INC_DIR)/spin1_api.h
	$(CC) $(CFLAGS) -o $@ $<
	
$(BUILD_DIR)/sark_div0.o: sark_div0.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h
	$(CC) $(CFLAGS) -o $@ $<

#-------------------------------------------------------------------------------

clean:
	$(RM) $(SARKLIB) $(SARKOBJ) $(SARK_ASM_OBJ)

install: $(SARKLIB)
	$(MKDIR) $(SPINN_DIRS)/libs
	$(INSTALL) -c -m644 $< $(SPINN_DIRS)/libs

.PHONY: clean
