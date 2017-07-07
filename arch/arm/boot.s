@ boot.s
@
@ Description: Entry point for the kernel


@ Define this as its own section. Used in linkscript
@ to ensure this in the first portion of the binary.
.section ".text.boot"


@ Entry point for the kernel
 .globl _start
 _start:
 	
 	@ Reserve 0x8000 space for the stack
 	mov sp, #0x8000
 
 	@ Clear out bss
 	ldr r4, =__bss_start
 	ldr r9, =__bss_end
 	mov r5, #0
 	mov r6, #0
 	mov r7, #0
 	mov r8, #0
 	b   compare
 
 loop:
 	@ Store zeroes and increment
 	stmia r4!, {r5-r8}
 
 compare:
 	@ If we haven't reached bss_end, loop back
 	cmp r4, r9
 	blo loop
 	
 	@ Start the kernel proper
 	bl kernel_main
 
 	
 @ In the event that kernel_main returns, loop forever
 halt:
 	wfe
 	b   halt
