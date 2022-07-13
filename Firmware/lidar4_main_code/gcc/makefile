#https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-win32.zip

gcc		=$(gcc_path)arm-none-eabi-gcc.exe
objcopy	=$(gcc_path)arm-none-eabi-objcopy.exe
objdump	=$(gcc_path)arm-none-eabi-objdump.exe

PROJ_NAME=lidar4
SOURCES := $(wildcard *.s *.c ../*.c ../*.h ../StdPeriphLib/src/*.c makefile)  

CFLAGS  = -Wall -std=gnu99 --specs=nano.specs -fsingle-precision-constant  -ffast-math -fdiagnostics-color=always
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6-m -msoft-float
CFLAGS += -fmessage-length=0 -Werror=implicit-function-declaration
#CFLAGS += -Os -ffunction-sections -fdata-sections -fomit-frame-pointer -flto -ffat-lto-objects
CFLAGS += -O2 -ffunction-sections -fdata-sections -fomit-frame-pointer -flto -ffat-lto-objects
#CFLAGS += -O2 -save-temps -fverbose-asm
CFLAGS += -finline-small-functions
CFLAGS += -fmerge-all-constants
CFLAGS += -frename-registers
CFLAGS += -freg-struct-return -fregmove -frename-registers -freorder-blocks -freorder-functions 
CFLAGS += -DSTM32F030 -I.. -I../.. -I../../StdPeriphLib/inc -I../../inc -include ../stm32f0xx_conf.h
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map -Wl,--print-memory-usage
#CFLAGS += -Dprintf=iprintf -u _printf_float --specs=nosys.specs
CFLAGS += -Dputch=putchar
CFLAGS += -Dgetch()=0
CFLAGS += -T../stm32f030f4.ld


build/$(PROJ_NAME).hex:build/$(PROJ_NAME).elf
	$(objcopy) -O ihex $^ $@
	$(objdump) -Ssxtw $^ >$^.lst

.ONESHELL:
build/$(PROJ_NAME).elf:$(SOURCES)  
	cd build
	$(gcc) $(CFLAGS) $(addprefix ../, $(filter %.s %.c,$^)) -o $(PROJ_NAME).elf

