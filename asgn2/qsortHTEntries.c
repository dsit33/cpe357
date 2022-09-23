#include "qsortHTEntries.h"
#include <stdlib.h>
#include <string.h>

typedef int (*fxCompare)(const void* a, const void* b);
int HTEntryCompare(HTEntry* a, HTEntry* b);
int charCompare(char* a, char* b);

void qsortHTEntries(HTEntry* entries, int numberOfEntries)
{
   fxCompare compare = (fxCompare)HTEntryCompare;
   qsort((void*)entries, numberOfEntries, sizeof(HTEntry), compare);
}

int HTEntryCompare(HTEntry* a, HTEntry* b)
{
   if (a->frequency - b->frequency == 0)
   {
      return charCompare((char*)b->data, (char*)a->data);
   }
   return b->frequency - a->frequency;
}

int charCompare(char* a, char* b)
{
   int i = 0;

   while(a[i] - b[i] == 0)
   {
      i++;
      if (strlen(a) == i || strlen(b) == i)
      {
         return ((strlen(a) > strlen(b)) ? 1 : -1);
      }
   }

   return a[i] - b[i];
}
