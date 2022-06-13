#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define MAXSIZE 512


static char *cmdLine[MAXARG];
static char ip[MAXARG];

static int cmdLineInd = 0;

void execute() {
   cmdLine[cmdLineInd] = 0;
   int r = fork();
   if(r < 0) {
      fprintf(2, "xargs: error in fork\n");
      exit(1);
   }
   if(r == 0) {
      int r = exec(cmdLine[0], cmdLine);
      fprintf(2, "xargs: error in exec %d\n", r);
      exit(1);
   } else {
      wait(0);
   }
}

void processinput() {
   int cmdLineIndOld = cmdLineInd;
   cmdLine[cmdLineInd++] = ip;
   execute();
   cmdLineInd = cmdLineIndOld;
}

void readinput() {
   int r = 0, i=0;
   char c;
   memset(ip, '\0', MAXARG);
   while(1) {
      r = read(0, &c, 1);
      if(r == 0) {
         exit(0);
      }
      if(r < 0) {
         fprintf(2, "xargs: error in reading input\n");
         exit(1);
      }
      if(c == '\n') {
         processinput(ip);
         break ;
      } else {
         ip[i++] = c;
      }
   }
}

void
prepaircmdline(int argc, char *argv[]) {
   for(cmdLineInd=0; cmdLineInd<argc; cmdLineInd++) {
      cmdLine[cmdLineInd] = argv[cmdLineInd];
   }
}

int
main(int argc, char *argv[])
{
   if(argc < 2) {
      fprintf(2, "usage: xargs cmd [params...]\n");
      exit(0);
   }
   prepaircmdline(argc-1, &argv[1]);
   while(1) { readinput(); }
   exit(0);
}
