#include <stdio.h>
#include <stddef.h>
#include <mink/driver/fb.h>
#include <mink/cpu/idt.h>
#include <mink/cpu/gdt.h>
#include <mink/driver/timer.h>

void kmain64() {
    fb_init();
    kprintf("[mink_kernel] Initialized framebuffer\n");

    gdt_init();
    kprintf("[mink_kernel] Initialized new GDT. Checking it:\n");
    for (int i = 0; i < 3; i++) {
        gdt_entry_t entry = gdt_entry_get(i);
        if (entry == 0) {
            kprintf("   - found: null descriptor\n");
        } else if (BIT(entry, 53)) {
            kprintf("   - found: code segment (%d)\n", entry);
        } else if (!BIT(entry, 53) && BIT(entry, 44)) {
            kprintf("   - found: data segment (%d)\n", entry);
        }
    }

    idt_init();
    kprintf("[mink_kernel] Initialized IDT, now handling interrupts\n");

    asm("INT $1");
    asm("INT $18");

    timer_init();
    kprintf("[mink_kernel] Initialized the timer\n");

    fb_put_str_at("[mink_kernel][ERROR] kmain returned", -1, -1, FB_COLOR(FB_LIGHT_RED, FB_BLACK));
}
