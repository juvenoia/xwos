#include "system.h"

#define MAXVA 0x40000000
#define PGSIZE 4096
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

extern char end[];

void kinit() {
  // now our system is on a single cpu. our lock just ignore the clk intr and prevent it from sched.

}