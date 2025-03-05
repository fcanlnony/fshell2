#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void
execvp_without_pipe(char** array)
{
  pid_t pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  else if (pid == 0) {
    if (execvp(array[0], array) < 0)
      fprintf(stderr, "fshell : %s : command not found.\n", array[0]);
    exit(EXIT_SUCCESS);
  } else
    waitpid(pid, NULL, 0);
}

void
execvp_with_pipe(char** arrayA, char** arrayB)
{
  pid_t parent = fork();
  if (parent == 0) {
    int pipefd[2];
    pipe(pipefd);
    pid_t child = fork();
    if (child == 0) {
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);
      if (execvp(arrayB[0], arrayB) < 0)
        fprintf(stderr, "fshell : %s : command not found.\n", arrayB[0]);
      exit(EXIT_SUCCESS);
    } else if (child > 0) {
      close(pipefd[0]);
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[1]);
      if (execvp(arrayA[0], arrayA) < 0)
        fprintf(stderr, "fshell : %s : command not found.\n", arrayA[0]);
      exit(EXIT_SUCCESS);
    }
  } else if (parent > 0)
    wait(NULL);
}
