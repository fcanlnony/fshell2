#ifndef FSHELL_CONFIG_H_
#define FSHELL_CONFIG_H_

#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../alias.h"

static inline char * get_fshell_config_path()
{
  struct passwd *pwd = getpwuid(getuid());
  char *config_path;
  if(strcmp(pwd->pw_name,"root")) {
    config_path = (char*)calloc(strlen(pwd->pw_name)+strlen("/home/")+strlen("/.fshellrc"),sizeof(char));
    asprintf(&config_path, "/home/%s/.fshellrc", pwd->pw_name);
    return config_path;
  }
  else {
    config_path = (char*)calloc(strlen("/root/.fshellrc"),sizeof(char));
    asprintf(&config_path, "/root/.fshellrc");
    return config_path;
  }
}

extern void fshell_init(alias_t head);

#endif /*FSHELL_CONFIG_H_*/

#ifdef FSHELL_32_BIT
#define FSHELL_INIT_FILE_ARRAY_NUM 20
#elif defined(FSHELL_64_BIT)
#define FSHELL_INIT_FILE_ARRAY_NUM 50
#else
#define FSHELL_INIT_FILE_ARRAY_NUM 20
#endif /*FSHELL_INIT_FILE_ARRAY_NUM*/