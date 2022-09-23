#include <stdio.h>
#include <stdlib.h>
#include <error.h>

int main()
{
   char* test;

   test = (char*)malloc(sizeof(char) * 10000000);
   if (test == NULL)
   {
      perror(NULL);
      exit(EXIT_FAILURE);
   }
   return 0;
}
