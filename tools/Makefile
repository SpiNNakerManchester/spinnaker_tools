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

MAKEFILES = spinnaker_tools.mk app.make
COMMON_MAKE = $(MAKEFILES:%=$(abspath $(CURRENT_DIR)/../make/%))

MKDIR := mkdir -p
INSTALL := install

BOOT = boot/sark.struct boot/spin1.conf boot/spin2.conf boot/spin3.conf boot/spin4.conf boot/spin5.conf
PERL = SpiNN/Boot.pm SpiNN/CLI.pm SpiNN/Cmd.pm SpiNN/SCP.pm SpiNN/Struct.pm SpiNN/Util.pm
TOOLS = arm2gas bmpc bmpstat gdb-spin h2asm mkaplx mkbin mkbuild sark.lnk sark.sct tubogrid tubotron ybug

BOOT_SRC = $(BOOT:%=$(CURRENT_DIR)/%)
PERL_SRC = $(PERL:%=$(CURRENT_DIR)/%)
TOOLS_SRC = $(TOOLS:%=$(CURRENT_DIR)/%)

INSTALL_ERROR := spinnaker_tools/tools install is only required if you are building in a temporary location.
INSTALL_ERROR += In which case you must set the environment variable SPINN_INSTALL_DIR to where the install should go.
INSTALL_ERROR += This environment variable must also exist for all higher level makes.

install:
	$(eval SPINN_INSTALL_DIR := $(strip $(if $(SPINN_INSTALL_DIR), $(SPINN_INSTALL_DIR), $(error $(INSTALL_ERROR)))))
	$(MKDIR) $(SPINN_INSTALL_DIR)/tools
	$(MKDIR) $(SPINN_INSTALL_DIR)/tools/SpiNN
	$(MKDIR) $(SPINN_INSTALL_DIR)/tools/boot
	$(MKDIR) $(SPINN_INSTALL_DIR)/make
	$(INSTALL) -c -m755 $(TOOLS_SRC) $(SPINN_INSTALL_DIR)/tools
	$(INSTALL) -c -m755 $(PERL_SRC) $(SPINN_INSTALL_DIR)/tools/SpiNN
	$(INSTALL) -c -m644 $(BOOT_SRC) $(SPINN_INSTALL_DIR)/tools/boot
	$(INSTALL) -c -m644 $(COMMON_MAKE) $(SPINN_INSTALL_DIR)/make
