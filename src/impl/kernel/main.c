#include "system.h"

void userInit() {
  for (;;) {
    ;
  }
}


void main() {
  cli();
  init_serial();
  idt_install();
  isrs_install();
  irq_install();
  timer_install();
  kinit();
  procinit();
  jmpUsermode();
}