################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include spin1_api/subdir.mk
-include scamp/subdir.mk
-include sark/subdir.mk
-include build/gnu/subdir.mk
-include bmp/subdir.mk
-include apps/simple/subdir.mk
-include apps/sdping/subdir.mk
-include apps/ring/subdir.mk
-include apps/random/subdir.mk
-include apps/pt_demo/subdir.mk
-include apps/life/subdir.mk
-include apps/interrupt/subdir.mk
-include apps/hello/subdir.mk
-include apps/heat_demo/subdir.mk
-include apps/gdb-test/subdir.mk
-include apps/data_abort/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: spinnaker_tools

# Tool invocations
spinnaker_tools: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C Linker'
	gcc  -o "spinnaker_tools" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) spinnaker_tools
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
