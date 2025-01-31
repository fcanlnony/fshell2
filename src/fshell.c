#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <signal.h>
#include <setjmp.h>
#include <wait.h>
#include <unistd.h>

#include "include/array.h"
#include "include/parse.h"
#include "include/memory.h"
#include "include/config.h"

#include "alias.h"
#include "fshell.h"
#include "base.h"
#include "exec.h"
#include "builtin.h"
#include "include/type.h"

static jmp_buf sig_back_while,sig_stop_fshell;
static void back_jump_sigINT()
{
  longjmp(sig_back_while, 1);
}

static void back_jump_sigSEGV()
{
  longjmp(sig_stop_fshell, 1);
}

_Noreturn void help()
{  
  printf("Options:\n\
--version,-v print version\n\
--help,-h get some help\n\
--compile-time,-ct print the compilation time\n");
  exit(0);
}

int main(int argc,char **argv)
{
  if(argc > 1) {
    if(argc == 2) {
      if(!strcmp(argv[1],"--version") || !strcmp(argv[1],"-v"))
	PRINT_VERSION(SHELL_NAME, FSHELL_VERSION);
      else if(!strcmp(argv[1],"--help") || !strcmp(argv[1],"-h"))
	help();
      else if(!strcmp(argv[1],"--compile-time") || !strcmp(argv[1],"-ct"))
	printf("Time:%s Date:%s\n",__TIME__,__DATE__);
      else printf("Unknow option :%s \n",argv[1]);
    } else help();
    exit(0);
  }
  signal(SIGINT,back_jump_sigINT);
  signal(SIGSEGV,back_jump_sigSEGV);
  user_t user = init_user_information(getusername(), getcurrentdir(),user);
  head_alias_t alias = init_alias_head(alias);
  char *readline_path = NULL;
  char *username = getusername();
  if(check_root(username) == true) 
    readline_path = readline_history_path(username, readline_path);
  else readline_path = readline_history_path(username, readline_path);
  read_history(readline_path);
  char *cd_history = NULL;
  fshell_init(alias);
  if(getenv("FSHELL_PIPE_NUM") != NULL)
    array_pipe_num = atoi(getenv("FSHELL_PIPE_NUM"));
  else array_pipe_num = 0;
  short check_num = 0;
  if(array_pipe_num == 0)
    array_pipe_num = 20;
  if(setjmp(sig_stop_fshell)) {
    printf("stop fshell\n");
    exit(0);
  }
  if(setjmp(sig_back_while)) {
      FREE_USERT_FUNC(user->userdir);
      printf("\n");
  }
  while(1) {
    user->userdir = getcurrentdir();
    char *prompt = fshell_prompt_readline(user->username, user->userdir, prompt);
    char *input = readline(prompt);
    if(!strcmp(input,"")) {
      FREE_USERT_FUNC(input);
      FREE_USERT_FUNC(prompt);
      FREE_USERT_FUNC(user->userdir);
      continue;
    }
    add_history(input);
    write_history(readline_path);
    if(!strcmp(input, "exit"))
      exit(0);
    if(check_and(input) == false) {
      if(check_pipe(input) == false) {
	char *array[array_pipe_num],*array_alias[array_pipe_num],*array_alias_tmp[array_pipe_num],*input_array[array_pipe_num];
	char *copy_input = (char*)calloc(count_for_strlcpy(input),sizeof(char));
	strlcpy(copy_input, input, count_for_strlcpy(input));
	array_parse(input, array);
	check_num = check_builtin_cmd(array[0]);
	if(check_num != NON_BUILTIN_CMD)
	  exec_builtin_cmd(array, check_num, alias, user->username, cd_history);
	else {
	  char *alias_cmd = getalias_command(alias, array[0]);
	  if(alias_cmd == NULL)
	    execvp_without_pipe(array);
	  else {
	    char *tmp_alias_cmd_copy = (char*)calloc(count_for_strlcpy(alias_cmd),sizeof(char));
	    strlcpy(tmp_alias_cmd_copy,alias_cmd,count_for_strlcpy(alias_cmd));
	    array_parse(tmp_alias_cmd_copy, array_alias_tmp);
	    unsigned short i,k;
	    for(i = 0;array_alias_tmp[i] != NULL;i++)
	      array_alias[i] = array_alias_tmp[i];
	    array_parse(copy_input, input_array);
	    if(input_array[1] != NULL)
	      for(k = 1;input_array[k] != NULL;i++,k++)
		array_alias[i] = input_array[k];
	    else array_alias[i] = NULL;
	    execvp_without_pipe(array_alias);
	    free(tmp_alias_cmd_copy);
	    free(copy_input);
	  }
	}
      } else {
	char *arrayA[array_pipe_num],*arrayB[array_pipe_num];
	char *arrayA_alias[array_pipe_num],*arrayA_alias_tmp[array_pipe_num],*input_arrayA[array_pipe_num];
	char *arrayB_alias[array_pipe_num],*arrayB_alias_tmp[array_pipe_num],*input_arrayB[array_pipe_num];
	char *tmp_aliasA_cmd_copy = NULL,*tmp_aliasB_cmd_copy = NULL;
	char *alias_cmd = NULL;
	pipe_t pipe_chain = array_pipe_parse(input, pipe_chain);
	char *copy_inputA = (char*)calloc(count_for_strlcpy(pipe_chain->sentence),sizeof(char));
	strlcpy(copy_inputA, pipe_chain->sentence, count_for_strlcpy(pipe_chain->sentence));
	array_parse(pipe_chain->sentence, arrayA);
	alias_cmd = getalias_command(alias, arrayA[0]);
	if(alias_cmd == NULL) {
	  array_parse(copy_inputA, arrayA_alias);
	} else {
	  tmp_aliasA_cmd_copy = (char*)calloc(count_for_strlcpy(alias_cmd),sizeof(char));
	  strlcpy(tmp_aliasA_cmd_copy,alias_cmd,count_for_strlcpy(alias_cmd));
	  array_parse(tmp_aliasA_cmd_copy, arrayA_alias_tmp);
	  unsigned short i,k;
	  for(i = 0;arrayA_alias_tmp[i] != NULL;i++) {
	    arrayA_alias[i] = arrayA_alias_tmp[i];
	  }
	  array_parse(copy_inputA, input_arrayA);
	  if(input_arrayA[1] != NULL)
	    for(k = 1;input_arrayA[k] != NULL;k++,i++) {
	      arrayA_alias[i] = input_arrayA[k];
	    }
	  else arrayA_alias[i] = NULL;
	}
	pipe_chain = pipe_chain->next;
	char *copy_inputB = (char*)calloc(count_for_strlcpy(pipe_chain->sentence),sizeof(char));
	strlcpy(copy_inputB, pipe_chain->sentence, count_for_strlcpy(pipe_chain->sentence));
	array_parse(pipe_chain->sentence, arrayB);
	alias_cmd = getalias_command(alias, arrayB[0]);
	if(alias_cmd == NULL) {
	  array_parse(copy_inputB, arrayB_alias);
	} else {
	  tmp_aliasB_cmd_copy = (char*)calloc(count_for_strlcpy(alias_cmd),sizeof(char));
	  strlcpy(tmp_aliasB_cmd_copy,alias_cmd,count_for_strlcpy(alias_cmd));
	  array_parse(tmp_aliasB_cmd_copy, arrayB_alias_tmp);
	  unsigned short i,k;
	  for(i = 0;arrayB_alias_tmp[i] != NULL;i++) {
	    arrayB_alias[i] = arrayB_alias_tmp[i];
	  }
	  array_parse(copy_inputB, input_arrayB);
	  if(input_arrayB[1] != NULL)
	    for(k = 1;input_arrayB[k] != NULL;k++,i++) {
	      arrayB_alias[i] = input_arrayB[k];
	    }
	  else arrayB_alias[i] = NULL;
	}
	execvp_with_pipe(arrayA_alias,arrayB_alias);
	if(tmp_aliasA_cmd_copy != NULL)
	  FREE_USERT_FUNC(tmp_aliasA_cmd_copy);
	if(tmp_aliasB_cmd_copy != NULL)
	  FREE_USERT_FUNC(tmp_aliasB_cmd_copy);
      }
    } else {
      cmd_t cmd_chain = array_chain_parse(input, cmd_chain);
      cmd_t current = cmd_chain;
      char *alias_cmd = NULL;
      while(1) {
	if(current->sentence != NULL) {
	  if(check_pipe(current->sentence) == false) {
	    char *array[array_pipe_num],*array_alias[array_pipe_num],*array_alias_tmp[array_pipe_num],*input_array[array_pipe_num];
	    char *copy_input = (char*)calloc(count_for_strlcpy(input),sizeof(char));
	    strlcpy(copy_input, current->sentence, count_for_strlcpy(current->sentence));
	    array_parse(current->sentence, array);
	    check_num = check_builtin_cmd(array[0]);
	    if(check_num != NON_BUILTIN_CMD)
	      exec_builtin_cmd(array, check_num, alias, user->username, cd_history);
	    else {
	      char *alias_cmd = getalias_command(alias, array[0]);
	      if(alias_cmd == NULL)
		execvp_without_pipe(array);
	      else {
		char *tmp_alias_cmd_copy = (char*)calloc(count_for_strlcpy(alias_cmd),sizeof(char));
		strlcpy(tmp_alias_cmd_copy,alias_cmd,count_for_strlcpy(alias_cmd));
		array_parse(tmp_alias_cmd_copy, array_alias_tmp);
		unsigned short i,k;
		for(i = 0;array_alias_tmp[i] != NULL;i++)
		  array_alias[i] = array_alias_tmp[i];
		array_parse(copy_input, input_array);
		if(input_array[1] != NULL)
		  for(k = 1;input_array[k] != NULL;i++,k++)
		    array_alias[i] = input_array[k];
		else array_alias[i] = NULL;
		execvp_without_pipe(array_alias);
		free(tmp_alias_cmd_copy);
		free(copy_input);
	      }
	    }
	  } else {
	    pipe_t pipe_chain = array_pipe_parse(current->sentence, pipe_chain);
	    char *arrayA[array_pipe_num],*arrayB[array_pipe_num];
	    char *arrayA_alias[array_pipe_num],*arrayA_alias_tmp[array_pipe_num],*input_arrayA[array_pipe_num];
	    char *arrayB_alias[array_pipe_num],*arrayB_alias_tmp[array_pipe_num],*input_arrayB[array_pipe_num];
	    char *tmp_aliasA_cmd_copy = NULL,*tmp_aliasB_cmd_copy = NULL;
	    char *alias_cmd = NULL;
	    char *copy_inputA = (char*)calloc(count_for_strlcpy(pipe_chain->sentence),sizeof(char));
	    strlcpy(copy_inputA, pipe_chain->sentence, count_for_strlcpy(pipe_chain->sentence));
	    array_parse(pipe_chain->sentence, arrayA);
	    alias_cmd = getalias_command(alias, arrayA[0]);
	    if(alias_cmd == NULL) {
	      array_parse(copy_inputA, arrayA_alias);
	    } else {
	      tmp_aliasA_cmd_copy = (char*)calloc(count_for_strlcpy(alias_cmd),sizeof(char));
	      strlcpy(tmp_aliasA_cmd_copy,alias_cmd,count_for_strlcpy(alias_cmd));
	      array_parse(tmp_aliasA_cmd_copy, arrayA_alias_tmp);
	      unsigned short i,k;
	      for(i = 0;arrayA_alias_tmp[i] != NULL;i++) {
		arrayA_alias[i] = arrayA_alias_tmp[i];
	      }
	      array_parse(copy_inputA, input_arrayA);
	      if(input_arrayA[1] != NULL)
		for(k = 1;input_arrayA[k] != NULL;k++,i++) {
		  arrayA_alias[i] = input_arrayA[k];
		}
	      else arrayA_alias[i] = NULL;
	    }
	    pipe_chain = pipe_chain->next;
	    char *copy_inputB = (char*)calloc(count_for_strlcpy(pipe_chain->sentence),sizeof(char));
	    strlcpy(copy_inputB, pipe_chain->sentence, count_for_strlcpy(pipe_chain->sentence));
	    array_parse(pipe_chain->sentence, arrayB);
	    alias_cmd = getalias_command(alias, arrayB[0]);
	    if(alias_cmd == NULL) {
	      array_parse(copy_inputB, arrayB_alias);
	    } else {
	      tmp_aliasB_cmd_copy = (char*)calloc(count_for_strlcpy(alias_cmd),sizeof(char));
	      strlcpy(tmp_aliasB_cmd_copy,alias_cmd,count_for_strlcpy(alias_cmd));
	      array_parse(tmp_aliasB_cmd_copy, arrayB_alias_tmp);
	      unsigned short i,k;
	      for(i = 0;arrayB_alias_tmp[i] != NULL;i++) {
		arrayB_alias[i] = arrayB_alias_tmp[i];
	      }
	      array_parse(copy_inputB, input_arrayB);
	      if(input_arrayB[1] != NULL)
		for(k = 1;input_arrayB[k] != NULL;k++,i++) {
		  arrayB_alias[i] = input_arrayB[k];
		}
	      else arrayB_alias[i] = NULL;
	    }
	    execvp_with_pipe(arrayA_alias,arrayB_alias);
	    if(tmp_aliasA_cmd_copy != NULL)
	      FREE_USERT_FUNC(tmp_aliasA_cmd_copy);
	    if(tmp_aliasB_cmd_copy != NULL)
	      FREE_USERT_FUNC(tmp_aliasB_cmd_copy);
	  }
	}
	if(current->next != NULL)
	  current = current->next;
	else break;
      }
    }
    cd_history = (char*)realloc(cd_history, strlen(user->userdir)*sizeof(char));
    strlcpy(cd_history,user->userdir,count_for_strlcpy(user->userdir));
    FREE_USERT_FUNC(input);
    FREE_USERT_FUNC(prompt);
    FREE_USERT_FUNC(user->userdir);
  }
}
    
