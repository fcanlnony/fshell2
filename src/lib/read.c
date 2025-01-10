#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "../include/read.h"
#include "../include/memory.h"

unsigned short fileline(char *content)
{
  int i = 0,j = 0,num = strlen(content);
  char copy[num];
  strlcpy(copy, content, count_for_strlcpy(content));
  while(copy[j] != EOF) {
    if(copy[j] == '\n') {
      i += 1;
    }
    j += 1;
  }
  return i;
}

unsigned short filesize(char *file)
{
  struct stat file_stat;
  if(stat(file, &file_stat) == -1)
    return 0;
  else return file_stat.st_size;
}

unsigned short check_linefeed(const char *string, unsigned short i)
{
  i = 0;
  while(*string == '\n') {
    i += 1;
    string++;
  }
  return i;
}
