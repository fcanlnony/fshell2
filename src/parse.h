#ifndef FSHELL_PARSE_H
#define FSHELL_PARSE_H

#include "array.h"

cmd_t array_chain_parse(char *input,cmd_t array_chain);

pipe_t array_pipe_parse(char *input,pipe_t pipe_chain);

#endif /*FSHELL_PARSE_H_*/
