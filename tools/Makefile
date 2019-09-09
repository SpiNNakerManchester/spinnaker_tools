# Copyright (c) 2019 The University of Manchester
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

INSTALL ?= install
INSTALL_TARGET = $(SPINN_DIRS)/tools
INSTALL_FILES = arm2gas bmpc bmpstat board.info h2asm mkaplx mkbin mkbuild sark.lnk sark.sct tubogrid tubotron ybug
INSTALL_BOOT = sark.struct spin1.conf spin2.conf spin3.conf spin4.conf spin5.conf
INSTALL_SPINN = Boot.pm CLI.pm Cmd.pm SCP.pm Struct.pm Util.pm

default:
	@echo There is no default build rule in this directory

install:
	$(INSTALL) -d $(INSTALL_TARGET) $(INSTALL_TARGET)/boot $(INSTALL_TARGET)/SpiNN
	$(INSTALL) -c -m644 $(INSTALL_FILES) $(INSTALL_TARGET)
	$(INSTALL) -c -m644 $(INSTALL_BOOT) $(INSTALL_TARGET)/boot
	$(INSTALL) -c -m644 $(INSTALL_SPINN) $(INSTALL_TARGET)/SpiNN
