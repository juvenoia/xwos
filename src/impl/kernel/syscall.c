#include "system.h"

uint64 sys_putc(char c) {
  write_serial(c);
  return 2140;
}