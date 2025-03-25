#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"

uint64 sys_mutex(void)
{
    struct file *f = 0;
    mutexalloc(&f);
    if (f == 0)
        return -1;

    int fd = fdalloc(f);
    if (fd < 0) {
        fileclose(f);
        return -1;
    }

    return fd;
}

uint64 sys_mutex_lock(void)
 {
     int fd;
     argint(0, &fd);
     if (fd < 0)
         return -1;
     struct file *f;
     f = myproc()->ofile[fd];
     if (f == 0 || f->type != FD_MUTEX || f->mutex == 0)
        return -1;
     acquiresleep(f->mutex);
     return 0;
 }

uint64 sys_mutex_unlock(void)
 {
     int fd;
     argint(0, &fd);
     if (fd < 0)
         return -1;
     struct file *f;
     f = myproc()->ofile[fd];
     if (f == 0 || f->type != FD_MUTEX)
        return -1;
     if (f->mutex->pid != myproc()->pid || !holdingsleep(f->mutex))
        return -1;
     releasesleep(f->mutex);
     return 0;
}
