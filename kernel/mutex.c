#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

int
mutexalloc(struct file **f)
{
  printf("Proc %d - called mutexalloc \n", myproc()->pid);
  struct sleeplock *mutex;

  mutex = 0;
  *f = 0;
  
  if((*f = filealloc()) == 0) {
    printf("Proc %d - failed in filealloc \n", myproc()->pid);
    return -1;
  }
  printf("Proc %d - succeded in filealloc\n", myproc()->pid);
  if((mutex = (struct sleeplock*)kalloc()) == 0) {
    fileclose(*f);
    printf("Proc %d - failed in kalloc \n", myproc()->pid);
    return -1;
  }
  printf("Proc %d - succeded in kalloc \n", myproc()->pid);
  initsleeplock(mutex, "m");

  (*f)->type = FD_MUTEX;
  (*f)->mutex = mutex;
  printf("Proc %d - mutexalloc succeded \n", myproc()->pid);
  return 0;
}


void
mutexclose(struct file *f)
{
  printf("Proc %d - called mutexclose \n", myproc()->pid);
  if (f == 0 || f->type != FD_MUTEX){
    printf("Proc %d - tried to close not mutex or null \n", myproc()->pid);
    return;
  }
  kfree((char *)f->mutex);
  f->mutex = 0;
  printf("Proc %d - mutexclose succeded \n", myproc()->pid);

}
