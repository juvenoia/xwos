#include "system.h"

const uint64 end = 0x0000000;
const uint64 MAXVA = 0x7000000; // 128 - 16 MB. you have 16MB for kernel space.
// WE HAVE 28599 PAGES.

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
  int c = 0;
  for (uint64 va = PGROUNDUP(end); va < MAXVA; va += PGSIZE) {
    uint64 *ptr = va;
    *ptr = 2140;
    if (*ptr == 2140)
      kfree(va);
    c ++;
  }
}

int c = 0;

void kfree(void *va) { // you should hold cli and sti when using kfree, in kernel mode.
  struct run *r;
  if ((uint64)va % PGSIZE != 0 || (char *)va < end || (uint64 *)va >= MAXVA) {
    printk("invalid kfree.\n");
    for (;;) ; //my halt
  }
  memset(va, 7, PGSIZE); // memset work on a char-wise schedule.
  r = (struct run*)va;
  r->next = kmem.freelist;
  kmem.freelist = r;
}

void *kalloc() {
  struct run *r = kmem.freelist;
  //cli();
  if (r)
    kmem.freelist = r->next;
  else {
    printk("no more valid space!\n");
    for (;;) ;
  }
  if ((uint64)r % PGSIZE != 0) {
    printk("not a valid Page!\n");
    for (;;) ;
  }
  //sti();
  if (r)
    memset(r, 5, PGSIZE);
  return (void *)r;
}
