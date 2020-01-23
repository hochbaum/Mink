#ifndef MINK_FB_H
#define MINK_FB_H

#include <stdint.h>

#define FB_ADDRESS 0x000B8000
#define FB_MAX_COL 80
#define FB_MAX_ROW 25

#define FB_PORT_COMMAND       0x3D4
#define FB_PORT_DATA          0x3D5
#define FB_COMMAND_HIGH_BYTE  14
#define FB_COMMAND_LOW_BYTE   15

#define FB_CURSOR_OFFSET(col, row) (2 * (row * FB_MAX_COL + col))
#define FB_GET_OFF_ROW(offset)     (offset / (2 * FB_MAX_COL))
#define FB_GET_OFF_COLUMN(offset)  ((offset - ((offset / (2 * FB_MAX_COL)) * 2 * FB_MAX_COL)) / 2)

typedef enum fb_color {
  FB_BLACK           = 0x0,
  FB_BLUE            = 0x1,
  FB_GREEN           = 0x2,
  FB_CYAN            = 0x3,
  FB_RED             = 0x4,
  FB_MAGENTA         = 0x5,
  FB_BROWN           = 0x6,
  FB_LIGHT_GREY      = 0x7,
  FB_DARK_GREY       = 0x8,
  FB_LIGHT_BLUE      = 0x9,
  FB_LIGHT_GREEN     = 0xA,
  FB_LIGHT_CYAN      = 0xB,
  FB_LIGHT_RED       = 0xC,
  FB_LIGHT_MAGENTA   = 0xD,
  FB_LIGHT_BROWN     = 0xE,
  FB_WHITE           = 0xF
} fb_color_t;

#define FB_COLOR(fg, bg) (((bg & 0x0F) << 4) | (fg & 0x0F))
#define FB_DEFAULT FB_COLOR(FB_LIGHT_GREY, FB_BLACK)

/**
 * Initializes the framebuffer by clearing it and setting the cursor properly.
 */
void fb_init();

/**
 * Clears the framebuffer.
 */
void fb_screen_clear();

/**
 * Prints a colored character at the specified location and returns the new
 * framebuffer offset.
 *
 * For coloring see FB_DEFAULT or use FB_COLOR.
 *
 * The only escape currently supported is newline.
 *
 * If row or column are set to < 0, the offset will be calculated for you.
 *
 * @param c         Character to print.
 * @param row       Row in the framebuffer.
 * @param column    Column in the framebuffer.
 * @param color     Foreground and background of the character.
 * @return New framebuffer offset.
 */
uint16_t fb_put_char_at(char c, int8_t row, int8_t column, uint8_t color);

/**
 * Prints a colored string at the specified location using fb_put_char_at and returns
 * the new framebuffer offset.
 *
 * For coloring see FB_DEFAULT or use FB_COLOR.
 *
 * The only escape currently supported is newline.
 *
 * If row or column are set to < 0, the offset will be calculated for you.
 *
 * @param str       String to print.
 * @param row       Row in the framebuffer.
 * @param column    Column in the framebuffer.
 * @param color     Foreground and background of the string.
 * @return New framebuffer offset.
 */
uint16_t fb_put_str_at(const char *str, int8_t row, int8_t column, uint8_t color);

#endif //MINK_FB_H
