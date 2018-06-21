#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "tty.h"

#define status get_status()

volatile uint8_t scan_code = 0;
volatile uint8_t prev_scan_code = 0;
volatile uint8_t bit_n = 0;
volatile uint8_t loc;


int main() {

    tty_init();

    while (1) {
        if (prev_scan_code == PS2_ENTER) {
            tty_newline();
            prev_scan_code = 0;
        } else if (prev_scan_code == PS2_BACKSPACE) {
            loc = lcd_getxy();
            if (~KEY_BREAK_STATUS && (loc != 0)) {
                if (loc == 64) { // (0, 1)
                    lcd_gotoxy(16, 0);
                }
                lcd_command(0x10); // move cursor back
                lcd_putc(' ');
                lcd_command(0x10);
            }
            prev_scan_code = 0;
        }

    }

    return 0;
}

ISR(INT) {

    if ((bit_n < 9) || (bit_n > 0)) {
        scan_code |= (get_bit() << bit_n);
    }

    if (bit_n == 10) { // full scan code

        scan_code >>= 1; // don't know why but this is needed to get the correct scan code
        if (~KEY_BREAK_STATUS) { // make press
            if (get_keystatus(scan_code)) {
                lcd_putc(get_char(scan_code));
            }
            prev_scan_code = scan_code;
        } else { // break press
            get_keystatus(scan_code); // reset modifier status
            update_status(KEY_BREAK_REGSTR);

        }
        bit_n = 0;
        scan_code = 0;
    } else {
        bit_n++;
    }
}
