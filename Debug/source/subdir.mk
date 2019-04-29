################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC_PWM.c \
../source/adc.c \
../source/mtb.c \
../source/pwm.c 

OBJS += \
./source/ADC_PWM.o \
./source/adc.o \
./source/mtb.o \
./source/pwm.o 

C_DEPS += \
./source/ADC_PWM.d \
./source/adc.d \
./source/mtb.d \
./source/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\ulab\Downloads\ACD_PWM\board" -I"C:\Users\ulab\Downloads\ACD_PWM\source" -I"C:\Users\ulab\Downloads\ACD_PWM" -I"C:\Users\ulab\Downloads\ACD_PWM\drivers" -I"C:\Users\ulab\Downloads\ACD_PWM\utilities" -I"C:\Users\ulab\Downloads\ACD_PWM\startup" -I"C:\Users\ulab\Downloads\ACD_PWM\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


