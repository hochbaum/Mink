#ifndef MINK_PL_H
#define MINK_PL_H

/* The privilege levels for a descriptor entry. Bits 45 and 46 are reserved for them.
 * 1 stands for level 0, which is the kernel mode whilst 3 stands for the user mode.
 *
 * Note that PL3 is shifted one bit further so it's actually positioned properly. */
#define PL0_KERNEL 1UL
#define PL3_USER   3UL

#endif //MINK_PL_H
