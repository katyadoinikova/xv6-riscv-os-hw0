#include "kernel/types.h"
 #include "kernel/fcntl.h"
 #include "user/user.h"

 static char digits[] = "0123456789abcdef";
 
 int
 main(int argc, char *argv[])
 {
   if(argc != 3){ 
     fprintf(2,"Error: Not correct num of arguments\n"); 
     exit(1); 
   }
   int fd = open(argv[1], O_RDONLY);
   int n = atoi(argv[2]); 
 
   if (fd < 0){ 
     fprintf(2,"Error^ couldn't open file\n"); 
     exit(1); 
   }
   unsigned char buf[16];
   int r = read(fd, buf, n  > 16 ? 16 : n);
   int readed_bytes = 0;
   while (r > 0 && readed_bytes < n){
     readed_bytes += r;
     for (int i = 0; i < r; i++){
       char readed[3];
       readed[0] = digits[buf[i]>>4];
       readed[1] = digits[buf[i]&0xf];
       readed[2] = ' ';
       write(1, readed, 3);
     }
     r = read(fd, buf, n - readed_bytes  > 16 ? 16 : n - readed_bytes);
   }
   write(1,"\n",1);
   close(fd);
   exit(0);
 }