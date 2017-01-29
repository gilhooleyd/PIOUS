# PiOuS Makefile
#
# TODO: Having a DEBUG flag that disabled compiler optimization
#       and enabled a debugf function or something would be nice.


#----- User Variables (EDIT THESE) -----#

ARMGNU ?= arm-none-eabi

PI_CC_OPS   := -Wall -nostdlib -nostartfiles -ffreestanding \
         	   -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a \
         	   -mtune=cortex-a7
PI_S_OPS    :=
QEMU_CC_OPS := -mcpu=arm926ej-s -g
QEMU_S_OPS  := -mcpu=arm926ej-s

PI_BUILD_DIR   := pi_build
QEMU_BUILD_DIR := qemu_build

PI_SRC   := entry.s main.c asm_utils.s framebuffer.c led.c mbox.c \
			screen.c utils.c
QEMU_SRC := qemu-entry.s qemu-test.c


#----- Derived Variables (LEAVE ALONE) -----#

PI_INTER_1   := $(PI_SRC:.c=.o)
PI_INTER_2   := $(PI_INTER_1:.s=.o)
QEMU_INTER_1 := $(QEMU_SRC:.c=.o)
QEMU_INTER_2 := $(QEMU_INTER_1:.s=.o)

PI_BUILD_OBJS   := $(addprefix $(PI_BUILD_DIR)/, $(PI_INTER_2))
QEMU_BUILD_OBJS := $(addprefix $(QEMU_BUILD_DIR)/, $(QEMU_INTER_2))

PI_TARGET   := $(PI_BUILD_DIR)/kernel
QEMU_TARGET := $(QEMU_BUILD_DIR)/kernel


#----- Make Commands -----#

all: pi qemu test

pi: kernel7.img

qemu: kernel.bin

test: test.o
	gcc test.o -o test

depend: .pi-depend .qemu-depend

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s
	rm -f .*depend
	rm -f .*.bak
	rm -f test
	rm -rf $(PI_BUILD_DIR)
	rm -rf $(QEMU_BUILD_DIR)


#----- Products -----#

kernel7.img: $(PI_BUILD_OBJS) linkscript
	$(ARMGNU)-ld $(PI_BUILD_OBJS) -T linkscript -o $(PI_TARGET).elf
	$(ARMGNU)-objdump -D $(PI_TARGET).elf > $(PI_TARGET).list
	$(ARMGNU)-objcopy $(PI_TARGET).elf -O ihex $(PI_TARGET).hex
	$(ARMGNU)-objcopy $(PI_TARGET).elf -O binary kernel7.img

kernel.bin: $(QEMU_BUILD_OBJS) qemu-linkscript
	$(ARMGNU)-ld $(QEMU_BUILD_OBJS) -T qemu-linkscript -o $(QEMU_TARGET).elf
	$(ARMGNU)-objdump -D $(QEMU_TARGET).elf > $(QEMU_TARGET).list
	$(ARMGNU)-objcopy $(QEMU_TARGET).elf -O ihex $(QEMU_TARGET).hex
	$(ARMGNU)-objcopy $(QEMU_TARGET).elf -O binary kernel.bin


#----- Track Dependencies -----#

.pi-depend: $(PI_SRC)
	rm -f .pi-depend
	$(ARMGNU)-gcc $(PI_CC_OPS) -MM $^ > ./.pi-depend
	sed -i.bak 's .*.o $(PI_BUILD_DIR)/& ' .pi-depend
	rm -f .pi-depend.bak

.qemu-depend: $(QEMU_SRC)
	rm -f .qemu-depend
	$(ARMGNU)-gcc $(QEMU_CC_OPS) -MM $^ > ./.qemu-depend
	sed -i.bak 's .*.o $(QEMU_BUILD_DIR)/& ' .qemu-depend
	rm -f .qemu-depend.bak

-include .pi-depend .qemu-depend


#----- Object Files -----#

$(PI_BUILD_DIR)/%.o: %.c | $(PI_BUILD_DIR)
	$(ARMGNU)-gcc $(PI_CC_OPS) -c $< -o $@

$(PI_BUILD_DIR)/%.o: %.s | $(PI_BUILD_DIR)
	$(ARMGNU)-as $(PI_S_OPS) $< -o $@

$(QEMU_BUILD_DIR)/%.o: %.c | $(QEMU_BUILD_DIR)
	$(ARMGNU)-gcc $(QEMU_CC_OPS) -c $< -o $@

$(QEMU_BUILD_DIR)/%.o: %.s | $(QEMU_BUILD_DIR)
	$(ARMGNU)-as $(QEMU_S_OPS) $< -o $@

test.o: test.c
	gcc -g -c test.c -o test.o


#----- Build Directories -----$

$(PI_BUILD_DIR):
	mkdir -p $(PI_BUILD_DIR)

$(QEMU_BUILD_DIR):
	mkdir -p $(QEMU_BUILD_DIR)
