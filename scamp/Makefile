BUILD_DIR := ../build/
APP := scamp-3
THUMB := 1
include ../make/Makefile.common

OBJS := spinn_phy.o spinn_srom.o spinn_net.o scamp-app.o \
       scamp-p2p.o scamp-nn.o scamp-cmd.o scamp-boot.o scamp-isr.o \
       $(APP).o

ifeq ($(GNU),1)
    SARKLIB := $(SPINN_LIB_DIR)/libsark.a
    CFLAGS += -Os -fdata-sections -ffunction-sections
else
    SARKLIB := $(SPINN_LIB_DIR)/sark.a
endif


$(APP).boot: $(OBJS) $(SARKLIB) sark.aplx boot_aplx.bin $(APP).sct
	$(SPINN_TOOLS_DIR)/mkbuild scamp > sark_build.c
	$(CC) sark_build.c
	perl mksv > scamp-3.sv
	perl mkpad sark.aplx 3584 > sark_pad.aplx
ifeq ($(GNU),1)
	$(LD) $(OBJS) sark_build.o $(SARKLIB) -o $(APP).elf
	$(OC) -O binary -j RO_DATA $(APP).elf RO_DATA.bin
	$(OC) -O binary -j RW_DATA $(APP).elf RW_DATA.bin
	$(SPINN_TOOLS_DIR)/mkbin RO_DATA.bin RW_DATA.bin > $(APP).bin
	$(RM) RO_DATA.bin RW_DATA.bin
	$(OD) $(APP).elf > $(APP).txt
else
	armlink --scatter=$(APP).sct --remove --entry cpu_reset $(OBJS) $(SARKLIB) \
	  sark_build.o --output $(APP).elf 
	fromelf $(APP).elf --bin --output $(APP).bin
	fromelf $(APP).elf -cds --output $(APP).txt
endif

#	mkaplx -scamp $(APP).elf > scamp.tab
#	cat scamp.tab boot_aplx.bin boot_aplx.bin scamp-3.sv sark_pad.aplx scamp-3.bin > $(APP).aplx

	$(SPINN_TOOLS_DIR)/mkaplx -nm $(NM) -boot $(APP).elf > boot.tab
	cat boot_aplx.bin boot.tab boot_aplx.bin scamp-3.sv sark_pad.aplx scamp-3.bin > $(APP).boot

	$(RM) boot.tab scamp.tab scamp-3.bin scamp-3.sv sark_pad.aplx
	$(RM) sark_build.c sark_build.o
#	$(LS) $(APP).aplx
	$(LS) $(APP).boot

install: $(APP).boot # $(APP).aplx
	$(MV) $(APP).boot ../boot/scamp.boot
#	$(MV) $(APP).aplx ../boot/scamp.aplx

boot_aplx.bin: boot_aplx.elf
ifeq ($(GNU),1)
	$(OC) -O binary -j RO_DATA boot_aplx.elf RO_DATA.bin
	$(OC) -O binary -j RW_DATA boot_aplx.elf RW_DATA.bin
	$(SPINN_TOOLS_DIR)/mkbin RO_DATA.bin RW_DATA.bin > boot_aplx.bin
	$(RM) RO_DATA.bin RW_DATA.bin
else
	$(OC) --bin -o boot_aplx.bin boot_aplx.elf
endif

sark.aplx: sark.o $(SARKLIB)
	mkbuild sark > sark_build.c
	$(CC) sark_build.c
	$(LD) sark.o sark_build.o $(SARKLIB) --output sark.elf
ifeq ($(GNU),1)
	$(OC) -O binary -j RO_DATA sark.elf RO_DATA.bin
	$(OC) -O binary -j RW_DATA sark.elf RW_DATA.bin
	$(SPINN_TOOLS_DIR)/mkbin RO_DATA.bin RW_DATA.bin > sark.bin
	$(RM) RO_DATA.bin RW_DATA.bin
	$(OD) > sark.txt sark.elf
else
	$(OC) --bin --output sark.bin sark.elf
	$(OD) sark.txt sark.elf
endif
	$(SPINN_TOOLS_DIR)/mkaplx -nm $(NM) sark.elf | cat - sark.bin > sark.aplx
	$(RM) sark_build.c sark_build.o sark.bin
	
ifeq ($(GNU),1)
%.gas: %.s
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

boot_aplx.elf: boot_aplx.gas spinnaker.gas sark.gas
	$(AS) -I $(BUILD_DIR) -o boot_aplx.o $<
	$(GP)-ld -Tboot.lnk -static --no-gc-sections --use-blx -nostartfiles boot_aplx.o -o boot_aplx.elf
		
else
boot_aplx.elf: boot_aplx.s spinnaker.s sark.s
	$(AS) boot_aplx.s
	armlink --ro_base 0 -o boot_aplx.elf boot_aplx.o
endif

spinnaker.s: $(SPINN_INC_DIR)/spinnaker.h
	h2asm $(SPINN_INC_DIR)/spinnaker.h > spinnaker.s

sark.s: $(SPINN_INC_DIR)/sark.h
	h2asm $(SPINN_INC_DIR)/sark.h > sark.s

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
	$(RM) $(OBJS) boot_aplx.o boot_aplx.bin boot_aplx.elf 
	$(RM) $(APP).txt $(APP).aplx $(APP).elf $(APP).bin $(APP).boot $(APP).tab
	$(RM) spinnaker.s sark.s sark_build.c sark_build.o sark.o sark.txt sark.elf
	$(RM) boot_aplx.gas spinnaker.gas sark.gas sark.aplx sark-pad.aplx


