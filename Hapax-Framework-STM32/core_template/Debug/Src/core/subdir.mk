################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/core/conf.c 

OBJS += \
./Src/core/conf.o 

C_DEPS += \
./Src/core/conf.d 


# Each subdirectory must supply rules for building sources it contributes
Src/core/conf.o: ../Src/core/conf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/utils" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx/wrappers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/core/conf.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

