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
  *f = filealloc();
  if(!(*f)) {
    printf("Proc %d - failed in filealloc \n", myproc()->pid);
    return -1;
  }
  printf("Proc %d - succeded in filealloc, file = %p\n", myproc()->pid, *f);
  if((mutex = (struct sleeplock*)kalloc()) == 0) {
    fileclose(*f);
    printf("Proc %d - failed in kalloc, file = %p \n", myproc()->pid, *f);
    return -1;
  }
  printf("Proc %d - succeeded in kalloc, mutex at %p, file = %p\n", myproc()->pid, mutex, *f);
  initsleeplock(mutex, "m");

  (*f)->type = FD_MUTEX;
  (*f)->mutex = mutex;
  (*f)->readable = 0;
  (*f)->writable = 0;
  printf("Proc %d - mutexalloc succeeded for mutex = %p, file = %p \n", myproc()->pid, mutex, *f);
  return 0;
}


void
mutexclose(struct file *f)
{
  printf("Proc %d - called mutexclose, file = %p \n", myproc()->pid, f);
  if (f == 0 || f->type != FD_MUTEX){
    printf("Proc %d - tried to close not mutex or null, file = %p \n", myproc()->pid, f);
    return;
  }
  printf("Proc %d - unlocked mutex 0x%p, file = %p\n", myproc()->pid, f->mutex, f);
  kfree((char *)f->mutex);
  f->mutex = 0;
  printf("Proc %d - mutexclose succeded, file = %p \n", myproc()->pid, f);

}
