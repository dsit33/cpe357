#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "mylib.h"

#define USAGE "pipeit command1 args1 ; command2 args2"

int find_semicolon(int argc, char* argv[])
{
   int i;

   for (i = 0; i < argc; i++)
   {
      if (!strcmp(argv[i], ";"))
         return i;
   }

   return -1;
}

void create_args1(int sc_idx, char* argv[], char* args1[])
{
   int i;
   
   for (i = 1; i < sc_idx; i++)
   {
      args1[i - 1] = (char*)malloc(strlen(argv[i]) * sizeof(char));
      strcpy(args1[i - 1], argv[i]);
   }
}

int main(int argc, char* argv[])
{
   pid_t child1, child2;
   int pipe1[2], sc_idx;
   char** args1;

   if ((sc_idx = find_semicolon(argc, argv)) == -1)
      usageAndExit(USAGE);

   args1 = (char**)malloc((sc_idx - 1) * sizeof(char*));
   create_args1(sc_idx, argv, args1);

   pipe(pipe1);

   child1 = fork();
   if (child1 == 0)
   {
      /* child1 */
      close(pipe1[0]);
      dup2(pipe1[1], STDOUT_FILENO);
      close(pipe1[1]);

      if (execvp(args1[0], &args1[0]) == -1)
         errorAndExit("execvp");
   }
   else if (child1 > 0)
   {
      /* parent */

      child2 = fork();
      if (child2 == 0)
      {
         close(pipe1[1]);
         dup2(pipe1[0], STDIN_FILENO);
         close(pipe1[0]);
         
         if (execvp(argv[sc_idx + 1], &argv[sc_idx + 1]) == -1)
            errorAndExit("execvp");
      }
      else if (child2 == -1)
         errorAndExit("fork");
      else
      {
         close(pipe1[0]);
         close(pipe1[1]);
         waitpid(child2, NULL, 0);
      }
   }
   else
   {
      /* error */
      errorAndExit("fork");
   }
   return 0;
}
