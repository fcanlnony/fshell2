#include "alias.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/memory.h"
#include "include/type.h"

static alias_t init_alias(alias_t alias_chain, const char *alias_name, const char *alias_command)
{
    alias_chain = (alias_t)malloc(sizeof(struct alias));
    char *alias_name_upload = (char *)calloc(count_for_strlcpy(alias_name), sizeof(char));
    char *alias_command_upload = (char *)calloc(count_for_strlcpy(alias_command), sizeof(char));
    strlcpy(alias_name_upload, alias_name, count_for_strlcpy(alias_name));
    strlcpy(alias_command_upload, alias_command, count_for_strlcpy(alias_command));
    alias_chain->alias_name = alias_name_upload;
    alias_chain->alias_command = alias_command_upload;
    alias_chain->next = NULL;
    return alias_chain;
}

head_alias_t init_alias_head(head_alias_t alias_head)
{
    alias_head = (head_alias_t)malloc(sizeof(struct head_alias));
    if (alias_head != NULL)
    {
        alias_head->next = NULL;
        return alias_head;
    }
    else
        return NULL;
}

head_alias_t upload_alias_node(head_alias_t alias_head, const char *alias_name, const char *alias_command)
{
    if (alias_head->next != NULL)
    {
        alias_t current = alias_head->next;
        while (1)
        {
            if (!strcmp(current->alias_name, alias_name))
            {
                char *alias_command_upload = (char *)calloc(count_for_strlcpy(alias_command), sizeof(char));
                strlcpy(alias_command_upload, alias_command, count_for_strlcpy(alias_command));
                current->alias_command = alias_command_upload;
                return alias_head;
            }
            else if (current->next != NULL)
                current = current->next;
            else if (current->next == NULL)
            {
                current->next = init_alias(current->next, alias_name, alias_command);
                return alias_head;
            }
        }
    }
    else
    {
        alias_head->next = init_alias(alias_head->next, alias_name, alias_command);
        return alias_head;
    }
}

head_alias_t remove_alias_node(head_alias_t alias_head, const char *alias_name)
{
    alias_t current = alias_head->next, prev = alias_head->next;
    if (current == NULL)
    {
        printf("fshell : unalias : no alias\n");
        return alias_head;
    }
    if (strcmp(current->alias_name, alias_name))
    {
        if (current->next != NULL)
        {
            current = current->next;
            while (1)
            {
                if (!strcmp(current->alias_name, alias_name))
                {
                    prev->next = current->next;
                    free(current->alias_name);
                    free(current->alias_command);
                    free(current);
                    return alias_head;
                }
                else if (current->next != NULL)
                {
                    prev = current;
                    current = current->next;
                }
                else
                    return alias_head;
            }
        }
        else
            return alias_head;
    }
    else
    {
        alias_head->next = current->next;
        return alias_head;
    }
}

char *getalias_command(const head_alias_t alias_head, const char *alias_name)
{
    if (alias_head->next != NULL)
    {
        alias_t current = alias_head->next;
        while (1)
        {
            if (!strcmp(current->alias_name, alias_name))
                return current->alias_command;
            else if (current->next != NULL)
                current = current->next;
            else
                return NULL;
        }
    }
    else
        return NULL;
}
