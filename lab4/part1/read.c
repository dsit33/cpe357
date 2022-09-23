#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
   ssize_t readSize;
   int fd, numBytes;
   char* buf;

   numBytes = atoi(argv[1]);
   buf = (char*)malloc(sizeof(char) * numBytes);

   fd = open("/usr/lib/locale/locale-archive", O_RDONLY);
   readSize = read(fd, buf, numBytes);
   while (readSize != 0 && readSize == numBytes)
   {
      readSize = read(fd, buf, numBytes);
   }
   free(buf);
   close(fd);

   return 0;
}
