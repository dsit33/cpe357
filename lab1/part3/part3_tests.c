#include "part3.h"
#include "checkit.h"

void testSum1()
{
   int nums[4] = {0, 3, 5, 2};
   checkit_int(sum(nums, 4), 10);
}

void testSum2()
{
   int nums[4] = {0, 3, 5, 2};
   checkit_int(sum(nums, 0), 0);
}

void testSum3()
{
   int nums[3] = {11, 39, 500};
   checkit_int(sum(nums, 3), 550);
}

void testSum()
{
   testSum1();
   testSum2();
   testSum3();
}

int main()
{
   testSum();

   return 0;
}
