#include <stdio.h>

#include "checkit.h"
#include "part2.h"

void test_calc_1()
{
   checkit_double(calc(-1.0), 40);
}

void test_calc_2()
{
   checkit_double(calc(12.0), 49);
}

void test_calc_3()
{
   checkit_double(calc(27.0), 58);
}

void test_calc()
{
   test_calc_1();
   test_calc_2();
   test_calc_3();
}

int main(void)
{
   test_calc();

   return 0;
}
