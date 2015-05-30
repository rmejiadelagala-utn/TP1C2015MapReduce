################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Filesystem/AlgoritmoParaGuardardeFS.c \
../Filesystem/consolaFS.c \
../Filesystem/fsystem.c \
../Filesystem/funcionesDeEstructurasFileSystem.c 

OBJS += \
./Filesystem/AlgoritmoParaGuardardeFS.o \
./Filesystem/consolaFS.o \
./Filesystem/fsystem.o \
./Filesystem/funcionesDeEstructurasFileSystem.o 

C_DEPS += \
./Filesystem/AlgoritmoParaGuardardeFS.d \
./Filesystem/consolaFS.d \
./Filesystem/fsystem.d \
./Filesystem/funcionesDeEstructurasFileSystem.d 


# Each subdirectory must supply rules for building sources it contributes
Filesystem/%.o: ../Filesystem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


