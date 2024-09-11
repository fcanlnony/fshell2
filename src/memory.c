#include <string.h>

unsigned int count_for_strlcpy(char *point)
{
  return (sizeof(char)*strlen(point)+sizeof(char));
}
