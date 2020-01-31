#include <string.h>

void itoa(char *buffer, signed long long num) {
    int i = 0;
    do {
        buffer[i++] = num % 10 + '0';
    } while ((num /= 10) > 0);

    if (num < 0) {
        buffer[i++] = '-';
    }

    buffer[i] = 0;
    reverse(buffer);
}

void reverse(char *str) {
    int len = strlen(str) - 1;
    for (int i = 0; i < len; i++, len--) {
        char c = str[i];
        str[i] = str[len];
        str[len] = c;
    }
}

size_t strlen(const char *str) {
    size_t i = 0;
    for (; *str; i++, str++);
    return i;
}

void *memset(void *v, int c, size_t len) {
    unsigned char *dest = v;
    while (len-- > 0) {
        *dest++ = (unsigned char) c;
    }
    return v;
}
