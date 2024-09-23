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
  char *username = (char*)malloc(sizeof(char)*strlen(pwd->pw_name));
  strlcpy(username,pwd->pw_name,count_for_strlcpy(pwd->pw_name));
  return username;
}

char* getcurrentdir()
{
  char *tmp_dir = malloc(sizeof(char)*MAX_DIR);
  tmp_dir = getcwd(tmp_dir, MAX_DIR);
  char *dir = (char*)malloc(sizeof(char)*strlen(tmp_dir));
  strlcpy(dir,tmp_dir,count_for_strlcpy(tmp_dir));
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
  prompt = (char*)malloc(sizeof(char)*(strlen(username)+strlen(currentdir)+strlen(" ")*4+strlen("[]")+strlen(">")*3));
  sprintf(prompt, "%s [ %s ]\n>>> ",username,currentdir);
  return prompt;
}

char* readline_history_path(char *username,char *path)
{
  if(!strcmp(username,"root")) {
    path = (char*)calloc(strlen("/root/.fshell_historys"),sizeof(char));
    strlcpy(path, "/root/.fshell_historys",count_for_strlcpy("/root/.fshell_historys"));
    return path;
  } else {
    path = (char*)calloc((strlen("/home//.fshell_historys")+strlen(username)),sizeof(char));
    strlcpy(path, "/home/", count_for_strlcpy("/home"));
    strcat(path,username);
    strcat(path,"/.fshell_history");
    return path;
  }
  return NULL;
}
