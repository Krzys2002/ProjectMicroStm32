################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/Src/log_router.c \
../Components/Src/logbuf.c \
../Components/Src/net_discovery.c \
../Components/Src/net_server.c \
../Components/Src/net_tx.c \
../Components/Src/uart_tx.c 

OBJS += \
./Components/Src/log_router.o \
./Components/Src/logbuf.o \
./Components/Src/net_discovery.o \
./Components/Src/net_server.o \
./Components/Src/net_tx.o \
./Components/Src/uart_tx.o 

C_DEPS += \
./Components/Src/log_router.d \
./Components/Src/logbuf.d \
./Components/Src/net_discovery.d \
./Components/Src/net_server.d \
./Components/Src/net_tx.d \
./Components/Src/uart_tx.d 


# Each subdirectory must supply rules for building sources it contributes
Components/Src/%.o Components/Src/%.su Components/Src/%.cyclo: ../Components/Src/%.c Components/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"/Users/krzysztofsawicki/Documents/Put/sem5/microProc/Project/ProjeckKSMSIM/STM32/Components/Inc" -I"/Users/krzysztofsawicki/Documents/Put/sem5/microProc/Project/ProjeckKSMSIM/STM32/Core/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-Src

clean-Components-2f-Src:
	-$(RM) ./Components/Src/log_router.cyclo ./Components/Src/log_router.d ./Components/Src/log_router.o ./Components/Src/log_router.su ./Components/Src/logbuf.cyclo ./Components/Src/logbuf.d ./Components/Src/logbuf.o ./Components/Src/logbuf.su ./Components/Src/net_discovery.cyclo ./Components/Src/net_discovery.d ./Components/Src/net_discovery.o ./Components/Src/net_discovery.su ./Components/Src/net_server.cyclo ./Components/Src/net_server.d ./Components/Src/net_server.o ./Components/Src/net_server.su ./Components/Src/net_tx.cyclo ./Components/Src/net_tx.d ./Components/Src/net_tx.o ./Components/Src/net_tx.su ./Components/Src/uart_tx.cyclo ./Components/Src/uart_tx.d ./Components/Src/uart_tx.o ./Components/Src/uart_tx.su

.PHONY: clean-Components-2f-Src

