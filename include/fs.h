#ifndef FS_H
#define FS_H

#include <avr/io.h>
#include <avr/eeprom.h>
#include "lcd.h"

#define ROOT_DIR 128
#define EOF 0x56

struct Directory { // 16 bytes / 128 bits
    char name;
    uint8_t* files[15]; // Note: root directory holds directory addresses
    uint8_t* addr;
};

struct File {
    char name;
};

void mkfs();
void fs_init();
void mkdir(char** args);
void gen_dir(uint16_t addr);
struct Directory* get_pwd();
void rm(char** args);
void cd(char** args);
void touch(char name);
void ls(char**args);

#endif /* FS_H */
