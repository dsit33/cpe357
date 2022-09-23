#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "priorityQueue.h"

/* huffman tree node struct */
typedef struct node{
   int data;
   int freq;
   struct node* left;
   struct node* right;
} hNode;

/* dynamically allocate space for an hNode */
hNode* createNode(int data, int freq, hNode* left, hNode* right);

/* attempt to add the char to the array of entries. This function
 * returns the frequency of the added data */
int addEntry(hNode*** entries, unsigned* blockSize, unsigned numItems, int data);

/* loop through the array of entries and look to see if data is
 * a duplicate. If it is, return a pointer to the existing node
 * else return NULL */
hNode* lookUp(hNode** entries, unsigned numItems, int data);

/* turn the array into a proper huffman tree */
hNode* buildTree(void* pHead);

/* recursively print the huffman tree codes */
void printCodes(hNode* head, char* code, int numSteps);

#endif
