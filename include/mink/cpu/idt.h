#ifndef MINK_IDT_H
#define MINK_IDT_H

#include <stdint.h>
#include <stddef.h>
#include <mink/cpu/state.h>
#include <mink/cpu/gdt.h>
#include <mink/cpu/pl.h>

#define IDT_MAX_ENTRIES 256

typedef struct idt {
    uint16_t limit;
    uint64_t address;
} __attribute__((packed)) idt_t;

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t IGNORED
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t IGNORED
} __attribute__((packed)) idt_entry_t;

typedef void (*isr_t)(registers_t registers);

void idt_init();

void idt_catch_all(registers_t registers);

void idt_entry_set(uint16_t index, uint64_t base, uint16_t selector, uint8_t attributes);

void idt_handler_add(uint32_t interrupt, isr_t handler);

void __native_lidt(idt_t *idt);

void __native_sti();

void __native_cli();

#define ISR_DECLARE(n) extern void _isr_##n();
#define ISR_SET(n) idt_entry_set(n, (uint64_t) &_isr_##n, 0x8, 0x8E);

ISR_DECLARE(0)
ISR_DECLARE(1)
ISR_DECLARE(2)
ISR_DECLARE(3)
ISR_DECLARE(4)
ISR_DECLARE(5)
ISR_DECLARE(6)
ISR_DECLARE(7)
ISR_DECLARE(8)
ISR_DECLARE(9)
ISR_DECLARE(10)
ISR_DECLARE(11)
ISR_DECLARE(12)
ISR_DECLARE(13)
ISR_DECLARE(14)
ISR_DECLARE(15)
ISR_DECLARE(16)
ISR_DECLARE(17)
ISR_DECLARE(18)
ISR_DECLARE(19)
ISR_DECLARE(20)
ISR_DECLARE(21)
ISR_DECLARE(22)
ISR_DECLARE(23)
ISR_DECLARE(24)
ISR_DECLARE(25)
ISR_DECLARE(26)
ISR_DECLARE(27)
ISR_DECLARE(28)
ISR_DECLARE(29)
ISR_DECLARE(30)
ISR_DECLARE(31)

#endif //MINK_IDT_H
