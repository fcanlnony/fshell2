#ifndef FSHELL_ALIAS_H_
#define FSHELL_ALIAS_H_

struct alias {
  char *alias_name;
  char *alias_command;
  struct alias *next;
};
typedef struct alias * alias_t;

extern alias_t init_alias(alias_t alias_chain,const char *alias_name,const char *alias_command);
extern alias_t upload_alias_node(alias_t alias_chain,const char *alias_name,const char *alias_command);
extern alias_t remove_alias_node(alias_t alias_chain,const char *alias_name);
extern char*  getalias_command(const alias_t alias_chain,const char *alias_name);

#endif /*FSHELL_ALIAS_H_*/
