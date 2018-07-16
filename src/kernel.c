#include "../include/kernel.h"
#include <util/delay.h>

void boot() {
    lcd_init(LCD_DISP_ON_CURSOR);
    lcd_puts("Booting lumOS.");
    _delay_ms(500);
    lcd_putc('.');
    _delay_ms(500);
    lcd_putc('.');
    _delay_ms(500);
    lcd_gotoxy(0, 1);
    lcd_puts("Loading fs.");
    _delay_ms(500);
    lcd_putc('.');
    _delay_ms(500);
    lcd_putc('.');
    _delay_ms(500);
    fs_init();
    lcd_clrscr();
    lcd_puts("Found keyboard");
    _delay_ms(500);
    lcd_gotoxy(0, 1);
    lcd_puts("Starting.");
    _delay_ms(500);
    lcd_putc('.');
    _delay_ms(500);
    lcd_putc('.');
    _delay_ms(1000);
    tty_init();
}

void quickboot() {
    lcd_init(LCD_DISP_ON_CURSOR);
    fs_init();
    tty_init();
}
