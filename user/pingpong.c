#include "kernel/types.h"
#include "user/user.h"

int
main()
{
   int p[2];   // 0: read  1: write
   char c = 'a';
   int r = 0;
   if(pipe(p) < 0) {
      fprintf(2, "pingpong: pipe error\n");
      exit(1);
   }
   int pid = fork();
   if(pid < 0) {
      fprintf(2, "pingpong: fork error\n");
      exit(1);
   }

   if(pid == 0) {
      r = read(p[0], &c, 1);
      if(r <= 0) {
         fprintf(2, "pingpong: child read error\n");
         exit(1);
      }
      printf("%d: received ping\n", getpid());
      r = write(p[1], &c, 1);
      if(r <= 0) {
         fprintf(2, "pingpong: child write error\n");
         exit(1);
      }
   } else {
      r = write(p[1], &c, 1);
      if(r <= 0) {
         fprintf(2, "pingpong: parent write error\n");
         exit(1);
      }
      wait(0);
      r = read(p[0], &c, 1);
      if(r <= 0) {
         fprintf(2, "pingpong: parent read error\n");
         exit(1);
      }
      printf("%d: received pong\n", getpid());
   }
   close(p[0]);
   close(p[1]);
   exit(0);
}
