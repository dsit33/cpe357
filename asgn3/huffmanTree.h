#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <stdint.h>
#include "priorityQueue.h"

/* code pair struct for easy encoding */
typedef struct codepair{
   int data;
   int freq;
   char* code;
} CodePair;

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

/* print out the binary representation of an int */
void bin(uint8_t n);

/* recursively generate the huffman tree codes */
void generateCodes(hNode* head, char* curCode, int numSteps, CodePair* codeTable, int* numItems);

/* find code for easier encoding */
char* findCode(int c, CodePair* codeTable, int numItems);

/* convert a str of binary bits to a value */
uint8_t convertbin(char* binstr);

/* attach the char code to the running byte, return the number of bits read if
 * not all chars were transfered over */
int attach(char* dest, char* src);

#endif
