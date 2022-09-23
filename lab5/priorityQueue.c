#include <stdio.h> 
#include <stdlib.h>
#include "huffmanTree.h"
#include "priorityQueue.h"

pqNode* newNode(void* entry) 
{ 
   pqNode* temp = (pqNode*)malloc(sizeof(pqNode));
   temp->entry = entry;
   temp->next = NULL;
   return temp;
}

pqNode* peek(pqNode** head) 
{ 
   return (*head);
}

pqNode* pop(pqNode** head)
{ 
   pqNode* temp = *head;
   (*head) = (*head)->next;
   return temp;
} 

void push(pqNode** head, void* node) 
{
   hNode* entry = (hNode*)node;
   pqNode* start = (*head); 
   pqNode* temp = newNode(entry); 

   if (((hNode*)(*head)->entry)->freq > entry->freq) { 
      temp->next = *head; 
      (*head) = temp; 
   } 
   else { 
      while (start->next != NULL && ((hNode*)start->next->entry)->freq 
         < entry->freq) { 
        start = start->next; 
      } 

      temp->next = start->next; 
      start->next = temp; 
   } 
} 

int isEmpty(pqNode** head) 
{ 
   return (*head) == NULL; 
}

void printQueue(pqNode* head)
{
   pqNode* temp;
   
   while (head->next != NULL)
   {
      temp = head->next;
      printf("%c: %d\n", ((hNode*)head->entry)->data, 
         ((hNode*)head->entry)->freq);
      head = temp;
   }
   printf("%c: %d\n", ((hNode*)head->entry)->data, 
      ((hNode*)head->entry)->freq);
}
