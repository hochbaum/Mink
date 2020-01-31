#ifndef MINK_STRING_H
#define MINK_STRING_H

#include <stdint.h>
#include <stddef.h>

/**
 * Converts an 8byte number to a string into the provided buffer using decimal representation.
 *
 * @param buffer String buffer to use.
 * @param num Number to convert.
 */
void itoa(char *buffer, signed long long num);

/**
 * Reverses the provided string.
 *
 * @param str String to reverse.
 */
void reverse(char *str);

/**
 * Returns the total length of a string.
 *
 * @param str String to check size of.
 * @return Size of the string.
 */
size_t strlen(const char *str);

void *memset(void *v, int c, size_t len);

#endif //MINK_STRING_H
