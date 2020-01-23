#ifndef MINK_STDIO_H
#define MINK_STDIO_H

/**
 * A very basic implementation of printf.
 * So far only string replacement is supported.
 *
 * @param fmt Format.
 * @param ... Replacements.
 */
void kprintf(char *fmt, ...);

#endif //MINK_STDIO_H
