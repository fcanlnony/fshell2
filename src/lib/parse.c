#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/array.h"
#include "../include/memory.h"
#include "../include/parse.h"

cmd_or_pipe_chain_t *array_vanilla_parse(char *input, cmd_or_pipe_chain_t *vanilla_chain, char *symbol)
{
    vanilla_chain = (cmd_or_pipe_chain_t *)malloc(sizeof(struct cmd_or_pipe_chain));
    if (vanilla_chain == NULL)
        return (cmd_or_pipe_chain_t *)NULL;
    cmd_t current = vanilla_chain;
    char *tmp_str = NULL;
    tmp_str = strtok(input, symbol);
    if (tmp_str != NULL)
    {
        char *new = NULL;
        new = mstrcpy(new, tmp_str);
        current->sentence = new;
        while (1)
        {
            tmp_str = strtok(NULL, symbol);
            if (tmp_str != NULL)
            {
                current->next = (cmd_or_pipe_chain_t *)malloc(sizeof(struct cmd_or_pipe_chain));
                current = current->next;
                char *new2 = NULL;
                new2 = mstrcpy(new2, tmp_str);
                current->sentence = new2;
                current->next = NULL;
            }
            else
                break;
        }
        return vanilla_chain;
    }
    else
        return (cmd_or_pipe_chain_t *)NULL;
}

/*
cmd_t array_chain_parse(char *input, cmd_t array_chain)
{
    return array_vanilla_parse(input, array_chain, "&&");
}

pipe_t array_pipe_parse(char *input, pipe_t pipe_chain)
{
  return array_vanilla_parse(input, array_parse(char *string, char **array), char *symbol)
}
*/

bool check_and(const char *string)
{
    if (strstr(string, "&&") != NULL)
        return true;
    else
        return false;
}

bool check_pipe(const char *string)
{
    if (strchr(string, '|') != NULL)
        return true;
    else
        return false;
}

bool check_builtin(const char *string)
{
    if (!strcmp(string, "exit"))
        return true;
    else
        return false;
}

bool check_root(const char *username)
{
    if (!strcmp(username, "root"))
        return true;
    else
        return false;
}
