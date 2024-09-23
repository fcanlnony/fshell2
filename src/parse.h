#ifndef FSHELL_PARSE_H
#define FSHELL_PARSE_H

#include "array.h"
#include <stdbool.h>
#include <string.h>

extern cmd_t array_chain_parse(char *input,cmd_t array_chain);

extern pipe_t array_pipe_parse(char *input,pipe_t pipe_chain);

static inline void array_parse(char *string,char **array)
{
  array[0] = strtok(string," ");
  short i = 0;
  while(array[i] != NULL)
    array[++i] = strtok(NULL," ");
}

extern bool check_and(char *string);
extern bool check_pipe(char *string);
extern bool check_builtin(char *string);
extern bool check_root(char *username);

#endif /*FSHELL_PARSE_H_*/
