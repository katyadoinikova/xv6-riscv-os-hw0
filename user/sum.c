#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char c; int ret_value;
  int MAX_LEN = 50;
  char buffer[MAX_LEN];
  int i;
  for(i = 0; i < MAX_LEN; i++ ){
    ret_value = read(0, &c, 1);
    if(ret_value <= 0)
      break;
    buffer[i] = c;
    if (c == '\n')
      break;
  }
  if (ret_value < 0){
    fprintf(2, "%s\n", "Reading error");
    exit(1);
  }
  if (i == 0){
    fprintf(2, "%s\n", "Nothing entered");
    exit(1);
  }
  if (i == MAX_LEN){
    fprintf(2, "Error: %s\n", "Buffer is overflowed");
    exit(1);
  }
  buffer[i] = '\0';
  printf("|%s|\n", buffer);
  
  char* buf = buffer;
  char* first = buf;
  if (*buf == ' '){
    fprintf(2, "Error: %s\n", "First number is empty");
    exit(1);
  }
  while (*buf != ' ' && *buf != '\0'){
     if ('0' > *buf || *buf > '9')
         break;
     buf++;
  }
  if (*buf == '\0'){
    fprintf(2, "Error: %s\n", "Second number is empty");
    exit(1);
  }
  if (*buf != ' '){
     fprintf(2, "Error: %s\n", "There are letters in first number");
     exit(1);
  }
  *buf = '\0';
   
  buf++;
  char* second = buf;

  if (*buf == ' ') {
     fprintf(2, "Error: %s\n", "More than 1 space");
     exit(1);
  }
  if (*buf == '\0'){
    fprintf(2, "Error: %s\n", "Second number is empty");
    exit(1);
  }
  while (*buf != '\0'){
     if ('0' > *buf || *buf > '9'){
       fprintf(2, "Error: %s\n", "There are letters in second number");
       exit(1);
     }
     buf++;
  }
  
  int a = atoi(first);
  int b = atoi(second);

  printf("Sum: %d\n", add(a, b));

  exit(0);
}
