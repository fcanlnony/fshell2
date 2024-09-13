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

char *array[] = {NULL};
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
    fflush(stdin);
    if(!strcmp(input, "exit"))
      exit(0);
    if(check_and(input) == false) {
      if(check_pipe(input) == false) {
	array_parse(input, array);
	execvp_without_pipe(array);
      }
    } else {
      if(check_pipe(input) == false) {
	cmd_t cmd_chain = array_chain_parse(input, cmd_chain);
	cmd_t current = cmd_chain;
	while(1) {
	  if(current->sentence != NULL) {
	    array_parse(current->sentence, array);
	    execvp_without_pipe(array);
	  }
	  if(current->next != NULL) {
	    current = current->next;
	  } else break;
	}	
      }
      free(input);
      fflush(stdout);
      free(user->username);
      free(user->userdir);
      free(user);
    }
  }
}
