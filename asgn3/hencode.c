#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "priorityQueue.h"
#include "huffmanTree.h"
#include "mylib.h"

#define USAGE "hencode infile [ outfile ]\n"

int comp(const void* a, const void* b)
{
   CodePair* cp1, *cp2;
   cp1 = (CodePair*)a;
   cp2 = (CodePair*)b;

   return cp1->data - cp2->data;
}

int parseFile(int inFd, hNode*** entries, unsigned* blockSize)
{
   int c;
   int numItems = 0;

   while (sysread(inFd, &c, sizeof(char)) != 0)
   {
      if (addEntry(entries, blockSize, numItems, c) == 1)
         numItems++;
   }
   return numItems;
}

void printHeader(int outFd, CodePair* codeTable, int numItems)
{
   int i;
   CodePair temp;
   int c;
   uint32_t freq, count;

   count = numItems;
   syswrite(outFd, &count, sizeof(count));

   for (i = 0; i < numItems; i++)
   {
      temp = codeTable[i];
      c = temp.data;
      freq = temp.freq;
      syswrite(outFd, &c, sizeof(uint8_t));
      syswrite(outFd, &freq, sizeof(freq));
   }
}

void printBody(int inFd, int outFd, CodePair* codeTable, int numItems)
{
   int c;
   char* code;
   char holder[9] = {0};
   int offset, i;
   uint8_t byte;

   /* return to start of file and generate body using codes */
   lseek(inFd, 0, SEEK_SET);
   
   while (sysread(inFd, &c, sizeof(char)) != 0)
   {
      /* if the char isnt in the codeTable, report error */
      if ((code = findCode((char)c, codeTable, numItems)) == NULL)
      {
         fprintf(stderr, "error finding code\n");
         exit(EXIT_FAILURE);
      }

      /* if code has too many chars for holder, print out the code and
       * clear holder to start over */
      offset = attach(holder, code);
      if (strlen(holder) == 8)
      {
         byte = convertbin(holder);
         syswrite(outFd, &byte, sizeof(byte));
         memset(holder, 0, 9);
      }

      /* this will only be true when code was too big for holder */
      if (offset != 0)
         attach(holder, code + offset);
   }

   /* if the holder isn't full, pad with zeros */
   if ((offset = strlen(holder)) == 0)
      return;
   if (offset != 8)
   {
      for (i = 0; i < (8 - offset); i++)
         attach(holder, "0");
   }
   byte = convertbin(holder);
   syswrite(outFd, &byte, sizeof(byte));
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
   CodePair* codeTable;
   int inFd, outFd, numItems, itemsInTable = 0;
   unsigned blockSize = 16;
   char code[9];

   outFd = STDOUT_FILENO; /* default the out fd to stdout */
   
   /* if the number of args are off, print the usage */
   if (argc < 2 || argc > 3)
      usageAndExit(USAGE);
   
   /* if the optional out file exists, create it for writing */
   if (argc == 3)
      outFd = sysopen(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0644);
   inFd = sysopen(argv[1], O_RDONLY, 0644);
   
   /* allocate space for an array of nodes to be turn into a tree */
   entries = (hNode**)malloc(sizeof(hNode*) * blockSize);
   
   /* populate the array and return the number of items */
   if ((numItems = parseFile(inFd, &entries, &blockSize)) == 0)
   {
      printHeader(outFd, NULL, 0);
      sysclose(inFd);
      sysclose(outFd);
      free(entries);
      return 0;
   }

   /* create a priority queue of the elements to be turn into a tree */
   populateQueue(&pqHead, entries, numItems);
   tHead = buildTree(&pqHead);
   
   /* allocate space for an array of chars and their respective freq and codes */
   codeTable = (CodePair*)malloc(sizeof(CodePair) * numItems);
   generateCodes(tHead, code, 0, codeTable, &itemsInTable);

   /* so that the output will match the instructors */
   qsort(codeTable, itemsInTable, sizeof(CodePair), comp);

   /* print out the info to the specified out file */
   printHeader(outFd, codeTable, numItems);
   printBody(inFd, outFd, codeTable, numItems);

   /* properly close files and clean up mallocs */
   sysclose(inFd);
   sysclose(outFd);

   return 0;
}
