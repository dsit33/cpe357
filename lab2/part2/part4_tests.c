#include "part4.h"
#include "checkit.h"

void testStrLower1()
{
   char word[6] = "HELLO";
   char dest[6];

   str_lower(word, dest);
   checkit_string(dest, "hello");
}

void testStrLower2()
{
   char word[6] = "HeLlo";
   char dest[6];

   str_lower(word, dest);
   checkit_string(dest, "hello");
}

void testStrLower()
{
   testStrLower1();
   testStrLower2();
}

void testStrMutate1()
{
   char word[6] = "HeLlo";

   str_lower_mutate(word);
   checkit_string(word, "hello");
}

void testStrMutate2()
{
   char word[10] = "DErEk SiT";

   str_lower_mutate(word);
   checkit_string(word, "derek sit");
}

void testStrMutate()
{
   testStrMutate1();
   testStrMutate2();
}

int main()
{
   testStrLower();
   testStrMutate();

   return 0;
}
