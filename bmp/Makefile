#
# Makefile for BMP firmware
#

BOOT_OBJ = bmp_clock.o bmp_flash.o bmp_crc.o 

MAIN_OBJ = bmp_can.o bmp_net.o bmp_cmd.o bmp_event.o bmp_hw.o \
	   bmp_io.o bmp_flash.o bmp_ssp.o bmp_crc.o bmp_eth.o bmp_i2c.o \
	   bmp_main.o

SOURCES  = bmp_init.c bmp_can.c bmp_net.c bmp_cmd.c bmp_event.c \
	   bmp_io.c bmp_flash.c bmp_ssp.c bmp_crc.c bmp_eth.c bmp_i2c.c \
	   bmp_main.c bmp_date bmp_hw.c bmp_boot.c bmp_clock.c bmp.h

BOOT_SCT = bmp_boot.sct
MAIN_SCT = bmp_main.sct

CFLAGS = -c --c99 --cpu=Cortex-M3 -I ../include

BOOT_LFLAGS = --entry=boot_proc --scatter=$(BOOT_SCT)
MAIN_LFLAGS_L = --entry=c_main --scatter=$(MAIN_SCT) --predefine="-DBASE=0x10000"
MAIN_LFLAGS_H = --entry=c_main --scatter=$(MAIN_SCT) --predefine="-DBASE=0x20000"

CC = armcc
LD = armlink
OBJCOPY = fromelf
OBJDUMP = fromelf -cds
RM = /bin/rm -f
MV = /bin/mv


all: bmp_main.elf bmp_boot.elf

bmp_boot.elf: $(BOOT_OBJ) $(BOOT_SCT) bmp_boot.c
	perl -pe "s/BUILD_DATE/time/e" bmp_boot.c > bmp_tmp.c
	$(CC) $(CFLAGS) -o bmp_boot.o bmp_tmp.c
	$(RM) bmp_tmp.c 

	$(LD) $(BOOT_LFLAGS) $(BOOT_OBJ) bmp_boot.o -o bmp_boot.elf
	$(OBJDUMP) bmp_boot.elf -o bmp_boot.txt
	$(OBJCOPY) --bin --output bin/bmp_boot bmp_boot.elf
	perl sum.pl bin/bmp_boot bin/BOOT

bmp_main.elf: $(MAIN_OBJ) $(MAIN_SCT) bmp_init.c
	perl -pe "s/BUILD_DATE/time/e" bmp_init.c > bmp_tmp.c
	$(CC) $(CFLAGS) -o bmp_init.o bmp_tmp.c
	$(RM) bmp_tmp.c 

	$(LD) $(MAIN_LFLAGS_L) $(MAIN_OBJ) bmp_init.o -o bmp_main.elf
	$(OBJDUMP) bmp_main.elf -o bmp_main.txt
	$(OBJCOPY) --bin --output bin/bmp_main bmp_main.elf
	perl sum.pl bin/bmp_main bin/PROGL

	$(LD) $(MAIN_LFLAGS_H) $(MAIN_OBJ) bmp_init.o -o bmp_main.elf
	$(OBJCOPY) --bin --output bin/bmp_main bmp_main.elf
	perl sum.pl bin/bmp_main bin/PROGH

# bmp_boot.o: bmp_boot.c bmp.h
# 	$(CC) $(CFLAGS) $<

# bmp_init.o: bmp_init.c bmp.h
#	$(CC) $(CFLAGS) $<

bmp_crc.o: bmp_crc.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_clock.o: bmp_clock.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_main.o: bmp_main.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_hw.o: bmp_hw.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_can.o: bmp_can.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_eth.o: bmp_eth.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_i2c.o: bmp_i2c.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_net.o: bmp_net.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_ssp.o: bmp_ssp.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_cmd.o: bmp_cmd.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_event.o: bmp_event.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_io.o: bmp_io.c bmp.h
	$(CC) $(CFLAGS) $<

bmp_flash.o: bmp_flash.c bmp.h
	$(CC) $(CFLAGS) $<

tar:
	tar czf bmp.tgz Makefile sum.pl bmp.h \
	$(BOOT_SCT) $(MAIN_SCT) $(SOURCES) \
	lpc17xx.h lpc17xx_clkpwr.h lpc17xx_ssp.h lpc17xx_emac.h \
	lpc17xx_pinsel.h lpc_types.h core_cm3.h core_cmInstr.h core_cmFunc.h

clean:
	$(RM) $(BOOT_OBJ) bmp_boot.o bmp_boot.elf bmp_boot.txt
	$(RM) $(MAIN_OBJ) bmp_init.o bmp_main.elf bmp_main.txt
	$(RM) bmp_tmp.c bin/bmp_boot bin/bmp_main

.PHONY: all tar clean
