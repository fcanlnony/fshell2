#ifndef _FSHELL_EXEC_H_
#define _FSHELL_EXEC_H_

#include "include/array.h"

extern void execvp_without_pipe(char **array);
extern void execvp_with_multi_pipe(pipe_t pipe_chain);

#endif /*_FSHELL_EXEC_H_*/
