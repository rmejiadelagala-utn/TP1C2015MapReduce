################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Socketes/socketes/subdir.mk
-include Socketes/Debug/socketes/subdir.mk
-include Nodo/Debug/subdir.mk
-include Nodo/subdir.mk
-include Marta/Debug/subdir.mk
-include Marta/subdir.mk
-include Job/Debug/subdir.mk
-include Job/subdir.mk
-include Filesystem/Debug/subdir.mk
-include Filesystem/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libtp-2015-1c-socketes-planificados.so

# Tool invocations
libtp-2015-1c-socketes-planificados.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -shared -o "libtp-2015-1c-socketes-planificados.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LIBRARIES) libtp-2015-1c-socketes-planificados.so
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
