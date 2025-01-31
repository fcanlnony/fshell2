#ifndef FSHELL_TYPE_H
#define FSHELL_TYPE_H

struct alias {
  char *alias_name;
  char *alias_command;
  struct alias *next;
};
typedef struct alias * alias_t;

typedef struct user_information {
  const char *username;
  char *userdir;
} user;
typedef user * user_t;

struct head_alias {
  struct alias *next;
};
typedef struct head_alias * head_alias_t;

#endif /*FSHELL_TYPE_H*/
