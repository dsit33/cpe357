#include <stdio.h>

void function_one(int input)
{
   int function_one_local = input;
}

void function_two()
{
   int function_two_local;
}

int main(void)
{
   int first = 111;
   int second = 222;
   int *p = &first;

   function_one(357);
   function_two();

   return 0;
}
