#include "lcd.h"
#include "ps2.h"

const char keys[100] PROGMEM = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', // 0-9
                                '0', '0', '0', '`', '0', '0', '0', '0', '0', '0', // 10-19
                                'q', '1', '0', '0', '0', 'z', 's', 'a', 'w', '2', // 20-29
                                '0', '0', 'c', 'x', 'd', 'e', '4', '3', '0', '0', // 30-39
                                ' ', 'v', 'f', 't', 'r', '5', '0', '0', 'n', 'b', // 40-49
                                'h', 'g', 'y', '6', '0', '0', '0', 'm', 'j', 'u', // 50-59
                                '7', '8', '0', '0', ',', 'k', 'i', 'o', '0', '9', // 60-69
                                '0', '0', '.', '/', 'l', ';', 'p', '-', '0', '0', // 70-79
                                '0', '\'', '0', '[', '=', '0', '0', '0', '0', '0', // 80-89
                                ']', '0', '\\', '0', '0', '0', '0', '0', '0', '0', // 90-99
                            };

const char shift_keys[100] PROGMEM = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', // 0-9
                                    '0', '0', '0', '~', '0', '0', '0', '0', '0', '0', // 10-19
                                    'Q', '!', '0', '0', '0', 'Z', 'S', 'A', 'W', '@', // 20-29
                                    '0', '0', 'C', 'X', 'D', 'E', '$', '#', '0', '0', // 30-39
                                    ' ', 'V', 'F', 'T', 'R', '%', '0', '0', 'N', 'B', // 40-49
                                    'H', 'G', 'Y', '^', '0', '0', '0', 'M', 'J', 'U', // 50-59
                                    '&', '*', '0', '0', '<', 'K', 'I', 'O', ')', '(', // 60-69
                                    '0', '0', '>', '?', 'L', ':', 'P', '_', '0', '0', // 70-79
                                    '0', '\"', '0', '{', '+', '0', '0', '0', '0', '0', // 80-89
                                    '}', '0', '|', '0', '0', '0', '0', '0', '0', '0', // 90-99
                                };


volatile uint8_t scan_code;
volatile uint8_t bit_n = 0;
volatile uint8_t loc;
volatile uint8_t status = 0;

void kbd_init() {
    CLOCK_REGSTR &= ~(1 << CLOCK_PIN); // set clock input
    CLOCK_PORT |= (1 << CLOCK_PIN); // enable pullup

    DATA_REGSTR &= ~(1 << DATA_PIN); // set data input
    DATA_PORT |= (1 << DATA_PIN);   // enable pullup

    CTRL_REGSTR |= (1 << INT_CTRL); // trigger on falling edge
    MSK_REGSTR |= (1 << INT_MSK); // set interrupt

    sei(); // turn on interrupt
}

uint8_t get_bit() {
    if (PIND & (1 << DATA_PIN)) {
        return 1;
    } else {
        return 0;
    }
}

char get_char(volatile uint8_t *scan_code) {
    if (SHIFT_STATUS) {
        return pgm_read_word_near(shift_keys + *scan_code-1);
    } else {
        return pgm_read_word_near(keys + *scan_code-1);
    }

}

uint8_t get_keystatus(volatile uint8_t *scan_code) {
    if (*scan_code == PS2_BACKSPACE) {
        loc = lcd_getxy();
        if (~KEY_BREAK_STATUS && (loc != 0)) {
            if (loc == 64) { // (0, 1)
                lcd_gotoxy(16, 0);
            }
            lcd_command(0x10); // move cursor back
            lcd_putc(' ');
            lcd_command(0x10);
        }
    } else if (*scan_code == KEY_BREAK) {
        status ^= (1 << KEY_BREAK_REGSTR);
    } else if (*scan_code == PS2_LEFT_SHIFT) {
        status ^= (1 << SHIFT_REGSTR);
    } else if (*scan_code == PS2_LEFT_CONTROL) {
        status ^= (1 << CONTROL_REGSTR);
    } else {
        return 1;
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
            if (get_keystatus(&scan_code)) {
                lcd_putc(get_char(&scan_code));
            }
        } else { // break press
            get_keystatus(&scan_code); // reset modifier status
            status ^= (1 << KEY_BREAK_REGSTR);

        }
        bit_n = 0;
        scan_code = 0;
    } else {
        bit_n++;
    }
}
