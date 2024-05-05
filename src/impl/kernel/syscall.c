#include "system.h"

uint64 sys_putc(char c) {
  write_serial(c);
  // after changing our syscall stub, your return value is at current.process.rax
  task_struct[task_struct[0].id].ctx.r9 = 2140;
  return 0;
}