#include "../include/ps2.h"

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


const uint8_t modifiers[6] PROGMEM = {PS2_LEFT_SHIFT, PS2_LEFT_CONTROL, KEY_BREAK, PS2_ENTER, PS2_BACKSPACE, PS2_KEYPAD};
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

char get_char(volatile uint8_t scan_code) {
    if (SHIFT_STATUS) {
        return pgm_read_word_near(shift_keys + scan_code-1);
    } else {
        return pgm_read_word_near(keys + scan_code-1);
    }

}

uint8_t get_keystatus(volatile uint8_t scan_code) { // TODO: change name
    uint8_t j;
    for (j = 0; j < 6; j++) {
        if (scan_code == pgm_read_byte_near(modifiers+j)) {
            update_status(j);
            return 0;
        }
    }

    if ((KEYPAD_STATUS) || (CONTROL_STATUS)) {
        return 0;
    }
    return 1;
}

uint8_t get_status() {
    return status;
}

void update_status(uint8_t modifier) { // TODO: create macro
    status ^= (1 << modifier);
}
