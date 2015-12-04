################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../archy.c \
../dict.c \
../encoder.c \
../rle.c \
../search.c 

OBJS += \
./archy.o \
./dict.o \
./encoder.o \
./rle.o \
./search.o 

C_DEPS += \
./archy.d \
./dict.d \
./encoder.d \
./rle.d \
./search.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


