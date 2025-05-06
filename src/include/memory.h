#ifndef FSHELL_MEMORY_H_
#define FSHELL_MEMORY_H_

#include <stdlib.h>
#include <string.h>

static inline unsigned short count_for_strlcpy(const char *pointer)
{
    return pointer != NULL ? (sizeof(char) * strlen(pointer) + sizeof(char)) : 0;
}

extern char *mstrcpy(char *dest, const char *src);

#endif /*FSHELL_MEMORY_H_*/
