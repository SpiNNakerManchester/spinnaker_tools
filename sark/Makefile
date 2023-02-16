# Copyright (c) 2015 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

.PHONY: clean
