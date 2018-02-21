################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bmp/bmp_boot.c \
../bmp/bmp_can.c \
../bmp/bmp_clock.c \
../bmp/bmp_cmd.c \
../bmp/bmp_crc.c \
../bmp/bmp_eth.c \
../bmp/bmp_event.c \
../bmp/bmp_flash.c \
../bmp/bmp_hw.c \
../bmp/bmp_i2c.c \
../bmp/bmp_init.c \
../bmp/bmp_io.c \
../bmp/bmp_main.c \
../bmp/bmp_net.c \
../bmp/bmp_ssp.c 

OBJS += \
./bmp/bmp_boot.o \
./bmp/bmp_can.o \
./bmp/bmp_clock.o \
./bmp/bmp_cmd.o \
./bmp/bmp_crc.o \
./bmp/bmp_eth.o \
./bmp/bmp_event.o \
./bmp/bmp_flash.o \
./bmp/bmp_hw.o \
./bmp/bmp_i2c.o \
./bmp/bmp_init.o \
./bmp/bmp_io.o \
./bmp/bmp_main.o \
./bmp/bmp_net.o \
./bmp/bmp_ssp.o 

C_DEPS += \
./bmp/bmp_boot.d \
./bmp/bmp_can.d \
./bmp/bmp_clock.d \
./bmp/bmp_cmd.d \
./bmp/bmp_crc.d \
./bmp/bmp_eth.d \
./bmp/bmp_event.d \
./bmp/bmp_flash.d \
./bmp/bmp_hw.d \
./bmp/bmp_i2c.d \
./bmp/bmp_init.d \
./bmp/bmp_io.d \
./bmp/bmp_main.d \
./bmp/bmp_net.d \
./bmp/bmp_ssp.d 


# Each subdirectory must supply rules for building sources it contributes
bmp/%.o: ../bmp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


