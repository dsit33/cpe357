#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "mylib.h"

#define CURRENT "."
#define PARENT ".."
#define PATH_MAX 2048

int mygetcwd(char* cwd)
{
   DIR* parent_dir;
   struct dirent* file;
   struct stat st;
   char path[PATH_MAX], holder[PATH_MAX];
   dev_t cur_dev;
   ino_t cur_ino;

   while (1)
   {
      /*stat current directory*/
      if (stat(CURRENT, &st) == -1)
         return 0;
      /*save ino and device numbers to check for root later*/
      cur_ino = st.st_ino;
      cur_dev = st.st_dev;

      /*open and stat the parent directory*/
      if ((parent_dir = opendir(PARENT)) == NULL)
         return 0;

      if (stat(PARENT, &st) == -1)
         return 0;
      
      /*if parent ino and dev numbers are the same, we are at root*/
      if (cur_ino == st.st_ino && cur_dev == st.st_dev)
         break;

      /*iterate all files in parent directory*/
      while ((file = readdir(parent_dir)) != NULL)
      {
         sprintf(path, "../%s", file->d_name);

         /*find the name of our current directory from the parent*/
         if (stat(path, &st) == -1)
            return 0;
         
         /*returns true if we have found our current directory*/
         if (cur_ino == st.st_ino && cur_dev == st.st_dev)
         {
            /*build path*/
            strcpy(holder, cwd);
            memset(cwd, 0, PATH_MAX);
            strcat(cwd, "/");
            strcat(cwd, file->d_name);
            strcat(cwd, holder);
            break;
         }
      }

      /*change directory to parent and repeat process*/
      if (chdir(PARENT) == -1)
         return 0;
      if (closedir(parent_dir) == -1)
         return 0;
   }

   return 1;
}

int main()
{
   char buf[PATH_MAX];
   char cwd[PATH_MAX] = {0};

   if (mygetcwd(cwd))
      printf("%s\n", cwd);
   else
      errorAndExit("mypwd");
   getcwd(buf, PATH_MAX);
   printf("getcwd: %s\n", buf);

   return 0;
}
