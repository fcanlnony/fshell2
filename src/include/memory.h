#ifndef FSHELL_MEMORY_H_
#define FSHELL_MEMORY_H_

#include <stdlib.h>
#include <string.h>

static inline unsigned short count_for_strlcpy(const char *pointer)
{
    return pointer != NULL ? (sizeof(char) * strlen(pointer) + sizeof(char)) : 0;
}

static inline char *mstrcpy(char *dest, const char *src)
{
    if (src != NULL)
    {
        if (dest == NULL)
        {
            dest = (char *)calloc(count_for_strlcpy(src), sizeof(char));
        }
        else
        {
            if (sizeof(dest) != sizeof(src))
                dest = (char *)realloc(dest, count_for_strlcpy(src) * sizeof(char));
            else
	      memset(dest, 0, count_for_strlcpy(src) * sizeof(char));
        }
        strlcpy(dest, src, count_for_strlcpy(src));
        return dest;
    }
    else
        return (char *)NULL;
}

#endif /*FSHELL_MEMORY_H_*/
