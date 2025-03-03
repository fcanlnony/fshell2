#ifndef FSHELL_READ_H_
#define FSHELL_READ_H_

extern unsigned short filesize (char *file);
extern unsigned short check_linefeed (const char *string, unsigned short i);
extern unsigned short get_file_line (char *filepath);

#endif /*FSHELL_READ_H_*/
