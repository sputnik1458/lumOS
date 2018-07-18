#ifndef COMMANDS_H
#define COMMANDS_H

#include "fs.h"
#include "tty.h"

void _mkdir(char** args);
void _rm(char** args);
void _cd(char** args);
void _touch(char name);
void _ls(char**args);
void _pwd(char** args);

#endif /* COMMANDS_H */
