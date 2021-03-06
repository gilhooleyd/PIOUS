@ asm_utils.s
@
@ Description: Utility methods written in assembly.


@ Memory Read/Write convenience functions
.globl PUT32
PUT32:
	str r1, [r0]
	bx  lr

.globl PUT16
PUT16:
	strh r1, [r0]
	bx   lr

.globl PUT8
PUT8:
	strb r1, [r0]
	bx   lr

.globl GET32
GET32:
	ldr r0, [r0]
	bx  lr


@ Gets program counter value
.globl GETPC
GETPC:
	mov r0, lr
	bx  lr


@ Dummy function (for preventing code from
@ being optimized out by the compiler)
.globl dummy
dummy:
	bx lr
