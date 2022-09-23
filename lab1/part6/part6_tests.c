#include <stdio.h>

#include "checkit.h"
#include "part6.h"

/* define testing functions */
void test_create_point1(void)
{
   Point p = create_point(1.9, -2.7);

   checkit_double(p.x, 1.9);
   checkit_double(p.y, -2.7);
}

void test_create_point2(void)
{
   Point p = create_point(0.2, 12.1);

   checkit_double(p.x, 0.2);
   checkit_double(p.y, 12.1);
}

void test_create_point(void)
{
   test_create_point1();
   test_create_point2();
}

void testSquare1()
{
   Rectangle r = {create_point(0.0, 4.0), create_point(4.0, 0.0)};

   checkit_boolean(isSquare(r), 1);
}

void testSquare2()
{
   Rectangle r = {create_point(0.0, 2.0), create_point(6.0, 0.0)};

   checkit_boolean(isSquare(r), 0);
}

void testSquare()
{
   testSquare1();
   testSquare2();
}

int main(int arg, char *argv[])
{
   /* call testing function(s) here */
   test_create_point();
   testSquare();

   return 0;
}
