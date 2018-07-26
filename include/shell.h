#ifndef SHELL_H
#define SHELL_H

#define BUF_SIZE 16
#define NUM_COMMANDS 8

void parse_input(char* input);
void run_command(char** tokens);

#endif /* SHELL_H */
