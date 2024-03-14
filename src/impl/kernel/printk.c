#include <stdarg.h>
#include "system.h"

static char digits[] = "0123456789abcdef";

void printint(int xx, int base, int sign) {
  char buf[16];
  uint32 x;
  if (sign && (sign = (xx < 0)))
    x = -xx;
  else
    x = xx;
  int i = 0;
  do {
    buf[i ++] = digits[x % base];
    x /= base;
  } while (x != 0);
  if (sign)
    buf[i ++] = '-';
  while ((-- i) >= 0)
    write_serial(buf[i]);
}

void printptr(uint64 x) {
  write_serial('0');
  write_serial('x');
  for (int i = 0; i < (sizeof (uint64)) * 8 / 4; i ++) {
    write_serial(digits[x >> ((sizeof (uint64)) * 8 - 4)]);
    x <<= 4;
  }
}

void printk(char *fmt, ...) {
  va_list ap;
  if (fmt == 0) {
    printk("null pointer..\n");
    for (;;) ;
  }
  va_start(ap, fmt);
  for (int i = 0; ;) {
    char c = fmt[i] & 0xff;
    if (c == 0)
      break;
    if (c != '%') {
      write_serial(c);
      i ++;
    } else {
      char d = fmt[++ i] & 0xff;
      switch (d) {
        case 'p':
          printptr(va_arg(ap, uint64));
          i ++;
          break;
        case 'd':
          printint(va_arg(ap, uint32), 10, 1);
          i ++;
          break;
        case 'x':
          printint(va_arg(ap, uint32), 16, 1);
          i ++;
          break;
        case 's':
          char *s = va_arg(ap, char*);
          if (s == 0)
            s = "(null)";
          for (; *s != 0; s ++)
            write_serial(*s);
          i ++;
          break;
        case '%':
          write_serial('%');
          i ++;
          break;
        case '0':
          break;
        default:
          write_serial('%');
          write_serial(d);
          i ++;
          break;
      }
    }
  }
  va_end(ap);
}

//void
//printk(char *fmt, ...)
//{
//  va_list ap;
//  int i, c;
//  char *s;
//
//  va_start(ap, fmt);
//  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
//    if(c != '%'){
//      write_serial(c);
//      continue;
//    }
//    c = fmt[++i] & 0xff;
//    if(c == 0)
//      break;
//    switch(c){
//      case 'd':
//        printint(va_arg(ap, int), 10, 1);
//        break;
//      case 'x':
//        printint(va_arg(ap, int), 16, 1);
//        break;
//      case 'p':
//        printptr(va_arg(ap, uint64));
//        break;
//      case 's':
//        if((s = va_arg(ap, char*)) == 0)
//        s = "(null)";
//        for(; *s; s++)
//          write_serial(*s);
//        break;
//      case '%':
//        write_serial('%');
//        break;
//      default:
//        // Print unknown % sequence to draw attention.
//        write_serial('%');
//        write_serial(c);
//        break;
//    }
//  }
//  va_end(ap);
//}
