#ifndef FSHELL_BASE_H
#define FSHELL_BASE_H

#include "include/type.h"
#include <pwd.h>
#include <sys/types.h>

extern char *getusername ();
extern char *getcurrentdir ();

extern user_t init_user_information (const char *username, char *userdir,
                                     user_t user);

extern char *fshell_prompt_readline (const char *username,
                                     const char *currentdir, char *prompt);
extern char *readline_history_path (const char *username, char *path);

#define MAX_DIR 256

#endif /*FSHELL_BASE_H*/

#ifndef FREE_USERT_FUNC
#define FREE_USERT_FUNC(X) (free (X), X = NULL)
#endif /*FREE_USERT_FUNC*/
