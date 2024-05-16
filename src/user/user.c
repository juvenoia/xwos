#include "ulib.h"

int main() {
  int p = fork();
  if (p == 0) {
    for (;;)
      uprintf("son\n");
  } else {
    for (;;)
      uprintf("father\n"); // the question has been found. rbp should be 16 bit aligned.. but how to do this?
  }
  // ok now bug caused by uprintf has been fixed. next we need to follow xv6 to make some syscall and user procs.
}