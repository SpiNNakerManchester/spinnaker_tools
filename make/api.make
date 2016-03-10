
# Makefile for spin1_api

GNU := 0
API := 0
THUMB := 1
LIB := 1
DEBUG := 1

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

  LD := $(GP)-gcc -T$(LSCRIPT) -Wl,-e.cpu_reset -Wl,--gc-sections -Wl,--use-blx

  AR := $(GP)-ar -rcs
  OC := $(GP)-objcopy
  OD := $(GP)-objdump -dxt > $(APP).txt

else
  AS := armasm --keep --cpu=5te --apcs /interwork

  CA := armcc -c --c99 --cpu=5te --apcs /interwork  --min_array_alignment=4 \
	-I $(INC_DIR)

  CT := $(CA) --thumb -DTHUMB

ifeq ($(LIB),1)
  CFLAGS += --split_sections
endif

  LSCRIPT := $(LIB_DIR)/sark.sct

  LD := armlink --scatter=$(LSCRIPT) --remove --entry cpu_reset

  AR := armar -rsc
  OC := fromelf
  OD := fromelf -cds --output $(APP).txt

endif

ifeq ($(DEBUG),1)
  CFLAGS += -g
  AFLAGS += -g
endif

ifeq ($(THUMB),1)
  CC := $(CT)
else
  CC := $(CA)
endif

RM := \rm -f

#-------------------------------------------------------------------------------

APIOBJ := spin1_api.o spin1_isr.o

ifeq ($(GNU),1)
  LIBNAME := libspin1_api
  SARKOBJ := ../sark/libsark.o 
else
  LIBNAME := spin1_api
  SARKOBJ := ../sark/sark.o 
endif

APILIB := $(LIBNAME).a

#-------------------------------------------------------------------------------

# APILIB (default target)

$(APILIB): $(SARKOBJ) $(APIOBJ)
	$(RM) $(APILIB)
	$(AR) $(APILIB) $(SARKOBJ) $(APIOBJ)

#-------------------------------------------------------------------------------

# Install

install: $(APILIB)
	\cp -p $(APILIB) ../lib/$(APILIB)

#-------------------------------------------------------------------------------

# API

spin1_api.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h $(INC_DIR)/spin1_api.h \
	$(INC_DIR)/spin1_api_params.h spin1_api.c 
	$(CC) $(CFLAGS) spin1_api.c

spin1_isr.o: $(INC_DIR)/spinnaker.h $(INC_DIR)/sark.h $(INC_DIR)/spin1_api.h \
	$(INC_DIR)/spin1_api_params.h spin1_isr.c 
	$(CA) $(CFLAGS) spin1_isr.c

#-------------------------------------------------------------------------------

clean:
	$(RM) $(APIOBJ) $(APILIB)
