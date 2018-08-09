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

uint8_t* allocate(uint8_t size) {
    struct Directory* pwd = get_pwd();
    uint16_t i, addr = 0, free_space = 0;

    for (i = 18; i < 128 && free_space < size; i++) {
        addr = i;
        if (!(eeprom_read_byte((uint8_t*)i))) {
            free_space++;
        } else {
            if (free_space) {
                free_space = 0;
            }
        }
    }

    for (i = 0; i < size; i++, addr--) {
        eeprom_write_byte((uint8_t*)addr, (uint8_t)0xFF);
    }
    addr++; // TODO: remove
    addr *= 8;

    for (i = 0; i < 16; i++) {
        if ((uint16_t)pwd->files[i] == 0xFF) {
            pwd->files[i] = (uint8_t*)addr;
            eeprom_write_byte((uint8_t*)i+(uint16_t)pwd->addr+1, (uint16_t)pwd->files[i]); // write addr to root dir array
            break;
        }
    }

    return (uint8_t*)addr;
}

void clear(uint8_t* addr) {
    struct Directory* pwd = get_pwd();
    uint16_t i, file_table_addr, size;

    file_table_addr = (uint16_t)addr / 8;

    if (pwd->name == '/') {
        size = 2;
    } else {
        // text file size loop
    }

    for (i = 0; i < size; i++, file_table_addr++) {
        eeprom_write_byte((uint8_t*)file_table_addr, 0x00);
    }

}


struct Directory* get_pwd() {
    return &pwd;
}
