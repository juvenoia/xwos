#include <stdarg.h>
#include "system.h"
#include "ulib.h"

char udigits[] = "0123456789abcdef";

void uprintint(int xx, int base, int sign) {
  char buf[16];
  uint32 x;
  if (sign && (sign = (xx < 0)))
    x = -xx;
  else
    x = xx;
  int i = 0;
  do {
    buf[i ++] = udigits[x % base];
    x /= base;
  } while (x != 0);
  if (sign)
    buf[i ++] = '-';
  while ((-- i) >= 0)
    putc(buf[i]);
}

void uprintptr(uint64 x) {
  putc('0');
  putc('x');
  for (int i = 0; i < (sizeof (uint64)) * 8 / 4; i ++) {
    putc(udigits[x >> ((sizeof (uint64)) * 8 - 4)]);
    x <<= 4;
  }
}

void uprintf(char *fmt, ...) {
  // you should check out if rbp is 16 bit align. i assume this will
  va_list ap;
  if (fmt == 0) {
    uprintf("null pointer..\n");
    for (;;) ;
  }
  va_start(ap, fmt);
  for (int i = 0; ;) {
    char c = fmt[i] & 0xff;
    if (c == 0)
      break;
    if (c != '%') {
      putc(c);
      i ++;
    } else {
      char d = fmt[++ i] & 0xff;
      switch (d) {
        case 'p':
          uprintptr(va_arg(ap, uint64));
          i ++;
          break;
        case 'd':
          uprintint(va_arg(ap, uint32), 10, 1);
          i ++;
          break;
        case 'x':
          uprintint(va_arg(ap, uint32), 16, 1);
          i ++;
          break;
        case 's':
          char *s = va_arg(ap, char*);
          if (s == 0)
            s = "(null)";
          for (; *s != 0; s ++)
            putc(*s);
          i ++;
          break;
        case '%':
          putc('%');
          i ++;
          break;
        case '0':
          break;
        default:
          putc('%');
          putc(d);
          i ++;
          break;
      }
    }
  }
  va_end(ap);
}