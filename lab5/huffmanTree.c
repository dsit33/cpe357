#include <stdio.h>
#include <stdlib.h>
#include "priorityQueue.h"
#include "huffmanTree.h"

hNode* createNode(int data, int freq, hNode* left, hNode* right)
{
   hNode* new = (hNode*)malloc(sizeof(hNode));

   new->data = data;
   new->freq = freq;
   new->left = left;
   new->right = right;

   return new;
}

int addEntry(hNode*** entries, unsigned* blockSize, unsigned numItems, int data)
{
   hNode* node;

   /* if the data is a duplicate, return the incremented frequency */
   if ((node = lookUp(*entries, numItems, data)) != NULL)
   {
      return ++(node->freq);
   }

   /* data is new, add to array, resize when necessary */
   node = createNode(data, 1, NULL, NULL);
   if (numItems >= *blockSize * .7)
   {
      *blockSize *= 2;
      *entries = (hNode**)realloc(*entries, sizeof(hNode*) * *blockSize);
   }
   (*entries)[numItems] = node;

   return node->freq;
}

hNode* lookUp(hNode** entries, unsigned numItems, int data)
{
   int i;
   hNode* node;
   
   /* if this is a duplicate, return pointer to exisitng node*/
   for (i = 0; i < numItems; i++)
   {
      node = entries[i];
      if (data == node->data)
         return node;
   }

   return NULL;
}

hNode* buildTree(void* pHead)
{
   pqNode* temp1, *temp2;
   pqNode** pqHead = (pqNode**)pHead;
   hNode* head, *node1, *node2;

   while ((*pqHead)->next != NULL)
   {
      temp1 = pop(pqHead);
      temp2 = pop(pqHead);
      node1 = temp1->entry;
      node2 = temp2->entry;
      head = createNode(0, (node1->freq) + (node2->freq),
         node1, node2);
      if (isEmpty(pqHead))
         *pqHead = newNode(head);
      else
         push(pqHead, head);
      free(temp1);
      free(temp2);      
   }
   temp1 = pop(pqHead);
   head = temp1->entry;
   free(temp1);

   return head;
}

void printCode(hNode* node, char* code)
{
   printf("0x%02x: %s\n", node->data, code);
}

void printCodes(hNode* head, char* code, int numSteps)
{
   if (head->data == 0)
   {
      code[numSteps] = '0';
      printCodes(head->left, code, numSteps + 1);
      code[numSteps] = '1';
      printCodes(head->right, code, numSteps + 1);

      return;
   }
   code[numSteps] = '\0';
   printCode(head, code);
}

