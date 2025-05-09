#ifndef _FSHELL_MEMORY_H_
#define _FSHELL_MEMORY_H_

#include <stdlib.h>
#include <string.h>

static inline unsigned short count_for_strlcpy(const char *pointer)
{
    return pointer != NULL ? (sizeof(char) * strlen(pointer) + sizeof(char)) : 0;
}

extern char *mstrcpy(char *dest, const char *src);

#endif /*_FSHELL_MEMORY_H_*/
