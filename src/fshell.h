#ifndef _FSHELL2_H_
#define _FSHELL2_H_

#define SHELL_NAME "fshell"
#define FSHELL_VERSION "0.0.7"

_Noreturn void help();
unsigned short array_pipe_num = 0;

#endif /*_FSHELL2_H_*/

#ifndef PRINT_VERSION
#define PRINT_VERSION(NAME, VERSION) (printf("%s version: %s \n", NAME, VERSION))
#endif /*PRINT_VERSION*/
