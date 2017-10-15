################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bbbio/src/bbbio.c \
../bbbio/src/bbbio_pwmss.c 

OBJS += \
./bbbio/src/bbbio.o \
./bbbio/src/bbbio_pwmss.o 

C_DEPS += \
./bbbio/src/bbbio.d \
./bbbio/src/bbbio_pwmss.d 


# Each subdirectory must supply rules for building sources it contributes
bbbio/src/%.o: ../bbbio/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	arm-linux-gnueabihf-gcc -I"/root/git/BBBIO/bbbio/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


