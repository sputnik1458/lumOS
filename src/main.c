#include "tty.h"
#include "fs.h"
#include <avr/eeprom.h>
#include <stdlib.h>

volatile uint8_t data = 0;
volatile uint8_t scan_code = 0;
volatile uint8_t bit_n = 0;

int main() {

    tty_init();
    flush_buffer();

    while (1) {
        // if (prev_scan_code != 0) {
        //     special_input();
        // }
        //eeprom_write_byte((uint8_t*)1, c);
        // data = eeprom_read_byte((uint8_t*)128);
        // int k;
        // for (k = 7; k >= 0; k--) {
        //     out = (data >> k) & 1;
        //     if (out) {
        //         lcd_putc('1');
        //     } else {
        //         lcd_putc('0');
        //     }
        // }
        //lcd_putc(data);
        if (scan_code != 0) {
            scan_code = tty_output(scan_code); // always 0
        }
    }
    return 0;
}





ISR(INT) {

    if ((bit_n < 9) || (bit_n > 0)) {
        data |= (get_bit() << bit_n);
    }

    if (bit_n == 10) { // full scan code

        data >>= 1; // don't know why but this is needed to get the correct scan code
        bit_n = 0;
        scan_code = data;
        data = 0;
    } else {
        bit_n++;
    }
}
