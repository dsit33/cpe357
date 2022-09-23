#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
   pid_t pid, ret;
   int exit_st, status = 0;

   if (argc < 2)
   {
      printf("usage: tryit command\n");
      exit(EXIT_FAILURE);
   }
   
   if ((pid = fork()) == -1)
   {
      perror("fork");
      exit(EXIT_FAILURE);
   }
   else if (pid == 0)
   {
      if (execvp(argv[1], &argv[1]) == -1)
      {
         perror("exec");
         exit(EXIT_FAILURE);
      }
   }
   else
   {
      if ((ret = wait(&status)) == -1)
      {
         perror("wait");
         exit(EXIT_FAILURE);
      }
      
      printf("Process %u ", ret);
      if (WIFEXITED(status) && !WEXITSTATUS(status))
         printf("succeeded.\n");
      else
         printf("exited with an error value.\n");
   }

   return 0;
}
