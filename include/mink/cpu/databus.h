#ifndef MINK_DATABUS_H
#define MINK_DATABUS_H

#include <stdint.h>

/**
 * This function sends the specified byte to a specific I/O bus address.
 *
 * @param port Address of the I/O port.
 * @param data Byte to send.
 */
void outb(uint16_t port, uint8_t data);

/**
 * This function reads a byte from the specified I/O bus address.
 *
 * @param port Address of the I/O port.
 * @return Read byte.
 */
uint8_t inb(uint16_t port);

#endif //MINK_DATABUS_H
