#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

short execvp_without_pipe(char **array)
{
  pid_t pid = fork();
  if(pid < 0) {
    return -1;
  } else if(pid == 0) {
    if(execvp(array[0],array) < 0)
      fprintf(stderr, "fshell : %s : command not found.\n", array[0]);
    return -1;
  } else wait(NULL);
  return 0;
}
/*
void execvp_with_pipe(pipe_t pipe_chain)
{
  
}*/
