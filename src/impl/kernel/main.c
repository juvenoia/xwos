#include "system.h"

void userInit() {
  int p = fork();
  for (;; ) {
    //uprintf("%d\n", p);
  }
}


void main() {
  init_serial();
  cli();
  idt_install();
  isrs_install();
  irq_install();
  timer_install();
  kinit();
  procinit();
  sti();
  jmpUsermode();
}