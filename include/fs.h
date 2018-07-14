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
struct Directory fs_init();
void mkdir(char name, struct Directory *pwd);
void rm(char name, struct Directory *pwd);
struct Directory cd(char name, struct Directory pwd);
void touch(char name);
void ls(struct Directory *dir);

#endif /* FS_H */
