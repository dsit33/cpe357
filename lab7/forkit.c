#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
   pid_t pid;

   printf("Hello, world!\n");

   if ((pid = fork()) == -1)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   else if (pid == 0)
   {
      printf("This is the child, pid %u\n", getpid());
   }
   else   
   {
      wait(NULL);
      printf("This is the parent, pid %u\n", getpid());
      printf("This is the parent, pid %u, signing off\n", getpid());
   }
   return 0;
}
