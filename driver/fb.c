#include <mink/driver/fb.h>
#include <mink/cpu/databus.h>

static uint8_t *fb = (uint8_t *) FB_ADDRESS;

static uint16_t fb_cursor_get_offset() {
    outb(FB_PORT_COMMAND, FB_COMMAND_HIGH_BYTE);
    uint32_t offset = inb(FB_PORT_DATA) << 8;
    outb(FB_PORT_COMMAND, FB_COMMAND_LOW_BYTE);
    offset += inb(FB_PORT_DATA);
    return offset * 2;
}

static void fb_cursor_move_to(uint16_t pos) {
    outb(FB_PORT_COMMAND, FB_COMMAND_HIGH_BYTE);
    outb(FB_PORT_DATA, (pos / 2) >> 8);
    outb(FB_PORT_COMMAND, FB_COMMAND_LOW_BYTE);
    outb(FB_PORT_DATA, (pos / 2) & 0xFF);
}

void fb_init() {
    fb_screen_clear();
    fb_cursor_move_to(FB_CURSOR_OFFSET(0, 0));
}

void fb_screen_clear() {
    for (int i = 0; i < FB_MAX_COL * FB_MAX_ROW; i++) {
        fb[i * 2] = ' ';
        fb[i * 2 + 1] = FB_DEFAULT;
    }

    fb_cursor_move_to(FB_CURSOR_OFFSET(0, 0));
}

uint16_t fb_put_char_at(char c, int8_t row, int8_t column, uint8_t color) {
    uint16_t offset;
    if (row >= 0 && column >= 0) {
        offset = FB_CURSOR_OFFSET(column, row);
    } else {
        offset = fb_cursor_get_offset();
    }

    switch (c) {
        case '\n':
            row = FB_GET_OFF_ROW(offset) + 1;
            offset = FB_CURSOR_OFFSET(0, row);
            break;

        default:
            fb[offset] = c;
            fb[offset + 1] = color;
            offset += 2;
            break;
    }

    fb_cursor_move_to(offset);
    return offset;
}

uint16_t fb_put_str_at(const char *str, int8_t row, int8_t column, uint8_t color) {
    uint16_t offset;
    if (row < 0 || column < 0) {
        offset = fb_cursor_get_offset();
        row = FB_GET_OFF_ROW(offset);
        column = FB_GET_OFF_COLUMN(offset);
    } else {
        offset = FB_CURSOR_OFFSET(column, row);
    }

    while (*str) {
        offset = fb_put_char_at(*str++, row, column, color);
        row = FB_GET_OFF_ROW(offset);
        column = FB_GET_OFF_COLUMN(offset);
    }

    return offset;
}

void fb_del() {
    uint16_t offset = fb_cursor_get_offset() - 2;
    uint8_t row = FB_GET_OFF_ROW(offset);
    uint8_t column = FB_GET_OFF_COLUMN(offset);

    fb_put_char_at(' ', row, column, FB_DEFAULT);
    fb_cursor_move_to(offset);
}
