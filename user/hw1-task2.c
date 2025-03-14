#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_LEN 128

int smart_write(int fd, const void* buf, int len) {
    const char *ptr = (const char*) buf;
    while (len > 0) {
        int ret = write(fd, ptr, len);
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
         if (close(pipefd[1]) == -1){
            fprintf(2, "Error: pipefd[1] isn't closed in child");
            exit(2);
         }
         if (close(0) == -1){
            fprintf(2, "Error: 0 isn't closed");
            exit(2);
         }
         if (dup(pipefd[0]) == -1){
            fprintf(2, "Error: pipefd[0] duplicate isn't created");
            exit(2);
         }
         if (close(pipefd[0]) == -1){
            fprintf(2, "Error: pipefd[0] isn't closed");
            exit(2);
         }
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
                  memcpy(buf + pos, argv[i], l);
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
         if (wait(0) < 0){
	    fprintf(2, "Error: no child");
            exit(2);
         }
         exit(0);
    }
}
