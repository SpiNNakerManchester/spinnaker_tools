################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../apps/hello/hello.c 

OBJS += \
./apps/hello/hello.o 

C_DEPS += \
./apps/hello/hello.d 


# Each subdirectory must supply rules for building sources it contributes
apps/hello/%.o: ../apps/hello/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


