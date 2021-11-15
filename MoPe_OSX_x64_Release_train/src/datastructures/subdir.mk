################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/datastructures/DataVectors.cpp \
../src/datastructures/SimilarityNode.cpp \
../src/datastructures/VectorNode.cpp 

OBJS += \
./src/datastructures/DataVectors.o \
./src/datastructures/SimilarityNode.o \
./src/datastructures/VectorNode.o 

CPP_DEPS += \
./src/datastructures/DataVectors.d \
./src/datastructures/SimilarityNode.d \
./src/datastructures/VectorNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/datastructures/%.o: ../src/datastructures/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Volumes/HOME/Users/anestisgk/Anestis's Important Data/EclipseProjects/MoPe/src" -O3 -Wall -c -fmessage-length=0 -m64 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


