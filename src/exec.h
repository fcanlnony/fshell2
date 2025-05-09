#ifndef _FSHELL_EXEC_H_
#define _FSHELL_EXEC_H_

#include "include/array.h"

extern void execvp_without_pipe(char **array);
extern void execvp_with_pipe(char **arrayA, char **arrayB);

#endif /*_FSHELL_EXEC_H_*/
