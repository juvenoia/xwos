#include "system.h"

#define MAXVA 0x8000000

uint64 PGSIZE = 0x1000;

#define VA2IDX(x) (((uint64) x) / (PGSIZE))
uint64 PGROUNDDOWN(void *va) {
  uint64 adr = va;
  return adr - (adr % PGSIZE);
}
uint64 PGROUNDUP(void *va) {
  uint64 adr = va;
  if (adr % PGSIZE == 0)
    return adr;
  return PGROUNDDOWN(adr) + PGSIZE;
}

uint64 allocPgtbl() {
  // return an already mapped 0x7000000~MAX pgtbl, for user usage.
  uint64 *pgtblBase = (uint64 *)kalloc();
  for (uint64 pa = 0x7000000; pa < 0x8000000; pa += PGSIZE) {
    // map it to itself.
    if (mappages(pgtblBase, pa, pa) == 0) {
      printk("allocPgtbl, no more pages.\n");
      for (;;) ;
    }
  }
  return pgtblBase;
}

uint64 kallocPgtbl() {
  uint64 *pgtblBase = (uint64 *)kalloc();
  return pgtblBase;
}

void swtchPgtbl(uint64 pgtbl) {
  uint64 cr3_v;
  asm volatile("mov %%cr3, %0" : "=r" (cr3_v));
  cr3_v = cr3_v & (1 << 4);
  cr3_v |= pgtbl;
  asm volatile("mov %0, %%cr3" : : "r" (cr3_v));
}

uint64 shift[] = {39, 30, 21, 12};

uint64 *walk(uint64 *pgtbl, uint64 va, int crt) {
  // go for va in pgtbl. if crt=1, it will create any level of pte/pde that not exist.
  for (int i = 0; i < 3; i ++) {
    int idx = (va >> (shift[i]) & 511);
    if ((pgtbl[idx] & 3) != 3) {
      if (crt == 0)
        return 0x8000000;
      uint64 *np = (uint64 *)kalloc();
      memset(np, 0, PGSIZE);
      pgtbl[idx] = ((uint64)np) | (0b111);
    }
    pgtbl = (pgtbl[idx] >> 12) << 12;
  }
  return pgtbl;
}

int mappages(uint64 *pgtbl, uint64 va, uint64 pa) {
  uint64 *pgbase = walk(pgtbl, va, 1);
  if (pgbase == 0x8000000)
    return 0;
  pgbase[(va >> shift[3]) & (511)] = pa | (0b111);
  return 1;
}

uint64 *fwalk(uint64 *pgtbl, uint64 va) {
  uint64 *pgbase = walk(pgtbl, va, 0);
  if (pgbase == 0x8000000)
    return 0x8000000;
  uint64 pte = pgbase[(va >> shift[3]) & (511)];
  if (pte & 1)
    return (pte >> 12) << 12;
  return 0x8000000;
}

int either_copyout(int user_dst, uint64 dst, void *src, uint64 len) {
  if (user_dst) {
    int pid = task_struct[0].id;
    return copyout(task_struct[pid].pgtbl, dst, src, len);
  } else {
    memcpy((uint8 *)dst, src, len);
    return 0;
  }
}

// Copy from kernel to user.
// Copy len bytes from src to virtual address dstva in a given page table.
// Return 0 on success, -1 on error.
int copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len) {
  while(len > 0){
    uint64 va = PGROUNDDOWN(dstva);
    if(va >= MAXVA)
      return -1;
    uint64 pa = fwalk(pagetable, va);
    if (pa == 0x8000000) // such pte does not exist in user's pgtbl
      return -1;
    uint64 n = PGSIZE - (dstva - va);
    if(n > len)
      n = len;
    memcpy((void *)(pa + (dstva - va)), src, n);
    len -= n;
    src += n;
    dstva = va + PGSIZE;
  }
  return 0;
}

// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
int copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len) {
  while(len > 0){
    uint64 va = PGROUNDDOWN(srcva);
    uint64 pa = fwalk(pagetable, va);
    if(pa == 0x8000000)
      return -1;
    uint64 n = PGSIZE - (srcva - va);
    if(n > len)
      n = len;
    memcpy(dst, (void *)(pa + (srcva - va)), n);
    len -= n;
    dst += n;
    srcva = va + PGSIZE;
  }
  return 0;
}

int either_copyin(void *dst, int user_src, uint64 src, uint64 len) {
  if (user_src) {
    int pid = task_struct[0].id;
    return copyin(task_struct[pid].pgtbl, dst, src, len);
  } else {
    memcpy(dst, (char *)src, len);
    return 0;
  }
}