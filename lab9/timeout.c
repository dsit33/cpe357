#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

pid_t pid;

void timer_handler(int sig)
{
   printf("Timer has expired\n");
   kill(pid, SIGQUIT);
   exit(EXIT_SUCCESS);
}


void child_handler(int sig)
{
   printf("Child process terminated\n");
   exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
   struct sigaction time, child;
   struct itimerval timer;

   if (argc < 3)
   {
      printf("usage: timeout seconds [ program [...] ]\n");
      exit(EXIT_FAILURE);
   }

   if ((pid = fork()) == -1)
   {
      perror(argv[0]);
      exit(EXIT_FAILURE);      
   }
   else if (pid == 0)
   {
      if (execvp(argv[2], &(argv[2])) == -1)
      {
         perror(NULL);
         exit(EXIT_FAILURE);
      }
   }
   else
   {
      memset(&time, 0, sizeof(time));
      memset(&child, 0, sizeof(child));
      child.sa_handler = &child_handler;
      time.sa_handler = &timer_handler;
      sigaction(SIGCHLD, &child, NULL);
      sigaction(SIGALRM, &time, NULL);

      timer.it_value.tv_sec = atoi(argv[1]);
      timer.it_value.tv_usec = 0;
      timer.it_interval.tv_sec = 0;
      timer.it_interval.tv_usec = 0;
      setitimer(ITIMER_REAL, &timer, NULL);
      
      while(1);
   }
   return 0;

}
