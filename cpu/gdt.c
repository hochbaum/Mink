#include <mink/cpu/gdt.h>

static gdt_t *gdt;
static gdt_entry_t gdt_entries[GDT_MAX_ENTRIES];

uint64_t gdt_init() {
    gdt->limit = (sizeof(gdt_entries) * GDT_MAX_ENTRIES) - 1;
    gdt->address = (uint64_t) &gdt_entries;

    // null descriptor.
    gdt_entry_add(0, 0, 0);
    // code segment.
    gdt_entry_add(1, GDT_CODE | GDT_PRESENT | GDT_TYPE | GDT_EXEC | GDT_RW, PL0_KERNEL);
    // data segment.
    gdt_entry_add(2, GDT_PRESENT | GDT_TYPE | GDT_RW, PL0_KERNEL);

    __native_lgdt(gdt);
    return (uint64_t) &gdt_entries[1];
}

void gdt_entry_add(uint8_t index, uint64_t flags, uint8_t privilege) {
    if (index != 0) {
        flags |= 45 << privilege;
    }
    gdt_entries[index] = flags;
}

gdt_entry_t gdt_entry_get(uint8_t index) {
    return gdt_entries[index];
}

void __native_lgdt(gdt_t *gdt_ptr) {
    asm("LGDT (%0)" : : "r" (gdt_ptr));
}
