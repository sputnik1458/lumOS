#include "../include/tty.h"
#include "../include/fs.h"


volatile uint8_t data = 0;
volatile uint8_t scan_code = 0;
volatile uint8_t bit_n = 0;

int main() {
    //mkfs();
    struct Directory pwd = fs_init();
    //mkdir('$', &pwd);
    tty_init();
    flush_buffer();

    //rm('p', &pwd);
    //ls(&pwd);

    while (1) {
        if (scan_code != 0) {
            scan_code = tty_output(scan_code); // always 0
        }
    }
    return 0;
}





ISR(INT) {

    if ((bit_n < 9) || (bit_n > 0)) {
        data |= (get_bit() << bit_n);
    }

    if (bit_n == 10) { // full scan code

        data >>= 1; // don't know why but this is needed to get the correct scan code
        scan_code = data;
        bit_n = 0;
        data = 0;
    } else {
        bit_n++;
    }
}
