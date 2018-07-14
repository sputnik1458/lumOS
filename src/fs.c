#include "../include/fs.h"
#include <stdlib.h>
#include <stdint.h>

void mkfs() {
    uint16_t i;
    for (i = 0; i < 18; i++) { // file table & root directory usage
        eeprom_write_byte((uint8_t*)i, (uint8_t)0xFF);
    }
    for (i = 18; i < 128; i++) {
        eeprom_write_byte((uint8_t*)i, (uint8_t)0);
    }
    for (i = 128; i < 1024; i++) {
        eeprom_write_byte((uint8_t*)i, (uint8_t)0xFF);
    }
    eeprom_write_byte((uint8_t*)ROOT_DIR, '/');
}

struct Directory fs_init() {
    struct Directory root_dir;
    root_dir.addr = (uint8_t*)ROOT_DIR;
    root_dir.name = eeprom_read_byte(root_dir.addr);
    uint16_t i;
    for (i = ROOT_DIR+1; i < ROOT_DIR+16; i++) {
        root_dir.files[i-ROOT_DIR-1] = (uint8_t*)(uint16_t)eeprom_read_byte((uint8_t*)i);
    }
    return root_dir;
}

void mkdir(char name, struct Directory *pwd) {
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

void rm(char name, struct Directory *pwd) {
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

void touch(char name) {

}

struct Directory cd(char name, struct Directory pwd) {
    struct Directory dir = pwd;
    uint16_t i, j, addr;

    if (name == ' ') {
        dir = fs_init(); // TODO: change
    } else {
        for (i = 0; i < 15; i++) {
            addr = (uint16_t)pwd.files[i];
            if (eeprom_read_byte((uint8_t*)addr) == name) {
                dir.addr = (uint8_t*)addr;
                dir.name = name;
                for (j = addr+1; j < addr+16; j++) {
                    dir.files[j-addr-1] = (uint8_t*)(uint16_t)eeprom_read_byte((uint8_t*)j);
                }
                break;
            }
        }
    }
    return dir;
}

void ls(struct Directory *dir) {
    uint8_t i;
    for (i = 0; i < 15; i++) {
        if ((uint16_t)dir->files[i] != 0xFF) {
            lcd_putc(eeprom_read_byte(dir->files[i]));
        }
    }
}
