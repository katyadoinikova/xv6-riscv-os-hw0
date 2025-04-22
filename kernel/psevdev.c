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

#include "psevdev.h"

static uint64 count = 0;
static uint64 urandom_seed = 1;
struct spinlock lock;

static uint8 
random_byte(void) 
{
  acquire(&lock);
  urandom_seed = urandom_seed * 1664525 + 1013904223;
  release(&lock);
  return (urandom_seed >> 56);
}

int
psevdev_read(short minor, int user_dst, uint64 dst, int n)
{
  if (n < 0) {
    return -1;
}
  switch(minor) {
    case NULLDEV:
      return 0;
    
    case ZERODEV: {
      char zero = 0;
      for(int i = 0; i < n; i++) {
        if(either_copyout(user_dst, dst + i, &zero, 1) < 0)
          return -1;
      }
      return n;
    }
    
    case URANDOM: {
      uint8 byte;
      for(int i = 0; i < n; i++) {
        byte = random_byte();
        if(either_copyout(user_dst, dst + i, &byte, 1) < 0){
            return -1;
        }
      }
      return n;
    }
    
    case NULLSTAT:
      if (n != sizeof(uint64))
        return -1;
      acquire(&lock);
      uint64 val = count;
      release(&lock);
      if (either_copyout(user_dst, dst, &val, sizeof(val)) < 0)
        return -1;
      return sizeof(uint64);
    
    default:
      return -1;
  }
}

int
psevdev_write(short minor, int user_src, uint64 src, int n)
{
  uint64 seed;
  switch(minor) {
    case NULLDEV:
      return n;
    
    case ZERODEV:
      return -1;
    
    case URANDOM:
      if (n != sizeof(uint64))
        return -1;
      if (either_copyin(&seed, user_src, src, sizeof(uint64)) < 0)
        return -1;
      acquire(&lock);
      urandom_seed = seed;
      release(&lock);
      return sizeof(uint64);
    
    case NULLSTAT:
      acquire(&lock);
      count += n;
      release(&lock);
      return n;
    
    default:
      return -1;
  }
}



void psevdev_init(void) {
    initlock(&lock, "ldev_lock");
    devsw[PSEV_DEV].read = psevdev_read;
    devsw[PSEV_DEV].write = psevdev_write;
}