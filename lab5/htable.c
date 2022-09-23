#include <stdio.h>
#include <stdlib.h>
#include "priorityQueue.h"
#include "huffmanTree.h"
#include "myError.h"

#define USAGE "htable <filename>\n"

int parseFile(FILE* in, hNode*** entries, unsigned* blockSize)
{
   int c, numItems = 0;

   while ((c = fgetc(in)) != EOF)
   {
      if (addEntry(entries, blockSize, numItems, c) == 1)
         numItems++;
   }
   return numItems;
}

FILE* openFile(char* fname, char* mode)
{
   FILE* file;

   if ((file = fopen(fname, mode)) == NULL)
      perror(fname);

   return file;
}

void printArray(hNode** entries, unsigned numItems)
{
   int i;

   for (i = 0; i < numItems; i++)
   {
      printf("%c: %d\n", entries[i]->data, entries[i]->freq);
   }
}

void populateQueue(pqNode** pqHead, hNode** entries, unsigned numItems)
{
   int i;

   *pqHead = newNode(entries[0]);

   for (i = 1; i < numItems; i++)
      push(pqHead, entries[i]);
}

int main(int argc, char* argv[])
{
   pqNode* pqHead;
   hNode* tHead;
   hNode** entries;
   int numItems;
   unsigned blockSize = 16;
   char code[256];
   FILE* in;

   if (argc > 2)
      usageAndExit(USAGE);
    
   if (argc == 2)
      in = openFile(argv[1], "r");
   else
      in = stdin;
   
   entries = (hNode**)malloc(sizeof(hNode*) * blockSize);
   
   numItems = parseFile(in, &entries, &blockSize);
   
   fclose(in);

   populateQueue(&pqHead, entries, numItems);
   
   tHead = buildTree(&pqHead);

   printCodes(tHead, code, 0);
   
   return 0;
}
