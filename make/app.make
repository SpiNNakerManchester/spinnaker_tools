# Copyright (c) 2015-2023 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

ifndef APP
    $(error APP is not defined.  Please rerun with APP=<app_name>)
endif

ifndef OBJECTS
    OBJECTS = $(BUILD_DIR)$(APP).o
endif

include $(SPINN_DIRS)/make/spinnaker_tools.mk

all: $(APP_OUTPUT_DIR)$(APP).aplx

clean:
	$(RM) $(OBJECTS) $(BUILD_DIR)$(APP).txt $(APP_OUTPUT_DIR)$(APP).aplx $(BUILD_DIR)$(APP).elf

.PHONY: all clean
