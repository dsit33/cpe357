#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
   pid_t child1, child2;
   int ab[2], bc[2], ca[2], num = 0;

   pipe(ab);
   pipe(ca);

   if ((child1 = fork()) == 0)
   {
      pipe(bc);

      if ((child2 = fork()) == 0)
      {
         close(ab[0])
         close(bc[0]);
      }
      else if (child2 == -1)
      {
         perror("child2");
         exit(EXIT_FAILURE);
      }
      else
      {

      }

   }
   else if (child1 == -1)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   else
   {
      close(ab[0]);
      close(ca[1]);
      printf("Pick an integer: ");
      scanf("%d", &num);
      write(ab[1], &num, sizeof(num));
      while (read(ca[0], &num, sizeof(num)) == //run until pipe has info
      printf("new num: %d\n", num);
   }
   return 0;
}
