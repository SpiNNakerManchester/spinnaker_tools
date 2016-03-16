
APP := scamp-3
INC_DIR := ../include
LIB_DIR := ../lib

CC := armcc -c --cpu=5te --c99 -Ospace --apcs /interwork --thumb \
   --min_array_alignment=4 -I $(INC_DIR)
AS := armasm --keep --cpu=5te --apcs /interwork
OBJCOPY := fromelf

RM := /bin/rm -f
LS := /bin/ls -l
MV := mv

OBJS := spinn_phy.o spinn_srom.o spinn_net.o scamp-app.o \
       scamp-p2p.o scamp-nn.o scamp-cmd.o scamp-boot.o scamp-isr.o \
       $(APP).o

SARKLIB := $(LIB_DIR)/sark.a


$(APP).boot: $(OBJS) $(SARKLIB) sark.aplx boot_aplx.bin $(APP).sct
	mkbuild scamp > sark_build.c
	$(CC) sark_build.c
	armlink --scatter=$(APP).sct --remove --entry cpu_reset $(OBJS) $(SARKLIB) \
	  sark_build.o --output $(APP).elf 
	perl mksv > scamp-3.sv
	perl mkpad sark.aplx 3584 > sark_pad.aplx
	fromelf $(APP).elf --bin --output $(APP).bin
	fromelf $(APP).elf -cds --output $(APP).txt

#	mkaplx -scamp $(APP).elf > scamp.tab
#	cat scamp.tab boot_aplx.bin boot_aplx.bin scamp-3.sv sark_pad.aplx scamp-3.bin > $(APP).aplx

	mkaplx -boot $(APP).elf > boot.tab
	cat boot_aplx.bin boot.tab boot_aplx.bin scamp-3.sv sark_pad.aplx scamp-3.bin > $(APP).boot

	$(RM) boot.tab scamp.tab scamp-3.bin scamp-3.sv sark_pad.aplx
	$(RM) sark_build.c sark_build.o
#	$(LS) $(APP).aplx
	$(LS) $(APP).boot

install: $(APP).boot # $(APP).aplx
	$(MV) $(APP).boot ../boot/scamp.boot
#	$(MV) $(APP).aplx ../boot/scamp.aplx

boot_aplx.bin: boot_aplx.s spinnaker.s sark.s
	$(AS) boot_aplx.s
	armlink --ro_base 0 -o boot_aplx.elf boot_aplx.o
	fromelf --bin -o boot_aplx.bin boot_aplx.elf

sark.aplx: sark.o $(SARKLIB)
	mkbuild sark > sark_build.c
	$(CC) sark_build.c
	armlink --scatter=$(LIB_DIR)/sark.sct --remove --entry cpu_reset sark.o sark_build.o \
	  $(SARKLIB) --output sark.elf
	fromelf sark.elf --bin --output sark.bin
	fromelf sark.elf -cds --output sark.txt
	mkaplx sark.elf | cat - sark.bin > sark.aplx
	$(RM) sark_build.c sark_build.o sark.bin

sark.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark.c
	$(CC) sark.c

spinnaker.s: $(INC_DIR)/spinnaker.h
	h2asm $(INC_DIR)/spinnaker.h > spinnaker.s

sark.s: $(INC_DIR)/sark.h
	h2asm $(INC_DIR)/sark.h > sark.s

spinn_phy.o: spinn_phy.c spinn_phy.h $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) spinn_phy.c

spinn_srom.o: spinn_srom.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) spinn_srom.c

spinn_net.o: spinn_net.c spinn_net.h $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) spinn_net.c

scamp-p2p.o: scamp-p2p.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) scamp-p2p.c

scamp-nn.o: scamp-nn.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) scamp-nn.c

scamp-cmd.o: scamp-cmd.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) scamp-cmd.c

scamp-isr.o: scamp-isr.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) scamp-isr.c

scamp-app.o: scamp-app.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) scamp-app.c

scamp-boot.o: scamp-boot.c $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h scamp.h
	$(CC) scamp-boot.c

$(APP).o: $(APP).c spinn_net.h spinn_phy.h $(INC_DIR)/spinnaker.h \
	$(INC_DIR)/sark.h scamp.h
	$(CC) $(APP).c

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
	$(RM) spinnaker.s sark.s sark_build.c sark_build.o


