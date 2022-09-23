#ifndef MYTAR_H
#define MYTAR_H

#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/sysmacros.h>

#define RECORD 10240
#define REG 0
#define NORM '0'
#define HARDLINK '1'
#define SYMLINK '2'
#define CHAR '3'
#define BLOCK '4'
#define DIRECT '5'
#define FIFO '6'
#define CONT '7'
#define DEFAULT S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH

typedef struct tar {
   char file_name[100];
   unsigned int beginning;
   union {
      struct {
         char name[100];
         char mode[8];
         char uid[8];
         char gid[8];
         char size[12];
         char mtime[12];
         char chksum[8];
         char type;
         char link_name[100];
         char ustar[8];
         char owner_name[32];
         char group_name[32];
         char major_num[8];
         char minor_num[8];
         char prefix[155];
      };
      char block[512];
   };
   struct tar* next;
} Tar;

/* read tar file */
int read_tar(int fd, Tar** archive, int verb);

/* write to tar file */
int write_tar(int fd, Tar** archive, size_t numFiles, char** files, int verb);

/* write entries to tar file */
int write_entries(int fd, Tar** archive, Tar** head, size_t numFiles, 
   char** files, int *offset, int verb);

/* write the end metadata to the archive */
int write_end(int fd, int size, int verb);

/* list contents of archive */
int list_tar(FILE* file, Tar* archive, size_t numFiles, char** files, int verb);

/* list single entry in archive */
int list_entry(FILE* file, Tar* archive, unsigned int* max_size,
   size_t numFiles, char** files, int verb);

/* extract files from a previously created archive */
int extract_tar(int fd, Tar* archive, size_t numFiles, char** files, int verb);

/* extract single entry from archive */
int extract_entry(int fd, Tar* entry, int verb);

/* update an exisitng archive with new files */
int update_tar(int fd, Tar** archive, size_t numFiles, char** files, int verb);

/* remove specific entries from an archive */
int remove_tar(int fd, Tar** archive, size_t numFiles, char** files, int verb);

/* list all the files missing from the directory */
int diff_tar(FILE* file, Tar* archive, int verb);

/* free tar archive */
void destroy_tar(Tar* archive);

/* read file and get metadata */
int get_tar_data(Tar* ent, char* file_name, int verb);

/* see if the file exists in the tar already */
Tar* exists_in_tar(Tar* archive, char* file_name, char original);

/* calculate the checksum */
unsigned int calculate_checksum(Tar* ent);

/* find matching entry and return the index */
int find_match(Tar* entry, size_t numFiles, char* bad, char** files);

/* function that builds all directories from a path name */
int my_mkdir(char* directory, int mode, int verb);

/* free the give tar struct */
void free_tar(Tar* ent);

void peek_tar_data(Tar* archive);

void peek_entry(Tar* ent);

#endif
