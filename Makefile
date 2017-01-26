# PiOuS Makefile
#
# TODO: Having a DEBUG flag that disabled compiler optimization
#       and enabled a debugf function or something would be nice.


#----- Macros and options -----#

ARMGNU ?= arm-none-eabi

CCOPS  = -Wall -nostdlib -nostartfiles -ffreestanding \
         -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a \
         -mtune=cortex-a7
CLOPS  = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
OOPS   = -std-compile-opts

#----- qemu commands -----#
q-entry: qemu/entry.s
	$(ARMGNU)-as -mcpu=arm926ej-s qemu/entry.s -o qemu/entry.o

q-img: qemu/test.c 
	$(ARMGNU)-gcc -c -mcpu=arm926ej-s -g qemu/test.c -o qemu/test.o 

q-elf: q-img q-entry
	$(ARMGNU)-ld -T qemu/linker.ld qemu/test.o qemu/entry.o -o qemu/kernel.elf

q-bin: q-elf
	$(ARMGNU)-objcopy -O binary qemu/kernel.elf qemu/kernel.bin

q-all: q-bin

#----- Make Commands -----#

all: kernel.img

test: test.o
	gcc test.o -o test

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s
	rm -f test


#----- Products -----#

kernel.img: linkscript main.o asm_utils.o entry.o framebuffer.o \
            led.o mbox.o screen.o utils.o
	$(ARMGNU)-ld entry.o main.o asm_utils.o framebuffer.o led.o \
		mbox.o screen.o utils.o -T linkscript -o main.elf
	$(ARMGNU)-objdump -D main.elf > main.list
	$(ARMGNU)-objcopy main.elf -O ihex main.hex
	$(ARMGNU)-objcopy main.elf -O binary kernel7.img


#----- Object Files -----#

main.o: main.c globals.h framebuffer.h led.h mbox.h screen.h \
        utils.h image_data.h
	$(ARMGNU)-gcc $(CCOPS) -c main.c -o main.o

asm_utils.o: asm_utils.s
	$(ARMGNU)-as asm_utils.s -o asm_utils.o

entry.o: entry.s
	$(ARMGNU)-as entry.s -o entry.o

framebuffer.o: framebuffer.c framebuffer.h globals.h mbox.h
	$(ARMGNU)-gcc $(CCOPS) -c framebuffer.c -o framebuffer.o

led.o: led.c led.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c led.c -o led.o

mbox.o: mbox.c mbox.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c mbox.c -o mbox.o

screen.o: screen.c screen.h teletext.h
	$(ARMGNU)-gcc $(CCOPS) -c screen.c -o screen.o

utils.o: utils.c utils.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c utils.c -o utils.o

test.o: test.c teletext.h
	gcc -c test.c -o test.o
