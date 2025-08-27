#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "include/array.h"
#include "include/parse.h"

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
    pid_t parent = fork();
    if (parent == 0)
    {
        int pipefd[2];
        pipe(pipefd);
        pid_t child = fork();
        if (child == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(arrayB[0], arrayB) < 0)
                fprintf(stderr, "fshell : %s : command not found.\n", arrayB[0]);
            exit(EXIT_SUCCESS);
        }
        else if (child > 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            if (execvp(arrayA[0], arrayA) < 0)
                fprintf(stderr, "fshell : %s : command not found.\n", arrayA[0]);
            exit(EXIT_SUCCESS);
        }
    }
    else if (parent > 0)
        wait(NULL);
}

void execvp_with_multi_pipe(pipe_t pipe_chain)
{
    int pipe_count = 0;
    pipe_t current = pipe_chain;
    while (current != NULL)
    {
        pipe_count++;
        current = current->next;
    }
    
    if (pipe_count <= 1)
        return;
    
    if (pipe_count == 2)
    {
        char **arrayA = (char **)calloc(20, sizeof(char *));
        char **arrayB = (char **)calloc(20, sizeof(char *));
        
        array_parse(pipe_chain->sentence, arrayA);
        array_parse(pipe_chain->next->sentence, arrayB);
        
        execvp_with_pipe(arrayA, arrayB);
        
        free(arrayA);
        free(arrayB);
        return;
    }
    
    int (*pipes)[2] = malloc((pipe_count - 1) * sizeof(int[2]));
    pid_t *pids = malloc(pipe_count * sizeof(pid_t));
    
    for (int i = 0; i < pipe_count - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    
    current = pipe_chain;
    for (int i = 0; i < pipe_count; i++)
    {
        pids[i] = fork();
        
        if (pids[i] == 0)
        {
            if (i > 0)
            {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            if (i < pipe_count - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            for (int j = 0; j < pipe_count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            char **args = (char **)calloc(20, sizeof(char *));
            array_parse(current->sentence, args);
            
            if (execvp(args[0], args) < 0)
            {
                fprintf(stderr, "fshell : %s : command not found.\n", args[0]);
                exit(EXIT_FAILURE);
            }
        }
        else if (pids[i] < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        current = current->next;
    }
    
    for (int i = 0; i < pipe_count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    for (int i = 0; i < pipe_count; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
    
    free(pipes);
    free(pids);
}
