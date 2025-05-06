#include "../include/memory.h"
#include <stdio.h>

char *mstrcpy(char *dest, const char *src)
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
