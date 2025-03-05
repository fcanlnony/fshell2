#include "../include/config.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../builtin.h"
#include "../include/array.h"
#include "../include/memory.h"
#include "../include/parse.h"
#include "../include/read.h"
#include "../include/type.h"

void
fshell_init(head_alias_t head)
{
  char* file_path = get_fshell_config_path();
  int size = filesize(file_path), line, getsize;
  FILE* fp = fopen(file_path, "r");
  if (fp != NULL) {
    line = get_file_line(file_path);
    if (line > 1) {
      char* content = NULL;
      if (line >= 4) {
        content = (char*)calloc(size / 2 + sizeof(char), sizeof(char));
        getsize = size / 2;
      } else {
        content = (char*)calloc(size + sizeof(char), sizeof(char));
        getsize = size;
      }
      cmd_t cmd = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
      cmd_t copy = cmd;
      copy->next = NULL;
      while (fgets(content, getsize, fp) != NULL) {
        copy->next = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
        copy = copy->next;
        copy->sentence =
          (char*)calloc(count_for_strlcpy(content), sizeof(char));
        strlcpy(copy->sentence, content, count_for_strlcpy(content));
        if (copy->sentence[strlen(copy->sentence) - 1] == '\n')
          copy->sentence[strlen(copy->sentence) - 1] = '\0';
        copy->next = NULL;
        free(content);
        content = (char*)calloc(getsize, sizeof(char));
      }
      cmd_t copy2 = cmd->next;
      int FLAG = NON_BUILTIN_CMD;
      char* array_sentence[FSHELL_INIT_FILE_ARRAY_NUM] = { NULL };
      while (copy2->sentence != NULL) {
        array_parse(copy2->sentence, array_sentence);
        FLAG = check_builtin_cmd(array_sentence[0]);
        if (FLAG != NON_BUILTIN_CMD)
          exec_builtin_cmd(array_sentence, FLAG, head, NULL, NULL);
        else
          printf(
            "fshell: init file: error: %s FLAG %d\n", array_sentence[0], FLAG);
        if (copy2->next != NULL) {
          if (strcmp(copy2->sentence, ""))
            copy2 = copy2->next;
          else
            break;
        } else
          break;
      }
    } else if (line == 1) {
      char* content = (char*)calloc(size, sizeof(char));
      if (fgets(content, size, fp) != NULL) {
        if (content[strlen(content) - 1] == '\n')
          content[strlen(content) - 1] = '\0';
        char* array[FSHELL_INIT_FILE_ARRAY_NUM] = { NULL };
        array_parse(content, array);
        int FLAG = check_builtin_cmd(array[0]);
        if (FLAG != NON_BUILTIN_CMD)
          exec_builtin_cmd(array, FLAG, head, NULL, NULL);
        else
          printf("fshell: init file: error: %s FLAG %d\n", array[0], FLAG);
      }
    }
    fclose(fp);
  }
  free(file_path);
}
