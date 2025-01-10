#ifndef FSHELL_ALIAS_H_
#define FSHELL_ALIAS_H_

#include "include/type.h"

extern alias_t init_alias(alias_t alias_chain,const char *alias_name,const char *alias_command);
extern alias_t upload_alias_node(alias_t alias_chain,const char *alias_name,const char *alias_command);
extern alias_t remove_alias_node(alias_t alias_chain,const char *alias_name);
extern char*  getalias_command(const alias_t alias_chain,const char *alias_name);

#endif /*FSHELL_ALIAS_H_*/
