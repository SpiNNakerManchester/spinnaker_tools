################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../spin1_api/spin1_api.c \
../spin1_api/spin1_isr.c 

OBJS += \
./spin1_api/spin1_api.o \
./spin1_api/spin1_isr.o 

C_DEPS += \
./spin1_api/spin1_api.d \
./spin1_api/spin1_isr.d 


# Each subdirectory must supply rules for building sources it contributes
spin1_api/%.o: ../spin1_api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


