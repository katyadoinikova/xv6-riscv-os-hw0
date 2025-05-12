#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "rtc.h"

struct spinlock lock;

#define RTCREG(addr) (*(volatile uint32 *)(addr))

void init_rtc(void){
    initlock(&lock, "rtc_lock");
}

uint32
rtc_read_low(void)
{
    return RTCREG(RTC_LOW);
}

uint32
rtc_read_high(void)
{
    return RTCREG(RTC_HIGH);
}

uint64
rtc_read(void)
{
    acquire(&lock);
    uint32 low, high;

    low = rtc_read_low();
    high = rtc_read_high();

    uint64 t = ((uint64)high << 32) | low;
    release(&lock);
    return t;
}
