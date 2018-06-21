#include "tty.h"

void tty_init() {
    kbd_init();
    lcd_init(LCD_DISP_ON_CURSOR);
    tty_newline();
}

void tty_newline() {
    lcd_clrscr();
    lcd_home();
    lcd_putc('~');
    lcd_putc(' ');
}
