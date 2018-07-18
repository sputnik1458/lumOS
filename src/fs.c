#include "../include/fs.h"

struct Directory pwd;

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

void fs_init() {
    gen_dir(ROOT_DIR);
}

void gen_dir(uint16_t addr) {
    pwd.addr = (uint8_t*)addr;
    pwd.name = eeprom_read_byte(pwd.addr);

    int i;
    for (i = addr+1; i < addr+16; i++) {
        pwd.files[i-addr-1] = (uint8_t*)(uint16_t)eeprom_read_byte((uint8_t*)i);
    }
}

struct Directory* get_pwd() {
    return &pwd;
}
