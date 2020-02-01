#ifndef MINK_8259PIC_H
#define MINK_8259PIC_H

#include <stdint.h>

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_DATA (PIC1+1)
#define PIC2_DATA (PIC2+1)

#define ICW1_ICW4    0x1
#define ICW1_INIT    0x10

#define ICW1_X86     0x1

#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACK 0x20

void pic_ack(uint32_t interrupt);

void pic_remap_irq(uint8_t offset1, uint8_t offset2);

#endif //MINK_8259PIC_H
