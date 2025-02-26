#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid = fork();
    switch(pid){
    case -1:
        fprintf(2, "Error: fork isn't created");
        exit(2);
    case 0:
       sleep(80);
       exit(1);
    default:
       printf("Parent PID = %d, child PID = %d\n", getpid(), pid);
       int status;
       int p = wait(&status);
       printf("Finished process: %d, status: %d\n", p, status);

       exit(0);

    }

}
