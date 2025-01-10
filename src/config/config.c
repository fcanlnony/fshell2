#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "../include/parse.h"
#include "../include/config.h"
#include "../include/read.h"
#include "../include/type.h"
#include "../include/array.h"
#include "../include/memory.h"
#include "../builtin.h"

static cmd_t array_chain_parse_config(char *content,cmd_t array_chain)
{
  array_chain = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
  unsigned short i = 0;
  cmd_t current = array_chain;
  char *tmp_str = NULL;
  tmp_str = strtok(content,";");
  if(tmp_str != NULL) {
    char *new = (char*)calloc(strlen(tmp_str),sizeof(char));
    strlcpy(new,tmp_str,count_for_strlcpy(tmp_str));
    current->sentence = new;
    while(1) {
      tmp_str = strtok(NULL,";");
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

void fshell_init(alias_t head)
{
  char *file_path = get_fshell_config_path();
  int size = filesize(file_path), line = fileline(file_path),getsize;
  FILE *fp = fopen(file_path, "r");
  if(fp != NULL) {
    char *array = NULL;
    if(line >= 4) {
      array = (char*)calloc(size/2, sizeof(char));
      getsize = size/2;
    }
    else {
      array = (char*)calloc(size, sizeof(char));
      getsize = size;
    }
    cmd_t cmd = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
    cmd_t copy = cmd;
    copy->next = NULL;
    int i = 0;
    while(fgets(array, getsize, fp) != NULL) {
      copy->next = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
      copy = copy->next;
      copy->sentence = (char*)calloc(count_for_strlcpy(array), sizeof(char));
      strlcpy(copy->sentence, array, count_for_strlcpy(array));
      copy->sentence[strlen(copy->sentence) - 1] = '\0';
      copy->next = NULL;
      free(array);
      array = (char*)calloc(getsize, sizeof(char));
    }
    cmd_t copy2 = cmd->next;
    int FLAG = NON_BUILTIN_CMD;
    char *array_sentence[FSHELL_INIT_FILE_ARRAY_NUM] = {NULL};
    while(copy2->sentence != NULL) {
      array_parse(copy2->sentence, array_sentence);
      FLAG = check_builtin_cmd(array_sentence[0]);
      if(FLAG != NON_BUILTIN_CMD)
	exec_builtin_cmd(array_sentence, FLAG, head, NULL, NULL);
      else printf("fshell: init file: error: %s FLAG %d\n", array_sentence[0], FLAG);
      if(copy2->next != NULL)
	copy2 = copy2->next;
      else break;
    }
    fclose(fp);
  }
  printf("define:%d",FSHELL_INIT_FILE_ARRAY_NUM);
}
