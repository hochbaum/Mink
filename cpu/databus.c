#include <mink/cpu/databus.h>

void outb(uint16_t port, uint8_t data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint8_t inb(uint16_t port) {
    uint8_t byte;
    asm("in %%dx, %%al" : "=a" (byte) : "d" (port));
    return byte;
}
