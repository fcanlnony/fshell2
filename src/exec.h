#ifndef FSHELL_EXEC_H_
#define FSHELL_EXEC_H_

#include "array.h"

extern short execvp_without_pipe(char **array);
extern short execvp_with_pipe(char **arrayA,char **arrayB);

#endif /*FSHELL_EXEC_H_*/
