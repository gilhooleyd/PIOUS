@ entry.s
@
@ Description: Entry point for the kernel


@ Entry point for the kernel
.globl _start
_start:
	
	@ Reserve 0x8000 space for the stack
	mov sp, #0x8000

	@ Start the kernel proper
	bl  kernel_main

	
@ In the event that kernel_main returns, loop forever
hang:
	b   hang

