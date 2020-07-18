# Copyright (c) 2017-2019 The University of Manchester
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

DIRS = sark spin1_api
INSTALL_EXTRA_DIRS = make include tools

GNU = 1

all: $(DIRS)
	@for d in $(DIRS); do $(MAKE) -C $$d GNU=$(GNU) || exit $$?; done

clean: $(DIRS)
	@for d in $(DIRS); do $(MAKE) -C $$d GNU=$(GNU) clean || exit $$?; done

install: $(DIRS) $(INSTALL_EXTRA_DIRS)
	@test "x$$SPINN_DIRS" = "x" && { \
		echo "Please set the SPINN_DIRS environment variable to where you want to install to"; \
		exit 1; \
	}
	@for d in $(DIRS) $(INSTALL_EXTRA_DIRS); do \
		$(MAKE) -C $$d GNU=$(GNU) install || exit $$?; \
	done
