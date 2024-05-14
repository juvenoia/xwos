#include "ulib.h"

char s[] = "test from putc\n";

int main() {
  uprintf("hello, world!\n");
  for (int i = 0; i < 15; i ++) {
    int p = putc(s[i]);
    uprintf("%d\n", p);
  }
  for (;;) ;
}