################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sark/sark_alloc.c \
../sark/sark_base.c \
../sark/sark_div0.c \
../sark/sark_event.c \
../sark/sark_hw.c \
../sark/sark_io.c \
../sark/sark_isr.c \
../sark/sark_pkt.c \
../sark/sark_timer.c 

OBJS += \
./sark/sark_alloc.o \
./sark/sark_base.o \
./sark/sark_div0.o \
./sark/sark_event.o \
./sark/sark_hw.o \
./sark/sark_io.o \
./sark/sark_isr.o \
./sark/sark_pkt.o \
./sark/sark_timer.o 

C_DEPS += \
./sark/sark_alloc.d \
./sark/sark_base.d \
./sark/sark_div0.d \
./sark/sark_event.d \
./sark/sark_hw.d \
./sark/sark_io.d \
./sark/sark_isr.d \
./sark/sark_pkt.d \
./sark/sark_timer.d 


# Each subdirectory must supply rules for building sources it contributes
sark/%.o: ../sark/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


