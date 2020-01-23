#include <stdio.h>
#include <stdarg.h>
#include <mink/driver/fb.h>

void kprintf(char *fmt, ...) {
    va_list ap;
    char *sval;

    va_start(ap, fmt);
    for (char *p = fmt; *p; ++p) {
        if (*p != '%') {
            fb_put_char_at(*p, -1, -1, FB_DEFAULT);
            continue;
        }

        switch (*++p) {
            case 's':
                sval = va_arg(ap, char*);
                fb_put_str_at(sval, -1, -1, FB_DEFAULT);
                break;

            case '%':
                fb_put_char_at('%', -1, -1, FB_DEFAULT);
                break;
        }
    }
    va_end(ap);
}
