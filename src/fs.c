#include "fs.h"

void mkfs() {
    int i;
    for (i = 0; i < 128; i++) {
        eeprom_write_byte((uint8_t*)i, (uint8_t)0);
    }
}
