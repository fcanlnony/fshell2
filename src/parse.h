#ifndef FSHELL_PARSE_H
#define FSHELL_PARSE_H

#include "array.h"
#include <stdbool.h>

cmd_t array_chain_parse(char *input,cmd_t array_chain);

pipe_t array_pipe_parse(char *input,pipe_t pipe_chain);

void array_parse(char *string,char **array);

bool check_and(char *string);
bool check_pipe(char *string);
bool check_builtin(char *string);

#endif /*FSHELL_PARSE_H_*/
