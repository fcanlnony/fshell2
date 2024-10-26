#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

short execvp_without_pipe(char **array)
{
  pid_t pid = fork();
  if(pid < 0)
    return -1;
  else if(pid == 0) {
    if(execvp(array[0],array) < 0)
      fprintf(stderr, "fshell : %s : command not found.\n", array[0]);
    exit(0);
  } else wait(NULL);
  return 0;
}

short execvp_with_pipe(char **arrayA,char **arrayB)
{
  pid_t parent = fork();
  if(parent == 0) {
    int pipefd[2];
    pipe(pipefd);
    pid_t child = fork();
    if(child == 0) {
      close(pipefd[1]);
      dup2(pipefd[0],0);
      if(execvp(arrayB[0],arrayB) < 0)
	fprintf(stderr, "fshell : %s : command not found.\n", arrayB[0]);
      exit(0);
    } else if(child > 0) {
      close(pipefd[0]);
      dup2(pipefd[1],1);
      if(execvp(arrayA[0],arrayA) < 0)
	fprintf(stderr, "fshell : %s : command not found.\n", arrayA[0]);
      exit(0);
    }
  } else if(parent > 0)
    wait(NULL);
  return 0;
}
