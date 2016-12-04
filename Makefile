# PiOuS Makefile
#
# TODO: Clean this up, and filter out what's unnecessary.
# 		Also, having a DEBUG flag that disabled compiler optimization
# 		and enabled a debugf function or something would be nice.


#----- Macros and options -----#

ARMGNU ?= arm-none-eabi

CCOPS  = -Wall -O2 -nostdlib -nostartfiles -ffreestanding \
		 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a \
		 -mtune=cortex-a7
CLOPS  = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
OOPS   = -std-compile-opts


# WARNING:
# (Lance) Beyond here, I dont know what the f*** is going on. Send help.


#----- Make Commands -----#

all: gcc clang

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s


#----- Products -----#

gcc: kernel.img

clang: start.bin

kernel.img: loader start.o globals.o framebuffer.o led.o mbox.o utils.o
	$(ARMGNU)-ld start.o globals.o framebuffer.o led.o mbox.o utils.o \
		-T loader -o start.elf
	$(ARMGNU)-objdump -D start.elf > start.list
	$(ARMGNU)-objcopy start.elf -O ihex start.hex
	$(ARMGNU)-objcopy start.elf -o binary kernel7.img

start.bc: start.c globals.h framebuffer.h led.h mbox.h utils.h image_data.h
	clang $(CLOPS) -c start.c -o start.bc

# (Lance) What the hell is this?!?
periph.bc: periph.c
	clang $(CLOPS) -c periph.c -o periph.bc

start.clang.elf: loader start.bc periph.bc globals.o framebuffer.o \
				 led.o mbox.o utils.o
	llvm-link periph.bc start.bc -o start.nopt.bc
	opt $(OOPS) start.nopt.bc -o start.opt.bc
	llc $(LLCOPS) -filetype=obj start.opt.bc -o start.clang.o
	$(ARMGNU)-ld -o start.clang.elf -T loader start.clang.o globals.o \
		framebuffer.o led.o mbox.o utils.o
	$(ARMGNU)-objdump -D start.clang.elf > start.clang.list

start.bin: start.clang.elf
	$(ARMGNU)-objcopy start.clang.elf start.clang.bin -O binary


#----- Object Files -----#

start.o: start.c globals.h framebuffer.h led.h mbox.h utils.h image_data.h
	$(ARMGNU)-gcc $(CCOPS) -c start.c -o start.o

globals.o: globals.s
	$(ARMGNU)-as globals.s -o globals.o

framebuffer.o: framebuffer.c framebuffer.h globals.h mbox.h
	$(ARMGNU)-gcc $(CCOPS) -c framebuffer.c -o framebuffer.o

led.o: led.c led.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c led.c -o led.o

mbox.o: mbox.c mbox.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c mbox.c -o mbox.o

utils.o: utils.c utils.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c utils.c -o utils.o
