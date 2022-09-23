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
}

void checkAlloc(void* mem)
{
   if (mem == NULL)
      errorAndExit();
}
