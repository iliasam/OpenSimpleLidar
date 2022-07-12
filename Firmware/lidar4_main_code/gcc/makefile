#https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-win32.zip

gcc		=$(gcc_path)arm-none-eabi-gcc.exe
objcopy	=$(gcc_path)arm-none-eabi-objcopy.exe
objdump	=$(gcc_path)arm-none-eabi-objdump.exe

PROJ_NAME=lidar4
SOURCES := $(wildcard *.s *.c ../*.c ../*.h ../StdPeriphLib/src/*.c makefile)  

CFLAGS  = -Wall -std=gnu99 --specs=nano.specs -fsingle-precision-constant  -ffast-math -fdiagnostics-color=always
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6-m -msoft-float
CFLAGS += -finline-small-functions
CFLAGS += -fmessage-length=0 -Werror=implicit-function-declaration
CFLAGS += -Os -ffunction-sections -fdata-sections -fomit-frame-pointer -flto -ffat-lto-objects
CFLAGS += -DSTM32F030 -I.. -I../StdPeriphLib/inc -I../inc -include stm32f0xx_conf.h
CFLAGS += -Wl,--gc-sections -Wl,-Map=build/$(PROJ_NAME).map -Wl,--print-memory-usage
#CFLAGS += -Dprintf=iprintf -u _printf_float --specs=nosys.specs
CFLAGS += -Dputch=putchar
CFLAGS += -Dgetch()=0
CFLAGS += -Tstm32f030f4.ld


build/$(PROJ_NAME).hex:build/$(PROJ_NAME).elf
	$(objcopy) -O ihex $^ $@
	$(objdump) -Ssxtw $^ >$^.lst

build/$(PROJ_NAME).elf:$(SOURCES)
	$(gcc) $(CFLAGS) $(filter %.s %.c,$^) -o $@

