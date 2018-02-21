################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../apps/heat_demo/heat_demo.c 

OBJS += \
./apps/heat_demo/heat_demo.o 

C_DEPS += \
./apps/heat_demo/heat_demo.d 


# Each subdirectory must supply rules for building sources it contributes
apps/heat_demo/%.o: ../apps/heat_demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


