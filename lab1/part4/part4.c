#include "part4.h"
#include <stdio.h>
#include <ctype.h>

void str_lower(char* src, char* dest)
{
   int idx = 0;
   while (src[idx] != '\0')
   {
      dest[idx] = tolower(src[idx]);
      idx++;
   }
    
   dest[idx] = '\0';
}

void str_lower_mutate(char* str)
{
   int idx = 0;
   while (str[idx] != '\0')
   {
      str[idx] = tolower(str[idx]);
      idx++;
   }
}
