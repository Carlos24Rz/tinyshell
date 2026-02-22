#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define SHELL_PROMPT_MSG "tinyshell> "
/* Get shell prompt line from standard input s*/
ssize_t shell_prompt_getline(char **line, size_t *line_buffer_size)
{
  printf("%s", SHELL_PROMPT_MSG);
  fflush(stdout);

  return getline(line, line_buffer_size, stdin);
}

#define TOKENIZER_WHITESPACE_SEP " \t\r\n"
#define TOKENIZER_LIST_GROWTH_SIZE 10
/* Tokenize a string into a list of arguments by splitting whitespace characters */
char **shell_tokenizer(char *str, size_t str_len, size_t *argc)
{
  char *next_token = NULL;
  char **argv = NULL;
  size_t max_argc = TOKENIZER_LIST_GROWTH_SIZE;
  size_t arg_i;

  assert(str != NULL && str_len > 0);

  argv = (char **)malloc(sizeof(char *)*max_argc);

  next_token = strtok(str, TOKENIZER_WHITESPACE_SEP);
  arg_i = 0;

  while (next_token != NULL)
  {
    /* We reserve at least one element at the end to accound for the NULL argument */
    if (arg_i == max_argc - 1)
    {
      max_argc += TOKENIZER_LIST_GROWTH_SIZE;
      argv = realloc(argv, sizeof(char *) * (max_argc));
    }

    argv[arg_i++] = next_token;

    next_token = strtok(NULL, TOKENIZER_WHITESPACE_SEP);
  }

  if (arg_i == 0)
  {
    free(argv);
    *argc = 0;
    return NULL;
  }

  /* Add NULL argument at the end of the list */
  argv[arg_i++] = NULL;

  *argc = arg_i;

  return argv;
}

int main(void)
{
  char *line = NULL;
  ssize_t line_len;
  size_t line_alloc_size;

  while ((line_len = shell_prompt_getline(&line, &line_alloc_size)) != -1)
  {
    size_t argc;
    size_t i;
    pid_t pid;
    char **argv = NULL;
    int fd;

    /* Remove new line character */
    if (line_len > 0 && line[line_len-1] == '\n') {
      line[line_len-1] = '\0';
      line_len--;
    }

    if (line_len == 0)
      continue;

    argv = shell_tokenizer(line, line_len, &argc);

    assert(argv != NULL && argc > 0);

    /* Create child process */
    fflush(stderr);
    fflush(stdout);
    pid = fork();

    switch (pid) {
      /* Fail to create child process */
      case -1:
        fprintf(stderr, "Failed to spawn child process\n");
        exit(EXIT_FAILURE);
      /* Child process */
      case 0:
      {
        /* Execute command on this process */
        execvp(argv[0], argv);
        exit(EXIT_SUCCESS);
      }
      /* Parent process */
      default:
      {
        int wstatus;
        pid_t w;

        /* Wait for children to complete */
        w = waitpid(pid, &wstatus, 0);
      }
    }

    free(argv);
  }

  free(line);

  return 0;

}