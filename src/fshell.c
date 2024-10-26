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

#include "alias.h"
#include "fshell.h"
#include "array.h"
#include "base.h"
#include "parse.h"
#include "exec.h"
#include "builtin.h"
#include "memory.h"

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
  user_t user = NULL;
  alias_t alias = init_alias(alias, " ", " ");
  char *readline_path = NULL;
  char *username = getusername();
  if(check_root(username) == true) 
    readline_path = readline_history_path(username, readline_path);
  else readline_path = readline_history_path(username, readline_path);
  read_history(readline_path);
  char *cd_history = NULL;
  volatile short check_num;
  if(setjmp(sig_stop_fshell)) {
    printf("stop fshell\n");
    exit(0);
  }
  while(1) {
    user = init_user_information(getusername(), getcurrentdir(),user);
    char *prompt = fshell_prompt_readline(user->username, user->userdir, prompt);
    char *input = readline(prompt);
    if(!strcmp(input,"")) {
      FREE_USERT_FUNC(input);
      FREE_USERT_FUNC(prompt);
      FREE_USERT_FUNC(user->username);
      FREE_USERT_FUNC(user->userdir);
      FREE_USERT_FUNC(user);
      continue;
    }
    if(setjmp(sig_back_while)) {
      FREE_USERT_FUNC(prompt);
      FREE_USERT_FUNC(user->username);
      FREE_USERT_FUNC(user->userdir);
      FREE_USERT_FUNC(user);
      printf("\n");
      continue;
    }
    add_history(input);
    write_history(readline_path);
    if(!strcmp(input, "exit"))
      exit(0);
    if(check_and(input) == false) {
      if(check_pipe(input) == false) {
	char *array[] = {NULL};
	array_parse(input, array);
	check_num = check_builtin_cmd(array[0]);
	if(check_num != NON_BUILTIN_CMD)
	  exec_builtin_cmd(array, check_num, alias, user->username, cd_history);
	else {
	  char *alias_cmd = getalias_command(alias, array[0]);
	  if(alias_cmd != NULL)
	    array[0] = alias_cmd;
	  execvp_without_pipe(array);
	}
      } else {
	char *arrayA[20] = {NULL},*arrayB[20] = {NULL};
	char *alias_cmd = NULL;
	pipe_t pipe_chain = array_pipe_parse(input, pipe_chain);
	array_parse(pipe_chain->sentence, arrayA);
	alias_cmd = getalias_command(alias, arrayA[0]);
	if(alias_cmd != NULL)
	  arrayA[0] = alias_cmd;
	pipe_chain = pipe_chain->next;
	array_parse(pipe_chain->sentence, arrayB);
	alias_cmd = getalias_command(alias, arrayB[0]);
	if(alias_cmd != NULL)
	  arrayB[0] = alias_cmd;
	execvp_with_pipe(arrayA,arrayB);
      }
    } else {
      cmd_t cmd_chain = array_chain_parse(input, cmd_chain);
      cmd_t current = cmd_chain;
      char *arrayA[20] = {NULL},*arrayB[20] = {NULL};
      char *alias_cmd = NULL;
      while(1) {
	if(current->sentence != NULL) {
	  if(check_pipe(current->sentence) == false) {
	    char *array[] = {NULL};
	    array_parse(current->sentence, array);
	    char *alias_cmd = getalias_command(alias, array[0]);
	    if(alias_cmd != NULL)
	      array[0] = alias_cmd;
	    execvp_without_pipe(array);
	  } else {
	    pipe_t pipe_chain = array_pipe_parse(current->sentence, pipe_chain);
	    array_parse(pipe_chain->sentence, arrayA);
	    alias_cmd = getalias_command(alias, arrayA[0]);
	    if(alias_cmd != NULL)
	      arrayA[0] = alias_cmd;
	    pipe_chain = pipe_chain->next;
	    array_parse(pipe_chain->sentence, arrayB);
	    alias_cmd = getalias_command(alias, arrayB[0]);
	    if(alias_cmd != NULL)
	      arrayB[0] = alias_cmd;
	    execvp_with_pipe(arrayA,arrayB);
	    fflush(stdout);
	  }
	}
	if(current->next != NULL) {
	  current = current->next;
	} else break;
      }
    }
    cd_history = (char*)realloc(cd_history, strlen(user->userdir)*sizeof(char));
    strlcpy(cd_history,user->userdir,count_for_strlcpy(user->userdir));
    FREE_USERT_FUNC(input);
    FREE_USERT_FUNC(prompt);
    FREE_USERT_FUNC(user->username);
    FREE_USERT_FUNC(user->userdir);
    FREE_USERT_FUNC(user);
  }
}
