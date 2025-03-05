#ifndef FSHELL_ARRAY_H_
#define FSHELL_ARRAY_H_

typedef struct cmd_or_pipe_chain
{
    char *sentence;
    struct cmd_or_pipe_chain *next;
} cmd_or_pipe_chain_t;

typedef cmd_or_pipe_chain_t *cmd_t;
typedef cmd_or_pipe_chain_t *pipe_t;

#endif /*FSHELL_ARRAY_H_*/
