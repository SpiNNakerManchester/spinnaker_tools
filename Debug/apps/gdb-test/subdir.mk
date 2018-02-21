################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../apps/gdb-test/gdb-test.c 

OBJS += \
./apps/gdb-test/gdb-test.o 

C_DEPS += \
./apps/gdb-test/gdb-test.d 


# Each subdirectory must supply rules for building sources it contributes
apps/gdb-test/%.o: ../apps/gdb-test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


