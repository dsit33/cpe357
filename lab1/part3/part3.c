#include "part3.h"

int sum(int* nums, int len)
{
   int i, sum = 0;

   if (len <= 0)
      return 0;

   for (i = 0; i < len; i++)
      sum += nums[i];

   return sum;
}
