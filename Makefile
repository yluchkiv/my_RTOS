# cpu
CPU = -mcpu=cortex-m4 -mfloat-abi=hard -mthumb -gdwarf-5 -mfpu=fpv4-sp-d16

# C defines
C_DEFS = \
-DSTM32F303xE

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

#CC = D:\\ARM\\discont\\gcc-arm-none-eabi-10.3-2021.10\\bin\\arm-none-eabi-gcc.exe

CC = D:\\ARM\\ver12_2\\arm-gnu-toolchain-12.2.rel1-mingw-w64-i686-arm-none-eabi\\bin\\arm-none-eabi-gcc.exe

all: f.elf

main.o: main.c Makefile
	${CC} -c main.c ${CC_FLAGS}

startup.o: startup.c Makefile
	${CC} -c startup.c ${CC_FLAGS}

uart_diag.o: uart_diag.c Makefile
	${CC}  -c uart_diag.c ${CC_FLAGS}

tasks.o: 
	${CC}  -c ./FreeRTOS-Kernel/tasks.c ${CC_FLAGS}

port.o: 
	${CC}  -c ./FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c ${CC_FLAGS}

list.o: 
	${CC}  -c ./FreeRTOS-Kernel/list.c ${CC_FLAGS}

heap_1.o: 
	${CC}  -c ./FreeRTOS-Kernel/portable/MemMang/heap_1.c ${CC_FLAGS}


f.elf: startup.o main.o uart_diag.o  tasks.o port.o list.o heap_1.o 
	${CC} ${CC_FLAGS} -o f.elf main.o startup.o tasks.o port.o list.o heap_1.o uart_diag.o -specs=nosys.specs -u _print_float -nostartfiles -T linker_script.ld 
	# arm-none-eabi-objdump -D f.elf > f.elf.dump
	# arm-none-eabi-objcopy -O ihex $@ f.elf.ihex
	# arm-none-eabi-objcopy -O srec $@ f.elf.srec
	# arm-none-eabi-size $@

.PHONY: clean
clean:
	-rm ./*.o ./*.elf ./*.ihex ./*.dump ./*.srec
