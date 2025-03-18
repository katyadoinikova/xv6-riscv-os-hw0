#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/procinfo.h"
#include "user/user.h"

#define MAX_PROC 64

int
main(int argc, char *argv[]) {
   int buf_size = MAX_PROC;
   struct procinfo *plist = malloc(buf_size * sizeof(struct procinfo));
   if (plist == 0) {
        fprintf(2, "Error: memory allocation failed\n");
        exit(1);
    }
   int res = ps_listinfo(plist, buf_size);

   while (res == -2)
   {
      buf_size *= 2;
      free(plist);
      plist = malloc(buf_size * sizeof(struct procinfo));
      if (plist == 0) {
        fprintf(2, "Error: memory allocation failed\n");
        free(plist);
        exit(1);
      }
      res = ps_listinfo(plist, buf_size);
   }
   if (res == -4){
       fprintf(2, "Error: not correct state");
       free(plist);
       exit(2);
   }
   if (res == -3){
       fprintf(2, "Error: couldn't write in user space");
       free(plist);
       exit(2);
   }


for (int i = 0; i < res; i++) {
    printf("pid: %d\n", plist[i].pid);
    printf("name: %s\n", plist[i].name);
    printf("state: ");
    if (plist[i].state == PROC_SLEEPING)
          printf("sleeping; ");
    if (plist[i].state == PROC_RUNNABLE)
          printf("runnable; ");
    if (plist[i].state == PROC_RUNNING)
          printf("running; ");
    if (plist[i].state == PROC_ZOMBIE)
          printf("zombie; ");

    printf("\nppid: %d\n", plist[i].ppid);
    printf("pname: %s\n", plist[i].pname);
    printf("\n");
}
    printf("Number of processes = %d\n", res);
    free(plist);

    exit(0);
}
