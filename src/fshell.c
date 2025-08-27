#include "fshell.h"

#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include "alias.h"
#include "base.h"
#include "builtin.h"
#include "exec.h"
#include "include/array.h"
#include "include/config.h"
#include "include/memory.h"
#include "include/parse.h"
#include "include/type.h"

static jmp_buf sig_back_while, sig_stop_fshell;
static void
back_jump_sigINT ()
{
  longjmp (sig_back_while, 1);
}

static void
back_jump_sigSEGV ()
{
  longjmp (sig_stop_fshell, 1);
}

_Noreturn void
help ()
{
  printf ("Options:\n\
--version,-v print version\n\
--help,-h get some help\n\
--compile-time,-ct print the compilation time\n");
  exit (0);
}

int
main (int argc, char **argv)
{
  if (argc > 1)
    {
      if (argc == 2)
        {
          if (!strcmp (argv[1], "--version") || !strcmp (argv[1], "-v"))
            PRINT_VERSION (SHELL_NAME, FSHELL_VERSION);
          else if (!strcmp (argv[1], "--help") || !strcmp (argv[1], "-h"))
            help ();
          else if (!strcmp (argv[1], "--compile-time")
                   || !strcmp (argv[1], "-ct"))
            printf ("Time:%s Date:%s\n", __TIME__, __DATE__);
          else
            printf ("Unknow option :%s \n", argv[1]);
        }
      else
        help ();
      exit (0);
    }
  signal (SIGINT, back_jump_sigINT);
  signal (SIGSEGV, back_jump_sigSEGV);
  user_t user = init_user_information (getusername (), getcurrentdir (), user);
  head_alias_t alias = init_alias_head (alias);
  char *readline_path = readline_history_path (user->username, readline_path);
  read_history (readline_path);
  char *cd_history = NULL;
  fshell_init (alias);
  if (getenv ("FSHELL_PIPE_NUM") != NULL)
    array_pipe_num = atoi (getenv ("FSHELL_PIPE_NUM"));
  else
    array_pipe_num = 0;
  volatile short check_num = 0;
  if (array_pipe_num == 0)
    array_pipe_num = 20;
  if (setjmp (sig_stop_fshell))
    {
      printf ("stop fshell\n");
      exit (0);
    }
  if (setjmp (sig_back_while))
    {
      FREE_USERT_FUNC (user->userdir);
      printf ("\n");
    }
  while (1)
    {
      user->userdir = getcurrentdir ();
      char *prompt
          = fshell_prompt_readline (user->username, user->userdir, prompt);
      char *input = readline (prompt);
      if (!strcmp (input, ""))
        {
          FREE_USERT_FUNC (input);
          FREE_USERT_FUNC (prompt);
          FREE_USERT_FUNC (user->userdir);
          continue;
        }
      add_history (input);
      write_history (readline_path);
      if (!strcmp (input, "exit"))
        exit (0);
      if (check_and (input) == false)
        {
          if (check_pipe (input) == false)
            {
              char **array = (char **)calloc (array_pipe_num, sizeof (char *));
              char **array_alias
                  = (char **)calloc (array_pipe_num, sizeof (char *));
              char **array_alias_tmp
                  = (char **)calloc (array_pipe_num, sizeof (char *));
              char **input_array
                  = (char **)calloc (array_pipe_num, sizeof (char *));
              char *copy_input
                  = (char *)calloc (count_for_strlcpy (input), sizeof (char));
              strlcpy (copy_input, input, count_for_strlcpy (input));
              array_parse (input, array);
              check_num = check_builtin_cmd (array[0]);
              if (check_num != NON_BUILTIN_CMD)
                exec_builtin_cmd (array, check_num, alias, user->username,
                                  cd_history);
              else
                {
                  char *alias_cmd = getalias_command (alias, array[0]);
                  if (alias_cmd == NULL)
                    execvp_without_pipe (array);
                  else
                    {
                      char *tmp_alias_cmd_copy = (char *)calloc (
                          count_for_strlcpy (alias_cmd), sizeof (char));
                      strlcpy (tmp_alias_cmd_copy, alias_cmd,
                               count_for_strlcpy (alias_cmd));
                      array_parse (tmp_alias_cmd_copy, array_alias_tmp);
                      unsigned short i, k;
                      for (i = 0; array_alias_tmp[i] != NULL; i++)
                        array_alias[i] = array_alias_tmp[i];
                      array_parse (copy_input, input_array);
                      if (input_array[1] != NULL)
                        for (k = 1; input_array[k] != NULL; i++, k++)
                          array_alias[i] = input_array[k];
                      else
                        array_alias[i] = NULL;
                      execvp_without_pipe (array_alias);
                      free (tmp_alias_cmd_copy);
                      free (copy_input);
                      FREE_USERT_FUNC (array);
                      FREE_USERT_FUNC (array_alias);
                      FREE_USERT_FUNC (array_alias_tmp);
                      FREE_USERT_FUNC (input_array);
                    }
                }
            }
          else
            {
              pipe_t pipe_chain = array_pipe_parse (input, pipe_chain);

              int pipe_count = 0;
              pipe_t temp = pipe_chain;
              while (temp != NULL)
                {
                  pipe_count++;
                  temp = temp->next;
                }
              pipe_t processed_chain = pipe_chain;
              pipe_t current = processed_chain;
              while (current != NULL)
                {
                  char **array
                      = (char **)calloc (array_pipe_num, sizeof (char *));
                  char **array_alias
                      = (char **)calloc (array_pipe_num, sizeof (char *));
                  char **array_alias_tmp
                      = (char **)calloc (array_pipe_num, sizeof (char *));
                  char **input_array
                      = (char **)calloc (array_pipe_num, sizeof (char *));
                  char *copy_input = (char *)calloc (
                      count_for_strlcpy (current->sentence), sizeof (char));
                  strlcpy (copy_input, current->sentence,
                           count_for_strlcpy (current->sentence));
                  array_parse (current->sentence, array);

                  char *alias_cmd = getalias_command (alias, array[0]);
                  if (alias_cmd == NULL)
                    {
                      array_parse (copy_input, array_alias);
                    }
                  else
                    {
                      char *tmp_alias_cmd_copy = (char *)calloc (
                          count_for_strlcpy (alias_cmd), sizeof (char));
                      strlcpy (tmp_alias_cmd_copy, alias_cmd,
                               count_for_strlcpy (alias_cmd));
                      array_parse (tmp_alias_cmd_copy, array_alias_tmp);
                      unsigned short i, k;
                      for (i = 0; array_alias_tmp[i] != NULL; i++)
                        array_alias[i] = array_alias_tmp[i];
                      array_parse (copy_input, input_array);
                      if (input_array[1] != NULL)
                        for (k = 1; input_array[k] != NULL; i++, k++)
                          array_alias[i] = input_array[k];
                      else
                        array_alias[i] = NULL;
                      free (tmp_alias_cmd_copy);
                    }

                  free (current->sentence);
                  current->sentence = (char *)calloc (
                      count_for_strlcpy (array_alias[0]) + 256, sizeof (char));
                  strcpy (current->sentence, array_alias[0]);
                  for (int j = 1; array_alias[j] != NULL; j++)
                    {
                      strcat (current->sentence, " ");
                      strcat (current->sentence, array_alias[j]);
                    }

                  free (copy_input);
                  FREE_USERT_FUNC (array);
                  FREE_USERT_FUNC (array_alias);
                  FREE_USERT_FUNC (array_alias_tmp);
                  FREE_USERT_FUNC (input_array);
                  current = current->next;
                }
              execvp_with_multi_pipe (processed_chain);
            }
        }
      else
        {
          cmd_t cmd_chain = array_chain_parse (input, cmd_chain);
          cmd_t current = cmd_chain;
          while (1)
            {
              if (current->sentence != NULL)
                {
                  if (check_pipe (current->sentence) == false)
                    {
                      char **array
                          = (char **)calloc (array_pipe_num, sizeof (char *));
                      char **array_alias
                          = (char **)calloc (array_pipe_num, sizeof (char *));
                      char **array_alias_tmp
                          = (char **)calloc (array_pipe_num, sizeof (char *));
                      char **input_array
                          = (char **)calloc (array_pipe_num, sizeof (char *));
                      char *copy_input = (char *)calloc (
                          count_for_strlcpy (input), sizeof (char));
                      strlcpy (copy_input, current->sentence,
                               count_for_strlcpy (current->sentence));
                      array_parse (current->sentence, array);
                      check_num = check_builtin_cmd (array[0]);
                      if (check_num != NON_BUILTIN_CMD)
                        exec_builtin_cmd (array, check_num, alias,
                                          user->username, cd_history);
                      else
                        {
                          char *alias_cmd = getalias_command (alias, array[0]);
                          if (alias_cmd == NULL)
                            execvp_without_pipe (array);
                          else
                            {
                              char *tmp_alias_cmd_copy = (char *)calloc (
                                  count_for_strlcpy (alias_cmd),
                                  sizeof (char));
                              strlcpy (tmp_alias_cmd_copy, alias_cmd,
                                       count_for_strlcpy (alias_cmd));
                              array_parse (tmp_alias_cmd_copy,
                                           array_alias_tmp);
                              unsigned short i, k;
                              for (i = 0; array_alias_tmp[i] != NULL; i++)
                                array_alias[i] = array_alias_tmp[i];
                              array_parse (copy_input, input_array);
                              if (input_array[1] != NULL)
                                for (k = 1; input_array[k] != NULL; i++, k++)
                                  array_alias[i] = input_array[k];
                              else
                                array_alias[i] = NULL;
                              execvp_without_pipe (array_alias);
                              free (tmp_alias_cmd_copy);
                              free (copy_input);
                              FREE_USERT_FUNC (array);
                              FREE_USERT_FUNC (array_alias);
                              FREE_USERT_FUNC (array_alias_tmp);
                              FREE_USERT_FUNC (input_array);
                            }
                        }
                    }
                  else
                    {
                      pipe_t pipe_chain
                          = array_pipe_parse (current->sentence, pipe_chain);

                      int pipe_count = 0;
                      pipe_t temp = pipe_chain;
                      while (temp != NULL)
                        {
                          pipe_count++;
                          temp = temp->next;
                        }

                      pipe_t processed_chain = pipe_chain;
                      pipe_t current_pipe = processed_chain;
                      while (current_pipe != NULL)
                        {
                          char **array = (char **)calloc (array_pipe_num,
                                                          sizeof (char *));
                          char **array_alias = (char **)calloc (
                              array_pipe_num, sizeof (char *));
                          char **array_alias_tmp = (char **)calloc (
                              array_pipe_num, sizeof (char *));
                          char **input_array = (char **)calloc (
                              array_pipe_num, sizeof (char *));
                          char *copy_input = (char *)calloc (
                              count_for_strlcpy (current_pipe->sentence),
                              sizeof (char));
                          strlcpy (copy_input, current_pipe->sentence,
                                   count_for_strlcpy (current_pipe->sentence));
                          array_parse (current_pipe->sentence, array);

                          char *alias_cmd = getalias_command (alias, array[0]);
                          if (alias_cmd == NULL)
                            {
                              array_parse (copy_input, array_alias);
                            }
                          else
                            {
                              char *tmp_alias_cmd_copy = (char *)calloc (
                                  count_for_strlcpy (alias_cmd),
                                  sizeof (char));
                              strlcpy (tmp_alias_cmd_copy, alias_cmd,
                                       count_for_strlcpy (alias_cmd));
                              array_parse (tmp_alias_cmd_copy,
                                           array_alias_tmp);
                              unsigned short i, k;
                              for (i = 0; array_alias_tmp[i] != NULL; i++)
                                array_alias[i] = array_alias_tmp[i];
                              array_parse (copy_input, input_array);
                              if (input_array[1] != NULL)
                                for (k = 1; input_array[k] != NULL; i++, k++)
                                  array_alias[i] = input_array[k];
                              else
                                array_alias[i] = NULL;
                              free (tmp_alias_cmd_copy);
                            }

                          free (current_pipe->sentence);
                          current_pipe->sentence = (char *)calloc (
                              count_for_strlcpy (array_alias[0]) + 256,
                              sizeof (char));
                          strcpy (current_pipe->sentence, array_alias[0]);
                          for (int j = 1; array_alias[j] != NULL; j++)
                            {
                              strcat (current_pipe->sentence, " ");
                              strcat (current_pipe->sentence, array_alias[j]);
                            }

                          free (copy_input);
                          FREE_USERT_FUNC (array);
                          FREE_USERT_FUNC (array_alias);
                          FREE_USERT_FUNC (array_alias_tmp);
                          FREE_USERT_FUNC (input_array);
                          current_pipe = current_pipe->next;
                        }
                      execvp_with_multi_pipe (processed_chain);
                    }
                }
              if (current->next != NULL)
                current = current->next;
              else
                break;
            }
        }
      cd_history = (char *)realloc (cd_history,
                                    strlen (user->userdir) * sizeof (char));
      strlcpy (cd_history, user->userdir, count_for_strlcpy (user->userdir));
      FREE_USERT_FUNC (input);
      FREE_USERT_FUNC (prompt);
      FREE_USERT_FUNC (user->userdir);
    }
}
