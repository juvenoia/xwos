#include "system.h"

void user_test() {
  for (;;) {
    printk("user test\n");
  }
}


void main()
{
  // we donot need gdt install in 64bit mode anymore, so we cleared it out here.
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