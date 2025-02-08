
# Introduction
fshell is a simple shell write in C. The goal is to become a shell starter example.

## Feature
* |, &&
* built-in command
* init file support

### builtin command
* list alias - list all alias
* cd [dir] - switch dir_
<br>_cd .... - return previous folder_
* alias [alias name] [original name] - alias a to b
* unalias [alias name] ... - unalias a , ... can replace with your alias variable
* set [name] [value] - set env variable [name] = [value]
* unset [name] ... - unset a , ... can replace with your env variable
* print [name] ... - print a env value and alias value 
* load [file] ... - load a configuration file with built-in commands

### init file
* FSHELL_PIPE_NUM - the number of one-dimensional arrays of two-dimensional arrays to be used for pipe 
__Example: set FSHELL_PIPE_NUM 20__
_(can be left out if you don't know)_
* built-in command support (set, unset, alias, unalias, load)

#### Warning
* The file is located at ${HOME}/.fshellrc</p>
