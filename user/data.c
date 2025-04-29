#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NS_PER_SEC 1000000000
#define NS_PER_MILISEC 1000000
#define SEC_PER_MIN 60
#define MIN_PER_HOUR 60
#define HOUR_PER_DAY 24
#define DAYS_PER_YEAR 365
#define EPOCH_YEAR 1970

static int days_in_month[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int is_leap_year (int year){
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    if (year % 4 == 0) return 1;
    return 0;
}

void correct_print(int a, char c){
    char printed_char [2];
    printed_char[0] = c;
    printed_char[1] = '\0';
    printf("%s", printed_char);
    if (a < 10)
        printf("0");
    printf("%d", a);
}

int main(int argc, char *argv[]){
    uint64 rtc_nanoseconds = getrtc();
    uint64 seconds = rtc_nanoseconds / NS_PER_SEC;
    uint32 nanos = rtc_nanoseconds % NS_PER_SEC;
    
    uint32 minutes = seconds / SEC_PER_MIN;
    seconds %= SEC_PER_MIN;
    
    uint32 hours = minutes / MIN_PER_HOUR;
    minutes %= MIN_PER_HOUR;
    
    uint32 days = hours / HOUR_PER_DAY;
    hours %= HOUR_PER_DAY;

    int year = EPOCH_YEAR;
    while (1){
        if (days >= DAYS_PER_YEAR + is_leap_year(year)){
            year ++;
            days -= DAYS_PER_YEAR + is_leap_year(year);
        }
        else
            break;
    }
    int month = 0;
    int extra_day = 0;
    while (1){
        if (month == 1 && is_leap_year(year) == 1){
            extra_day = 1;
        }
        if (days >= days_in_month[month] + extra_day){
            days -= days_in_month[month] + extra_day;
            month ++;
        }
        else
            break;
        extra_day = 0;
    }

    printf("Current time: ");

    printf("%d", year);
    correct_print(month + 1, '-');
    correct_print(days + 1, '-');
    correct_print(hours, ' ');
    correct_print(minutes, ':');
    correct_print(seconds, ':');
    printf(".");

    uint32 mili_seconds = nanos / NS_PER_MILISEC;

    char mili_seconds_str[4];
    mili_seconds_str[3] = '\0';
    int i = 2;
    while (i >= 0){
        mili_seconds_str[i] = '0' + mili_seconds % 10;
        mili_seconds /= 10;
        i--;
    }
    printf("%s\n", mili_seconds_str);



}