#include "system.h"

void user_test() {
  for (;; ) {
    uprintf("i have done user-syscall stub.\n");
  }
}


void main() {
  init_serial(); // todo: multi-hart printk
  cli();
  idt_install();
  isrs_install();
  irq_install(); // todo: 现在的trapframe是否完全？
  timer_install();
  sti();
  kinit();
  jmpUsermode();
}