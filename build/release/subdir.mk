################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CXX_SRCS += \
../../Main.cxx 

CPP_SRCS += \
../../TimeDisplay.cpp 

OBJS += \
./TimeDisplay.o \
./Main.o 

CXX_DEPS += \
./Main.d 

CPP_DEPS += \
./TimeDisplay.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../../%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


