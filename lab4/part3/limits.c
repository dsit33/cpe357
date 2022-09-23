#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   printf("CHILD_MAX: %lu\n", sysconf(_SC_CHILD_MAX));
   printf("OPEN_MAX: %lu\n", sysconf(_SC_OPEN_MAX));
   printf("PAGESIZE: %lu\n", sysconf(_SC_PAGESIZE));

   return 0;
}
