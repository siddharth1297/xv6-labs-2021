#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define BUFFSIZE 512

char*
fmtname(char *path)
{
  char *p;
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void
find(char *path, char *name)
{
  char *p = path + strlen(path);
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch (st.type) {
  case T_FILE:
     if(strcmp(fmtname(path), name) == 0) {
        printf("%s\n", path);
     }
     break;
  case T_DIR:
      if(strlen(path) + 1 + DIRSIZ > BUFFSIZE){
         fprintf(2, "find: path too long\n");
         break;
       }
      *p++ = '/';

      while(read(fd, &de, sizeof(de)) == sizeof(de)){
         if(de.inum == 0) { break; }
         if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
            strcpy(p, de.name);
            find(path, name);
            *p = 0;
         }
      }
      // optional
      //*--p = 0;
     break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
   if(argc < 3) {
      fprintf(2, "usage: find directory file\n");
      exit(0);
   }
   find(argv[1], argv[2]);
   exit(0);
}
