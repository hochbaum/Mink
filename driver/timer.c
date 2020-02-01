#include <mink/driver/timer.h>
#include <stdint.h>
#include <mink/cpu/state.h>
#include <mink/driver/8259pic.h>
#include <mink/cpu/idt.h>
#include <mink/cpu/databus.h>

static uint64_t tick;

static void timer_handler(registers_t registers) {
    pic_ack(IRQ_TIMER);

    // unused.
    (void)registers;

    tick++;
}

void timer_init() {
    idt_handler_add(IRQ_TIMER, &timer_handler);

    uint32_t div = TIMER / TIMER_FREQ;
    uint8_t low = div & 0xFF;
    uint8_t high = (div >> 8) & 0xFF;

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}
