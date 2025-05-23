#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "alias.h"
#include "base.h"
#include "builtin.h"
#include "include/array.h"
#include "include/config.h"
#include "include/memory.h"
#include "include/parse.h"
#include "include/read.h"
#include "include/type.h"

static void LIST_CMD(char **array, head_alias_t head)
{
    if (array[1] == NULL || array[2] != NULL)
        printf("fshell: list: unknown usage\n");
    else if (!strcmp(array[1], "alias"))
    {
        alias_t current = head->next;
        unsigned short i = 0;
        if (current == NULL)
        {
            printf("fshell: list: no alias\n");
            return (void)-1;
        }
        while (1)
        {
            if (current->alias_name != NULL)
            {
                printf("%s => %s\n", current->alias_name, current->alias_command);
                i += 1;
            }
            if (current->next != NULL)
                current = current->next;
            else
            {
                printf("Total: %d\n", i);
                break;
            }
        }
    }
    else
        printf("fshell: %s : Invalid parameter\n", array[1]);
}

static void CD_CMD(char **array, const char *username, const char *cd_history)
{
    if (array[1] == NULL)
    {
        printf("fshell: cd: dir -> null; stop\n");
    }
    else if (array[2] != NULL)
        printf("fshell: cd: too many parameters\n");
    else if (!strncmp(array[1], "~", strlen("~") * sizeof(char)))
    {
        char *tmp_dir = NULL;
        if (!strcmp(username, "root"))
            asprintf(&tmp_dir, "/root/%s", array[1] + strlen("~") * sizeof(char));
        else
            asprintf(&tmp_dir, "/home/%s/%s", username, array[1] + strlen("~") * sizeof(char));
        if (access(tmp_dir, F_OK) == 0)
        {
            if (access(tmp_dir, X_OK) == 0)
                chdir(tmp_dir);
            else
                printf("fshell: cd: permission denied\n");
        }
        else
            printf("fshell: cd: no such directory\n");
        if (tmp_dir != NULL)
            free(tmp_dir);
    }
    else if (!strcmp(array[1], "...."))
    {
        if (cd_history != NULL)
        {
            chdir(cd_history);
        }
        else
            printf("fshell: cd: history path : null\n");
    }
    else
    {
        if (access(array[1], F_OK) == 0)
        {
            if (access(array[1], X_OK) == 0)
                chdir(array[1]);
            else
                printf("fshell: cd: permission denied\n");
        }
        else
            printf("fshell: cd: no such directory\n");
    }
}

static void ALIAS_CMD(char **array, head_alias_t head)
{
    if (array[2] == NULL)
    {
        printf("fshell: alias: unknown usage\n");
        return (void)-1;
    }
    if (array[3] == NULL)
        upload_alias_node(head, array[1], array[2]);
    else if (array[3] != NULL)
    {
        unsigned short i = 0;
        char *tmp_upload_alias = NULL;
        asprintf(&tmp_upload_alias, "%s", array[2]);
        for (i = 3; array[i] != NULL; i++)
            asprintf(&tmp_upload_alias, "%s %s", tmp_upload_alias, array[i]);
        upload_alias_node(head, array[1], tmp_upload_alias);
    }
}

static void UNALIAS_CMD(char **array, head_alias_t head)
{
    unsigned short n = 1;
    while (array[n] != NULL)
    {
        head = remove_alias_node(head, array[n]);
        n += 1;
    }
}

static void SET_ENV_CMD(char **array)
{
    if (array[2] == NULL || array[3] != NULL)
    {
        printf("fshell: set: unknown usage\n");
        return (void)-1;
    }
    static char tmp_env[FSHELL_ENV_SIZE];
    snprintf(tmp_env, FSHELL_ENV_SIZE, "%s=%s", array[1], array[2]);
    putenv(tmp_env);
}

static void UNSET_ENV_CMD(char **array)
{
    unsigned short n = 1;
    while (array[n] != NULL)
    {
        unsetenv(array[n]);
        n += 1;
    }
}

static void PRINT_CMD(char **array, head_alias_t head)
{
    unsigned short n = 1;
    while (array[n] != NULL)
    {
        if (n >= 2)
            printf("\n");
        char *tmp_put = getenv(array[n]), *tmp_put2 = getalias_command(head, array[n]);
        if (tmp_put != NULL)
            printf("env: %s => %s\n", array[n], tmp_put);
        else
            printf("env: %s : Invalid environment variable\n", array[n]);
        if (tmp_put2 != NULL)
            printf("alias: %s => %s\n", array[n], tmp_put2);
        else
            printf("alias: %s : Invalid alias variable\n", array[n]);
        n += 1;
    }
}

static void LOAD_CMD(char **array, head_alias_t head)
{
    if (array[1] == NULL || array[2] != NULL)
        printf("fshell: load: unknown usage\n");
    else
    {
        char *file_path = array[1];
        int size = filesize(file_path), line, getsize;
        FILE *fp = fopen(file_path, "r");
        if (fp != NULL)
        {
            line = get_file_line(file_path);
            if (line > 1)
            {
                char *content = NULL;
                if (line >= 4)
                {
                    content = (char *)calloc(size / 2 + sizeof(char), sizeof(char));
                    getsize = size / 2;
                }
                else
                {
                    content = (char *)calloc(size + sizeof(char), sizeof(char));
                    getsize = size;
                }
                cmd_t cmd = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
                cmd_t copy = cmd;
                copy->next = NULL;
                while (fgets(content, getsize, fp) != NULL)
                {
                    copy->next = (cmd_t)malloc(sizeof(struct cmd_or_pipe_chain));
                    copy = copy->next;
                    copy->sentence = (char *)calloc(count_for_strlcpy(content), sizeof(char));
                    strlcpy(copy->sentence, content, count_for_strlcpy(content));
                    if (copy->sentence[strlen(copy->sentence) - 1] == '\n')
                        copy->sentence[strlen(copy->sentence) - 1] = '\0';
                    copy->next = NULL;
                    free(content);
                    content = (char *)calloc(getsize, sizeof(char));
                }
                cmd_t copy2 = cmd->next;
                int FLAG = NON_BUILTIN_CMD;
                char *array_sentence[FSHELL_INIT_FILE_ARRAY_NUM] = {NULL};
                while (copy2->sentence != NULL)
                {
                    array_parse(copy2->sentence, array_sentence);
                    FLAG = check_builtin_cmd(array_sentence[0]);
                    if (FLAG != NON_BUILTIN_CMD)
                        exec_builtin_cmd(array_sentence, FLAG, head, NULL, NULL);
                    else
                        printf("fshell: init file: error: %s FLAG %d\n", array_sentence[0], FLAG);
                    if (copy2->next != NULL)
                    {
                        if (strcmp(copy2->sentence, ""))
                            copy2 = copy2->next;
                        else
                            break;
                    }
                    else
                        break;
                }
            }
            else if (line == 1)
            {
                char *content = (char *)calloc(size, sizeof(char));
                if (fgets(content, size, fp) != NULL)
                {
                    if (content[strlen(content) - 1] == '\n')
                        content[strlen(content) - 1] = '\0';
                    char *array_content[FSHELL_INIT_FILE_ARRAY_NUM] = {NULL};
                    array_parse(content, array_content);
                    int FLAG = check_builtin_cmd(array_content[0]);
                    if (FLAG != NON_BUILTIN_CMD)
                        exec_builtin_cmd(array_content, FLAG, head, NULL, NULL);
                    else
                        printf("fshell: load: error: %s FLAG %d\n", array_content[0], FLAG);
                }
            }
            fclose(fp);
        }
        else
            printf("fshell: load: %s : no such file\n", array[1]);
    }
}

short exec_builtin_cmd(char **array, const short FLAG, head_alias_t head, const char *username, const char *cd_history)
{
    switch (FLAG)
    {
    case LIST_CMD_BUILTIN: {
        LIST_CMD(array, head);
        break;
    }
    case CD_CMD_BUILTIN: {
        CD_CMD(array, username, cd_history);
        break;
    }
    case ALIAS_CMD_BUILTIN: {
        ALIAS_CMD(array, head);
        break;
    }
    case UNALIAS_CMD_BUILTIN: {
        UNALIAS_CMD(array, head);
        break;
    }
    case SET_ENV_CMD_BUILTIN: {
        SET_ENV_CMD(array);
        break;
    }
    case UNSET_ENV_CMD_BUILTIN: {
        UNSET_ENV_CMD(array);
        break;
    }
    case PRINT_CMD_BUILTIN: {
        PRINT_CMD(array, head);
        break;
    }
    case LOAD_CMD_BUILTIN: {
        LOAD_CMD(array, head);
        break;
    }
    default:
        break;
    }
    return 0;
}
