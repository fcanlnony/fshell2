#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <signal.h>
#include <setjmp.h>

#include "fshell.h"
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
  while(1) { 
    signal(SIGINT,back_jump);
    signal(SIGSEGV,back_jump);
    user = init_user_information(getusername(), getcurrentdir(),user);
    char *prompt = fshell_prompt_readline(user->username, user->userdir, prompt);
    char *input = readline(prompt);
    fflush(stdin);
    if(!strcmp(input, "exit"))
      exit(0);
    if(check_and(input) == false) {
      if(check_pipe(input) == false) {
	array_parse(input, array);
	execvp_without_pipe(array);
      }
    }
    free(input);
    fflush(stdout);
    free(user->username);
    free(user->userdir);
    free(user);
  }
}
