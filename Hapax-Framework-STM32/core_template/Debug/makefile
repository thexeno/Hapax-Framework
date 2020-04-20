################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/core/utils/subdir.mk
-include Src/core/portable/hal/stm32f103c8tx/wrappers/subdir.mk
-include Src/core/portable/hal/stm32f103c8tx/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/Legacy/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
core_template.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
core_template.list \

OBJCOPY_BIN += \
core_template.bin \


# All Target
all: core_template.elf secondary-outputs

# Tool invocations
core_template.elf: $(OBJS) $(USER_OBJS) C:\Users\Enrico\Documents\GitHub\Hapax-Framework\Hapax-Framework-STM32\core_template\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "core_template.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Enrico\Documents\GitHub\Hapax-Framework\Hapax-Framework-STM32\core_template\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="core_template.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

core_template.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "core_template.list"
	@echo 'Finished building: $@'
	@echo ' '

core_template.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "core_template.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
