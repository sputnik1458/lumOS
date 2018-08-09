#ifndef COMMANDS_H
#define COMMANDS_H

#include "fs.h"
#include "tty.h"

#define PORTB_CONST 8
#define PORTD_CONST 0

void _mkdir(char** args);
void _rm(char** args);
void _cd(char** args);
void _touch(char** args);
void _ls(char**args);
void _pwd(char** args);
void _set(char** args);
void _get(char** args);
void _read(char** args);
void _write(char** args);

#endif /* COMMANDS_H */
