#ifndef FSHELL_BASE_H
#define FSHELL_BASE_H

#include <pwd.h>
#include <sys/types.h>

extern char* getusername();
extern char* getcurrentdir();

typedef struct user_information {
  char *username;
  char *userdir;
} user;

typedef user * user_t;

extern user_t init_user_information(char *username,char *userdir,user_t user);

extern char* getusername_b(user_t user);
extern char* getcurrentdir_b(user_t user);
/*the B scheme to get the username and dir */

extern char* fshell_prompt_readline(char *username,char *currentdir,char *prompt);

#define MAX_DIR 256

#endif /*FSHELL_BASE_H*/

#ifndef FREE_USERT_FUNC
#define FREE_USERT_FUNC(X) (free(X),X = NULL)
#endif /*FREE_USERT_FUNC*/
