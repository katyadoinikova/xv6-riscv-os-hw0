#ifndef HW4_HELP_FUNC_H
#define HW4_HELP_FUNC_H

#include "types.h"
#include "riscv.h"

void visit_pages_and_print(int level, pagetable_t pages, uint64 buf, int len, int flags);
void visit_pages_and_remove_flags(int level, pagetable_t pages, uint64 buf, int len, int flags);
#endif // HW4_HELP_FUNC_H

