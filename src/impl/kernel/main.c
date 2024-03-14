#include "system.h"
void main()
{
  // we donot need gdt install in 64bit mode anymore, so we cleared it out here.
  init_serial(); // todo: multi-hart printk
  sti();
  idt_install();
  isrs_install();
  irq_install(); // todo: 现在的trapframe是否完全？
  timer_install();
  kinit(); // todo: 一个简单的模型。如何绕过rom?
  // todo: procinit 去查看 task struct的内容，以及如何进入ring3? 能否验证现在是不是ring3?
  // todo: keyboard intr.
  // todo: fs related. binit, iinit, fileinit, virtio_disk_init
  // todo: last: userinit.
  printk("my first kernel.. first!!!!!!\n");

  for (int i = 0; ; i ++) ;
}