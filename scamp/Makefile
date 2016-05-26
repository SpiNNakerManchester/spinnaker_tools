BUILD_DIR := build/
APP := scamp-3
THUMB := 1
GNU ?= 0
include ../make/Makefile.common

OBJECTS = spinn_phy.o spinn_srom.o spinn_net.o scamp-app.o \
       scamp-p2p.o scamp-nn.o scamp-cmd.o scamp-boot.o scamp-isr.o \
       $(APP).o
OBJS := $(OBJECTS:%.o=$(BUILD_DIR)%.o)

ifeq ($(GNU),1)
    SARKLIB := $(SPINN_LIB_DIR)/libsark.a
    CFLAGS += -Os -fdata-sections -ffunction-sections
else
    SARKLIB := $(SPINN_LIB_DIR)/sark.a
endif


$(APP).boot: $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP)_boot.tab $(BUILD_DIR)$(APP).sv sark_pad.aplx $(BUILD_DIR)$(APP).bin
#	$(SPINN_TOOLS_DIR)/mkaplx -scamp $(BUILD_DIR)$(APP).nm > $(BUILD_DIR)scamp.tab
#	cat $(BUILD_DIR)scamp.tab $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)scamp-3.sv $(BUILD_DIR)sark_pad.aplx $(BUILD_DIR)scamp-3.bin > $(APP).aplx
	
	cat $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP)_boot.tab $(BUILD_DIR)boot_aplx.bin $(BUILD_DIR)$(APP).sv sark_pad.aplx $(BUILD_DIR)$(APP).bin > $(APP).boot
#	$(LS) $(APP).aplx
	$(LS) $(APP).boot
	
$(BUILD_DIR)$(APP).elf: $(OBJS) $(SARK_LIB) $(BUILD_DIR)$(APP)_build.o $(APP).sct
ifeq ($(GNU),1)
	$(LD) $(OBJS) $(BUILD_DIR)_build.o $(SARKLIB) -o $(APP).elf
else
	armlink --scatter=$(APP).sct --remove --entry cpu_reset $(OBJS) $(SARKLIB) \
	  $(BUILD_DIR)$(APP)_build.o --output $(BUILD_DIR)$(APP).elf 
endif

%_boot.tab: %.nm
	$(SPINN_TOOLS_DIR)/mkaplx -boot $< > $@

%.sv:
	perl mksv > $@

sark_pad.aplx: sark.aplx
	perl mkpad $< 3584 > $@

install: $(APP).boot # $(APP).aplx
	$(MV) $(APP).boot ../tools/boot/scamp.boot
#	$(MV) $(APP).aplx ../tools/boot/scamp.aplx

$(BUILD_DIR)sark.elf: $(BUILD_DIR)sark.o $(BUILD_DIR)sark_build.o $(SARKLIB)
	 $(LD) $(BUILD_DIR)sark.o $(BUILD_DIR)sark_build.o $(SARKLIB) --output $@
	
ifeq ($(GNU),1)
$(BUILD_DIR)%.gas: $(BUILD_DIR)%.s
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

$(BUILD_DIR)%.gas: %.s
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

$(BUILD_DIR)boot_aplx.elf: $(BUILD_DIR)boot_aplx.gas $(BUILD_DIR)spinnaker.gas $(BUILD_DIR)sark.gas
	$(AS) -I $(BUILD_DIR) -o $@ $<
	$(GP)-ld -Tboot.lnk -static --no-gc-sections --use-blx -nostartfiles boot_aplx.o -o boot_aplx.elf
		
else
$(BUILD_DIR)boot_aplx.elf: boot_aplx.s $(BUILD_DIR)spinnaker.s $(BUILD_DIR)sark.s
	$(AS) -I $(BUILD_DIR) boot_aplx.s -o $(BUILD_DIR)boot_aplx.o
	armlink --ro_base 0 -o $@ $(BUILD_DIR)boot_aplx.o
endif

$(BUILD_DIR)%.s: $(SPINN_INC_DIR)/%.h
	h2asm $< > $@

scamp-isr.o: scamp-isr.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h scamp.h
	$(CC_NO_THUMB) $(CFLAGS) scamp-isr.c

tar:
	tar -C .. -czf /tmp/scamp.tgz scamp/spinn_phy.c \
	scamp/spinn_srom.c scamp/spinn_net.c scamp/scamp-app.c \
	scamp/scamp-p2p.c scamp/scamp-nn.c scamp/scamp-cmd.c \
	scamp/scamp-boot.c scamp/scamp-isr.c scamp/spinn_phy.h \
	scamp/scamp.h scamp/spinn_net.h scamp/spinn_srom.h scamp/spinn_regs.h \
	scamp/boot_aplx.s scamp/$(APP).c scamp/$(APP).sct scamp/sark.c \
	scamp/Makefile scamp/mkpad scamp/mksv

clean:
	$(RM) $(BUILD_DIR)* sark_pad.aplx
