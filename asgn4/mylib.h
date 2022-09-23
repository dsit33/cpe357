#ifndef MYLIB_H
#define MYLIB_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void errorAndExit(char* fname);
void usageAndExit(char* progUsage);
void checkAlloc(void* mem);
FILE* openFile(char* fname, char* mode);
int sysopen(char* pathname, int flags, mode_t mode);
int syswrite(int fd, const void* buf, size_t count);
void sysclose(int fd);
int sysread(int fd, void* buf, int bufsize);

#endif
