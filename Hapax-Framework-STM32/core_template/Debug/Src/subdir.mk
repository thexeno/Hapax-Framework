################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/utils" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx/wrappers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/utils" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx/wrappers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/utils" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Drivers/CMSIS/Include" -I"C:/Users/Enrico/Documents/GitHub/Hapax-Framework/Hapax-Framework-STM32/core_template/Src/core/portable/hal/stm32f103c8tx/wrappers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

