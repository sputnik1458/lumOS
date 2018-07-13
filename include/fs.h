#ifndef FS_H
#define FS_H

#include <avr/io.h>
#include <avr/eeprom.h>
#include "lcd.h"

#define ROOT_DIR 128
#define EOF 0x56

// struct RootDirectory {
//     char name[8];
//     uint8_t* directories[15]; // max 15 directories
// }

struct Directory { // 16 bytes / 128 bits
    char name;
    uint8_t* files[15]; // root dir holds directory addrs
    uint8_t* addr;
};

struct File {
    char name;
};

void mkfs();
struct Directory fs_init();
void mkdir(char name, struct Directory *pwd);
void touch(char name);
void ls(struct Directory *dir);

#endif /* FS_H */
