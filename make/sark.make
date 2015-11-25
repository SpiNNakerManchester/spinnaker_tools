
# Makefile for SARK

GNU := 0
API := 0
THUMB := 1
LIB := 1

INC_DIR := ../include
LIB_DIR := ../lib

#-------------------------------------------------------------------------------

ifeq ($(GNU),1)
  GP := arm-none-eabi

  AS := $(GP)-as --defsym GNU=1 -mthumb-interwork -march=armv5te

  CA := $(GP)-gcc -c -Os -mthumb-interwork -march=armv5te -std=gnu99 \
	-I $(INC_DIR)

  CT := $(CA) -mthumb -DTHUMB

ifeq ($(LIB),1)
  CFLAGS += -fdata-sections -ffunction-sections
endif

  SCRIPT := $(LIB_DIR)/sark.lnk

  LD := $(GP)-ld -i

  AR := $(GP)-ar -rcs
  OC := $(GP)-objcopy
  OD := $(GP)-objdump -dxt > $(APP).txt

  LIBNAME := libsark

else
  AS := armasm --keep --cpu=5te --apcs /interwork

  CA := armcc -c --c99 --cpu=5te -Ospace --apcs /interwork \
	--min_array_alignment=4 -I $(INC_DIR)

  CT := $(CA) --thumb -DTHUMB

ifeq ($(LIB),1)
  CFLAGS += --split_sections
endif

  SCRIPT := $(LIB_DIR)/sark.sct

  LD := armlink --partial

  AR := armar -rsc
  OC := fromelf
  OD := fromelf -cds --output $(APP).txt

  LIBNAME := sark

endif

SARKLIB := $(LIBNAME).a
SARKINT := $(LIBNAME).o

ifeq ($(API),1)
  CFLAGS += -DSARK_API
endif

ifeq ($(THUMB),1)
  CC := $(CT)
else
  CC := $(CA)
endif

RM := \rm -f

#-------------------------------------------------------------------------------

# SARK objects

SARKOBJ := sark_alib.o sark_base.o sark_event.o sark_timer.o \
	sark_hw.o sark_isr.o sark_alloc.o sark_io.o sark_pkt.o

#-------------------------------------------------------------------------------

# SARK library (default target)

$(SARKLIB): $(SARKOBJ)
	$(RM) $(SARKLIB)
	$(LD) -o $(SARKINT) $(SARKOBJ)
	$(AR) $(SARKLIB) $(SARKOBJ)

#-------------------------------------------------------------------------------

install: $(SARKLIB)
	\cp -p $(SARKLIB) ../lib/$(LIBNAME)-$(SPINN_STV).a
	\ln -sf $(LIBNAME)-$(SPINN_STV).a ../lib/$(SARKLIB)

#-------------------------------------------------------------------------------

# SARK assembler sources

ifeq ($(GNU),1)

spinnaker.gas: $(INC_DIR)/spinnaker.h
	h2asm $(INC_DIR)/spinnaker.h | arm2gas > spinnaker.gas

sark.gas: $(INC_DIR)/sark.h
	h2asm $(INC_DIR)/sark.h | arm2gas > sark.gas

sark_alib.gas: sark_alib.s
	arm2gas sark_alib.s > sark_alib.gas

sark_alib.o: sark_alib.gas spinnaker.gas sark.gas
	$(AS) -o sark_alib.o sark_alib.gas

else

spinnaker.s: $(INC_DIR)/spinnaker.h
	h2asm $(INC_DIR)/spinnaker.h > spinnaker.s

sark.s: $(INC_DIR)/sark.h
	h2asm $(INC_DIR)/sark.h > sark.s

sark_alib.o: sark_alib.s spinnaker.s sark.s
	$(AS) sark_alib.s

endif

#-------------------------------------------------------------------------------

# SARK C sources

sark_base.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h $(INC_DIR)/spin1_api.h \
	sark_base.c 
	$(CC) $(CFLAGS) sark_base.c

sark_io.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_io.c
	$(CC) $(CFLAGS) sark_io.c

sark_event.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_event.c
	$(CC) $(CFLAGS) sark_event.c

sark_timer.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_timer.c
	$(CC) $(CFLAGS) sark_timer.c

sark_alloc.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_alloc.c
	$(CC) $(CFLAGS) sark_alloc.c

sark_hw.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_hw.c
	$(CC) $(CFLAGS) sark_hw.c

sark_pkt.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_pkt.c
	$(CC) $(CFLAGS) sark_pkt.c

sark_isr.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h sark_isr.c
	$(CA) $(CFLAGS) sark_isr.c

#-------------------------------------------------------------------------------

tidy:
	$(RM) $(SARKOBJ) *.gas spinnaker.s sark.s

clean:
	$(RM) $(SARKLIB) $(SARKOBJ) $(SARKINT) *.gas spinnaker.s sark.s

