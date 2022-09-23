#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mylib.h"
#include "huffmanTree.h"

#define USAGE "hdecode [ ( infile | - ) [ outfile ] ]\n"

void getNumChars(int inFd, uint8_t* numChars)
{
   sysread(inFd, numChars, sizeof(*numChars));
}

int main(int argc, char* argv[])
{
   hNode* temp, hNode* tHead;
   pqNode *pHead;
   int inFd, outFd, i;
   uint8_t byte, numChars;
   uint32_t freq;

   if (argc > 3)
      usageAndExit(USAGE);

   if (argc >= 2)
      inFd = sysopen(argv[1], O_RDONLY, 0644);

   if (argc == 3)
      outFd = sysopen(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0644);

   /* read the first byte of the in file and store it as total chars */
   getNumChars(inFd, &numChars);

   /* for loop that goes from 0 to num of chars */
   for (i = 0; i < numChars; i++)
   {
      /* call read() and store the char in a pqNode */
      sysread(inFd, &byte, sizeof(byte));
      sysread(inFd, &freq, sizeof(freq));
      temp = createNode(byte, freq, NULL, NULL);

      /* push the new node onto a priority queue */
      push(&pHead, temp);
   }

   /* generate the tree from the priority queue */
   tHead = buildTree(&pHead);

   /* read the body of the out file and generate file from codes*/
   decodeBody(inFd, outFd, tHead);
}
