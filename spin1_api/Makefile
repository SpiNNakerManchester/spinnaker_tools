
# Makefile for spin1_api
BUILD_DIR := ../build
LIB := 1
include ../Makefile.common

#-------------------------------------------------------------------------------

APIOBJS := spin1_api.o spin1_isr.o
APIOBJ := $(APIOBJS:%.o=$(BUILD_DIR)/%.o)
HEADERS := spinnaker.h sark.h spin1_api.h spin1_api_params.h
HEADER := $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h)

ifeq ($(GNU),1)
  LIBNAME := libspin1_api
  SARKOBJ := $(SPINN_LIB_DIR)/libsark.o 
else
  LIBNAME := spin1_api
  SARKOBJ := $(SPINN_LIB_DIR)/sark.o 
endif

APILIB := $(SPINN_LIB_DIR)/$(LIBNAME).a

#-------------------------------------------------------------------------------

# APILIB (default target)

$(APILIB): $(SARKOBJ) $(APIOBJ)
	$(RM) $@
	$(AR) $@ $^

$(SARKOBJ):
	cd ../sark/; $(MAKE) $(SARKOBJ)

#-------------------------------------------------------------------------------

# API

$(BUILD_DIR)/spin1_api.o: spin1_api.c $(HEADER) 
	$(CC_THUMB) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/spin1_isr.o: spin1_isr.c $(HEADER) 
	$(CC) $(CFLAGS) -o $@ $<

#-------------------------------------------------------------------------------

clean:
	$(RM) $(APILIB) $(APIOBJ)
