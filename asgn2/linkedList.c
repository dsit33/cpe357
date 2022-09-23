#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"
#include "hashTable.h"

int outOfBounds(ListNode* list, int index);
int size(ListNode* list);

ListNode* addHead(ListNode *list, HTEntry value)
{
   ListNode* head;

   if (list == NULL)
   {
      return createNode(value);
   }
   head = createNode(value);
   head->next = list;

   return head;
}

ListNode* addTail(ListNode *list, HTEntry value)
{
   ListNode* current = list;

   if (list == NULL)
   {
      return createNode(value);
   }
   while (current->next != NULL)
   {
      current = current->next;
   }
   current->next = createNode(value);

   return list;
}

int outOfBounds(ListNode* list, int index)
{
   return (list == NULL || index < 0 || index > size(list) - 1);
}

ListNode* deleteNode(ListNode *list, int index)
{
   int i = 0;
   ListNode* toDelete;
   ListNode* current = list;

   if (outOfBounds(list, index))
   {
      printf("Index %d is out of bounds\n", index);
      return list;
   }
   if (index == 0)
   {
      free(list);
      return list->next;
   }
   /*go through nodes until you reach just before index*/
   while (i < index - 1)
   {
      current = current->next;
      i++;
   }
   toDelete = current->next;
   current->next = toDelete->next;
   free(toDelete);
   return list;
}

ListNode* createNode(HTEntry value)
{
   ListNode* new = (ListNode*)malloc(sizeof(ListNode));
   if (new == NULL)
   {
      fprintf(stderr, "malloc failure\n");
      exit(EXIT_FAILURE);
   }
   new->value = value;
   new->next = NULL;

   return new;
}

int size(ListNode* list)
{
   int i = 1;
   ListNode* current = list;

   if (list == NULL)
   {
      return 0;
   }
   while (current->next != NULL)
   {
      current = current->next;
      i++;
   }

   return i;
}

