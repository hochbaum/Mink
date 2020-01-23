GRUB2_MAGIC_NUMBER 	EQU 0xE85250D6
GRUB2_ARCH		EQU 0                  ; architecture, 0 stands for i386.
GRUB2_HEADER_SIZE	EQU _grub2_multiboot_header_end - _grub2_multiboot_header_begin
GRUB2_CHECKSUM		EQU (0x100000000 - (GRUB2_MAGIC_NUMBER + GRUB2_ARCH + GRUB2_HEADER_SIZE))

; the multiboot header MUST be located in the first 4KB of the kernel.
; the .boot section is set inside the LD linker script just for that.
SECTION .boot
_grub2_multiboot_header_begin:
    ; this is where the multiboot header for GRUB2 starts.
    ; the specification can be found at https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html.
    ; we start with the magic number, followed by the architecture ID (0 for i386 mode).
    ; GRUB2 also wants the total size of the header, which is why I used the 'begin' and
    ; 'end' labels here. The last things are a checksum of all these and the header tag
    ; containing type, flags and size of the tag.
    DD GRUB2_MAGIC_NUMBER
    DD GRUB2_ARCH
    DD GRUB2_HEADER_SIZE
    DD GRUB2_CHECKSUM

    ; header tag.
    DW 0                            ; type.
    DW 0                            ; flags.
    DD 8                            ; size.
_grub2_multiboot_header_end:

SECTION .text
BITS 32
GLOBAL _start
_start:
    ; GRUB2 boots us into protected (32bit) mode. Any 64bit instructions would crash the
    ; processor right now. In order to enter long mode, we have to disable paging first,
    ; which is already done (GRUB2 does not enable paging for us). The next step is to OR
    ; in the PAE-enable-bit in CR4. We then have to load CR3 with the address of our PML4.
    ; After that the EFER.LIME flag has to be set in our model-specific register. The last
    ; thing to do is finally enabling paging, which will put us into long mode.

    MOV ESP, _stack_begin           ; set up our stack.

    CALL _check_long_mode           ; check if the processor is running 64bit at all.
    CALL _load_page_tables          ; initialize the page tables using huge pages.
    CALL _enter_long_mode

    LGDT [_GDT64.Pointer]           ; FINALLY load the GDT and enter long mode!

    EXTERN _enter_kmain64
    JMP _GDT64.Code:_enter_kmain64

CPUID_LONG_BIT EQU 1 << 29

; this function checks whether long mode is available or not by using the CPUID.
; if long mode is not available, it jumps to the .Unsupported label, loads EAX
; with 0xDEADBEEF and loops forever so that the CPU is not going to crash.

; (TODO: handle 32bit processors a bit better, maybe print some text?)
_check_long_mode:
    ; call CPUID with the value you see below in order to check if CPUID is available.
    ; if not, EAX is not going to be updated and we can already be sure that the
    ; processor is not supporting long mode.
    MOV EAX, 0x80000000
    CPUID
    CMP EAX, 0x80000001
    JB 	.Unsupported

    ; call CPUID again and check for the long mode bit.
    ; if it's not set, long mode is unsupported.
    MOV EAX, 0x80000001
    CPUID
    TEST EDX, CPUID_LONG_BIT
    JZ 	.Unsupported

    RET
.Unsupported:
    ; just load EAX with 0xDEADBEEF and hang forever for debugging reasons.
    ; I might replace this with some text or so later.
    MOV EAX, 0xDEADBEEF
    JMP $

PAGE_DEFAULT  		EQU 0b00000011
PAGE_DEFAULT_HUGE	EQU 0b10000011

; this function maps the memory paging tables together.
_load_page_tables:
    ; this maps the P3 table to the first P4 table entry and sets it to
    ; 'present' and 'writable'.
    MOV EAX, _p3_table
    OR	EAX, PAGE_DEFAULT
    MOV [_p4_table], EAX

    ; this maps the P2 table to the first P3 table entry and sets it to
    ; 'present' and 'writable'
    MOV EAX, _p2_table
    OR	EAX, PAGE_DEFAULT
    MOV [_p3_table], EAX

    XOR ECX, ECX                    ; clear ECX just to be sure it does not contain any junk.
.Loop:
    MOV EAX, 0x200000               ; set size to 2MB because we are using HUGE pages.
    MUL ECX                         ; multiply with ECX (loop counter) so we can iterate.
    OR  EAX, PAGE_DEFAULT_HUGE      ; use same flags as above BUT also shift in the huge flag bit!
    MOV [_p2_table + ECX * 8], EAX  ; map the entry using ECX as the loop counter.

    INC ECX                         ; increment ECX (i++).
    CMP ECX, 512                    ; continue the loop 512 times (i < 512).
    JNE .Loop
    RET

CONTROL_FLAG_PAE_ENABLE 	EQU 1 << 5
CONTROL_FLAG_PAGING_ENABLE	EQU 1 << 31
MSR_ADDRESS			EQU 0xC0000080
MSR_FLAG_LONG_MODE		EQU 1 << 8

; this function enters long mode by enabling paging and setting the long mode bit
; inside the model-specific register.
_enter_long_mode:
    ; tell the CPU which paging table to use by loading its address into control register 3.
    MOV EAX, _p4_table
    MOV CR3, EAX

    ; enable Physical Address Extension by setting bit 5 of CR4.
    MOV EAX, CR4
    OR  EAX, CONTROL_FLAG_PAE_ENABLE
    MOV CR4, EAX

    ; set the long mode bit in the model-specific register.
    MOV ECX, MSR_ADDRESS
    RDMSR
    OR  EAX, MSR_FLAG_LONG_MODE
    WRMSR

    ; enable paging in CR0.
    MOV EAX, CR0
    OR  EAX, CONTROL_FLAG_PAGING_ENABLE
    MOV CR0, EAX

    RET

GDT64_FLAG_CODE_SEG	EQU 1 << 53 ; the segment is the code segment.
GDT64_ACCESS_PRESENT	EQU 1 << 47 ; the segment is present.
GDT64_ACCESS_CODE_DATA	EQU 1 << 44 ; the segment is either the code or data segment.
GDT64_ACCESS_EXECUTABLE EQU 1 << 43 ; the segment is executable.
GDT64_ACCESS_READ_WRITE EQU 1 << 41 ; the segment is readable/writable.

SECTION .rodata
_GDT64:
    DQ 0                            ; null descriptor.
.Code: EQU $ - _GDT64
    DQ GDT64_FLAG_CODE_SEG | GDT64_ACCESS_PRESENT | GDT64_ACCESS_CODE_DATA | GDT64_ACCESS_EXECUTABLE | GDT64_ACCESS_READ_WRITE
.Data: EQU $ - _GDT64
    DQ GDT64_ACCESS_PRESENT | GDT64_ACCESS_CODE_DATA | GDT64_ACCESS_READ_WRITE
.Pointer:
    DW $ - _GDT64 - 1
    DQ _GDT64

SECTION .bss
ALIGN 4096
; set up our memory paging tables in the BSS (uninitialized data) section.
; GRUB2 will zero this out for us!
_p4_table:
    RESB 4096
_p3_table:
    RESB 4096
_p2_table:
    RESB 4096

_stack_end:
    RESB 64
_stack_begin:
