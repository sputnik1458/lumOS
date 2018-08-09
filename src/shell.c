#include <string.h>
#include <util/delay.h>
#include "../include/shell.h"
#include "../include/commands.h"

char* commands[NUM_COMMANDS] = {"ls", "mkdir", "touch", "cd", "rm", "pwd", "set", "get", "read", "write"};
void (*builtin_func[])(char**) = {&_ls, &_mkdir, &_touch, &_cd, &_rm, &_pwd, &_set, &_get, &_read, &_write};

void parse_input(char* input) {
    char* tokens[8];
    char* tok;

    uint8_t i;

    for (tok = strtok(input, " "), i = 0; tok != NULL && i < 8; tok = strtok(NULL, " "), i++) {
        tokens[i] = tok;
    }

    run_command(tokens);
}

void run_command(char** tokens) {
    uint8_t i;
    for (i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(tokens[0], commands[i]) == 0) {
            (*builtin_func[i])(tokens);
        }
    }

}
