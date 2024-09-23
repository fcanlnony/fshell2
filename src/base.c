#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "base.h"
#include "memory.h"

char* getusername()
{
  struct passwd *pwd = getpwuid(getuid());
  char *username = (char*)calloc(strlen(pwd->pw_name),sizeof(char));
  strlcpy(username,pwd->pw_name,count_for_strlcpy(pwd->pw_name));
  return username;
}

char* getcurrentdir()
{
  char *dir = (char*)calloc(MAX_DIR,sizeof(char));
  dir = getcwd(dir, MAX_DIR);
  return dir;
}

user_t init_user_information(char *username, char *userdir,user_t user)
{
  user = (struct user_information*)malloc(sizeof(struct user_information));
  user->username = username;
  user->userdir = userdir;
  return user;
}

char* getusername_b(user_t user)
{
  return user->username;
}

char* getcurrentdir_b(user_t user)
{
  return user->userdir;
}

char* fshell_prompt_readline(char *username,char *currentdir,char *prompt)
{
  prompt = (char*)calloc((strlen(username)+strlen(currentdir)+strlen(" ")*4+strlen("[]")+strlen(">")*3),sizeof(char));
  sprintf(prompt, "%s [ %s ]\n>>> ",username,currentdir);
  return prompt;
}

char* readline_history_path(char *username,char *path)
{
  if(!strcmp(username,"root")) {
    path = (char*)calloc(strlen("/root/.fshell_historys"),sizeof(char));
    strncpy(path, "/root/.fshell_historys",sizeof(char)*strlen("/root/.fshell_historys"));
    return path;
  } else {
    path = (char*)calloc((strlen("/home//.fshell_historys")+strlen(username)),sizeof(char));
    strlcpy(path, "/home/", count_for_strlcpy("/home/"));
    strcat(path,username);
    strcat(path,"/.fshell_history");
    return path;
  }
  return NULL;
}
