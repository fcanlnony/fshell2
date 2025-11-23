#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execvp_without_pipe(char **array)
{
    pid_t pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    else if (pid == 0)
    {
        if (execvp(array[0], array) < 0)
            fprintf(stderr, "fshell : %s : command not found.\n", array[0]);
        exit(EXIT_SUCCESS);
    }
    else
        waitpid(pid, NULL, 0);
}

void execvp_with_pipe(char **arrayA, char **arrayB)
{
    int pipefd[2];
    pid_t pid_a, pid_b;
    if (pipe(pipefd) == -1)
        exit(EXIT_FAILURE);
    
    pid_a = fork();
    if (pid_a == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        if (execvp(arrayA[0], arrayA) < 0)
            fprintf(stderr, "fshell : %s : command not found.\n", arrayA[0]);
        exit(EXIT_FAILURE);
    }

    pid_b = fork();
    if (pid_b == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        if (execvp(arrayB[0], arrayB) < 0)
            fprintf(stderr, "fshell : %s : command not found.\n", arrayB[0]);
        exit(EXIT_FAILURE);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid_a, NULL, 0);
    waitpid(pid_b, NULL, 0);
}
