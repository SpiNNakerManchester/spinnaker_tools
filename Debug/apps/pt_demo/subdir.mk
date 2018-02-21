################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../apps/pt_demo/aggregator.c \
../apps/pt_demo/drawer.c \
../apps/pt_demo/listener.c \
../apps/pt_demo/tracer.c 

OBJS += \
./apps/pt_demo/aggregator.o \
./apps/pt_demo/drawer.o \
./apps/pt_demo/listener.o \
./apps/pt_demo/tracer.o 

C_DEPS += \
./apps/pt_demo/aggregator.d \
./apps/pt_demo/drawer.d \
./apps/pt_demo/listener.d \
./apps/pt_demo/tracer.d 


# Each subdirectory must supply rules for building sources it contributes
apps/pt_demo/%.o: ../apps/pt_demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


