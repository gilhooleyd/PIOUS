# PiOuS Makefile
#
# TODO: Having a DEBUG flag that disabled compiler optimization
#       and enabled a debugf function or something would be nice.


#----- User Variables (EDIT THESE) -----#

ARMGNU ?= arm-none-eabi-

PI_CC_OPS   := -Wall -nostdlib -nostartfiles -ffreestanding \
         	   -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a \
         	   -mtune=cortex-a7
PI_S_OPS    :=
QEMU_CC_OPS := -m32 -std=gnu99 -ffreestanding -O2 -Wextra -Wall
QEMU_S_OPS  := --32

PI_BUILD_DIR   := build_pi
QEMU_BUILD_DIR := build_qemu

PI_SRC   := arch/arm/boot.s main.c arch/arm/asm_utils.s framebuffer.c led.c mbox.c \
			screen.c utils.c
QEMU_SRC := arch/x86/boot.s arch/x86/kernel.c

INC := -I./include

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

all: pi qemu 

pi: kernel7.img

qemu: kernel.bin

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
	rm -rf $(PI_BUILD_DIR)
	rm -rf $(QEMU_BUILD_DIR)


#----- Products -----#

kernel7.img: $(PI_BUILD_OBJS) arch/arm/linker 
	$(ARMGNU)ld $(PI_BUILD_OBJS) -T arch/arm/linker -o $(PI_TARGET).elf
	$(ARMGNU)objdump -D $(PI_TARGET).elf > $(PI_TARGET).list
	$(ARMGNU)objcopy $(PI_TARGET).elf -O ihex $(PI_TARGET).hex
	$(ARMGNU)objcopy $(PI_TARGET).elf -O binary kernel7.img

kernel.bin: $(QEMU_BUILD_OBJS) arch/x86/linker
	ld -melf_i386 -T arch/x86/linker $(QEMU_BUILD_OBJS) -o $(QEMU_TARGET).elf
#	$(ARMGNU)-objdump -D $(QEMU_TARGET).elf > $(QEMU_TARGET).list
#	$(ARMGNU)-objcopy $(QEMU_TARGET).elf -O ihex $(QEMU_TARGET).hex
#	$(ARMGNU)-objcopy $(QEMU_TARGET).elf -O binary kernel.bin


#----- Track Dependencies -----#

.pi-depend: $(PI_SRC)
	rm -f .pi-depend
	$(ARMGNU)gcc $(PI_CC_OPS) $(INC) -MM $^ > ./.pi-depend
	sed -i.bak 's .*.o $(PI_BUILD_DIR)/& ' .pi-depend
	rm -f .pi-depend.bak

.qemu-depend: $(QEMU_SRC)
	rm -f .qemu-depend
	gcc $(QEMU_CC_OPS) $(INC) -MM $^ > ./.qemu-depend
	sed -i.bak 's .*.o $(QEMU_BUILD_DIR)/& ' .qemu-depend
	rm -f .qemu-depend.bak

-include .pi-depend .qemu-depend


#----- Object Files -----#

$(PI_BUILD_DIR)/%.o: %.c | $(PI_BUILD_DIR)
	$(ARMGNU)gcc $(PI_CC_OPS) $(INC) -c $< -o $@

$(PI_BUILD_DIR)/%.o: %.s | $(PI_BUILD_DIR)
	$(ARMGNU)as $(PI_S_OPS) $(INC) $< -o $@

$(QEMU_BUILD_DIR)/%.o: %.c | $(QEMU_BUILD_DIR)
	gcc $(QEMU_CC_OPS) $(INC) -c $< -o $@

$(QEMU_BUILD_DIR)/%.o: %.s | $(QEMU_BUILD_DIR)
	as $(QEMU_S_OPS) $(INC) $< -o $@

#----- Build Directories -----$

$(PI_BUILD_DIR):
	mkdir -p $(PI_BUILD_DIR)/arch/arm/

$(QEMU_BUILD_DIR):
	mkdir -p $(QEMU_BUILD_DIR)/arch/x86
