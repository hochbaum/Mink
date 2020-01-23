#include <stdio.h>
#include <mink/driver/fb.h>

void kmain64() {
    fb_init();
    kprintf("[mink_kernel] Initialized framebuffer\n");
    fb_put_str_at("[mink_kernel][ERROR] kmain returned", -1, -1, FB_COLOR(FB_LIGHT_RED, FB_BLACK));
}
