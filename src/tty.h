#ifndef TTY_H
#define TTY_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "ps2.h"

void tty_init();
void tty_newline();

#endif /* TTY_H */
