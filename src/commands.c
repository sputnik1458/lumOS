#include "../include/commands.h"
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void _mkdir(char** args) {
    struct Directory *pwd = get_pwd();
    if (pwd->name != '/') {
        lcd_puts(" Error, not in /");
        _delay_ms(500);
        return;
    }
    char name = args[1][0];
    uint16_t i, free_space = 0;
    uint16_t addr;
    // finds free free space in file table
    for (i = 16; i < 128 && free_space < 2; i++) {
        addr = i;
        if (!(eeprom_read_byte((uint8_t*)i))) {
            free_space++;
        } else {
            if (free_space) {
                free_space = 0;
            }
        }
    }
    eeprom_write_byte((uint8_t*)addr, (uint8_t)0xFF);
    addr -= 1;
    eeprom_write_byte((uint8_t*)addr, (uint8_t)0xFF);
    addr *= 8;
    // add to file list
    for (i = 0; i < 16; i++) {
        if ((uint16_t)pwd->files[i] == 0xFF) {
            pwd->files[i] = (uint8_t*)addr;
            eeprom_write_byte((uint8_t*)i+ROOT_DIR+1, (uint16_t)pwd->files[i]); // write addr to root dir array
            break;
        }
    }
    eeprom_write_byte((uint8_t*)addr, name);

}

void _rm(char** args) {
    struct Directory *pwd = get_pwd();
    char name = args[1][0];
    uint16_t i, file_table_addr;
    uint8_t* addr;

    for (i = 0; i < 15; i++) {
        addr = pwd->files[i];
        if (eeprom_read_byte(addr) == name) {
            file_table_addr = (uint16_t)addr / 8;
            eeprom_write_byte((uint8_t*)file_table_addr, 0x00);
            eeprom_write_byte((uint8_t*)file_table_addr+1, 0x00);
            pwd->files[i] = (uint8_t*)0xFF;
            eeprom_write_byte((uint8_t*)i+ROOT_DIR+1, (uint16_t)pwd->files[i]);
            break;
        }
    }
}

void _touch(char** args) {
    //struct Directory *pwd = get_pwd();

}

void _cd(char** args) {
    struct Directory *pwd = get_pwd();
    char name = args[1][0];
    uint16_t i, addr;

    if (name == '.') {
        gen_dir(ROOT_DIR);
        return;
    } else {
        for (i = 0; i < 15; i++) {
            addr = (uint16_t)pwd->files[i];
            if (eeprom_read_byte((uint8_t*)addr) == name) {
                gen_dir(addr);
                return;
            }
        }
    }
    lcd_puts("Dir not found");
    _delay_ms(500);
}

void _ls(char** args) {
    struct Directory *pwd = get_pwd();
    uint8_t i;
    for (i = 0; i < 15; i++) {
        if ((uint16_t)pwd->files[i] != 0xFF) {
            lcd_putc(eeprom_read_byte(pwd->files[i]));
            lcd_putc(' ');
        }
    }
    _delay_ms(500);
}

void _pwd(char** args) {
    struct Directory *pwd = get_pwd();
    lcd_putc(pwd->name);
    _delay_ms(500);
}

void _set(char** args) {
    char regstr = args[1][0];
    uint8_t pin = atoi(args[2]);
    uint8_t val = args[3][0] - '0';

    if (regstr == 'B') {
        pin -= PORTB_CONST;
        DDRB |= (val << pin);
        if (val == 1) {
            PORTB |= (1 << pin);
        } else {
            PORTB &= ~(1 << pin);
        }
    } else if (regstr == 'D') {
        pin -= PORTD_CONST;
        DDRD |= (val << pin);
        if (val == 1) {
            PORTD |= (1 << pin);
        } else {
            PORTD &= ~(1 << pin);
        }
    } else {
        lcd_puts("Invalid register");
    }
}

void _get(char** args) {
    char regstr = args[1][0];
    uint8_t pin = atoi(args[2]);

    if (regstr == 'B') {
        pin -= PORTB_CONST;
        DDRB &= ~(1 << pin);
        if (PINB & (1 << pin)) {
            lcd_putc('1');
        } else {
            lcd_putc('0');
        }
    } else if (regstr == 'D') {
        pin -= PORTD_CONST;
        DDRD &= ~(1 << pin);
        if (PIND & (1 << pin)) {
            lcd_putc('1');
        } else {
            lcd_putc('0');
        }
    } else {
        lcd_puts("Invalid register");
    }
}
