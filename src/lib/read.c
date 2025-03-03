#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/memory.h"
#include "../include/read.h"

static unsigned short
fileline (char *content)
{
  int i = 0, j = 0, num = count_for_strlcpy (content);
  char copy[num];
  strlcpy (copy, content, count_for_strlcpy (content));
  while (copy[j] != EOF)
    {
      if (copy[j] == '\n')
        {
          i += 1;
        }
      j += 1;
    }
  return i;
}

unsigned short
filesize (char *file)
{
  struct stat file_stat;
  if (stat (file, &file_stat) == -1)
    return 0;
  else
    return file_stat.st_size;
}

unsigned short
check_linefeed (const char *string, unsigned short i)
{
  i = 0;
  while (*string == '\n')
    {
      i += 1;
      string++;
    }
  return i;
}

unsigned short
get_file_line (char *filepath)
{
  int config_file = open (filepath, O_RDONLY);
  struct stat file_sb;
  stat (filepath, &file_sb);
  char *buf = (char *)calloc (file_sb.st_size / sizeof (char), sizeof (char));
  if (read (config_file, buf, file_sb.st_size / sizeof (char)) == 0)
    return 0;
  else
    {
      unsigned short line = fileline (buf);
      free (buf);
      return line;
    }
}
