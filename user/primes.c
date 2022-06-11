#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
   int p[2][2];
   int i, ind = 0, prime, r;
   if(pipe(p[ind]) < 0) {
      fprintf(2, "primes: pipe error\n");
      exit(1);
   }
   for(i=2; i<35; i++) {
      if(write(p[ind][1], &i, 4) != 4) {
         fprintf(2, "primes: pipe write error\n");
         exit(1);
      }
   }
   close(p[0][1]);
   while(fork() == 0) {
      close(p[ind][1]);
      if(read(p[ind][0], &prime, 4) == 4) {
         printf("prime %d\n", prime);
      } else {
         exit(0);
      }
      if(pipe(p[ind ^ 1]) < 0) {
         fprintf(2, "primes: pipe error\n");
         exit(1);
      }
      while(read(p[ind][0], &r, 4) == 4) {
         if(r % prime) {
            write(p[ind ^ 1][1], &r, 4);
         }
      }
      close(p[ind ^ 1][1]);
      ind = ind ^ 1;
   }
   wait(0);
   close(p[ind][0]);
   exit(0);
}
