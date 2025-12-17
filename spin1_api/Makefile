# Copyright (c) 2015 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

CURRENT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Makefile for spin1_api
ifeq ($(GNU),1)
    BUILD_DIR := $(abspath $(CURRENT_DIR)/../build/gnu)
else
    BUILD_DIR := $(abspath $(CURRENT_DIR)/../build/arm)
endif
override LIB := 1

include $(CURRENT_DIR)/../make/spinnaker_tools.mk

#-------------------------------------------------------------------------------

APIOBJS := spin1_api.o spin1_isr.o
APIOBJ := $(APIOBJS:%.o=$(BUILD_DIR)/%.o)
HEADERS := spin1_api.h spin1_api_params.h scamp_spin1_sync.h spin1_api_cpp.h
HEADER := $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h)

ifeq ($(GNU),1)
    LIBNAME := libspin1_api
else
    LIBNAME := spin1_api
endif

APILIB := $(CURRENT_DIR)../lib/$(LIBNAME).a

APIOPT := $(OSPACE)
CFLAGS += $(APIOPT)

#-------------------------------------------------------------------------------

# APILIB (default target)

$(APILIB): $(BUILD_DIR)/sark.o $(APIOBJ)
	$(RM) $@
	$(AR) $@ $^

$(BUILD_DIR)/sark.o:
	@$(MAKE) -C ../sark $@

#-------------------------------------------------------------------------------

# API

$(BUILD_DIR)/spin1_api.o: spin1_api.c $(HEADER)
	$(CC_THUMB) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/spin1_isr.o: spin1_isr.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $<

INSTALL_ERROR := spinnaker_tools install is only required if you are building in a temporary location.
INSTALL_ERROR += In which case you must set the environment variable SPINN_INSTALL_DIR to where the install should go.
INSTALL_ERROR += This environment variable must also exist for all higher level makes.

#-------------------------------------------------------------------------------
install: $(APILIB)
	$(eval SPINN_INSTALL_DIR := $(strip $(if $(SPINN_INSTALL_DIR), $(SPINN_INSTALL_DIR), $(error $(INSTALL_ERROR)))))
	$(MKDIR) $(SPINN_INSTALL_DIR)/lib
	$(MKDIR) $(SPINN_INSTALL_DIR)/include
	$(INSTALL) -c -m644 $< $(SPINN_INSTALL_DIR)/lib
	$(INSTALL) -c -m644 $(HEADERS:%.h=$(SPINN_INC_DIR)/%.h) $(SPINN_INSTALL_DIR)/include

clean:
	$(RM) $(APILIB) $(APIOBJ) $(BUILD_DIR)/sark.o

.PHONY: clean
