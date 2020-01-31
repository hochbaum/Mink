SECTION .text
BITS 64
GLOBAL _enter_kmain64
_enter_kmain64:
	EXTERN _stack_begin
	MOV RSP, _stack_begin	; set up our stack.

	EXTERN kmain64			; load the kmain64 symbol (defined in kmain.c)
	CALL kmain64			; enter the C code.

	JMP $					; loop forever.
