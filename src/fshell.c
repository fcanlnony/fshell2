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

#include "fshell.h"
#include "array.h"
#include "base.h"
#include "parse.h"
#include "exec.h"

static jmp_buf sig_back_while;
static void back_jump()
{
  longjmp(sig_back_while, 1);
}

int main(int argc,char **argv)
{
  if(setjmp(sig_back_while))
    printf("\n");
  user_t user = NULL;
  char *readline_path = NULL;
  if(check_root(getusername()) == true) 
    readline_path = readline_history_path(getusername(), readline_path);
  else readline_path = readline_history_path(getusername(), readline_path);
  read_history(readline_path);
  while(1) { 
    signal(SIGINT,back_jump);
    signal(SIGSEGV,back_jump);
    user = init_user_information(getusername(), getcurrentdir(),user);
    char *prompt = fshell_prompt_readline(user->username, user->userdir, prompt);
    char *input = readline(prompt);
    add_history(input);
    write_history(readline_path);
    if(!strcmp(input, "exit"))
      exit(0);
    if(check_and(input) == false) {
      if(check_pipe(input) == false) {
	char *array[] = {NULL};
	array_parse(input, array);
	execvp_without_pipe(array);
      } else {
	char *arrayA[20] = {NULL},*arrayB[20] = {NULL};
	pipe_t pipe_chain = array_pipe_parse(input, pipe_chain);
	array_parse(pipe_chain->sentence, arrayA);
	pipe_chain = pipe_chain->next;
	array_parse(pipe_chain->sentence, arrayB);
	execvp_with_pipe(arrayA,arrayB);
      }
    } else {
      cmd_t cmd_chain = array_chain_parse(input, cmd_chain);
      cmd_t current = cmd_chain;
      while(1) {
	if(current->sentence != NULL) {
	  if(check_pipe(current->sentence) == false) {
	    char *array[] = {NULL};
	    array_parse(current->sentence, array);
	    execvp_without_pipe(array);
	  } else {
	    char *arrayA[20] = {NULL},*arrayB[20] = {NULL};
	    pipe_t pipe_chain = array_pipe_parse(current->sentence, pipe_chain);
	    array_parse(pipe_chain->sentence, arrayA);
	    pipe_chain = pipe_chain->next;
	    array_parse(pipe_chain->sentence, arrayB);
	    execvp_with_pipe(arrayA,arrayB);
	  }
	}
	if(current->next != NULL) {
	  current = current->next;
	} else break;
      }
    }
    FREE_USERT_FUNC(input);
    FREE_USERT_FUNC(prompt);
    FREE_USERT_FUNC(user->username);
    FREE_USERT_FUNC(user->userdir);
    FREE_USERT_FUNC(user);
  }
}
