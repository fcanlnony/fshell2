#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/memory.h"
#include "../include/read.h"

static unsigned short fileline(const char *content, unsigned short len)
{
    unsigned short line = 0;
    size_t i = 0;

    while (i < len)
    {
        if (content[i] == '\n')
            line += 1;
        i += 1;
    } 
    if (len > 0 && content[len - 1] != '\n')
        line += 1;
    return line;
}

unsigned short filesize(char *file)
{
    struct stat file_stat;
    if (stat(file, &file_stat) == -1)
        return 0;
    else
        return file_stat.st_size;
}

unsigned short check_linefeed(const char *string, unsigned short i)
{
    i = 0;
    while (*string == '\n')
    {
        i += 1;
        string++;
    }
    return i;
}

unsigned short get_file_line(char *filepath)
{
    int config_file = open(filepath, O_RDONLY);
    struct stat file_sb;
    if (config_file == -1)
        return 0;
    if (fstat(config_file, &file_sb) == -1)
    {
        close(config_file);
        return 0;
    }
    if (file_sb.st_size == 0)
    {
        close(config_file);
        return 0;
    }
    char *buf = (char *)calloc(file_sb.st_size, sizeof(char));
    ssize_t n = read(config_file, buf, file_sb.st_size);
    close(config_file);
    if (n <= 0)
    {
        free(buf);
        return 0;
    }
    unsigned short line = fileline(buf, (size_t)n);
    free(buf);
    return line;
}
