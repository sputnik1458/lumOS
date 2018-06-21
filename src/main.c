#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ps2.h"
#include "lcd.h"

int main() {

    kbd_init();
    lcd_init(LCD_DISP_ON_CURSOR);

    while (1) {}

    return 0;
}
