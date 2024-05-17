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
  // a sector is, 512B.
  uint8 buffer[1024]; // Buffer to store 2 sectors (1024 bytes)
  read_sectors(buffer, 0, 2, 1); // Read 2 sectors from sector 0 on master drive
  // Print the first few bytes of the buffer for verification
  for (int i = 0; i < 1024; i++) {
    printk("%d ", (uint32) buffer[i]);
    if ((i + 1) % 16 == 0) {
      printk("\n");
    }
  }
  for (int i = 0; i < 1024; i++) {
    buffer[i] = 0x21;
  }
  write_sectors(buffer, 0, 2, 1); // Write 2 sectors from sector 0 on master drive
  read_sectors(buffer, 0, 2, 1); // Read 2 sectors from sector 0 on master drive
  for (int i = 0; i < 1024; i++) {
    printk("%d ", (uint32) buffer[i]);
    if ((i + 1) % 16 == 0) {
      printk("\n");
    }
  } // now be all 0x55, i.e. 16 + 5 + 5 = 85.
  for (;;) ;

}