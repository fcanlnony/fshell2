#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "alias.h"
#include "memory.h"

alias_t init_alias(alias_t alias_chain,const char *alias_name,const char *alias_command)
{
  alias_chain = (alias_t)malloc(sizeof(struct alias));
  char *alias_name_upload = (char*)calloc(strlen(alias_name),sizeof(char));
  char *alias_command_upload = (char*)calloc(strlen(alias_command),sizeof(char));
  strlcpy(alias_name_upload, alias_name, count_for_strlcpy(alias_name));
  strlcpy(alias_command_upload, alias_command, count_for_strlcpy(alias_command));
  alias_chain->alias_name = alias_name_upload;
  alias_chain->alias_command = alias_command_upload;
  alias_chain->next = NULL;
  return alias_chain;
}

alias_t upload_alias_node(alias_t alias_chain,const char *alias_name,const char *alias_command)
{
  alias_t current = alias_chain;
  while(1) {
    if(!strcmp(current->alias_name,alias_name)) {
      char *alias_command_upload = (char*)calloc(strlen(alias_command),sizeof(char));
      strlcpy(alias_command_upload, alias_command, count_for_strlcpy(alias_command));
      current->alias_command = alias_command_upload;
      return alias_chain;
    } else if(current->next != NULL)
      current = current->next;
    else if(current->next == NULL) {
      current->next = init_alias(current->next, alias_name, alias_command);
      return alias_chain;
    }
  }
  return alias_chain;
}

alias_t remove_alias_node(alias_t alias_chain,const char *alias_name)
{
  alias_t current = alias_chain,prev = alias_chain;
  current = current->next;
  if(current == NULL) {
    printf("fshell : unalias : no alias \n");
    return alias_chain;
  }
  while(1) {
      if(!strcmp(current->alias_name,alias_name)) {
	prev->next = current->next;
	free(current->alias_name);
	free(current->alias_command);
	free(current);
	return alias_chain;
      } else if(current->next != NULL) {
	prev = current;
	current = current->next;
      } else return alias_chain;
    }
  return alias_chain;
}

char* getalias_command(const alias_t alias_chain,const char *alias_name)
{
  alias_t current = alias_chain;
  while(1) {
    if(!strcmp(current->alias_name,alias_name))
      return current->alias_command;
    else if(current->next != NULL)
      current = current->next;
    else return NULL;
  }
}
