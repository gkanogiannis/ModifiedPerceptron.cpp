################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/ModifiedPerceptron_F1.cpp \
../src/core/Tester.cpp \
../src/core/Thresholder.cpp \
../src/core/Trainer.cpp 

OBJS += \
./src/core/ModifiedPerceptron_F1.o \
./src/core/Tester.o \
./src/core/Thresholder.o \
./src/core/Trainer.o 

CPP_DEPS += \
./src/core/ModifiedPerceptron_F1.d \
./src/core/Tester.d \
./src/core/Thresholder.d \
./src/core/Trainer.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/%.o: ../src/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Users\utumno\Google Drive\mope\MoPe\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


