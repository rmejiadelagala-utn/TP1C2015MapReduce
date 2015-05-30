################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Socketes/socketes/servidor.c \
../Socketes/socketes/servidorMultihilo.c 

OBJS += \
./Socketes/socketes/servidor.o \
./Socketes/socketes/servidorMultihilo.o 

C_DEPS += \
./Socketes/socketes/servidor.d \
./Socketes/socketes/servidorMultihilo.d 


# Each subdirectory must supply rules for building sources it contributes
Socketes/socketes/%.o: ../Socketes/socketes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


