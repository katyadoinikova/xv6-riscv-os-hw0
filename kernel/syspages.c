#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"


void visit_pages_and_print(int level, pagetable_t pages, uint64 buf, int len, int flags){
   if (level < 0) return;
   int is_buf_empty = 0;
   uint64 size_of_block = 1LL << (9 * level + PGSHIFT);
   if (buf == 0 || len == 0) is_buf_empty = 1;
   int virt_addr_ind = 0; int finish_page = 512;
        
   while ((!is_buf_empty && len > 0) || (is_buf_empty && virt_addr_ind < finish_page)){
          int virt_addr; pte_t* pte;
          if (is_buf_empty) virt_addr = virt_addr_ind;
          else virt_addr = (PGROUNDDOWN(buf) / size_of_block) % 512;
          pte = &pages[virt_addr];
          if (pte && (*pte & PTE_V) && (level > 0 || ((flags == 0) || ((flags & A_FLAG ) && (*pte & PTE_A)) || ((flags & D_FLAG) && (*pte & PTE_D))))) {
            pagetable_t next_level_page = (pagetable_t)PTE2PA(*pte);

            char flags_str[8] = {
            (*pte & PTE_R) ? 'R' : '_',
            (*pte & PTE_W) ? 'W' : '_',
            (*pte & PTE_X) ? 'X' : '_',
            (*pte & PTE_U) ? 'U' : '_',
            (*pte & PTE_G) ? 'G' : '_',
            (*pte & PTE_A) ? 'A' : '_',
            (*pte & PTE_D) ? 'D' : '_',
            '\0'
        };
            for (int i = level; i < 2; i++)
                printf(".....");
            if (virt_addr < 0x010)
                printf("0x00");
            else if (virt_addr < 0x100)
                printf("0x0");
            else
                printf("0x");
            printf("%x -> %p %s\n", virt_addr, next_level_page, flags_str);
            if (level > 0)
               visit_pages_and_print(level - 1, next_level_page, buf, len, flags);
          }
          virt_addr_ind ++;
          if (pte && (*pte & PTE_V) && !is_buf_empty){
               len -= size_of_block - (buf - PGROUNDDOWN(buf));
               buf =  PGROUNDDOWN(buf) +  size_of_block;
          }
     }
}

void visit_pages_and_remove_flags(int level, pagetable_t pages, uint64 buf, int len, int flags){
   if (level < 0) return;
   int is_buf_empty = 0;
   uint64 size_of_block = 1LL << (9 * level + PGSHIFT);
   if (buf == 0 || len == 0) is_buf_empty = 1;
   int virt_addr_ind = 0; int finish_page = 512;

   while ((!is_buf_empty && len > 0) || (is_buf_empty && virt_addr_ind < finish_page)){
          int virt_addr; pte_t* pte;
          if (is_buf_empty) virt_addr = virt_addr_ind;
          else virt_addr = (PGROUNDDOWN(buf) / size_of_block) % 512;
          pte = &pages[virt_addr];
          if (pte && (*pte & PTE_V) && (level > 0 || (((flags & A_FLAG) && (*pte & PTE_A)) || ((flags & D_FLAG) && (*pte & PTE_D))))) {
            pagetable_t next_level_page = (pagetable_t)PTE2PA(*pte);
            if ((flags & 1) && (*pte & PTE_A))
                 *pte &= ~PTE_A;
            if ((flags & 2) && (*pte & PTE_D))
                 *pte &= ~PTE_D;
            if (level > 0)
               visit_pages_and_remove_flags(level - 1, next_level_page, buf, len, flags);
          }
          virt_addr_ind ++;
          if (pte && (*pte & PTE_V) && !is_buf_empty){
               len -= size_of_block - (buf - PGROUNDDOWN(buf));
               buf =  PGROUNDDOWN(buf) +  size_of_block;
          }
     }
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
