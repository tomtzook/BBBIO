################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../examples/led/led.c 

OBJS += \
./examples/led/led.o 

C_DEPS += \
./examples/led/led.d 


# Each subdirectory must supply rules for building sources it contributes
examples/led/%.o: ../examples/led/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	arm-linux-gnueabihf-gcc -I"/root/git/BBBIO/bbbio/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


