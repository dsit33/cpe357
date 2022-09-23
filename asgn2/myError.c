#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include "myError.h"

void errorAndExit()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}

void usageAndExit(char* progUsage)
{
   printf("Usage: %s\n", progUsage);
   exit(EXIT_FAILURE);
}

void checkBlock(void* mem)
{
   if (mem == NULL)
      errorAndExit();
}
