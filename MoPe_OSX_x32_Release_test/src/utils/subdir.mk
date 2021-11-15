################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/DocumentClassVectorUtils.cpp \
../src/utils/PrecisionRecallEvaluation.cpp \
../src/utils/ScoredPrecisionRecallEvaluation.cpp \
../src/utils/StringTokenizer.cpp 

OBJS += \
./src/utils/DocumentClassVectorUtils.o \
./src/utils/PrecisionRecallEvaluation.o \
./src/utils/ScoredPrecisionRecallEvaluation.o \
./src/utils/StringTokenizer.o 

CPP_DEPS += \
./src/utils/DocumentClassVectorUtils.d \
./src/utils/PrecisionRecallEvaluation.d \
./src/utils/ScoredPrecisionRecallEvaluation.d \
./src/utils/StringTokenizer.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Volumes/HOME/Users/anestisgk/Anestis's Important Data/EclipseProjects/MoPe/src" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


