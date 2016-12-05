# PiOuS Makefile
#
# TODO: Clean this up, and filter out what's unnecessary.
#       Also, having a DEBUG flag that disabled compiler optimization
#       and enabled a debugf function or something would be nice.


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

# (Lance) What I think is happening:
# The Makefile never tries to build 'clang', and thus never makes it
# to building periph.bc or the .clang.elf file (which fails because
# periph.bc doesn't exist). From what I can tell, only building kernel.img
# is necessary for this to run correctly.


#----- Make Commands -----#

# (Lance) wha??
#gcc: kernel.img

#all: gcc clang

all: kernel.img

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

kernel.img: linkscript main.o asm_utils.o entry.o framebuffer.o \
            led.o mbox.o utils.o
	$(ARMGNU)-ld main.o asm_utils.o entry.o framebuffer.o led.o \
		mbox.o utils.o -T linkscript -o main.elf
	$(ARMGNU)-objdump -D main.elf > main.list
	$(ARMGNU)-objcopy main.elf -O ihex main.hex
	$(ARMGNU)-objcopy main.elf -O binary kernel7.img


# (Lance) beyond here is unnecessary?

#clang: main.bin
#
#main.bc: main.c globals.h framebuffer.h led.h mbox.h utils.h \
#         image_data.h teletext.h
#	clang $(CLOPS) -c main.c -o main.bc
#
## (Lance) What the hell is this?!?
#periph.bc: periph.c
#	clang $(CLOPS) -c periph.c -o periph.bc
#
#main.clang.elf: linkscript main.bc periph.bc asm_utils.o entry.o \
#                framebuffer.o led.o mbox.o utils.o
#	llvm-link periph.bc main.bc -o main.nopt.bc
#	opt $(OOPS) main.nopt.bc -o main.opt.bc
#	llc $(LLCOPS) -filetype=obj main.opt.bc -o main.clang.o
#	$(ARMGNU)-ld -o main.clang.elf -T linkscript main.clang.o \
#		asm_utils.o entry.o framebuffer.o led.o mbox.o utils.o
#	$(ARMGNU)-objdump -D main.clang.elf > main.clang.list
#
#main.bin: main.clang.elf
#	$(ARMGNU)-objcopy main.clang.elf main.clang.bin -O binary


#----- Object Files -----#

main.o: main.c globals.h framebuffer.h led.h mbox.h utils.h \
        image_data.h teletext.h
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

utils.o: utils.c utils.h globals.h
	$(ARMGNU)-gcc $(CCOPS) -c utils.c -o utils.o
