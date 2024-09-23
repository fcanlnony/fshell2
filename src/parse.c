#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "array.h"
#include "memory.h"

cmd_t array_chain_parse(char *input,cmd_t array_chain)
{
  array_chain = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
  cmd_t current = array_chain;
  char *tmp_str = NULL;
  tmp_str = strtok(input,"&&");
  if(tmp_str != NULL) {
    char *new = (char*)calloc(strlen(tmp_str),sizeof(char));
    strlcpy(new,tmp_str,count_for_strlcpy(tmp_str));
    current->sentence = new;
    while(1) {
      tmp_str = strtok(NULL,"&&");
      if(tmp_str != NULL) {
	current->next = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
	current = current->next;
	char *new2 = (char*)calloc(strlen(tmp_str),sizeof(char));
	strlcpy(new2,tmp_str,count_for_strlcpy(tmp_str));
	current->sentence = new2;
	current->next = NULL;
      } else break;
    }
    return array_chain;
  } else return NULL;
}

pipe_t array_pipe_parse(char *input,pipe_t pipe_chain)
{
  pipe_chain = (pipe_t)malloc(sizeof(struct cmd_or_pipe_chain));
  pipe_t current = pipe_chain;
  char *tmp_str = NULL;
  tmp_str = strtok(input,"|");
  if(tmp_str != NULL) {
    char *new = (char*)calloc(strlen(tmp_str),sizeof(char));
    strlcpy(new,tmp_str,count_for_strlcpy(tmp_str));
    current->sentence = new;
    while(1) {
      tmp_str = strtok(NULL,"|");
      if(tmp_str != NULL) {
	current->next = (pipe_t)malloc(sizeof(struct cmd_or_pipe_chain));
	current = current->next;
	char *new2 = (char*)calloc(strlen(tmp_str),sizeof(char));
	strlcpy(new2,tmp_str,count_for_strlcpy(tmp_str));
	current->sentence = new2;
	current->next = NULL;
      } else break;
    }
    return pipe_chain;
  } else return NULL;
}

void array_parse(char *string,char **array)
{
  array[0] = strtok(string," ");
  short i = 0;
  while(array[i] != NULL)
    array[++i] = strtok(NULL," ");
}

bool check_and(char *string)
{
  if(strstr(string, "&&") != NULL)
    return true;
  else return false;
}

bool check_pipe(char *string)
{
  if(strchr(string, '|') != NULL)
    return true;
  else return false;
}

bool check_builtin(char *string)
{
  if(!strcmp(string,"exit"))
    return true;
  else return false;
}

bool check_root(char *username)
{
  if(!strcmp(username,"root"))
    return true;
  else return false;
}
