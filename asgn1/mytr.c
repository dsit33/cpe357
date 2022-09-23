#include <stdio.h>
#include <stdlib.h>
#include "myError.h"
#include "ops.h"

int main(int argc, char* argv[])
{
   char set1[256] = {0};
   char set2[256] = {0};
   int c;
   ProgState prog;
   
   prog.set1 = set1;
   prog.set2 = set2;
   prog.dFlag = 0;
   
   validateArgc(argc);

   if (checkFlag(argv[1], &prog))
   {
      iterateSet(argv[2], &prog);
   }
   else
   {
      getSets(argv[1], argv[2], &prog);
   }

   while ((c = getchar()) != EOF)
   {
      if (isMatch(c, &prog))
      {
         if(!prog.dFlag)
            replace(c, &prog);
      }
      else
         putchar(c);
   }
   return 0;
}
