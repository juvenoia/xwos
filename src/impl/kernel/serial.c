#include "system.h"

const int PORT = 0x3F8;

void init_serial() {
  outportb(PORT + 1, 0x00); // Disable all intrs..
  outportb(PORT + 3, 0x80); // Enable DLAB
  outportb(PORT + 0, 0x03); // Set Divisor to 3(lo byte)
  outportb(PORT + 1, 0x00); //                 (hi byte)
  outportb(PORT + 3, 0x03); // 8 bit, ?
  outportb(PORT + 2, 0xC7); // Enable FIFO
  outportb(PORT + 4, 0x0B); // IRQs ennables, RTS/DSR/set
  outportb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outportb(PORT + 4, 0x0F);
}

int is_transmit_empty() {
  return inportb(PORT + 5) & 0x20;
}

void write_serial(char a) { // not sync version.. ask-and-wait.
  while (is_transmit_empty() == 0);
  outportb(PORT, a);
}