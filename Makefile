# cpu
CPU = -mcpu=cortex-m4 -mfloat-abi=hard -mthumb -gdwarf-5 -mfpu=fpv4-sp-d16

# C defines
C_DEFS = \
-DSTM32F303xE

# C sources
C_SOURCE = \
main.c \
startup.c \
uart_diag.c \
./FreeRTOS-Kernel/tasks.c \
./FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c \
./FreeRTOS-Kernel/list.c \
./FreeRTOS-Kernel/portable/MemMang/heap_1.c

# C includes
C_INCLUDES = \
-iquote ./Include \
-iquote ./ \
-I ./ \
-I ./STM32CubeF3/Drivers/CMSIS/Device/ST/STM32F3xx/Include/ \
-I ./STM32CubeF3/Drivers/CMSIS/Core/Include/ \
-I ./FreeRTOS-Kernel/include/ \
-I ./FreeRTOS-Kernel/portable/GCC/ARM_CM4F

CC_FLAGS = $(CPU) $(C_DEFS) $(C_INCLUDES) -std=c99 -Wall -O0 -g3 

GCC_PATH = D:\\ARM\\ver12_2\\arm-gnu-toolchain-12.2.rel1-mingw-w64-i686-arm-none-eabi\\bin

all:
	$(GCC_PATH)\\arm-none-eabi-gcc -o f.elf $(C_SOURCE) -specs=nosys.specs -u _print_float -nostartfiles -T linker_script.ld $(CC_FLAGS) $(C_INCLUDES)
	# $(GCC_PATH)\\arm-none-eabi-objdump -D f.elf > f.elf.dump
	$(GCC_PATH)\\arm-none-eabi-objcopy -O ihex f.elf f.elf.ihex
	$(GCC_PATH)\\arm-none-eabi-objcopy -O srec f.elf f.elf.srec
	$(GCC_PATH)\\arm-none-eabi-size f.elf

.PHONY: clean
clean:
	-rm ./*.o ./*.elf ./*.ihex ./*.dump ./*.srec
