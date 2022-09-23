#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mylib.h"

void errorAndExit(char* fname)
{
   perror(fname);
   exit(EXIT_FAILURE);
}

void usageAndExit(char* progUsage)
{
   printf("Usage: %s\n", progUsage);
   exit(EXIT_FAILURE);
}

void checkAlloc(void* mem)
{
   if (mem == NULL)
      errorAndExit(NULL);
}

FILE* openFile(char* fname, char* mode)
{
   FILE* file;

   if ((file = fopen(fname, mode)) == NULL)
      errorAndExit(fname);

   return file;
}

int sysopen(char* pathname, int flags, mode_t mode)
{
   int fd;

   if ((fd = open(pathname, flags, mode)) == -1)
      errorAndExit(pathname);

   return fd;
}

int syswrite(int fd, const void* buf, size_t count)
{
   int bytes;

   if ((bytes = write(fd, buf, count)) == -1)
      errorAndExit(NULL);

   return bytes;
}

void sysclose(int fd)
{
   if (close(fd) == -1)
      errorAndExit(NULL);
}

int sysread(int fd, void* buf, int bufsize)
{
   int bytes;

   if ((bytes = read(fd, buf, bufsize)) == -1)
      errorAndExit(NULL);

   return bytes;
}
