#ifndef FSHELL_ALIAS_H_
#define FSHELL_ALIAS_H_

#include "include/type.h"

extern head_alias_t
init_alias_head(head_alias_t alias_head);
extern head_alias_t
upload_alias_node(head_alias_t alias_head,
                  const char* alias_name,
                  const char* alias_command);
extern head_alias_t
remove_alias_node(head_alias_t alias_head, const char* alias_name);
extern char*
getalias_command(const head_alias_t alias_head, const char* alias_name);

#endif /*FSHELL_ALIAS_H_*/
