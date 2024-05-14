#include "system.h"

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