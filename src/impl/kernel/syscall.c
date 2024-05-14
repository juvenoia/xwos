#include "system.h"

uint64 sys_putc(char c) {
  uint64 arg0 = task_struct[0].ctx.rdi;
  write_serial(arg0);
  // after changing our syscall stub, your return value is at current.process.rax
  task_struct[task_struct[0].id].ctx.rax = 2140;
  return 0;
}