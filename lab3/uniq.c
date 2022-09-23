#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNextLine(char** curLine, FILE* in, int* blockSize)
{
    int c, len = 0;

    while ((c = fgetc(in)) != '\n' && c != EOF)
    {
       if (len >= *blockSize)
       {
          *blockSize = len * 2;
          *curLine = (char*)realloc(*curLine, sizeof(char) * *blockSize);
       }
       (*curLine)[len++] = c;
    }
    (*curLine)[len++] = c;
    (*curLine)[len++] = '\0';
    *blockSize = len;
    *curLine = (char*)realloc(*curLine, sizeof(char) * *blockSize);
 
    if (c == EOF)
    {
       return EOF;
    }

    return 0;
}

void putLine(char** curLine, char** prevLine, FILE* out, int* blockSize)
{
   if (*prevLine == NULL)
   {
      *prevLine = (char*)calloc(*blockSize, sizeof(char));
      strcpy(*prevLine, *curLine);
      fputs(*curLine, out);
      free(*curLine);
      *blockSize = 16;
      *curLine = (char*)calloc(*blockSize, sizeof(char));
      return;
   }
   if (strcmp(*curLine, *prevLine) != 0)
   {
      free(*prevLine);
      *prevLine = (char*)calloc(*blockSize, sizeof(char));
      strcpy(*prevLine, *curLine);
      fputs(*curLine, out);
      free(*curLine);
      *blockSize = 16;
      *curLine = (char*)calloc(*blockSize, sizeof(char));
      return;
   }
}

int main(int argc, char* argv[])
{
   int blockSize = 16;
   char* curLine, *prevLine = NULL;
   FILE* in = stdin;
   FILE* out = stdout;

   if (argc > 3)
   {
      fprintf(stderr, "uniq: extra operand %s\n", argv[3]);
      exit(EXIT_FAILURE);
   }

   if (argc > 1)
      in = fopen(argv[1], "r");

   if (argc > 2)
      out = fopen(argv[2], "w");

   curLine = (char*)malloc(sizeof(char) * blockSize);
   
   while (getNextLine(&curLine, in, &blockSize) != EOF)
   {
      putLine(&curLine, &prevLine, out, &blockSize);
   }
   free(prevLine);
   free(curLine);

   fclose(in);
   fclose(out);

   return 0;
}

