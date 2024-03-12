#include "system.h"
void main()
{
  // we donot need gdt install in 64bit mode anymore, so we cleared it out here.
  print_clear();
  print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
  idt_install();
  isrs_install();
  irq_install();
  timer_install();
  kinit(); // 30406539
  print_str("my first kernel.. first!!!!!!\n");
  /*
   * what caused bug here:
   * 1. you need to cli/sti when setting 8259
   * 2. hlt do not ignore intr. but no eoi, why more signals?
   * 3. still dk why it doesnt work when encountering a page-fault, the code does not get back! wtf?
   * */
  for (;;);
    //print_str("my first kernel..\n");
}