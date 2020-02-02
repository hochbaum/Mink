#ifndef MINK_KBD_H
#define MINK_KBD_H

#include <stdint.h>

#define IRQ_KBD 33
#define KBD_PORT 0x60
#define KBD_RELEASE 0x80

void kbd_init();

void kbd_map_key(uint8_t scancode, uint8_t ascii);

#define KBD_MAP(sc, ascii) kbd_map_key(sc, ascii);
#define KBD_MAP_UPPER(sc, ascii) kbd_map_key(sc+128, ascii);

#endif //MINK_KBD_H
