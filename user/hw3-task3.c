#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, void* argv){
   int m = mutex();
   if (m < 0){
     fprintf(2, "error: mutex isn't created");
     exit(1);
   }
   printf("test1\n");
   char buffer[1];
   int res = read(m, buffer, sizeof(buffer));
   if (res < 0)
       printf("Test 1 for read - passed \n");
   if (res >= 0)
       printf("Test 1 for read - failed \n");
   res = write(m, buffer, sizeof(buffer));
   if (res < 0)
       printf("Test 1 for write - passed \n");
   if (res >= 0)
       printf("Test 1 for write - failed \n");
   close(m);

   printf("\n");

   m = mutex();
   if (m < 0){
     fprintf(2, "error: mutex isn't created");
     exit(1);
   }
   printf("test2\n");
   if (mutex_lock(m) < 0) {
      fprintf(2, "error: mutex_lock failed");
      exit(1);
   }
   if (close(m) < 0)
       printf("Test 2 a) - failed \n");
   else
       printf("Test 2 a) - passed \n");
   int pid = fork();
   if (pid < 0){
      fprintf(2, "error: fork failed");
      exit(1);
   }
   if (pid == 0){
      if (close(m) < 0)
        printf("Test 2 b) - failed \n");
   else
       printf("Test 2 b) - passed \n");
   }
   else {
      sleep(15);
        if (mutex_unlock(m) < 0) {
            fprintf(2, "error: mutex_unlocked failed");
            exit(1);
        }
        if (close(m) < 0) {
            printf("mutex close failed in parent");
        }
        wait(0);
   }
   close(m);

   printf("\n");

   m = mutex();
   if (m < 0){
     fprintf(2, "error: mutex isn't created");
     exit(1);
   }
   printf("test3\n");
   pid = fork();
   if (pid < 0){
      fprintf(2, "error: fork failed");
      exit(1);
   }
   if (pid == 0){
     int m_child = mutex();
     if (m_child < 0){
       fprintf(2, "error: mutex isn't created in child");
       exit(1);
     }
     if (mutex_lock(m_child) < 0) {
       fprintf(2, "error: mutex_lock in child failed");
       exit(1);
    }
    printf("child exit with not closed locked mutex \n");
    exit(0);
  }
  else{
     wait(0);
     printf("Above messages are about closing mutex with exit \n");
  }

   close(m);

   printf("\n");

   m = mutex();
   if (m < 0){
     fprintf(2, "error: mutex isn't created");
     exit(1);
   }
   printf("test4\n");
   if (mutex_lock(m) < 0) {
      fprintf(2, "error: mutex_lock failed");
      exit(1);
   }
   pid = fork();
   if (pid < 0){
      fprintf(2, "error: fork failed");
      exit(1);
   }
   if (pid == 0){
      printf("child tries to unlock parent's mutex \n");
      if (mutex_unlock(m) < 0)
           printf("Test 4 - passed - child didn't succed\n");
      else
           printf("Test 4 - failed \n");
         exit(0);
   }
   else {
        sleep(30);
        wait(0);
   }
   exit(0);
}
