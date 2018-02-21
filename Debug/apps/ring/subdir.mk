################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../apps/ring/ring.c 

OBJS += \
./apps/ring/ring.o 

C_DEPS += \
./apps/ring/ring.d 


# Each subdirectory must supply rules for building sources it contributes
apps/ring/%.o: ../apps/ring/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


