#include "system.h"

const uint64 PGSIZE = 4096;
static uint64 PGROUNDDOWN(void *va) {
  uint64 adr = va;
  return adr - (adr % PGSIZE);
}
static uint64 PGROUNDUP(void *va) {
  uint64 adr = va;
  if (adr % PGSIZE == 0)
    return adr;
  return PGROUNDDOWN(adr) + PGSIZE;
}

const uint64 end = 0x01000000; //256M
const uint64 MAXVA = end + PGSIZE * 256; // 128 MB.

struct run {
    struct run *next;
};

struct {
  struct run *freelist;
} kmem;


void kinit() {
  // now our system is on a single cpu. our lock just ignore the clk intr and prevent it from sched.
  // this equals to a lock, we will modify this when we move on to a multi-core system
  struct run *r;
  for (uint64 va = PGROUNDUP(end); va < MAXVA; va += PGSIZE)
    kfree(va); //32 M available memory for us. not too big now! memory management is annoying.
  // TODO: alloc each task knlStk with its task_struct
}

void kfree(void *va) {
  cli();
  struct run *r;
  if ((uint64)va % PGSIZE != 0 || (char *)va < end || (uint64 *)va >= MAXVA) {
    printk("invalid kfree.\n");
    for (;;) ; //my halt
  }
  memset(va, 7, PGSIZE); // memset work on a char-wise schedule.
  r = (struct run*)va;
  r->next = kmem.freelist;
  kmem.freelist = r;
  sti();
}

void *kalloc() {
  struct run *r = kmem.freelist;
  cli();
  if (r)
    kmem.freelist = r->next;
  else {
    printk("no more valid space!\n");
    for (;;) ;
  }
  sti();
  if (r)
    memset(r, 5, PGSIZE);
  return (void *)r;
}
