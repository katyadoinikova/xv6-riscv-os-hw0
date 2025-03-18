#ifndef PROCINFO_H
#define PROCINFO_H

#include "types.h"

enum procstate_info {
    PROC_SLEEPING,
    PROC_RUNNABLE,
    PROC_RUNNING,
    PROC_ZOMBIE
};


struct procinfo {
    int pid;
    char name[16];
    enum procstate_info state;
    int ppid;
    char pname[16];
};

#endif // PROCINFO_H
