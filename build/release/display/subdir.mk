################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../../display/lcd.cpp 

OBJS += \
./display/lcd.o 

CPP_DEPS += \
./display/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
display/%.o: ../../display/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GPP Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


