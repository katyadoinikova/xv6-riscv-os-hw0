#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "elf.h"
#include "fs.h"
#include "hw4-help-func.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_print_pagetable(void)
{
  uint64 buf; int len, flags;
  argaddr(0, &buf);
  argint(1, &len);
  argint(2, &flags);

  if (flags < 0 || flags > 3) return -1;

  struct proc *p = myproc();
  pagetable_t pages = p->pagetable;
  printf("PAGETABLE 0x%p\n", pages);
  //printf("buffer: %ld\n", buf);
  /*if (buf != 0 && len > 0) {
        char temp;
        if (copyin(p->pagetable, (char *)&temp, buf, 1) < 0 ||
            copyin(p->pagetable, (char *)&temp, buf + len - 1, 1) < 0) {
            printf("Buffer isn't in address space");
            return -1;
        }
    }*/
  visit_pages_and_print(2, pages, buf, len, flags);


  return 0;
}


uint64
sys_remove_flags(void)
{
  uint64 buf; int len, flags;
  argaddr(0, &buf);
  argint(1, &len);
  argint(2, &flags);
  if (flags < 0 || flags > 3) return -1;
  struct proc *p = myproc();
  pagetable_t pages = p->pagetable;
  visit_pages_and_remove_flags(2, pages, buf, len, flags);
  return 0;
}
