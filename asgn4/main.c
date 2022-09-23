#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "mytar.h"

#define USAGE "mytar [ctxvS]f tarfile [ path [ ...  ]  ]"

int validateArgs(int argc, char* argv[], char** tar_name, char** files, int *verb)
{
   int i, length, numFiles = 0;
   char numFlags, flag;

   numFlags = flag = 0;

   if (argc < 3)
      usageAndExit(USAGE);

   if (strlen(argv[1]) < 2)
      usageAndExit(USAGE);

   if (!strchr(argv[1], 'f'))
      usageAndExit(USAGE);
   
   length = strlen(argv[2]);
   *tar_name = (char*)calloc(length + 1, sizeof(char));
   strcat(*tar_name, argv[2]);
   (*tar_name)[length] = '\0';

   if (strchr(argv[1], 'v'))
      *verb = 1;
   if (strchr(argv[1], 'c') && numFlags < 1)
   {
      numFlags++;
      flag = 'c';
   }
   if (strchr(argv[1], 'x') && numFlags < 1)
   {
      numFlags++;
      flag = 'x';
   }
   if (strchr(argv[1], 't') && numFlags < 1)
   {
      numFlags++;
      flag = 't';
   }

   /* get specified files */
   if (files)
   {
      for (i = 3; i < argc; i++)
      {
         length = strlen(argv[i]);
         files[numFiles] = (char*)calloc(length + 1, sizeof(char));
         strcpy(files[numFiles], argv[i]);
         files[numFiles++][length] = '\0';
      }
   }
   return flag;
}

void create_tar(char* tar_name, int numFiles, char** files, int verb)
{
   int fd;
   Tar* archive = NULL;

   fd = sysopen(tar_name, O_CREAT|O_WRONLY|O_TRUNC, 0666);
   if (write_tar(fd, &archive, numFiles, files, verb) == -1)
      fprintf(stderr, "Could not create tar %s\n", tar_name);
}

void print_tar(char* tar_name, int numFiles, char** files, int verb)
{
   int fd;
   Tar* archive = NULL;

   if ((fd = open(tar_name, O_RDWR|O_CREAT, 0666)) == -1)
   {
      fprintf(stderr, "DEBUG: issue occurs in initial opening\n");
      errorAndExit(tar_name);
   }
   if (read_tar(fd, &archive, verb) == -1)
      fprintf(stderr, "Could not read tar\n");
   if (list_tar(stdout, archive, numFiles, files, verb) == -1)
      fprintf(stderr, "Could not print tar\n");
}

void extract(char* tar_name, int numFiles, char** files, int verb)
{
   int fd;
   Tar* archive = NULL;

   if ((fd = open(tar_name, O_RDWR|O_CREAT, 0777)) == -1)
   {
      fprintf(stderr, "DEBUG: issue occurs in initial opening\n");
      errorAndExit(tar_name);
   }
   if (read_tar(fd, &archive, verb) == -1)
      fprintf(stderr, "Could not read tar\n");
   if (extract_tar(fd, archive, numFiles, files, verb) == -1)
      fprintf(stderr, "Could not extract tar\n");
}

int main(int argc, char* argv[])
{
   int numFiles, flag, verb = 0;
   char *tar_name, **files;

   if (argc >= 4)
   {
      numFiles = argc - 3;
      files = (char**)calloc(numFiles, sizeof(char*));
   }

   if ((flag = validateArgs(argc, argv, &tar_name, files, &verb)) == 0)
      usageAndExit(USAGE);

   switch (flag)
   {
      case 'c': create_tar(tar_name, numFiles, files, verb);
         break;
      case 't': print_tar(tar_name, numFiles, files, verb);
         break;
      case 'x': extract(tar_name, numFiles, files, verb);
         break;
      default:
         break;
   }

   return 0;
}
