#ifndef PS2_H
#define PS2_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define INT_CTRL ISC11
#define INT_MSK INT1
#define INT INT1_vect
#define DATA_REGSTR DDRD
#define DATA_PORT PORTD
#define DATA_PIN PD2
#define CLOCK_REGSTR DDRD
#define CLOCK_PORT PORTD
#define CLOCK_PIN PD3

#define CTRL_REGSTR EICRA
#define MSK_REGSTR EIMSK

#define PS2_LEFT_SHIFT 0x12
#define PS2_LEFT_CONTROL 0x14
#define PS2_CAPS_LOCK 0x58
#define PS2_ENTER 0x5A
#define PS2_BACKSPACE 0x66
#define KEY_BREAK 0x70
#define PS2_KEYPAD 0x60

#define KEYPAD_UP 0x75
#define KEYPAD_LEFT 0x6B
#define KEYPAD_RIGHT 0x74
#define KEYPAD_DOWN 0x72

#define SHIFT_REGSTR 0
#define CONTROL_REGSTR 1
#define KEY_BREAK_REGSTR 2
#define ENTER_REGSTR 3
#define BACKSPACE_REGSTR 4
#define KEYPAD_REGSTR 5

#define SHIFT_STATUS get_status() & (1 << SHIFT_REGSTR)
#define CONTROL_STATUS get_status() & (1 << CONTROL_REGSTR)
#define KEY_BREAK_STATUS get_status() & (1 << KEY_BREAK_REGSTR)
#define KEYPAD_STATUS get_status() & (1 << KEYPAD_REGSTR)

void kbd_init();
uint8_t get_bit();
char get_char(uint8_t scan_code);
uint8_t get_keystatus(uint8_t scan_code);
uint8_t get_key();
uint8_t get_status();
void update_status(uint8_t scan_code);

#endif /* PS2_H */
