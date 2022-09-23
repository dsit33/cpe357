#ifndef FILEOPS_H
#define FILEOPS_H

#include <stdio.h>
#include <stdlib.h>

void tryFile(char* argv[], int idx, FILE** files, int* numFiles);
FILE* openFile(char* fname, char* mode);
void iterateFiles(FILE** files, int numFiles, void* ht);

#endif
