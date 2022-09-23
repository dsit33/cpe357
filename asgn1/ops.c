#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ops.h"
#include "myError.h"

void validateArgc(int argc)
{
   if (argc != 3)
      usageAndExit(USAGE);
}

int checkFlag(char* arg, ProgState* prog)
{
   if (arg[0] != '-')
      return 0;
   if (strcmp(arg, "-d") == 0)
   {
      prog->dFlag = 1;
      return 1;
   }
   usageAndExit(USAGE);
   return 0;
}

void iterateSet(char* arg, ProgState* prog)
{
   int i;
   char c;

   for (i = 0; i < strlen(arg); i++)
   {
      c = getCharAt(arg, &i);
      (prog->set1)[(int)c] = c;
   }
}

void getSets(char* set1, char* set2, ProgState* prog)
{
   int len1, len2, numChars, i, j = 0;
   char c, lastC;
   i = 0;
   numChars = 0;
   len1 = strlen(set1);
   len2 = strlen(set2);

   while (i < len1)
   {
      c = getCharAt(set1, &i);
      (prog->set1)[(int)c] = c;
      if (numChars < len2)
      {
         lastC = getCharAt(set2, &j);
         (prog->set2)[(int)c] = lastC;
         j++;
      }
      else
         (prog->set2)[(int)c] = lastC;
      i++;
      numChars++;
   }
}

char getCharAt(char* arg, int* i)
{
   if (arg[*i] != '\\')
      return arg[*i];
   if (*i == strlen(arg) - 1)
      return 0;
   if (arg[*i + 1] == 'n')
   {
      (*i)++;
      return '\n';
   }
   if (arg[*i + 1] == 't')
   {
      (*i)++;
      return '\t';
   }
   if (arg[*i + 1] == '\\')
   {
      (*i)++;
      return '\\';
   }
   return arg[(*i)++];
}

int isMatch(int c, ProgState* prog)
{
   /*check to see if the char is placed in set1*/
   return (prog->set1)[c] != 0;
}

void replace(int c, ProgState* prog)
{
   /*using the passed char, print out the corresponding char in set2*/
   putchar((prog->set2)[c]);
}
