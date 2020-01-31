#ifndef MINK_STDIO_H
#define MINK_STDIO_H

/**
 * A very basic implementation of printf.
 *
 * %s: Replaces a string.
 * %d: Replaces a decimal number.
 * %%: Escapes a % sign.
 *
 * @param fmt Format.
 * @param ... Replacements.
 */
void kprintf(char *fmt, ...);

#endif //MINK_STDIO_H
