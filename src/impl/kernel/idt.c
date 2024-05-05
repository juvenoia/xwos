#include "system.h"

/* Defines an IDT entry */
struct idt_entry
{
    uint16 base_lo;
    uint16 sel;        /* Our kernel segment goes here! */
    uint8 always0;     /* This will ALWAYS be set to 0! */
    uint8 flags;       /* Set using the above table! */
    uint16 base_hi;
    uint32 base_3;
    uint32 zero;
} __attribute__((packed)) idt[256];

struct idt_ptr
{
    uint16 limit;
    uint64 base;
} __attribute__((packed)) idtp;



/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(uint8 num, uint64 base, uint16 sel, uint8 flags)
{
  /* We'll leave you to try and code this function: take the
  *  argument 'base' and split it up into a high and low 16-bits,
  *  storing them in idt[num].base_hi and base_lo. The rest of the
  *  fields that you must set in idt[num] are fairly self-
  *  explanatory when it comes to setup */

  /* The interrupt routine's base address */
  idt[num].base_lo = (base & 0xFFFF);
  idt[num].base_hi = (base >> 16) & 0xFFFF;

  /* The segment or 'selector' that this IDT entry will use
  *  is set here, along with any access flags */
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].flags = flags;

  idt[num].base_3 = (base >> 32 & 0xFFFFFFFF);
  idt[num].zero = 0;
}

void load_idt(struct idt_ptr *idtp) {
  asm volatile ("lidt %0" : : "m" (*idtp));
}
/* Installs the IDT */
void idt_install()
{
  /* Sets the special IDT pointer up, just like in 'gdt.c' */
  idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
  idtp.base = &idt;

  /* Clear out the entire IDT, initializing it to zeros */
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  /* Add any new ISRs to the IDT here using idt_set_gate */

  /* Points the processor's internal register to the new IDT */
  // write this in __Asm__
  load_idt(&idtp);
}