#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LEN 65536

int smart_write(int fd, const void* buf, int len) {
    const char *ptr = (const char*) buf;
    while (len > 0) {
        ssize_t ret = write(fd, ptr, len);
        if (ret < 0) return -1;
        len -= ret;
        ptr += ret;
    }
    return 0;
}



int main(int argc, char *argv[]) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error: pipe isn't created");
        exit(2);
    }

    int pid = fork();
    switch(pid){
      case -1:
         perror("Error: fork isn't created");
         exit(2);
      case 0:
         if (close(pipefd[1]) == -1){
            perror("Error: pipefd[1] isn't closed");
            exit(2);
         }
         char buf1[MAX_LEN];
         ssize_t bytes_read;
         while ((bytes_read = read(pipefd[0], buf1, MAX_LEN - 1)) > 0) {
             buf1[bytes_read] = '\0';
             printf("%s", buf1);
         }
         if (bytes_read < 0) {
             perror("Error: read error");
             close(pipefd[0]);
             exit(2);
         }
         if (close(pipefd[0]) == -1){
            perror("Error: pipefd[0] isn't closed");
            exit(2);
         }
         exit(0);
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
                    perror("Error: write error");
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
                    perror("Error: write error");
                    close(pipefd[1]);
                    exit(2);
                 }
                 pos = 0;
             }
         }
         if (pos != 0){
             int ret = smart_write(pipefd[1], buf, pos);
             if (ret < 0){
                 perror("Error: write error");
                 close(pipefd[1]);
                 exit(2);
             }
         }
         if (close(pipefd[1]) == -1){
            perror("Error: pipefd[1] isn't closed - writted data isn't saved");
            exit(2);
         }
         if (wait(0) < 0){
            perror("Error: no child");
            exit(2);
         }
         exit(0);
    }
}
