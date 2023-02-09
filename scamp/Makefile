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

APP := scamp-3
override THUMB := 1
override SCAMP := 1
include ../make/spinnaker_tools.mk

SCAMP_OBJS = spinn_phy.o spinn_srom.o spinn_net.o scamp-app.o \
             scamp-p2p.o scamp-nn.o scamp-cmd.o scamp-boot.o scamp-isr.o scamp-del.o \
             $(APP).o
OBJS := $(SCAMP_OBJS:%.o=$(BUILD_DIR)%.o)

ifeq ($(GNU),1)
    SARKLIB := $(SPINN_LIB_DIR)/libsark.a
    CFLAGS += -Os -fdata-sections -ffunction-sections
else
    SARKLIB := $(SPINN_LIB_DIR)/sark.a
endif

# Create the boot file (default)
$(APP).boot: $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP)_boot.tab $(BUILD_DIR)$(APP).sv $(BUILD_DIR)sark_pad.aplx $(BUILD_DIR)$(APP).bin
	cat $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP)_boot.tab $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP).sv $(BUILD_DIR)sark_pad.aplx $(BUILD_DIR)$(APP).bin > $(APP).boot
	$(LS) $(APP).boot

# Create the aplx file (optional)
$(APP).aplx: $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP)_boot.tab $(BUILD_DIR)$(APP).sv $(BUILD_DIR)sark_pad.aplx $(BUILD_DIR)$(APP).bin
	cat $(BUILD_DIR)$(APP)_scamp.tab $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP).sv $(BUILD_DIR)sark_pad.aplx $(BUILD_DIR)$(APP).bin > $(APP).aplx
	$(LS) $(APP).aplx

# Install the boot file (default)
install: $(APP).boot
	$(MV) $(APP).boot ../tools/boot/scamp.boot

# Install the aplx file (optional)
install-aplx: $(APP).aplx
	$(MV) $(APP).aplx ../tools/boot/scamp.aplx

# Build the SCAMP elf file - uses scatter file on ARM
$(BUILD_DIR)$(APP).elf: $(OBJS) $(SARK_LIB) $(BUILD_DIR)$(APP)_build.o $(APP).sct
ifeq ($(GNU),1)
	$(LD) $(OBJS) $(BUILD_DIR)$(APP)_build.o $(SARKLIB) -o $@
else
	armlink --scatter=$(APP).sct --remove --entry cpu_reset $(OBJS) $(SARKLIB) \
	  $(BUILD_DIR)$(APP)_build.o --output $@
endif

# Create an aplx header table for booting
%_boot.tab: %.nm
	$(SPINN_TOOLS_DIR)/mkaplx -boot $< > $@

# Create an aplx header table for aplx
%_scamp.tab: %.nm
	$(SPINN_TOOLS_DIR)/mkaplx -scamp $< > $@

# Create the sv structure from perl
%.sv:
	perl mksv > $@

# Create the padded sark APLX
$(BUILD_DIR)sark_pad.aplx: $(BUILD_DIR)sark.aplx
	perl mkpad $< 3584 > $@

# Create the SARK elf file (uses own objects)
$(BUILD_DIR)$(BUILD_DIR)sark.elf: $(BUILD_DIR)sark.o $(BUILD_DIR)sark_build.o $(SARKLIB)
	$(MKDIR) $(BUILD_DIR)$(BUILD_DIR)
	$(LD) $(BUILD_DIR)sark.o $(BUILD_DIR)sark_build.o $(SARKLIB) --output $@

# Create the boot_aplx elf file (uses a different linker script)
ifeq ($(GNU),1)
$(BUILD_DIR)%.gas: $(BUILD_DIR)%.s
	$(MKDIR) $(BUILD_DIR)
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

$(BUILD_DIR)%.gas: %.s
	$(MKDIR) $(BUILD_DIR)
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

$(BUILD_DIR)boot_aplx.elf: $(BUILD_DIR)boot_aplx.gas $(BUILD_DIR)spinnaker.gas $(BUILD_DIR)sark.gas
	$(MKDIR) $(BUILD_DIR)
	$(AS) -I $(BUILD_DIR) -o $(BUILD_DIR)boot_aplx.o $<
	$(GP)-ld -Tboot.lnk -static --no-gc-sections --use-blx $(BUILD_DIR)boot_aplx.o -o $@

else
$(BUILD_DIR)boot_aplx.elf: boot_aplx.s $(BUILD_DIR)spinnaker.s $(BUILD_DIR)sark.s
	$(MKDIR) $(BUILD_DIR)
	$(AS) -I $(BUILD_DIR) boot_aplx.s -o $(BUILD_DIR)boot_aplx.o
	armlink --ro_base 0 -o $@ $(BUILD_DIR)boot_aplx.o
endif

# Convert h file to s
$(BUILD_DIR)%.s: $(SPINN_INC_DIR)/%.h
	$(MKDIR) $(BUILD_DIR)
	$(MKDIR) $(BUILD_DIR)
	h2asm $< > $@

# Compile the interrupt service routines without THUMB
$(BUILD_DIR)scamp-isr.o: scamp-isr.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h scamp.h
	$(CC_NO_THUMB) $(CFLAGS) scamp-isr.c -o $@

# Compile the delegation boot-image copy and execute routine without THUMB
$(BUILD_DIR)scamp-del.o: scamp-del.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h scamp.h
	$(CC_NO_THUMB) $(CFLAGS) scamp-del.c -o $@

tar:
	tar -C .. -czf /tmp/scamp.tgz scamp/spinn_phy.c \
	scamp/spinn_srom.c scamp/spinn_net.c scamp/scamp-app.c \
	scamp/scamp-p2p.c scamp/scamp-nn.c scamp/scamp-cmd.c \
	scamp/scamp-boot.c scamp/scamp-isr.c scamp/scamp-del.c scamp/spinn_phy.h \
	scamp/scamp.h scamp/spinn_net.h scamp/spinn_regs.h \
	scamp/boot_aplx.s scamp/$(APP).c scamp/$(APP).sct scamp/sark.c \
	scamp/Makefile scamp/mkpad scamp/mksv scamp/$(APP).lnk scamp/boot.lnk

clean:
	$(RM) -r $(BUILD_DIR)* $(APP).boot

.PHONY: tar clean install install-aplx
