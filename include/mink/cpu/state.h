#ifndef MINK_CPU_H
#define MINK_CPU_H

#include <stdint.h>

typedef struct registers {
    // segment registers:
    volatile uint64_t DS;

    // general purpose registers:
    volatile uint64_t R15, R14, R13, R12, RBP, RBX, R11, R10, R9, R8, RAX, RCX, RDX, RSI, RDI;

    // IDT response:
    volatile uint64_t intno, errno;

    // stack state:
    volatile uint64_t RIP, CS, RFLAGS, USERRSP, SS;
} __attribute__((packed)) registers_t;

#endif //MINK_CPU_H
