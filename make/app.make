ifndef SPINN_DIRS
    $(error SPINN_DIRS is not set.  Please define SPINN_DIRS (possibly by running "source setup" in the spinnaker tools folder))
endif

ifndef APP
    $(error APP is not defined.  Please rerun with APP=<app_name>)
endif

ifndef OBJECTS
    OBJECTS = $(BUILD_DIR)$(APP).o
endif

include $(SPINN_DIRS)/make/Makefile.common

all: $(APP_OUTPUT_DIR)$(APP).aplx
	
clean:
	$(RM) $(OBJECTS) $(BUILD_DIR)$(APP).txt $(APP_OUTPUT_DIR)$(APP).aplx $(BUILD_DIR)$(APP).elf
