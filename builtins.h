/* Builtins.h: Handle execution of the shell builtin commands */
#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdbool.h>

bool handle_builtin(char *argv[], int argc);


#endif /* BUILTINS_H */