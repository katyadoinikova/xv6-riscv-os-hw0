#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/stat.h"

int main(){
   struct procinfo small_plist[1];
   int res = ps_listinfo(small_plist, 1);
   if (res == -2)
      printf("first test passed: buffer is too small\n");
   else
      printf("first test failed\n");
    
   struct procinfo *invalid_address = (struct procinfo *)0x12345678;
   res = ps_listinfo(invalid_address, 5);
   if (res == -3)
      printf("second test passed: not correct address\n");
   else
      printf("second test failed\n");

   struct procinfo plist[NPROC];
   res = ps_listinfo(plist, NPROC);
   if (res <= 0 || res > 3)
      printf("third test failed\n");
   else
      printf("third test passed: not error for correct data\n");

   res = ps_listinfo(0, NPROC + 1);
   if (res <= 0 || res > NPROC)
      printf("forth test failed\n");
   else
      printf("forth test passed: not error for NULL\n");
   exit(0);
}
