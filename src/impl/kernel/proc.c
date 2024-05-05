#include "system.h"

proc task_struct[NPROC]; // 0 for current running process!

int currentPid = 1;

int getPid() {
  return ++ currentPid;
}

void procinit() {
  uint64 cr3_v;
  asm volatile("mov %%cr3, %0" : "=r" (cr3_v));
  cr3_v = (cr3_v >> 4) << 4;
  for (int i = 1; i < NPROC; i ++) {
    task_struct[i].state = PROC_UNUSED;
    task_struct[i].kpgtbl = cr3_v; // pid=1 will have a 1G-Grad pgtbl
    //uint64 *knlStk = (uint64 *)kalloc();
    task_struct[i].knlStk = 0x8000000 - i * PGSIZE;
    task_struct[i].id = i;
  }
  task_struct[0].id = 1;
  task_struct[1].state = PROC_RUNNING;
  //printk("knlStk: %p\n", task_struct[1].knlStk); //knlStk: 0x0000000006fff000
  //updateKernelStack(task_struct[1].knlStk); // every time you change the current running proc, corresponding knlStk should change.
}

int allocProcStruct() {
  for (int i = 1; i < NPROC; i ++) {
    if (task_struct[i].state == PROC_UNUSED) {
      task_struct[i].state = PROC_RUNNABLE;
      return i;
    }
  }
  return -1;
}

uint64 sys_fork() {
  // -1 indicates fork failure.
  // 0 indicate it is a son
  // > 0 indicate its son pid (father)
  int p = allocProcStruct();
  if (p == -1) // no more task_struct.
    return -1;
  uint64 *pgtbl = allocPgtbl(); // allocate a new pgtbl, kernel already done.
  for (uint64 pa = 0; pa < 0x7000000; pa += PGSIZE) {
    if (walk(task_struct[0].pgtbl, pa, 0) != 0x8000000) { // walk just return a valid pgtbl addr, if valid. 0x8000000 indicates that is not valid.
      // fork is called by a syscall, current id is saved in task_struct[0].
      uint64 *npage = (uint64 *)kalloc();
      mappages(pgtbl, pa, npage);
    }
  }
  task_struct[p].pgtbl = pgtbl;
  // now we have the same pgtbl.
  // we copy the current ctx.
  task_struct[p].ctx = task_struct[0].ctx; // now we have the exact ctx. im not sure if this is able to assign?
  task_struct[task_struct[0].id].ctx.r9 = p; // father return its id.
  task_struct[p].ctx.r9 = 0; // son, fork return 0!
  task_struct[p].state = PROC_RUNNABLE;
  // copy you current program stack. when you have done 'exec', this would be all user-data..
  return 0;
}