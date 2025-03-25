#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_args(int argc, char *argv[], int is_with_mutex, int m) {
  char cur_symbol[2];
  cur_symbol[1] = '\0';
  if (is_with_mutex > 0){
    for (int i = 0; i < argc; i++){

            if (mutex_lock(m) < 0) {
              fprintf(2, "Error: failed to lock mutex");
              exit(1);
            }
            for (int j = 0; argv[i][j] != '\0'; j++){
              cur_symbol[0] = argv[i][j];
              printf("%d: arg %d, char '%s'\n", getpid(), i, cur_symbol);
            }
            if (mutex_unlock(m) < 0) {
              fprintf(2, "Error: failed to unlock mutex");
              exit(1);
            }
        }
    
  } else {
    for (int i = 0; i < argc; i++)
        for (int j = 0; argv[i][j] != '\0'; j++){
            cur_symbol[0] = argv[i][j];
            printf("%d: arg %d, char '%s'\n", getpid(), i, cur_symbol);
     }
  }
}

int main(int argc, char *argv[]) {
    int m = mutex();
    if (mutex < 0) {
        fprintf(2, "Error: failed to create mutex");
        exit(1);
    }

   printf("Print without mutex\n");
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Error: failed to create fork");
        exit(1);
    }
    print_args(argc, argv, 0, 0);
    if (pid == 0)
         exit(0);
    else
         wait(0);

    printf("Print with mutex\n");
    pid = fork();
    if (pid < 0) {
        fprintf(2, "Error: failed to create fork");
        exit(1);
    }
    print_args(argc, argv, 1, m);
    if (pid == 0)
         exit(0);
    else
         wait(0);


    close(m);
    exit(0);
}

