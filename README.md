
# Introduction
fshell is a simple shell write in C.The goal is to become a shell starter example.

## Feature
* pipe,&&
* built-in command
* init file support

### builtin command
* list alias - list all alias
* cd dir - switch dir_
<br>_cd .... - return previous folder_
* alias a b - alias a to b
* unalias a ... - unalias a , ... can replace with your alias variable
* set a b - set env variable a = b
* unset a ... - unset a , ... can replace with your env variable
* print a ... - print a env value and alias value 

### init file
* FSHELL_PIPE_NUM - the number of one-dimensional arrays of two-dimensional arrays to be used for pipe 
_(can be left out if you don't know)_
__Example: set FSHELL_PIPE_NUM 20__
* alias x x - same usage as the builtin command
* set x x - same usage as the builtin command

#### Warning
__The file is located at ${HOME}/.fshellrc__
__The file is located at ${HOME}/.fshellrc__
