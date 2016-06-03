
# Makefile for spin1_api
ifeq ($(GNU),1)
    BUILD_DIR := ../build/gnu
else
    BUILD_DIR := ../build/arm
endif
include ../make/Makefile.common
LIB := 1

#-------------------------------------------------------------------------------

APIOBJS := spin1_api.o spin1_isr.o
APIOBJ := $(APIOBJS:%.o=$(BUILD_DIR)/%.o)
HEADERS := spinnaker.h sark.h spin1_api.h spin1_api_params.h
HEADER := $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h)

ifeq ($(GNU),1)
  LIBNAME := libspin1_api
else
  LIBNAME := spin1_api
endif

APILIB := $(SPINN_LIB_DIR)/$(LIBNAME).a

APIOPT := $(OSPACE)
CFLAGS += $(APIOPT)

#-------------------------------------------------------------------------------

# APILIB (default target)

$(APILIB): $(BUILD_DIR)/sark.o $(APIOBJ)
	$(RM) $@
	$(AR) $@ $^

$(BUILD_DIR)/sark.o:
	cd ../sark/; $(MAKE) $@

#-------------------------------------------------------------------------------

# API

$(BUILD_DIR)/spin1_api.o: spin1_api.c $(HEADER) 
	$(CC_THUMB) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/spin1_isr.o: spin1_isr.c $(HEADER) 
	$(CC) $(CFLAGS) -o $@ $<

#-------------------------------------------------------------------------------

clean:
	$(RM) $(APILIB) $(APIOBJ) $(BUILD_DIR)/sark.o
