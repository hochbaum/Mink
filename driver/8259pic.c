#include <mink/driver/8259pic.h>
#include <mink/cpu/databus.h>

uint8_t isirq(uint32_t interrupt) {
    return interrupt >= PIC1_START_INTERRUPT && interrupt <= PIC2_END_INTERRUPT;
}

void pic_ack(uint32_t interrupt) {
    if (isirq(interrupt)) {
        // the IRQ is handled by PIC1 because it's one of the first 8.
        if (interrupt < PIC2_START_INTERRUPT) {
            outb(PIC1, PIC_ACK);
        // the IRQ is handled by PIC2 because it's one of the last 8.
        } else {
            outb(PIC2, PIC_ACK);
        }
    }
}

void pic_remap_irq(uint8_t offset1, uint8_t offset2) {
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1, ICW1_INIT | ICW1_ICW4);
    outb(PIC2, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);

    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW1_X86);
    outb(PIC2_DATA, ICW1_X86);

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}
