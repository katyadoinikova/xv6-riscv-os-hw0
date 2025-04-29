#include "types.h"
#include "memlayout.h"
#include "riscv.h"

#define RTCREG(addr) (*(volatile uint32 *)(addr))

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
    uint32 low, high;

    low = rtc_read_low();
    high = rtc_read_high();

    return ((uint64)high << 32) | low;
}
