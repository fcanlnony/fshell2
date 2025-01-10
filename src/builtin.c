#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#include "builtin.h"
#include "alias.h"
#include "base.h"
#include "include/memory.h"

short exec_builtin_cmd(char **array, const short FLAG, alias_t head, const char *username,const char *cd_history)
{
  switch(FLAG)
    {
    case LIST_CMD_BUILTIN: {
      if(array[1] == NULL || array[2] != NULL)
	printf("fshell : list : unknown usage\n");
      else if(!strcmp(array[1],"alias")) {
	alias_t current = head;
	if(current == NULL)
	  return -1;
	unsigned short i = 0;
	current = current->next;
	if(current == NULL) {
	  printf("fshell : list : no alias\n");
	  return -1;
	}
	while(1) {
	  if(current->alias_name != NULL) {
	    printf("%s => %s\n",current->alias_name,current->alias_command);
	    i += 1;
	  }
	  if(current->next != NULL)
	    current = current->next;
	  else {
	    printf("Total: %d\n",i);
	    break;
	  }
	}
      } else printf("fshell : %s : Invalid parameter\n",array[1]);
      break;
    }
    case CD_CMD_BUILTIN: {
      if(array[1] == NULL) {
	printf("cd : dir -> null; stop\n");
      } else if(array[2] != NULL)
	printf("cd : too many parameters\n");
      else if(!strncmp(array[1],"~",strlen("~")*sizeof(char))) {
	if(!strcmp(username,"root")) {
	  char *tmp_dir = (char*)calloc(strlen(array[1])-strlen("~")+strlen("/root"), sizeof(char));
	  asprintf(&tmp_dir, "/root/%s", array[1]+strlen("~")*sizeof(char));
	  chdir(tmp_dir);
	  free(tmp_dir);
	} else {
	  char *tmp_dir = (char*)calloc(strlen(array[1])-strlen("~")+strlen(username)+strlen("/home/"),sizeof(char));
	  asprintf(&tmp_dir, "/home/%s/%s", username, array[1]+strlen("~")*sizeof(char));
	  chdir(tmp_dir);
	  free(tmp_dir);
	}
      } else if(!strcmp(array[1],"....")) {
	if(cd_history != NULL) {
	  chdir(cd_history);
	} else printf("fshell : cd : history path : null\n");
      } else chdir(array[1]);
      break;
    }
    case ALIAS_CMD_BUILTIN: {
      if(array[2] == NULL) {
	printf("fshell : alias : unknown usage\n");
	return -1;
      }
      if(array[3] == NULL)
	upload_alias_node(head, array[1], array[2]);
      else if(array[3] != NULL) {
	unsigned int len = 0;
	unsigned short i = 0;
	for(i = 2;array[i] != NULL;i++)
	  len += strlen(array[i]);
	char *tmp_upload_alias = (char*)calloc(len+(i-2),sizeof(char));
	strlcpy(tmp_upload_alias, array[2], count_for_strlcpy(array[2]));
	for(i = 3;array[i] != NULL;i++)
	  asprintf(&tmp_upload_alias, "%s %s",tmp_upload_alias,array[i]);
	upload_alias_node(head, array[1], tmp_upload_alias);
      }
      break;
    }
    case UNALIAS_CMD_BUILTIN: {
      unsigned short n = 1;
      while(array[n] != NULL) {
	head = remove_alias_node(head, array[n]);
	n += 1;
      }
      break;
    }
    case SET_ENV_CMD_BUILTIN: {
      if(array[2] == NULL || array[3] != NULL) {
	printf("fshell : set : unknown usage\n");
	return -1;
      }
      char *tmp_env = (char*)calloc(strlen(array[1])+strlen(array[2])+strlen("="),sizeof(char));
      asprintf(&tmp_env, "%s=%s", array[1], array[2]);
      putenv(tmp_env);
      break;
    }
    case UNSET_ENV_CMD_BUILTIN: {
      unsigned short n = 1;
      while(array[n] != NULL) {
	unsetenv(array[n]);
	n += 1;
      }
      break;
    }
    case PRINT_CMD_BUILTIN: {
      unsigned short n = 1;
      while(array[n] != NULL) {
	if(n >= 2)
	  printf("\n");
	char *tmp_put = getenv(array[n]),*tmp_put2 = getalias_command(head, array[n]);
	if(tmp_put != NULL)
	  printf("env : %s => %s\n",array[n],tmp_put);
	else printf("env : %s : Invalid environment variable\n",array[n]);
	if(tmp_put2 != NULL)
	  printf("alias : %s => %s\n",array[n],tmp_put2);
	else printf("alias : %s : Invalid alias variable\n",array[n]);
	n += 1;
      }
      break;
    }
    default:
      break;
    }
  return 0;
}
