#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sigint(int sig)
{
   printf("User must type quit to end\n");
}

void sigquit(int sig)
{
   printf("Terminating\n");
   exit(EXIT_SUCCESS);
}

int main()
{
   int loop = 1;
   char buf[512];

   signal(SIGINT, sigint);
   signal(SIGQUIT, sigquit);
   while(loop)
   {
      printf("Type a string to be repeated or 'quit' to stop\n");
      fgets(buf, 512, stdin);
      if (strncmp(buf, "quit", 4) == 0)
      {
         kill(getpid(), SIGQUIT);
         loop = 0;
      }
      printf("%s\n", buf);
   }
   return 0;
}
