#ifndef TTY_H
#define TTY_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "ps2.h"

void tty_init();
void tty_newline();
void tty_scroll();
uint8_t tty_output(uint8_t scan_code);
void mod_output(uint8_t scan_code);
void flush_buffer();

#endif /* TTY_H */
