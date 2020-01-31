#ifndef MINK_GDT_H
#define MINK_GDT_H

#include <stdint.h>
#include <stddef.h>
#include <mink/cpu/pl.h>

/* 2 entries for code + data segments and one for the null descriptor.
 * This is gonna be extended as soon as user mode is available. */
#define GDT_MAX_ENTRIES 3

/**
 * The global descriptor table.
 *
 * Limit is the 2 bytes sized size of the entries.
 * Address is the address of the entries.
 */
typedef struct gdt {
    uint16_t limit;
    uint64_t address;
} __attribute__((packed)) gdt_t;

/**
 * A GDT entry is 64 bits long.
 *
 * Since a lot of the entry is ignored on X86-64, I decided to not use a struct but a uint64_t which can be configured
 * using bit manipulation for this.
 */
typedef uint64_t gdt_entry_t;

/**
 * Initializes and loads the global descriptor table with 3 entries for null descriptor, code and data segment (kernel).
 * The return value is the address of the code segment, stored in RAX for a far jump.
 *
 * This function is called from assembly code in entry.s.
 *
 * @return Address of the code segment.
 */
uint64_t gdt_init();

/**
 * Adds a gdt_entry_t to the global descriptor table.
 *
 * Index is the index inside the entry array.
 * Flags are bit shifted flags that, for example, determine the type of the entry. Take a look at the defines below.
 * Privilege determines whether the segment is for kernel or for user mode. It could also have been passed via flags
 *           but I made it a parameter so that you get the idea that it's REQUIRED.
 *
 * @param index Index of the entry.
 * @param flags Bit flags of the entry.
 * @param privilege Privilege level of the entry.
 */
void gdt_entry_add(uint8_t index, uint64_t flags, uint8_t privilege);

/**
 * Returns the global descriptor entry at the specified index.
 *
 * @param index Index of the entry.
 * @return Entry at the specified index.
 */
gdt_entry_t gdt_entry_get(uint8_t index);

/* Some of the global descriptor table entry flags.
 * These are by far not all but pretty much all needed for 64bit computing. */
#define GDT_CODE    (1UL << 53)
#define GDT_PRESENT (1UL << 47)
#define GDT_TYPE    (1UL << 44)
#define GDT_EXEC    (1UL << 43)
#define GDT_RW      (1UL << 41)

/**
 * Performs the LGDT assembly instruction which loads the provided global descriptor table.
 *
 * @param gdt_ptr Pointer to the global descriptor table.
 */
void __native_lgdt(gdt_t *gdt_ptr);

#endif //MINK_GDT_H
