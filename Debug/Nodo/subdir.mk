################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Nodo/Pruebas.c \
../Nodo/interfaces.c \
../Nodo/nodo.c \
../Nodo/nodoFunciones.c \
../Nodo/redirec_stdin_stdout.c 

OBJS += \
./Nodo/Pruebas.o \
./Nodo/interfaces.o \
./Nodo/nodo.o \
./Nodo/nodoFunciones.o \
./Nodo/redirec_stdin_stdout.o 

C_DEPS += \
./Nodo/Pruebas.d \
./Nodo/interfaces.d \
./Nodo/nodo.d \
./Nodo/nodoFunciones.d \
./Nodo/redirec_stdin_stdout.d 


# Each subdirectory must supply rules for building sources it contributes
Nodo/%.o: ../Nodo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


