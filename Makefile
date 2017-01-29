# PiOuS Makefile
#
# TODO: Having a DEBUG flag that disabled compiler optimization
#       and enabled a debugf function or something would be nice.


#----- User Variables (EDIT THESE) -----#

ARMGNU ?= arm-none-eabi

ARM_CC_OPS := -Wall -nostdlib -nostartfiles -ffreestanding \
         	  -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a \
         	  -mtune=cortex-a7
ARM_S_OPS  :=
X86_CC_OPS := -m32 -std=gnu99 -ffreestanding -O2 -Wextra -Wall
X86_S_OPS  := --32

INC_DIR := include

ARM_SRC_DIR := arch/arm
X86_SRC_DIR := arch/x86

PI_BUILD_DIR   := build_pi
QEMU_BUILD_DIR := build_qemu


PI_SRC   := $(ARM_SRC_DIR)/boot.s main.c $(ARM_SRC_DIR)/asm_utils.s framebuffer.c led.c mbox.c \
			screen.c utils.c drivers/tty-stub.c
QEMU_SRC   := $(X86_SRC_DIR)/boot.s main.c  drivers/led-stub.c \
			utils.c drivers/vga-x86.c


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

kernel7.img: $(PI_BUILD_OBJS) $(ARM_SRC_DIR)/linker
	$(ARMGNU)-ld $(PI_BUILD_OBJS) -T $(ARM_SRC_DIR)/linker -o $(PI_TARGET).elf
	$(ARMGNU)-objdump -D $(PI_TARGET).elf > $(PI_TARGET).list
	$(ARMGNU)-objcopy $(PI_TARGET).elf -O ihex $(PI_TARGET).hex
	$(ARMGNU)-objcopy $(PI_TARGET).elf -O binary kernel7.img

kernel.bin: $(QEMU_BUILD_OBJS) $(X86_SRC_DIR)/linker
	ld -melf_i386 -T $(X86_SRC_DIR)/linker $(QEMU_BUILD_OBJS) -o $(QEMU_TARGET).elf
	objdump -D $(QEMU_TARGET).elf > $(QEMU_TARGET).list
	objcopy $(QEMU_TARGET).elf -O ihex $(QEMU_TARGET).hex
	objcopy $(QEMU_TARGET).elf -O binary kernel.bin


#----- Track Dependencies -----#

.pi-depend: $(PI_SRC)
	rm -f .pi-depend
	$(ARMGNU)-gcc $(ARM_CC_OPS) -I $(INC_DIR) -MM $^ > ./.pi-depend
	sed -i.bak 's .*.o $(PI_BUILD_DIR)/& ' .pi-depend
	rm -f .pi-depend.bak

.qemu-depend: $(QEMU_SRC)
	rm -f .qemu-depend
	gcc $(X86_CC_OPS) -I $(INC_DIR) -MM $^ > ./.qemu-depend
	sed -i.bak 's .*.o $(QEMU_BUILD_DIR)/& ' .qemu-depend
	rm -f .qemu-depend.bak

-include .pi-depend .qemu-depend


#----- Object Files -----#

$(PI_BUILD_DIR)/%.o: %.c | $(PI_BUILD_DIR)
	$(ARMGNU)-gcc $(ARM_CC_OPS) -I $(INC_DIR) -c $< -o $@

$(PI_BUILD_DIR)/%.o: %.s | $(PI_BUILD_DIR)
	$(ARMGNU)-as $(ARM_S_OPS) -I $(INC_DIR) $< -o $@

$(QEMU_BUILD_DIR)/%.o: %.c | $(QEMU_BUILD_DIR)
	gcc $(X86_CC_OPS) -I $(INC_DIR) -c $< -o $@

$(QEMU_BUILD_DIR)/%.o: %.s | $(QEMU_BUILD_DIR)
	as $(X86_S_OPS) -I $(INC_DIR) $< -o $@


#----- Build Directories -----$

$(PI_BUILD_DIR):
	mkdir -p $(PI_BUILD_DIR)/$(ARM_SRC_DIR)
	mkdir -p $(PI_BUILD_DIR)/drivers

$(QEMU_BUILD_DIR):
	mkdir -p $(QEMU_BUILD_DIR)/$(X86_SRC_DIR)
	mkdir -p $(QEMU_BUILD_DIR)/drivers
