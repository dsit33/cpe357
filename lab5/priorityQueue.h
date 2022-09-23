#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

typedef struct pqnode {
   void* entry;
   struct pqnode* next;
} pqNode;

pqNode* newNode(void* entry);
pqNode* peek(pqNode** head);
pqNode* pop(pqNode** head);
void push(pqNode** head, void* entry);
int isEmpty(pqNode** head);
void printQueue(pqNode* head);

#endif
