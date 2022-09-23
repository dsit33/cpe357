#include <stdlib.h>
#include <stdio.h>
#include "myError.h"
#include "hashTable.h"
#include "fileOps.h"
#include "qsortHTEntries.h"
#include <string.h>

#define DEFAULT 10
#define USAGE "fw [-n num] [file1 [file2...]]"

unsigned myHash(const void* data)
{
   unsigned hash;
   const char* str = data;

   for (hash = 0; *str; str++)
      hash = *str + 31 * hash;

   return hash;
}

int compareString(const void* a, const void* b)
{
   return strcmp(a, b);
}

void destroyString(const void* str)
{
   free((char*)str);
}

int checkFlag(char* argv[], int idx)
{
   int n = DEFAULT;

   if (argv[idx][1] != 'n')
      usageAndExit(USAGE);

   if ((n = atoi(argv[idx + 1])) == 0)
      usageAndExit(USAGE);

   return n;
}

int checkArgs(int argc, char* argv[], FILE** files, int* numFiles)
{
   int i, n = DEFAULT;

   if (argc == 1)
   {
      files[(*numFiles)++] = stdin;
      return n;
   }

   for (i = 1; i < argc; i++)
   {
      if (argv[i][0] == '-')
      {
         n = checkFlag(argv, i++);
      }
      else
         tryFile(argv, i, files, numFiles);
   }

   return n;
}

int main(int argc, char* argv[])
{
   HTEntry* array;
   void* ht;
   HTFunctions funcs = {myHash, compareString, destroyString};
   int i, numFiles, numWords = DEFAULT;
   unsigned listSize = 0;
   unsigned sizes[3] = {31, 67, 137};
   FILE** files = (FILE**)malloc(sizeof(FILE*) * argc);
   checkBlock(files);

   numFiles = 0;
   ht = htCreate(&funcs, sizes, 3, .7);
   
   /*iterate args to get input file(s) and flag*/ 
   numWords = checkArgs(argc, argv, files, &numFiles);

   files = (FILE**)realloc(files, sizeof(FILE*) * numFiles);
   checkBlock(files);

   /*iterate through input file(s), ht should be full after this call*/
   iterateFiles(files, numFiles, ht);
            
   /*sort data structure by frequency, secondary by ascii*/
   array = htToArray(ht, &listSize);
   qsortHTEntries(array, listSize);

   /*print list*/
   printf("The top %d words (out of %d) are:\n", numWords, listSize);

   for (i = 0; i < numWords && i < listSize; i++)
      printf("%9u %s\n", array[i].frequency, (char*)(array[i].data));
   
   free(files);
   free(array);
   htDestroy(ht);

   return 0;
}
