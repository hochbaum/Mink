SECTION .text
BITS 64
GLOBAL _enter_kmain64
_enter_kmain64:
	EXTERN kmain64	; load the kmain64 symbol (defined in kmain.c)
	CALL kmain64	; enter the C code.
	JMP $		; loop forever.
