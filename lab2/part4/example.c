#include <stdio.h>

#include "fact.h"

int main(void)
{
   int i;

   for (i = 0; i < 5; i++)
   {
      printf("%d\n", fact(i));
   }

   return 0;
}
