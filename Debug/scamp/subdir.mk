################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../scamp/sark.c \
../scamp/scamp-3.c \
../scamp/scamp-app.c \
../scamp/scamp-boot.c \
../scamp/scamp-cmd.c \
../scamp/scamp-isr.c \
../scamp/scamp-nn.c \
../scamp/scamp-p2p.c \
../scamp/spinn_net.c \
../scamp/spinn_phy.c \
../scamp/spinn_srom.c 

OBJS += \
./scamp/sark.o \
./scamp/scamp-3.o \
./scamp/scamp-app.o \
./scamp/scamp-boot.o \
./scamp/scamp-cmd.o \
./scamp/scamp-isr.o \
./scamp/scamp-nn.o \
./scamp/scamp-p2p.o \
./scamp/spinn_net.o \
./scamp/spinn_phy.o \
./scamp/spinn_srom.o 

C_DEPS += \
./scamp/sark.d \
./scamp/scamp-3.d \
./scamp/scamp-app.d \
./scamp/scamp-boot.d \
./scamp/scamp-cmd.d \
./scamp/scamp-isr.d \
./scamp/scamp-nn.d \
./scamp/scamp-p2p.d \
./scamp/spinn_net.d \
./scamp/spinn_phy.d \
./scamp/spinn_srom.d 


# Each subdirectory must supply rules for building sources it contributes
scamp/%.o: ../scamp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


