#include <limits.h>
#include "linkedList.h"
#include "hashTable.h"
#include <assert.h>

typedef struct {
   HTFunctions funcs;
   unsigned* sizes;
   int numSizes;
   int curSize;
   float loadFactor;
   unsigned numUnique;
   unsigned total;
   ListNode** entries;
} HashTable;

void checkAlloc(void* mem);
void destroyWithFlag(HashTable* ht);
void destroyNoFlag(HashTable* ht);
int rehashNeeded(HashTable* ht);
HashTable* rehash(void* hashTable);
void newChain(HashTable* ht, void* data, int idx);
unsigned duplicate(HashTable* ht, void* data, int idx);
void collision(HashTable* ht, HTEntry entry, int idx);

void checkAlloc(void* mem)
{
   if (mem == NULL)
   {
      fprintf(stderr, "malloc failure\n");
      exit(EXIT_FAILURE);
   }
}

void* htCreate(
   HTFunctions *functions,
   unsigned sizes[],
   int numSizes,
   float rehashLoadFactor)
{
   int i;
   HashTable* ht;
   HTFunctions funcs;

   assert(numSizes >= 1);
   for (i = 0; i < numSizes - 1; i++)
      assert(sizes[i] < sizes[i + 1] && sizes[i] > 0);
   assert(rehashLoadFactor > 0.0 && rehashLoadFactor <= 1.0);

   ht = calloc(sizeof(HashTable), 1);
   checkAlloc((void*)ht);

   funcs.hash = functions->hash;
   funcs.destroy = functions->destroy;
   funcs.compare = functions->compare;
   ht->funcs = funcs;

   ht->numUnique = 0;
   ht->total = 0;

   ht->sizes = (unsigned*)malloc(sizeof(unsigned) * numSizes);
   checkAlloc((void*)(ht->sizes));
   for (i = 0; i < numSizes; i++)
      ht->sizes[i] = sizes[i];

   ht->curSize = 0;
   ht->numSizes = numSizes;
   ht->loadFactor = rehashLoadFactor;
   
   ht->entries = (ListNode**)calloc(sizeof(ListNode*), sizes[ht->curSize]);
   checkAlloc((void*)(ht->entries));

   for (i = 0; i < sizes[0]; i++)
      assert(ht->entries[i] == NULL);

   return ht;
}

void htDestroy(void *hashTable)
{
   int i, destroyFlag = 1;
   ListNode* head, *temp;
   HashTable* ht = (HashTable*)hashTable;
   
   if (ht->funcs.destroy == NULL)
      destroyFlag = 0;
   /*for each HTEntry, call user destroy function on each ListNode's value*/
   for (i = 0; i < htCapacity(hashTable); i++)
   {
      head = ht->entries[i];
      while (head != NULL)
      {
         temp = head->next;
         if (destroyFlag)
            (ht->funcs).destroy(head->value.data);
         free(head);
         head = temp;
      }
   }
   free(ht->entries);
   free(ht->sizes);
   free(ht);
}

int rehashNeeded(HashTable* ht)
{
   if (ht->loadFactor == 1.0)
      return 0;
   return (htUniqueEntries((void*)ht) + 1 >= htCapacity((void*)ht)
      * ht->loadFactor) && ht->curSize != ht->numSizes - 1;
}

HashTable* rehash(void* hashTable)
{
   int i, freq;
   ListNode* node, *temp;
   HashTable* ht;
   ListNode** prevEntries;
   ht = (HashTable*)(hashTable);
   prevEntries = ht->entries;
   ht->entries = (ListNode**)calloc(ht->sizes[ht->curSize], sizeof(ListNode*));
   
   ht->numUnique = 0;
   ht->total = 0;
   /*deep copy all entries from old HashTable and rehash into new one*/
   for (i = 0; i < ht->sizes[ht->curSize - 1]; i++)
   {
      node = prevEntries[i];
      while (node != NULL)
      {
         temp = node->next;
         for (freq = 0; freq < node->value.frequency; freq++)
            htAdd((void*)ht, node->value.data);
         free(node);
         node = temp;
      }
   }
   
   free(prevEntries);

   return ht;
}

void newChain(HashTable* ht, void* data, int idx)
{
   HTEntry entry;
   entry.data = data;
   entry.frequency = 1;
   ht->entries[idx] = addHead(ht->entries[idx], entry);
   (ht->numUnique)++;
   (ht->total)++;
}

void collision(HashTable* ht, HTEntry entry, int idx)
{
   (ht->numUnique)++;
   (ht->total)++;
   (entry.frequency)++;
   ht->entries[idx] = addTail(ht->entries[idx], entry);
}

unsigned duplicate(HashTable* ht, void* data, int idx)
{
   ListNode* node;

   (ht->total)++;
   node = ht->entries[idx];
   while ((ht->funcs).compare(node->value.data, data) != 0)
      node = node->next;

   node->value.frequency += 1;
   return node->value.frequency;
}

unsigned htAdd(void *hashTable, void *data)
{
   int idx;
   HTEntry lookUp;
   HashTable* ht = (HashTable*)hashTable;
   assert(data != NULL);

   if (rehashNeeded(ht)) 
   {
      (ht->curSize)++;
      ht = rehash(hashTable);
   }

   idx = (ht->funcs).hash(data) % htCapacity((void*)ht);

   /*search to see if the entry already exists*/
   lookUp = htLookUp((void*)ht, data);

   /*if it wasn't already in the hashTable add a new chain*/
   if (lookUp.data == NULL)
   {
      newChain(ht, data, idx);
      return 1;
   }

   /*if the value hashes to the same index as an unequal value, add to chain*/
   if (lookUp.frequency == 0)
   {
      collision(ht, lookUp, idx);
      return 1;
   }

   /*there was already an entry for this value, return frequency*/
   return duplicate(ht, data, idx);
}

HTEntry htLookUp(void *hashTable, void *data)
{
   int idx;
   ListNode* node;
   HTEntry entry;
   HashTable* ht = (HashTable*)hashTable;
   entry.data = NULL;
   entry.frequency = 0;
   
   assert(data != NULL);

   idx = (ht->funcs).hash(data) % htCapacity(hashTable);

   /*there is no chain at this index*/
   if ((node = ht->entries[idx]) == NULL)
      return entry;

   /*compare all values in the chain against the parameter*/
   while (node != NULL)
   {
      if ((ht->funcs).compare(node->value.data, data) == 0)
      {
         return node->value;
      }
      node = node->next;
   }

   /*there was a chain here but the value is unequal to any others*/
   entry.data = data;
   entry.frequency = 0;

   return entry;
}

HTEntry* htToArray(void *hashTable, unsigned *size)
{
   int i, hashIdx;
   HTEntry* array;
   ListNode* node;
   HashTable* ht = (HashTable*)hashTable;
   if ((*size = htUniqueEntries(hashTable)) == 0)
      return NULL;

   array = (HTEntry*)malloc(sizeof(HTEntry) * (*size));
   
   i = 0;
   hashIdx = 0;
   while (i < *size)
   {
      node = ht->entries[hashIdx];
      while (node != NULL)
      {
         array[i++] = node->value;
         node = node->next;
      }
      hashIdx++;
   }

   return array;
}

unsigned htCapacity(void *hashTable)
{
   return ((HashTable*)hashTable)->sizes[((HashTable*)hashTable)->curSize];
}

unsigned htUniqueEntries(void *hashTable)
{
   return ((HashTable*)hashTable)->numUnique;
}

unsigned htTotalEntries(void *hashTable)
{
   return ((HashTable*)hashTable)->total;
}

HTMetrics htMetrics(void *hashTable)
{
   int i, chainLength = 0;
   ListNode* head;
   HashTable* ht = (HashTable*)hashTable;
   HTMetrics metrics = {0};

   for (i = 0; i < htCapacity(hashTable); i++)
   {
      chainLength = 0;
      if ((head = ht->entries[i]) != NULL)
      {
         ++(metrics.numberOfChains);
      }
      while (head != NULL)
      {
         ++chainLength;
         head = head->next;
      }
      metrics.maxChainLength = (chainLength > metrics.maxChainLength) ?
         chainLength : metrics.maxChainLength;
   }
 
   metrics.avgChainLength = (float)htUniqueEntries(hashTable)
      / metrics.numberOfChains;

   return metrics;
}

