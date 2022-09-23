#include <stdio.h>
#include <stdlib.h>
#include "checkit.h"

void swap(int* first, int* second)
{
   int temp;

   temp = *first;
   *first = *second;
   *second = temp;
}

void testSwap1()
{
   int first, second;

   first = 100;
   second = 357;

   swap(&first, &second);
   checkit_int(first, 357);
   checkit_int(second, 100);
}

void testSwap2()
{
   int first, second;

   first = -158;
   second = 3248;
   
   swap(&first, &second);
   checkit_int(first, 3248);
   checkit_int(second, -158);
}

void testSwap()
{
   testSwap1();
   testSwap2();
}

int main()
{
   testSwap();
   return 0;
}
