#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <sys/types.h>

#include "fshell.h"
#include "memory.h"
#include "array.h"
#include "base.h"

int main(int argc,char **argv)
{
  user_t user = init_user_information(getusername(), getcurrentdir(),user);
  char *prompt = fshell_prompt_readline(user->username,user->userdir,prompt);
  readline(prompt);
}
