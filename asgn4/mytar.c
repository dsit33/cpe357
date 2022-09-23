#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "mylib.h"
#include "mytar.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define VERBOSE(f, message, format) { if (verb) fprintf(f, message, format); }
#define ERROR perror("mytar"); return -1;

int iszeroblk(char* buf, unsigned size)
{
   int i;

   for (i = 0; i < size; i++)
   {
      if (buf[i] != 0)
         return 0;
   }
   return 1;
}

unsigned int otoui(char* octal, unsigned int size)
{
   unsigned int result = 0;
   int i;
   
   /* for loop to convert a string of octal characters to unsigned */
   for (i = 0; i < size && octal[i]; i++)
      result = (result << 3) | (unsigned int)(octal[i] - '0');

   return result;
}

unsigned int calculate_checksum(Tar* ent)
{
   int i;
   unsigned int sum = 0;

   memset(ent->chksum, ' ', sizeof(char) * 8);

   for (i = 0; i < 500; i++)
      sum += (unsigned char)(ent->block[i]);

   sprintf(ent->chksum, "%06o", sum);
   ent->chksum[6] = '\0';
   ent->chksum[7] = ' ';

   return sum;
}

void set_entry(Tar* ent, char* file_name, int offset, struct stat st, int verb)
{
   memset(ent, 0, sizeof(Tar));
   strncpy(ent->file_name, file_name, 100);
   strncpy(ent->name, file_name + offset, 100);
   sprintf(ent->mode, "%07o", st.st_mode & 0777);
   sprintf(ent->uid, "%07o", st.st_uid);
   sprintf(ent->gid, "%07o", st.st_gid);
   sprintf(ent->size, "%011o", (unsigned int)st.st_size);
   sprintf(ent->mtime, "%011o", (int)st.st_mtime);
   strncpy(ent->group_name, "None", 4);
   memcpy(ent->ustar, "ustar\00000", 8);
}

int get_tar_data(Tar* ent, char* file_name, int verb)
{
   mode_t mode;
   int offset = 0;
   struct stat st;
   struct group* g;

   if (ent == NULL)
      return -1;

   if (lstat(file_name, &st) == -1)
   {
      ERROR
   }

   if (strncmp(file_name, "/", 1) == 0)
      offset = 1;
   if (strncmp(file_name, ".", 1) == 0)
      offset = 2;
   if (strncmp(file_name, "..", 2) == 0)
      offset = 3;

   set_entry(ent, file_name, offset, st, verb);

   mode = (st.st_mode & S_IFMT);
   if (mode == S_IFREG)
      ent->type = NORM;
   else if (mode == S_IFLNK)
   {
      ent->type = SYMLINK;
      strncpy(ent->size, "00000000000", 11);

      if (readlink(file_name, ent->link_name, 100) == -1)
      {
         ERROR
      }
   }
   else if (mode == S_IFCHR)
   {
      ent->type = CHAR;
      sprintf(ent->major_num, "%08o", major(st.st_dev));
      sprintf(ent->minor_num, "%08o", minor(st.st_dev));
   }
   else if (mode == S_IFBLK)
   {
      ent->type = BLOCK;
      sprintf(ent->major_num, "%08o", major(st.st_dev));
      sprintf(ent->minor_num, "%08o", minor(st.st_dev));
   }
   else if (mode == S_IFDIR)
      ent->type = DIRECT;
   else if (mode == S_IFIFO)
      ent->type = FIFO;
   else
   {
      ent->type = -1;
      return -1;
   }

   if (getlogin_r(ent->owner_name, 32) != 0)
   {
      ERROR
   }

   if ((g = getgrgid(st.st_gid)) != NULL)
      strncpy(ent->group_name, g->gr_name, 100);

   calculate_checksum(ent);
   return 0;
}

int read_tar(int fd, Tar** archive, int verb)
{
   char dub_zero = 0;
   unsigned offset, seek_jump, count = 0;
   Tar** temp = archive;

   offset = 0;

   while (!dub_zero)
   {
      *temp = (Tar*)malloc(sizeof(Tar));
      sysread(fd, (*temp)->block, 512);

      if (iszeroblk((*temp)->block, 512))
      {
         sysread(fd, (*temp)->block, 512);
         if (iszeroblk((*temp)->block, 512))
         {
            free_tar(*temp);
            *temp = NULL;
            dub_zero = 1;

            if (lseek(fd, RECORD - (offset % RECORD), SEEK_CUR) == -1)
            {
               ERROR
            }
         }
      }

      if (!dub_zero)
      {
         (*temp)->beginning = offset;
         seek_jump = otoui((*temp)->size, 11);

         if (seek_jump % 512 != 0)
            seek_jump += 512 - (seek_jump % 512);

         offset += 512 + seek_jump;
         if (lseek(fd, seek_jump, SEEK_CUR) == -1)
         {
            ERROR
         }

         temp = &((*temp)->next);
      }
      count++;
   }
   return count;
}

void free_tar(Tar* archive)
{
   Tar* temp;

   while (archive)
   {
      temp = archive->next;
      free(archive);
      archive = temp;
   }
}

int list_tar(FILE* file, Tar* archive, size_t numFiles, char** files, int verb)
{
   unsigned int max = 0;

   if (!verb)
      return 0;

   if (numFiles != 0 && files == NULL)
      return -1;

   while (archive)
   {
      if (list_entry(file, archive, &max, numFiles, files, verb) == -1)
         return -1;
      archive = archive->next;
   }
   return 0;
}

void print_f_details(FILE* file, Tar* ent, char* buf, unsigned int* max_size)
{
   int result, i;
   time_t mtime;
   struct tm* time;
   mode_t mode = otoui(ent->mode, 7);
   char mode_str[26] = {"-hlcbdp-"[ent->type ? ent->type - '0' : 0],
      mode & S_IRUSR ? 'r' : '-',
      mode & S_IWUSR ? 'w' : '-',
      mode & S_IXUSR ? 'x' : '-',
      mode & S_IRGRP ? 'r' : '-',
      mode & S_IWGRP ? 'w' : '-',
      mode & S_IXGRP ? 'x' : '-',
      mode & S_IROTH ? 'r' : '-',
      mode & S_IWOTH ? 'w' : '-',
      mode & S_IXOTH ? 'x' : '-',
      0};

   fprintf(file, "%s %s/%s ", mode_str, ent->owner_name, ent->group_name);
   
   if (ent->type == CHAR || ent->type == BLOCK)
      result = sprintf(buf, "%d,%d", otoui(ent->major_num, 7), 
         otoui(ent->minor_num, 7));
   else
      result = sprintf(buf, " %7u", otoui(ent->size, 11));

   *max_size = MAX(*max_size, result);

   for (i = 0; i < (*max_size - result); i++)
      fprintf(file, " ");
   fprintf(file, "%s", buf);

   mtime = otoui(ent->mtime, 11);
   time = localtime(&mtime);
   fprintf(file, " %d-%02d-%02d %02d:%02d ", time->tm_year + 1900, 
      time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min);
}

int list_entry(FILE* file, Tar* ent, unsigned int* max_size, 
   size_t numFiles, char** files, int verb)
{
   int i;
   char print, buf[22] = {0};

   print = (numFiles != 0) ? 0 : 1;
   for (i = 0; i < numFiles; i++)
   {
      if (strncmp(ent->name, files[i], MAX(strlen(ent->name), 
            strlen(files[i])) != 0))
         print = 1;
   }
   if (print != 0)
   {
      if (verb)
         print_f_details(file, ent, buf, max_size);
      fprintf(file, "%s", ent->name);
         
      if (verb)
      {
         if (ent->type == HARDLINK)
            fprintf(file, " link to %s", ent->link_name);
         if (ent->type == SYMLINK)
            fprintf(file, " -> %s", ent->link_name);
      }
      fprintf(file, "\n");
   }
 
   return 0;
}

int write_tar(int fd, Tar** archive, size_t numFiles, char** files, int verb)
{
   int seek_jump, offset = 0;
   Tar** temp = archive;

   if (archive == NULL)
      return -1;
   
   if (*temp != NULL)
   {
      while ((*temp) != NULL && (*temp)->next != NULL)
         temp = &((*temp)->next);

      seek_jump = 512 + otoui((*temp)->size, 11);

      /* if the offset isn't a full block, round to the nearest number that is */
      if (seek_jump % 512 != 0)
         seek_jump += 512 - (seek_jump % 512);

      offset = (*temp)->beginning + seek_jump;
      if (lseek(fd, offset, SEEK_SET) == -1)
      {
         ERROR
      }
   }

   if (write_entries(fd, temp, archive, numFiles, files, &offset, verb) == -1)
      return -1;

   if (write_end(fd, offset, verb) == -1)
      return -1;
   
   /* clear all tar names in archive */
   temp = archive;
   while (*temp != NULL)
   {
      memset((*temp)->name, 0, 100);
      temp = &((*temp)->next);
   }

   return offset;
}

int write_end(int fd, int size, int verb)
{
   int i, padding = 0;
   
   if (size != 512)
      padding = 512 - (size % 512);

   for (i = 0; i < padding; i++)
   {
      if (write(fd, "\0", 1) == -1)
      {
         ERROR
      }
   }

   for (i = 0; i < 512; i++)
   {
      if (write(fd, "\0", 1) == -1)
      {
         ERROR
      }
   }
   return padding;
}

int write_entries(int fd, Tar** archive, Tar** head, size_t numFiles, 
   char** files, int* offset, int verb)
{
   DIR* dir;
   struct dirent* d_ent;
   unsigned size, padding;
   int i, bytes, length, len2, newfd, seen = 0;
   char buf[512], *parent_dir, *path_name;
   Tar *search, **temp = archive;

   if (numFiles && files == NULL)
      return -1;

   for (i = 0; i < numFiles; i++)
   {
      *temp = (Tar*)calloc(1, sizeof(Tar));

      if (get_tar_data(*temp, files[i], verb) == -1)
         return -1;

      if (i != 0)
         *archive = *temp;

      (*temp)->beginning = *offset;

      if ((*temp)->type == DIRECT)
      {
         length = strlen((*temp)->name);
         parent_dir = (char*)calloc(length + 1, sizeof(char));
         strncpy(parent_dir, (*temp)->name, length);

         if (length < 99 && (*temp)->name[length - 1] != '/')
         {
            (*temp)->name[length] = '/';
            (*temp)->name[length + 1] = '\0';
            calculate_checksum(*temp);
         }

         VERBOSE(stdout, "%s\n", (*temp)->name);
         syswrite(fd, (*temp)->block, 512);

         if ((dir = opendir(parent_dir)) == NULL)
         {
            ERROR
         }
         
         while ((d_ent = readdir(dir)))
         {
            len2 = strlen(d_ent->d_name);
            if (strncmp(d_ent->d_name, ".", len2) != 0 &&
               strncmp(d_ent->d_name, "..", len2) != 0)
            {
               path_name = (char*)calloc(length + len2 + 2, sizeof(char));
               sprintf(path_name, "%s/%s", parent_dir, d_ent->d_name);

               if (write_entries(fd, &((*temp)->next), head, 1, &path_name,
                  offset, verb) == -1)
                  return -1;

               while ((*temp)->next != NULL)
                  temp = &((*temp)->next);

               free(path_name);
            }
         }
         free(parent_dir);
         closedir(dir);
         return 0;
      }
      
      /* this will only execute if the passed entry is not a directory */
      VERBOSE(stdout, "%s\n", (*temp)->name);

      if ((*temp)->type == REG || (*temp)->type == NORM || 
         (*temp)->type == CONT || (*temp)->type == SYMLINK)
      {
         /* if the file was already put into the archive */
         search = exists_in_tar(*head, files[i], 1);
         seen = (search != (*temp));
         syswrite(fd, (*temp)->block, 512);
         if (seen)
         {
            (*temp)->type = HARDLINK;
            strncpy((*temp)->link_name, (*temp)->name, 100);
            strncpy((*temp)->size, "00000000000", 11);
            calculate_checksum(*temp);
         }
      }

      if ((*temp)->type == REG || (*temp)->type == NORM || 
         (*temp)->type == CONT)
      {
         /* archive the file if it hasn't already been added */
         if (!seen)
         {
            newfd = open((*temp)->name, O_RDONLY);

            while ((bytes = sysread(newfd, buf, 512)) > 0)
               syswrite(fd, buf, bytes);

            close(newfd);
         }
      }

      /* check to see if zeros need to be added for padding */
      size = otoui((*temp)->size, 11);
      padding = 512 - (size % 512);

      if (padding != 512)
      {
         for (i = 0; i < padding; i++)
            syswrite(fd, "\0", 1);

         *offset += padding;
      }
      *offset += size;
      temp = &((*temp)->next);
   }

   /* add space for size of metadata */
   *offset += 512;
   return 0;
}

int extract_tar(int fd, Tar* archive, size_t numFiles, char** files, int verb)
{
   int i, result = 0;
   
   /* numFiles will be non-zero if there were specified files to be extracted */
   if (numFiles != 0)
   {
      if (files == NULL)
         VERBOSE(stderr, "non-zero file count with empty file array%s", "\n");
      
      while (archive != NULL)
      {
         for (i = 0; i < numFiles; i++)
         {
            if (strncmp(archive->name, files[i], 
               MAX(strlen(archive->name), strlen(files[i]))) == 0)
            {
               if (lseek(fd, archive->beginning, SEEK_SET) == -1)
               {
                  ERROR
               }
               
               if (extract_entry(fd, archive, verb) == -1)
                  result = -1;
            }
         }
         archive = archive->next;
      }
      return result;
   }

   /* the following will only execute if there were no specified files */
   if (lseek(fd, 0, SEEK_SET) == -1)
   {
      ERROR
   }

   while (archive != NULL)
   {
      if (extract_entry(fd, archive, verb) == -1)
         result = -1;
      archive = archive->next;
   }

   return result;
}

int extract_entry(int fd, Tar* ent, int verb)
{
   unsigned int size, bytes_read;
   int newfd, length, offset = 0;
   char* path_name, buf[512];

   VERBOSE(stdout, "%s\n", ent->name);

   if (ent->type == REG || ent->type == NORM || ent->type == CONT)
   {
      if ((length = strlen(ent->name)) == 0)
         VERBOSE(stderr, "trying to extract file with empty name%s", "\n");

      path_name = (char*)calloc(length + 1, sizeof(char));
      strncpy(path_name, ent->name, length);

      /* decrement length until the first slash is found to remove file */
      while (--length > 0 && path_name[length] != '/');
      path_name[length] = '\0';
      
      if (my_mkdir(path_name, DEFAULT, verb) == -1)
      {
         VERBOSE(stderr, "could not make directory %s\n", path_name);
         free(path_name);
         return -1;
      }
      free(path_name);
      
      /* create file and write it */
      size = otoui(ent->size, 11);
      
      /* open new file for writing with the old permissions */
      newfd = sysopen(ent->name, O_WRONLY|O_CREAT|O_TRUNC, 
         otoui(ent->mode, 7) & 0777);

      if (lseek(fd, 512 + ent->beginning, SEEK_SET) == -1)
      {
         ERROR
      }

      while (offset < size)
      {
         bytes_read = sysread(fd, buf, 512);
         if (syswrite(newfd, buf, bytes_read) != bytes_read)
         {
            ERROR
         }
         offset += bytes_read;
      }
      close(newfd);
      return 0;
   }
   if (ent->type == CHAR)
   {
      if (mknod(ent->name, otoui(ent->mode, 7) | S_IFCHR, 
         (otoui(ent->major_num, 7) << 20) | otoui(ent->minor_num, 7)) == -1)
      {
         ERROR
      }
      return 0;
   }
   if (ent->type == BLOCK)
   {
      if (mknod(ent->name, otoui(ent->mode, 7) | S_IFBLK,
         (otoui(ent->major_num, 7) << 20) | otoui(ent->minor_num, 7)) == -1)
      {
         ERROR
      }
      return 0;
   }
   if (ent->type == HARDLINK)
   {
      if (link(ent->link_name, ent->name) == -1)
      {
         ERROR
      }
      return 0;
   }
   if (ent->type == SYMLINK)
   {
      if (symlink(ent->link_name, ent->name) == -1)
      {
         ERROR
      }
      return 0;
   }
   if (ent->type == DIRECT)
   {
      if (my_mkdir(ent->name, otoui(ent->mode, 7) & 0777, verb) == -1)
      {
         ERROR
      }
      return 0;
   }
   if (ent->type == FIFO)
   {
      if (mkfifo(ent->name, otoui(ent->mode, 7) & 0777) == -1)
      {
         ERROR
      }
      return 0;
   }
   return 0;
}

Tar* exists_in_tar(Tar* archive, char* file_name, char original)
{
   char* comp;

   while (archive)
   {
      comp = (original == 0) ? archive->name : archive->file_name;
      if (strncmp(comp, file_name, MAX(strlen(comp), strlen(file_name) + 1)) == 0)
         return archive;
      archive = archive->next;
   }
   return NULL;
}

int find_match(Tar* entry, size_t numFiles, char* bad, char** files)
{
   size_t i;

   if (!entry)
      return -1;

   if (numFiles == 0)
      return 0;

   if (numFiles != 0 && files == NULL)
      return -1;

   for (i = 0; i < numFiles; i++)
   {
      if (!bad[i])
         if (!strncmp(entry->name, files[i], MAX(strlen(entry->name), 
            strlen(files[i])) + 1))
            return i + 1;
   }
   return 0;
}

int my_mkdir(char* directory, int mode, int verb)
{
   int length;
   char* path_name, *c;

   length = strlen(directory);
   path_name = (char*)calloc(length + 1, sizeof(char));

   strncpy(path_name, directory, length);

   /* clear the last slash in the path */
   if (path_name[length - 1] == '/')
      path_name[length - 1] = 0;

   for (c = path_name + 1; *c; c++)
   {
      /* once we find the first end of path name create directory */
      if (*c == '/')
      {
         *c = '\0';

         if (mkdir(path_name, mode ? mode : (DEFAULT)) == -1 && errno != EEXIST)
         {
            ERROR
         }
         *c = '/';
      }
   }

   if (mkdir(path_name, mode ? mode : (DEFAULT)) == -1 && errno != EEXIST)
   {
      ERROR
   }

   free(path_name);
   return 0;
}

void peek_entry(Tar* ent)
{
   time_t time = otoui(ent->mtime, 12);
   printf("File: %s\n", ent->name);   
   printf("Mode: %03o\n", otoui(ent->mode, 8));
   printf("DEBUG: user id: %d\n", geteuid());
   printf("DEBUG: group id: %d\n", getgid());
   printf("Owner UID: %u\n", otoui(ent->uid, 8));
   printf("Owner GID: %u\n", otoui(ent->gid, 8));
   printf("File Size: %d\n", otoui(ent->size, 12));
   printf("Time     : %s", asctime(gmtime(&time)));
   printf("Checksum : %s\n", ent->chksum);
   printf("Type     : ");

   switch(ent->type)
   {
      case REG: case NORM:
         printf("Normal");
         break;
      case HARDLINK:
         printf("Hard Link");
         break;
      case SYMLINK:
         printf("Symbolic Link");
         break;
      case CHAR:
         printf("Character Special");
         break;
      case BLOCK:
         printf("Block Special");
         break;
      case DIRECT:
         printf("Directory");
         break;
      case FIFO:
         printf("FIFO");
         break;
      case CONT:
         printf("Contiguous File");
         break;
      default:
         break;
   }

   printf(" (%c)\n", ent->type ? ent->type : '0');
   printf("Link Name: %s\n", ent->link_name);
   printf("Ustar    : %c%c%c%c%c\\%2x\\%2x\\%02x\n", ent->ustar[0], ent->ustar[1],
      ent->ustar[2], ent->ustar[3], ent->ustar[4], ent->ustar[5], ent->ustar[6],
      ent->ustar[7]);
   printf("Username : %s\n", ent->owner_name);
   printf("Group    : %s\n", ent->group_name);
   printf("Major    : %s\n", ent->major_num);
   printf("Minor    : %s\n", ent->minor_num);
   printf("Prefix   : %s\n", ent->prefix);
   printf("\n");
}

void peek_tar_data(Tar* archive)
{
   while (archive)
   {
      peek_entry(archive);
      archive = archive->next;
   }
}
