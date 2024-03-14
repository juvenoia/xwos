#include "system.h"

//const uint64 MAXVA = 0x10000000 + 0x00400000; // 256M + 16M.
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

//extern char end[];
const uint64 end = 0x01000000; //256M
const uint64 MAXVA = end + PGSIZE * 8196; // 128 MB.

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
  for (uint64 va = PGROUNDUP(end); va < MAXVA; va += PGSIZE) {
    //print_num(va);
    kfree(va); //32 M available memory for us. not too big now! memory management is annoying.
  }
  printk("this is page counts.");
  r = kmem.freelist;
  int p = 0;
  while (r) {
    r = r->next;
    p ++;
  }
  printk("%d\n", p);
}

void kfree(void *va) {
  cli(); // this equals to a lock, we will modify this when we move on to a multi-core system
  struct run *r;
  if ((uint64)va % PGSIZE != 0 || (char *)va < end || (uint64 *)va >= MAXVA) {
    printk("invalid kfree.\n");
    for (;;) ; //my halt
  }
  memset(va, 7, PGSIZE); // memset work on a char-wise schedule.
  r = (struct run*)va;
  //print_num(kmem.freelist);
  r->next = kmem.freelist;
  //print_num(r->next);
  kmem.freelist = r;
  //print_num(kmem.freelist);
  //print_num(kmem.freelist->next);
  //print_num(r->next);
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
