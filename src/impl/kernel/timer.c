/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Timer driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks)
{
  unsigned long eticks;

  eticks = timer_ticks + ticks;
  while(timer_ticks < eticks);
}

void timer_phase(int hz)
{
  int divisor = 1193180 / hz;       /* Calculate our divisor */
  outportb(0x43, 0x36);             /* Set our command byte 0x36 */
  outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
  outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}


/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
  /* Installs 'timer_handler' to IRQ0 */
  timer_phase(100000);
  //sti();
}