#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
   int numBytes;
   size_t readSize;
   char* buf;
   FILE* in = fopen("/usr/lib/locale/locale-archive", "r");

   numBytes = atoi(argv[1]);
   buf = (char*)malloc(sizeof(char) * numBytes);

   readSize = fread(buf, sizeof(char), numBytes, in);
   while (readSize != 0)
      readSize = fread(buf, sizeof(char), numBytes, in);
   
   free(buf);
   fclose(in);
   return 0;
}
