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
  //jmpUsermode();
  struct buf *b = bread(1, 1);
  int i = 0;
  for (uint8 *ch = b->data; ch != b->data + 512; ch ++, i ++) {
    printk("%d ", *ch);
    if (i % 16 == 0)
      printk("\n");
  }
  for (;;) ;
}