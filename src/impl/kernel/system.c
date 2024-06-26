#include "system.h"

void *memcpy(void *dest, const void *src, size_t count)
{
  const char *sp = (const char *)src;
  char *dp = (char *)dest;
  for(; count != 0; count--) *dp++ = *sp++;
  return dest;
}

void *memset(void *dest, char val, size_t count)
{
  char *temp = (char *)dest;
  for( ; count != 0; count--) *temp++ = val;
  return dest;
}

uint16 *memsetw(uint16 *dest, uint16 val, size_t count)
{
  uint16 *temp = (uint16 *)dest;
  for( ; count != 0; count--) *temp++ = val;
  return dest;
}

size_t strlen(const char *str)
{
  size_t retval;
  for(retval = 0; *str != '\0'; str++) retval++;
  return retval;
}

int strncmp(const char *p, const char *q, uint32 n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uint8)*p - (uint8)*q;
}

char* strncpy(char *s, const char *t, int n)
{
  char *os;
  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = 0;
  return os;
}

uint8 inportb (uint16 _port)
{
  uint8 rv;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
  return rv;
}

void outportb (uint16 _port, uint8 _data)
{
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void cli() // close intr
{
  __asm__ __volatile__ ("cli");
}

void sti() // start intr
{
  __asm__ __volatile__ ("sti");
}

extern void jump_usermode();

void usermode() {
  jump_usermode();
}