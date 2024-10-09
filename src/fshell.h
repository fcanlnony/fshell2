#ifndef FSHELL2_H_
#define FSHELL2_H_

#define SHELL_NAME "fshell"
#define FSHELL_VERSION "0.0.2"

_Noreturn void help();

#endif /*FSHELL2_H_*/

#ifndef PRINT_VERSION
#define PRINT_VERSION(NAME,VERSION) (printf("%s version: %s \n",NAME,VERSION))
#endif /*define macro function*/ 
