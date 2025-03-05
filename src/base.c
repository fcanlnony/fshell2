#include "base.h"

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/memory.h"

char *getusername()
{
    struct passwd *pwd = getpwuid(getuid());
    char *username = (char *)calloc(count_for_strlcpy(pwd->pw_name), sizeof(char));
    strlcpy(username, pwd->pw_name, count_for_strlcpy(pwd->pw_name));
    return username;
}

char *getcurrentdir()
{
    char *dir = (char *)calloc(MAX_DIR, sizeof(char));
    dir = getcwd(dir, MAX_DIR);
    return dir;
}

user_t init_user_information(const char *username, char *userdir, user_t user)
{
    user = (struct user_information *)malloc(sizeof(struct user_information));
    user->username = username;
    user->userdir = userdir;
    return user;
}

char *fshell_prompt_readline(const char *username, const char *currentdir, char *prompt)
{
    if (!strcmp(username, "root"))
        asprintf(&prompt, "\033[1;31m%s [ \033[0m%s\033[1;31m ]\033[0m\n>>> ", username, currentdir);
    else
        asprintf(&prompt, "\033[1;32m%s [ \033[0m%s\033[1;32m ]\033[0m\n>>> ", username, currentdir);
    return prompt;
}

char *readline_history_path(const char *username, char *path)
{
    if (!strcmp(username, "root"))
    {
        asprintf(&path, "/root/.fshell_historys");
        return path;
    }
    else
    {
        asprintf(&path, "/home/%s/.fshell_historys", username);
        return path;
    }
    return NULL;
}
