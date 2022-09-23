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
   fprintf(stderr, "Usage: %s\n", progUsage);
   exit(EXIT_FAILURE);
}

void checkAlloc(void* mem)
{
   if (mem == NULL)
      errorAndExit();
}
