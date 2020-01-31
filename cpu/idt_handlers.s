%MACRO no_errno_handler 1
GLOBAL _isr_%1
_isr_%1:
	CLI
	PUSH QWORD 0
	PUSH QWORD %1
	JMP  _idt_stub
%ENDMACRO

%MACRO errno_handler 1
GLOBAL _isr_%1
_isr_%1:
	CLI
	PUSH QWORD %1
	JMP  _idt_stub
%ENDMACRO

SECTION .text
GLOBAL _idt_stub
EXTERN idt_catch_all
_idt_stub:
	PUSH RDI
	PUSH RSI
	PUSH RDX
	PUSH RCX
	PUSH RAX
	PUSH R8
	PUSH R9
	PUSH R10
	PUSH R11
	PUSH RBX
	PUSH RBP
	PUSH R12
	PUSH R13
	PUSH R14
	PUSH R15

	XOR RAX, RAX	; clear out RAX.
	MOV AX, DS		; push data segment.
	PUSH RAX

	MOV RDI, RSP	; move stack pointer into RDI, as it's used for the first argument.

	CALL idt_catch_all 	; call the IDT main handler.

	POP RAX

	POP R15
	POP R14
	POP R13
	POP R12
	POP RBP
	POP RBX
	POP R11
	POP R10
	POP R9
	POP R8
	POP RAX
	POP RCX
	POP RDX
	POP RSI
	POP RDI

	ADD RSP, 16		; clean up the pushes we made in the interrupt handler.
	IRETQ

no_errno_handler 0
no_errno_handler 1
no_errno_handler 2
no_errno_handler 3
no_errno_handler 4
no_errno_handler 5
no_errno_handler 6
no_errno_handler 7
errno_handler 8
no_errno_handler 9
errno_handler 10
errno_handler 11
errno_handler 12
errno_handler 13
errno_handler 14
no_errno_handler 15
no_errno_handler 16
errno_handler 17
no_errno_handler 18
no_errno_handler 19
no_errno_handler 20
no_errno_handler 21
no_errno_handler 22
no_errno_handler 23
no_errno_handler 24
no_errno_handler 25
no_errno_handler 26
no_errno_handler 27
no_errno_handler 28
no_errno_handler 29
no_errno_handler 30
no_errno_handler 31
