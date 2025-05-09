#ifndef _FSHELL_PARSE_H
#define _FSHELL_PARSE_H

#include "array.h"
#include <stdbool.h>
#include <string.h>

extern cmd_or_pipe_chain_t *array_vanilla_parse(char *input, cmd_or_pipe_chain_t *vanilla_chain, char *symbol);

static inline void array_parse(char *string, char **array)
{
    array[0] = strtok(string, " ");
    short i = 0;
    while (array[i] != NULL)
        array[++i] = strtok(NULL, " ");
}

extern bool check_and(const char *string);
extern bool check_pipe(const char *string);
extern bool check_builtin(const char *string);
extern bool check_root(const char *username);

#define array_chain_parse(input, array_chain) (array_vanilla_parse(input, array_chain, "&&"))
#define array_pipe_parse(input, pipe_chain) (array_vanilla_parse(input, pipe_chain, "|"))

#endif /*_FSHELL_PARSE_H_*/
