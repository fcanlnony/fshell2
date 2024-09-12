#ifndef FSHELL_EXEC_H_
#define FSHELL_EXEC_H_

#include "array.h"

extern short execvp_without_pipe(char **array);
extern void execvp_with_pipe(pipe_t pipe_chain);

#endif /*FSHELL_EXEC_H_*/
