################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../apps/data_abort/data_abort.c 

OBJS += \
./apps/data_abort/data_abort.o 

C_DEPS += \
./apps/data_abort/data_abort.d 


# Each subdirectory must supply rules for building sources it contributes
apps/data_abort/%.o: ../apps/data_abort/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


