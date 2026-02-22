#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "builtins.h"

typedef struct builtin_cmd
{
  const char *cmd_str;
  void (*cmd_handle)(char **argv, int argc);
} builtin_cmd;

void handle_cd(char **argv, int argc);

static builtin_cmd builtin_table[] = {
  {"cd", handle_cd}
};

#define BUILTIN_TABLE_NUM_ENTRIES (sizeof(builtin_table) / sizeof(builtin_table[0]))

bool handle_builtin(char **argv, int argc)
{
  size_t i;

  for (i = 0; i < BUILTIN_TABLE_NUM_ENTRIES; i++)
  {
    if (strcmp(builtin_table[i].cmd_str, argv[0]) == 0)
    {
      builtin_table[i].cmd_handle(argv, argc);

      return true;
    }
  }

  return false;
}

void handle_cd(char **argv, int argc)
{
  /* Ignore empty path */
  if (argc < 2)
    return;

  if (chdir(argv[1]) != 0)
  {
    fprintf(stderr, "%s\n", strerror(errno));
  }
}