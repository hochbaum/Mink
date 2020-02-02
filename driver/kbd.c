#include <mink/driver/kbd/kbd.h>
#include <mink/cpu/state.h>
#include <mink/cpu/databus.h>
#include <mink/driver/8259pic.h>
#include <mink/cpu/idt.h>
#include <mink/driver/kbd/deDE-darwin.h>
#include <string.h>
#include <mink/driver/fb.h>

static char key_bindings[256];
static char key_buffer[256];
static uint8_t shift_pressed;

static void kbd_handler(registers_t registers) {
    uint8_t scancode = inb(KBD_PORT);
    pic_ack(IRQ_KBD);

    if (scancode > 57) {
        uint8_t relcode = scancode - KBD_RELEASE;
        if (relcode == KBD_LSHIFT || relcode == KBD_RSHIFT) {
            shift_pressed = 0;
        }

        return;
    }

    switch (scancode) {
        case KBD_BACKSPACE: {
            uint8_t len = strlen(key_buffer);
            if (len > 0) {
                key_buffer[len - 1] = 0;
                fb_del();
            }

            break;
        }

        case KBD_RETURN:
            key_buffer[0] = 0;
            // send input.
            fb_put_char_at('\n', -1, -1, FB_DEFAULT);
            break;

        case KBD_RSHIFT:
        case KBD_LSHIFT:
            shift_pressed = 1;
            break;

        default: {
            uint8_t len = strlen(key_buffer);
            char letter = key_bindings[(shift_pressed ? 128 : 0) + scancode];

            key_buffer[len] = letter;
            key_buffer[len + 1] = 0;

            fb_put_char_at(letter, -1, -1, FB_DEFAULT);
            break;
        }
    }

    // unused, bypass error.
    (void)registers;
}

void kbd_map_key(uint8_t scancode, uint8_t ascii) {
    key_bindings[scancode] = ascii;
}

void kbd_init() {
    // default binding will be a question mark.
    memset(&key_bindings, '?', 256);

    KBD_MAP(KBD_1, '1') KBD_MAP_UPPER(KBD_1, '!')
    KBD_MAP(KBD_2, '2') KBD_MAP_UPPER(KBD_2, '"')
    KBD_MAP(KBD_3, '3') // §
    KBD_MAP(KBD_4, '4') KBD_MAP_UPPER(KBD_4, '$')
    KBD_MAP(KBD_5, '5') KBD_MAP_UPPER(KBD_5, '%')
    KBD_MAP(KBD_6, '6') KBD_MAP_UPPER(KBD_6, '&')
    KBD_MAP(KBD_7, '7') KBD_MAP_UPPER(KBD_7, '/')
    KBD_MAP(KBD_8, '8') KBD_MAP_UPPER(KBD_8, '(')
    KBD_MAP(KBD_9, '9') KBD_MAP_UPPER(KBD_9, ')')
    KBD_MAP(KBD_0, '0') KBD_MAP_UPPER(KBD_0, '=')

    KBD_MAP(KBD_Q, 'q') KBD_MAP_UPPER(KBD_Q, 'Q')
    KBD_MAP(KBD_W, 'w') KBD_MAP_UPPER(KBD_W, 'W')
    KBD_MAP(KBD_E, 'e') KBD_MAP_UPPER(KBD_E, 'E')
    KBD_MAP(KBD_R, 'r') KBD_MAP_UPPER(KBD_R, 'R')
    KBD_MAP(KBD_T, 't') KBD_MAP_UPPER(KBD_T, 'T')
    KBD_MAP(KBD_Z, 'z') KBD_MAP_UPPER(KBD_Z, 'Z')
    KBD_MAP(KBD_U, 'u') KBD_MAP_UPPER(KBD_U, 'U')
    KBD_MAP(KBD_I, 'i') KBD_MAP_UPPER(KBD_I, 'I')
    KBD_MAP(KBD_O, 'o') KBD_MAP_UPPER(KBD_O, 'O')
    KBD_MAP(KBD_P, 'p') KBD_MAP_UPPER(KBD_P, 'P')
    KBD_MAP(KBD_A, 'a') KBD_MAP_UPPER(KBD_A, 'A')
    KBD_MAP(KBD_S, 's') KBD_MAP_UPPER(KBD_S, 'S')
    KBD_MAP(KBD_D, 'd') KBD_MAP_UPPER(KBD_D, 'D')
    KBD_MAP(KBD_F, 'f') KBD_MAP_UPPER(KBD_F, 'F')
    KBD_MAP(KBD_G, 'g') KBD_MAP_UPPER(KBD_G, 'G')
    KBD_MAP(KBD_H, 'h') KBD_MAP_UPPER(KBD_H, 'H')
    KBD_MAP(KBD_J, 'j') KBD_MAP_UPPER(KBD_J, 'J')
    KBD_MAP(KBD_K, 'k') KBD_MAP_UPPER(KBD_K, 'K')
    KBD_MAP(KBD_L, 'l') KBD_MAP_UPPER(KBD_L, 'L')
    KBD_MAP(KBD_Y, 'y') KBD_MAP_UPPER(KBD_Y, 'Y')
    KBD_MAP(KBD_X, 'x') KBD_MAP_UPPER(KBD_X, 'X')
    KBD_MAP(KBD_C, 'c') KBD_MAP_UPPER(KBD_C, 'C')
    KBD_MAP(KBD_V, 'v') KBD_MAP_UPPER(KBD_V, 'V')
    KBD_MAP(KBD_B, 'b') KBD_MAP_UPPER(KBD_B, 'B')
    KBD_MAP(KBD_N, 'n') KBD_MAP_UPPER(KBD_N, 'N')
    KBD_MAP(KBD_M, 'm') KBD_MAP_UPPER(KBD_M, 'M')

    KBD_MAP(KBD_SPACE, ' ') KBD_MAP_UPPER(KBD_SPACE, ' ')
    KBD_MAP_UPPER(KBD_SS, '?')

    key_buffer[0] = 0;
    idt_handler_add(IRQ_KBD, &kbd_handler);
}
