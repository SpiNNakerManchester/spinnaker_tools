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
INSTALL_TARGET = $(SPINN_DIRS)/make
INSTALL_FILES = sark_cpp.h sark.h spin1_api_cpp.h spin1_api_params.h spin1_api.h spinnaker.h version.h

default:
	@echo There is no default build rule in this directory

install:
	$(MKDIR) $(INSTALL_TARGET)
	$(INSTALL) -c -m644 $(INSTALL_FILES) $(INSTALL_TARGET)
