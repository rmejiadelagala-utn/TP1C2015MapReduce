################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Marta/MaRTA.c \
../Marta/atencionJobs.c 

OBJS += \
./Marta/MaRTA.o \
./Marta/atencionJobs.o 

C_DEPS += \
./Marta/MaRTA.d \
./Marta/atencionJobs.d 


# Each subdirectory must supply rules for building sources it contributes
Marta/%.o: ../Marta/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


