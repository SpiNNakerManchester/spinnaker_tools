# Copyright (c) 2014 The University of Manchester
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

# Common includes for making SpiNNaker binaries

# Set to 1 for GNU tools, 0 for ARM
GNU := 1

# Set to 1 if using SARK/API (0 for SARK)
API := 1

# Set to 1 to make Thumb code (0 for ARM)
THUMB := 0

# Set to 1 to include debug info in ELF file
DEBUG := 1

# Set to 1 if making a library (advanced!)
LIB := 0

# Set to 1 if building scamp (advanced!)
SCAMP := 0

# Prefix for GNU tool binaries
GP := arm-none-eabi

# If SPINN_DIRS is not defined, this is an error!
ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

ifndef APP_OUTPUT_DIR
    APP_OUTPUT_DIR := ./
endif

ifndef BUILD_DIR
    ifeq ($(GNU),1)
        BUILD_DIR := build/gnu/
    else
        BUILD_DIR := build/arm/
    endif
endif

ifeq ($(DEBUG),1)
.SECONDARY: $(BUILD_DIR)$(APP).elf
endif

SPINN_LIB_DIR = $(SPINN_DIRS)/lib
SPINN_INC_DIR = $(SPINN_DIRS)/include
SPINN_TOOLS_DIR = $(SPINN_DIRS)/tools
SPINN_MAKE_LIB_DIR = $(SPINN_DIRS)/make
SPINN_SCAMP_DIR = $(SPINN_DIRS)/scamp

# ------------------------------------------------------------------------------
# Tools

ifeq ($(GNU),1)

    # GNU Compiler (gcc) settings
    AS := $(GP)-as --defsym GNU=1 -mthumb-interwork -march=armv5te
    CC_NO_THUMB  := $(GP)-gcc -c -mthumb-interwork -march=armv5te -std=gnu99 -ffreestanding -I $(SPINN_INC_DIR)
    CXX_NO_THUMB := $(GP)-gcc -c -mthumb-interwork -march=armv5te -std=c++11 -ffreestanding -I $(SPINN_INC_DIR) -fno-rtti -fno-exceptions
    CC_THUMB  := $(CC_NO_THUMB) -mthumb -DTHUMB
    CXX_THUMB := $(CXX_NO_THUMB) -mthumb -DTHUMB

    OSPACE := -Os
    OTIME := -Ofast
    ALL_WARNINGS := -Wall -Wextra

    # scamp needs the stacks in a different address of DTCM
    ifeq ($(SCAMP), 1)
        LD_LNK := $(SPINN_SCAMP_DIR)/scamp-3.lnk
    else
        LD_LNK := $(SPINN_TOOLS_DIR)/sark.lnk
    endif

    ifeq ($(LIB), 1)
        CFLAGS += -fdata-sections -ffunction-sections
        LD := $(GP)-ld -i
    else
        LD := $(GP)-gcc -T$(LD_LNK) -Wl,-e,cpu_reset -Wl,-static -ffreestanding -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,--use-blx -nostartfiles -static
        LFLAGS += -L $(SPINN_LIB_DIR)
    endif

    AR := $(GP)-ar -rcs
    OC := $(GP)-objcopy
    OD := $(GP)-objdump -dxt >
    NM := $(GP)-nm
    SPINN_LIBS += $(SPINN_LIB_DIR)/libspin1_api.a

else
    # ARM Compiler settings
    AS := armasm --keep --cpu=5te --apcs interwork
    CC_NO_THUMB := armcc -c --c99 --cpu=5te --apcs interwork --min_array_alignment=4 -I $(SPINN_INC_DIR)
    CXX_NO_THUMB := armcc -c --cpp11 --cpu=5te --apcs interwork --min_array_alignment=4 -I $(SPINN_INC_DIR) --no_rtti --no_exceptions
    CC_THUMB := $(CC_NO_THUMB) --thumb -DTHUMB
    CXX_THUMB := $(CXX_NO_THUMB) --thumb -DTHUMB

    OSPACE := -Ospace
    OTIME := -Otime
    ALL_WARNINGS :=

    # suppress linker warning about empty RW sections when building scamp
    ifeq ($(SCAMP), 1)
        LD_FLAG := --diag_suppress L6329W
    endif

    ifeq ($(LIB), 1)
        CFLAGS += --split_sections
        LD := armlink --partial
    else
        LD = armlink --scatter=$(SPINN_TOOLS_DIR)/sark.sct $(LD_FLAG) --remove --entry cpu_reset
    endif

    AR := armar -rsc
    OC := fromelf
    OD := fromelf -cds --output
    NM := nm
    SPINN_LIBS += $(SPINN_LIB_DIR)/spin1_api.a
endif

RM := rm -f
RMDIR := rmdir
CAT := cat
LS := ls -l
MKDIR := mkdir -p
CP := cp
MV := mv

ifeq ($(THUMB),1)
  CC := $(CC_THUMB)
  CXX := $(CXX_THUMB)
else
  CC := $(CC_NO_THUMB)
  CXX := $(CXX_NO_THUMB)
endif

ifeq ($(DEBUG),1)
  CFLAGS += -g
  AFLAGS += -g
endif


# Primary target is an APLX file - built from the ELF
#  1) Make an APLX header from the ELF file with "mkaplx" and concatenate
#     that with the binary to make the APLX file
#  2) "ls" the APLX file
$(APP_OUTPUT_DIR)%.aplx: $(BUILD_DIR)%.bin $(BUILD_DIR)%.nm
	$(MKDIR) $(APP_OUTPUT_DIR)
	$(SPINN_TOOLS_DIR)/mkaplx $(BUILD_DIR)$*.nm | $(CAT) - $(BUILD_DIR)$*.bin > $@

# Create a list of the objects in the file using nm
$(BUILD_DIR)%.nm: $(BUILD_DIR)%.elf
	$(NM) $< > $@
	
# Create a binary file which is the concatenation of RO and RW sections
$(BUILD_DIR)%.bin: $(BUILD_DIR)%.elf
ifeq ($(GNU),1)
	$(OC) -O binary -j RO_DATA -j EX_DATA $< $(BUILD_DIR)RO_DATA.bin
	$(OC) -O binary -j RW_DATA $< $(BUILD_DIR)RW_DATA.bin
	ls -l $(BUILD_DIR)RO_DATA.bin
	$(SPINN_TOOLS_DIR)/mkbin $(BUILD_DIR)RO_DATA.bin $(BUILD_DIR)RW_DATA.bin > $@
	$(RM) $(BUILD_DIR)RO_DATA.bin $(BUILD_DIR)RW_DATA.bin
else
	$(OC) --bin --output $@ $<
endif

# Build the ELF file
#  1) Link application object(s), build file and library to make the ELF
#  2) Create a list file
$(BUILD_DIR)%.elf: $(OBJECTS) $(BUILD_DIR)%_build.o
	# spinnaker_tools.mk elf
	$(LD) $(LFLAGS) $(OBJECTS) $(BUILD_DIR)$*_build.o $(LIBRARIES) $(SPINN_LIBS) -o $@
	$(OD) $(BUILD_DIR)$*.txt $@

# Build sark_build.o
$(BUILD_DIR)%_build.c:
	$(MKDIR) $(BUILD_DIR)
	$(SPINN_TOOLS_DIR)/mkbuild $* > $@

$(BUILD_DIR)%.o: %.c
	# spinnaker_tools.mk c
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)%.o: %.cpp
	# spinnaker_tools.mk cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -o $@ $<
