#ifndef FSHELL_BUILTIN_H_
#define FSHELL_BUILTIN_H_

#include <string.h>

#define LIST_CMD_BUILTIN 0
#define CD_CMD_BUILTIN 1
#define ALIAS_CMD_BUILTIN 2
#define UNALIAS_CMD_BUILTIN 3
#define SET_ENV_CMD_BUILTIN 4
#define UNSET_ENV_CMD_BUILTIN 5
#define PRINT_CMD_BUILTIN 6
#define LOAD_CMD_BUILTIN 7

#define NON_BUILTIN_CMD -1

static inline short check_builtin_cmd(char *array) {
  if(!strcmp(array,"list"))
    return LIST_CMD_BUILTIN;
  else if(!strcmp(array,"cd"))
    return CD_CMD_BUILTIN;
  else if(!strcmp(array,"alias"))
    return ALIAS_CMD_BUILTIN;
  else if(!strcmp(array,"unalias"))
    return UNALIAS_CMD_BUILTIN;
  else if(!strcmp(array,"set"))
    return SET_ENV_CMD_BUILTIN;
  else if(!strcmp(array,"unset"))
    return UNSET_ENV_CMD_BUILTIN;
  else if(!strcmp(array,"print"))
    return PRINT_CMD_BUILTIN;
  else if(!strcmp(array,"load"))
    return LOAD_CMD_BUILTIN;
  else return NON_BUILTIN_CMD;
}

#include "alias.h"
#include "base.h"
extern short exec_builtin_cmd(char **array,const short FLAG,alias_t head,const char *username,const char *cd_history);

#endif /*FSHELL_BUILTIN_H_*/
