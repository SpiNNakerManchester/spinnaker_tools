
# Makefile for SARK
BUILD_DIR := ../build
LIB := 1
include ../Makefile.common

CFLAGS += -DSARK_API

#-------------------------------------------------------------------------------
ifeq ($(GNU),1)
    LIBNAME := libsark
    AS_END := gas
else
    LIBNAME := sark
    AS_END := s
endif

SARKLIB := $(SPINN_LIB_DIR)/$(LIBNAME).a
SARKINT := $(SPINN_LIB_DIR)/$(LIBNAME).o

#-------------------------------------------------------------------------------

# SARK objects

SARKOBJS := sark_alib.o sark_base.o sark_event.o sark_timer.o \
	sark_hw.o sark_isr.o sark_alloc.o sark_io.o sark_pkt.o
SARK_ASM_OBJS := sark_alib.end spinnaker.end sark.end
SARKOBJ = $(SARKOBJS:%.o=$(BUILD_DIR)/%.o)
SARK_ASM_OBJ = $(SARK_ASM_OBJS:%.end=$(BUILD_DIR)/%.$(AS_END))

#-------------------------------------------------------------------------------

# SARK library (default target)
$(SARKLIB): $(SARKOBJ)
	$(RM) $@
	$(AR) $@ $^

$(SARKINT): $(SARKOBJ)
	$(RM) $@
	$(LD) -o $(SARKINT) $^

#-------------------------------------------------------------------------------

# SARK assembler sources

ifeq ($(GNU),1)

$(BUILD_DIR)/%.gas:	$(SPINN_INC_DIR)/%.h
	$(SPINN_TOOLS_DIR)/h2asm $< | $(SPINN_TOOLS_DIR)/arm2gas > $@

$(BUILD_DIR)/%.gas: %.s
	$(SPINN_TOOLS_DIR)/arm2gas $< > $@

$(BUILD_DIR)/sark_alib.o: $(BUILD_DIR)/sark_alib.gas $(BUILD_DIR)/spinnaker.gas $(BUILD_DIR)/sark.gas
	$(AS) -I $(BUILD_DIR) -o $@ $<

else

$(BUILD_DIR)/%.s: $(SPINN_INC_DIR)/%.h
	$(SPINN_TOOLS_DIR)/h2asm $< > $@

$(BUILD_DIR)/sark_alib.o: sark_alib.s $(BUILD_DIR)/spinnaker.s $(BUILD_DIR)/sark.s
	$(AS) -o $@ $<

endif

#-------------------------------------------------------------------------------

# SARK C sources

$(BUILD_DIR)/%.o: %.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h $(SPINN_INC_DIR)/spin1_api.h
	$(CC_THUMB) $(CFLAGS) -o $@ $<
	
$(BUILD_DIR)/sark_isr.o: sark_isr.c $(SPINN_INC_DIR)/spinnaker.h $(SPINN_INC_DIR)/sark.h $(SPINN_INC_DIR)/spin1_api.h
	$(CC) $(CFLAGS) -o $@ $<

#-------------------------------------------------------------------------------

clean:
	$(RM) $(SARKLIB) $(SARKINT) $(SARKOBJ) $(SARK_ASM_OBJ)
