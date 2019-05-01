################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC_PWM.c \
../source/adc.c \
../source/delay.c \
../source/keypad.c \
../source/lcd.c \
../source/lcd_state.c \
../source/mapper.c \
../source/mtb.c \
../source/pwm.c \
../source/range_pair.c \
../source/semihost_hardfault.c \
../source/sweep.c \
../source/tick.c \
../source/uart.c 

OBJS += \
./source/ADC_PWM.o \
./source/adc.o \
./source/delay.o \
./source/keypad.o \
./source/lcd.o \
./source/lcd_state.o \
./source/mapper.o \
./source/mtb.o \
./source/pwm.o \
./source/range_pair.o \
./source/semihost_hardfault.o \
./source/sweep.o \
./source/tick.o \
./source/uart.o 

C_DEPS += \
./source/ADC_PWM.d \
./source/adc.d \
./source/delay.d \
./source/keypad.d \
./source/lcd.d \
./source/lcd_state.d \
./source/mapper.d \
./source/mtb.d \
./source/pwm.d \
./source/range_pair.d \
./source/semihost_hardfault.d \
./source/sweep.d \
./source/tick.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\ulab\Downloads\ACD_PWM\board" -I"C:\Users\ulab\Downloads\ACD_PWM\source" -I"C:\Users\ulab\Downloads\ACD_PWM" -I"C:\Users\ulab\Downloads\ACD_PWM\drivers" -I"C:\Users\ulab\Downloads\ACD_PWM\utilities" -I"C:\Users\ulab\Downloads\ACD_PWM\startup" -I"C:\Users\ulab\Downloads\ACD_PWM\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


