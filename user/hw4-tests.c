#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int global_var = 100;
int
main(int argc, char *argv[])
{
  printf("Pages at start, with global var \n");
  if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}
  printf("\n\n\n");
  
  int stack_var = 5;
  printf("Pages with stack var %d \n", stack_var);
  if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}
  printf("\n\n\n");
  
   int arr1[10000];
   arr1[0] = 5;
   printf("Pages with stack array, first elem: %d\n", arr1[0]);
   if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");

   char *arr2 = sbrk(2*4096);
   if ((uint64)arr2 == -1) {
       printf("error: sbrk failed");
       exit(1);
    }
 
   memset(arr2, 1, 2*4096);
   printf("Pages with heap array, size: %d\n", 2*4096);
   if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");

   printf("Pages from heap\n");
   if (print_pagetable((uint64*)(arr2) + 1, 4096, 0) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");

   printf("Delete A flags from heap \n");
   if (remove_flags((uint64*)(arr2) + 1, 4096, 1) < 0){ printf("error in removing flags from pagetable"); exit(1);}
   if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");

   printf("Delete AD flags \n");
   if (remove_flags(0, 0, 3) < 0){ printf("error in removing flags from pagetable"); exit(1);}
   if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");


   int x1 = arr2[0];
   int x2 = global_var;
   int x3 = stack_var;
   int x4 = arr1[0];
   printf("Pages after reading vars (ouput with A flags), read vars: %d %d %d %d", x1, x2, x3, x4);
   if (print_pagetable(0U, 0, 1) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");

   global_var++;
   arr2[0]++;
   stack_var ++;
   arr1[0]++;
   arr2[5000]++;
   printf("Pages after changing vars (ouput with D flags)");
   if (print_pagetable(0U, 0, 2) < 0){ printf("error in printing pagetable"); exit(1);}
   printf("\n\n\n");

   sbrk(-2*4096);
   printf("Pages after free\n");
   if (print_pagetable(0U, 0, 0) < 0){ printf("error in printing pagetable"); exit(1);}

   exit(0);
}
