#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_LEN 128

int smart_write(int fd, char* str, int len){
     char *ptr = str;
     while (len > 0) {
       int ret = write (fd, str, len);
       if (ret < 0) return -1;
       len -= ret;
       ptr += ret;
     }
     return 0;
}


int main(int argc, char *argv[]) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(2, "Error: pipe isn't created");
        exit(2);
    }

    int pid = fork();
    switch(pid){
      case -1:
         fprintf(2, "Error: fork isn't created");
         close(pipefd[0]);
         close(pipefd[1]);
         exit(2);
      case 0:
         close(pipefd[1]);
         close(0);
         dup(pipefd[0]);
         close(pipefd[0]);
         char *arg[] = {"/wc", 0};
         exec("/wc", arg);
         fprintf(2, "Error: exec failed");
         exit(2);
      default:
         close(pipefd[0]);
         char buf[MAX_LEN];
         int pos = 0;
         for (int i = 0; i < argc;){
             int l = strlen(argv[i]);
             if (pos + l + 1 <= MAX_LEN){
                  memmove(buf + pos, argv[i], l);
                  pos += l;
                  buf[pos] = '\n';
                  pos++;
                  i++;
             }
             else if (pos == 0){
                  int ret = smart_write (pipefd[1], argv[i], l);
                  if (ret < 0){
                    fprintf(2, "Error: write error");
                    close(pipefd[1]);
                    exit(2);
                  }
                  buf[pos] = '\n';
                  pos++;
                  i++;
             }
             else{
                 int ret = smart_write(pipefd[1], buf, pos);
                 if (ret < 0){
                    fprintf(2, "Error: write error");
                    close(pipefd[1]);
                    exit(2);
                 }
                 memset(buf, 0, pos);
                 pos = 0;
             }
         }
         if (pos != 0){
             int ret = smart_write(pipefd[1], buf, pos);
             if (ret < 0){
                 fprintf(2, "Error: write error");
                 close(pipefd[1]);
                 exit(2);
             }
         }
         if (close(pipefd[1]) == -1){
            fprintf(2, "Error: pipefd[1] isn't closed - writted data isb't saved");
            exit(2);
         }
         wait(0);
         exit(0);
    }
}
