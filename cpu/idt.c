#include <mink/cpu/idt.h>
#include <mink/cpu/databus.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <mink/driver/fb.h>

static idt_t idt;
static idt_entry_t idt_entries[IDT_MAX_ENTRIES];
static isr_t routines[IDT_MAX_ENTRIES];

static const char *messages[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",

        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved",
        "Intel Reserved"
};

void idt_init() {
    idt.limit = (sizeof(idt_entry_t) * IDT_MAX_ENTRIES) - 1;
    idt.address = (uint64_t) &idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * IDT_MAX_ENTRIES);

    ISR_SET(0)
    ISR_SET(1)
    ISR_SET(2)
    ISR_SET(3)
    ISR_SET(4)
    ISR_SET(5)
    ISR_SET(6)
    ISR_SET(7)
    ISR_SET(8)
    ISR_SET(9)
    ISR_SET(10)
    ISR_SET(11)
    ISR_SET(12)
    ISR_SET(13)
    ISR_SET(14)
    ISR_SET(15)
    ISR_SET(16)
    ISR_SET(17)
    ISR_SET(18)
    ISR_SET(19)
    ISR_SET(20)
    ISR_SET(21)
    ISR_SET(22)
    ISR_SET(23)
    ISR_SET(24)
    ISR_SET(25)
    ISR_SET(26)
    ISR_SET(27)
    ISR_SET(28)
    ISR_SET(29)
    ISR_SET(30)
    ISR_SET(31)

    __native_lidt(&idt);
}

void idt_entry_set(uint16_t index, uint64_t base, uint16_t selector, uint8_t attributes) {
    idt_entries[index].offset_low = base & 0xFFFF;
    idt_entries[index].selector = selector;
    idt_entries[index].attributes = attributes;
    idt_entries[index].offset_middle = (base >> 16) & 0xFFFF;
    idt_entries[index].offset_high = (base >> 32) & 0xFFFF;
}

void idt_catch_all(registers_t registers) {
    uint32_t index = registers.intno;
    // not an IRQ if ID is less than 32 since we remapped it.
    if (index < 32) {
        uint8_t red_on_black = FB_COLOR(FB_LIGHT_RED, FB_BLACK);

        fb_put_str_at("[mink_kernel][ERROR] CPU fired exception: ", -1, -1, red_on_black);
        fb_put_str_at(messages[index], -1, -1, red_on_black);

        asm("HLT");
        return;
    }

    isr_t isr = routines[index];
    if (isr != NULL) {
        isr(registers);
    }
}

void idt_handler_add(uint32_t interrupt, isr_t handler) {
    if (interrupt < IDT_MAX_ENTRIES && routines[interrupt] == NULL) {
        routines[interrupt] = handler;
    }
}

void __native_lidt(idt_t *idt) {
    asm("LIDT (%0)" : : "r" (idt));
}

void __native_sti() {
    asm("STI");
}

void __native_cli() {
    asm("CLI");
}
