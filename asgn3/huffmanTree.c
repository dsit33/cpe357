#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
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

void addCode(CodePair* codeTable, int freq, int data, char* code, int idx)
{
   CodePair cp;
   cp.data = data;
   cp.freq = freq;
   cp.code = code;

   codeTable[idx] = cp;
}

void bin(uint8_t n)
{
   unsigned i;
   for (i = 1 << 7; i > 0; i = i / 2)
      (n & i)? printf("1"): printf("0");
   printf("\n");
}

void generateCodes(hNode* head, char* curCode, int numSteps, CodePair* codeTable,
   int* numItems)
{
   char* finalCode;
   
   if (head->data == 0)
   {
      curCode[numSteps] = '0';
      generateCodes(head->left, curCode, numSteps + 1, codeTable, numItems);
      curCode[numSteps] = '1';
      generateCodes(head->right, curCode, numSteps + 1, codeTable, numItems);

      return;
   }
   curCode[numSteps] = '\0';
   finalCode = (char*)malloc(sizeof(char) * (numSteps + 1));
   strcpy(finalCode, curCode);

   printf("DEBUG: adding code: %s for char: %c\n", finalCode, head->data);
   addCode(codeTable, head->freq, head->data, finalCode, (*numItems)++);
}

char* findCode(int c, CodePair* codeTable, int numItems)
{
   CodePair cp;
   int i;

   for (i = 0; i < numItems; i++)
   {
      cp = codeTable[i];
      if (cp.data == c)
         return cp.code;
   }
   return NULL;
}

uint8_t convertbin(char* binstr)
{
   int i;
   uint8_t bit, val = 0;

   for (i = 0; i < 8; i++)
   {
      bit = 1 << (7 - i);
      if (binstr[i] == '1')
         val = val ^ bit;
   }
   return val;
}

int attach(char* dest, char* src)
{
   int bitsRead, result, i, j = 0;

   bitsRead = 0;
   i = strlen(dest);
   
   for (j = 0; j < strlen(src) && strlen(dest) < 8; j++)
   {
      dest[i++] = src[j];
      bitsRead++;
   }
   
   (bitsRead == strlen(src)) ? (result =  0) : (result = bitsRead);
   return result;
}
