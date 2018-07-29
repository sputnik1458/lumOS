#include "../include/tty.h"
#include <string.h>

uint8_t loc = 0;
uint8_t line_i = 0;
char curr_line[17];
char buffer[64];
uint8_t buf_i = 0;
char clipboard[64];


void tty_init() {
    kbd_init();
    tty_newline();
}

void tty_newline() {
    //scan_code = 0;
    line_i = 0;
    flush_buffer();
    lcd_clrscr();
    lcd_putc('~');
    lcd_putc(' ');
}

void tty_scroll() {
    lcd_clrscr();
    lcd_puts(curr_line);
    lcd_gotoxy(0, 1);
}

void tty_output(uint8_t scan_code) {
    char c;
    if (~KEY_BREAK_STATUS) { // make press
        if (get_keystatus(scan_code)) {
            if (line_i == 16) {
                tty_scroll();
                line_i = 0;
            }
            c = get_char(scan_code);
            loc = lcd_getxy();
            if ((loc >= 16) && (loc <= 79)) {
                curr_line[line_i] = c;
                line_i++;
            }
            buffer[buf_i] = c;
            buf_i++;
            lcd_putc(c);
        } else {
            special_key(scan_code);
        }
    } else { // break press
        get_keystatus(scan_code); // reset modifier status
        update_status(KEY_BREAK_REGSTR);
    }

}

void special_key(uint8_t scan_code) {
    if (scan_code == PS2_ENTER) {
        lcd_gotoxy(0, 1);
        parse_input(buffer);
    } else if (scan_code == PS2_BACKSPACE) {
        loc = lcd_getxy();
        if (~KEY_BREAK_STATUS && (loc != 0)) {
            if (loc == 64) { // (0, 1)
                lcd_gotoxy(16, 0);
                line_i = 0;
            } else {
                line_i--;
            }
            buffer[buf_i] = 0;
            buf_i--;
            lcd_command(LCD_MOVE_CURSOR_LEFT);
            lcd_putc(' ');
            lcd_command(LCD_MOVE_CURSOR_LEFT);

        }
    } else if ((scan_code == KEYPAD_DOWN) || (scan_code == 0x4B)) { // down arrow or ctrl-L
        tty_newline();
    // } else if (scan_code == KEYPAD_LEFT) {
    //     lcd_command(LCD_MOVE_CURSOR_LEFT);
    //     buf_i--;
    // } else if (scan_code == KEYPAD_RIGHT) {
    //     lcd_command(LCD_MOVE_CURSOR_RIGHT);
    //     buf_i++;
    } else if (scan_code == 0x21) { // ctrl-c
        strcpy(clipboard, buffer);
    } else if (scan_code == 0x22) { // ctrl-x
        strcpy(clipboard, buffer);
        tty_newline();
    } else if (scan_code == 0x2A) { // ctrl-v
        lcd_puts(clipboard);
    }
}

void flush_buffer() {
    uint8_t j;
    for (j = 0; j < 64; j++) {
        buffer[j] = 0;
    }
    buf_i = 0;
}
