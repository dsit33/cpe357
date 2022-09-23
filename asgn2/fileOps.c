#include "myError.h"
#include "fileOps.h"
#include "hashTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define OUT 0
#define IN 1

void tryFile(char* argv[], int idx, FILE** files, int* numFiles)
{
   files[(*numFiles)++] = openFile(argv[idx], "r");;
}

FILE* openFile(char* fname, char* mode)
{
   FILE* file;

   if ((file = fopen(fname, mode)) == NULL)
      perror(fname);

   return file;
}

void appendChar(char** word, int* blockSize, int wordLength, int c)
{
   if (wordLength >= *blockSize)
   {
      *blockSize *= 2;
      *word = (char*)realloc(*word, sizeof(char) * *blockSize);
      checkBlock(*word);
   }
   (*word)[wordLength] = c;
}

void parseFile(FILE* file, void* ht)
{
   int c, blockSize, wordLength, inWord = OUT;
   char* word;
   
   wordLength = 0;
   blockSize = 8;
   word = (char*)malloc(sizeof(char) * blockSize);
   checkBlock(word);

   while ((c = fgetc(file)) != EOF)
   {
      c = tolower(c);
      /*start new word*/
      if (isalpha(c))
      {
         inWord = IN;
         appendChar(&word, &blockSize, wordLength, c);
         wordLength++;
      }
      /*end of word*/
      else if (inWord && !isalpha(c))
      {
         /*set state to outside of word and shrink mem block*/
         inWord = OUT;
         blockSize = wordLength + 1;
         word = (char*)realloc(word, sizeof(char) * blockSize);
         checkBlock(word);
         word[wordLength] = '\0';

         /*if word was a duplicate, free the mem block*/
         if (htAdd(ht, word) > 1)
            free(word);

         /*create space for new word*/
         word = (char*)malloc(sizeof(char) * blockSize);
         checkBlock(word);
         wordLength = 0;
      }
   }
   if (wordLength == 0)
   {
      free(word);
      return;
   }
   blockSize = wordLength + 1;
   word = (char*)realloc(word, sizeof(char) * blockSize);
   checkBlock(word);
   word[wordLength] = '\0';
   if (htAdd(ht, word) > 1)
      free(word);
}

void iterateFiles(FILE** files, int numFiles, void* ht)
{
   int i;

   for (i = 0; i < numFiles; i++)
   {
      if (files[i] != NULL)
      {
         parseFile(files[i], ht);
         fclose(files[i]);
      }
   }
}
