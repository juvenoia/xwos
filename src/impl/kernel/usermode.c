#include "system.h"

struct gdt_entry {
    unsigned int limit_low              : 16;
    unsigned int base_low               : 24;
    unsigned int accessed               :  1;
    unsigned int read_write             :  1; // readable for code, writable for data
    unsigned int conforming_expand_down :  1; // conforming for code, expand down for data
    unsigned int code                   :  1; // 1 for code, 0 for data
    unsigned int code_data_segment      :  1; // should be 1 for everything but TSS and LDT
    unsigned int DPL                    :  2; // privilege level
    unsigned int present                :  1;
    unsigned int limit_high             :  4;
    unsigned int available              :  1; // only used in software; has no effect on hardware
    unsigned int long_mode              :  1;
    unsigned int big                    :  1; // 32-bit opcodes for code, uint32 stack for data
    unsigned int gran                   :  1; // 1 to use 4k page addressing, 0 for byte addressing
    unsigned int base_high              :  8;
} __attribute__((packed));

typedef struct gdt_entry gdt_entry_bits;

struct tss_entry_struct {
    uint32 prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
    uint32 esp0;     // The stack pointer to load when changing to kernel mode.
    uint32 ss0;      // The stack segment to load when changing to kernel mode.
    // Everything below here is unused.
    uint32 esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
    uint32 ss1;
    uint32 esp2;
    uint32 ss2;
    uint32 cr3;
    uint32 eip;
    uint32 eflags;
    uint32 eax;
    uint32 ecx;
    uint32 edx;
    uint32 ebx;
    uint32 esp;
    uint32 ebp;
    uint32 esi;
    uint32 edi;
    uint32 es;
    uint32 cs;
    uint32 ss;
    uint32 ds;
    uint32 fs;
    uint32 gs;
    uint32 ldt;
    uint16 trap;
    uint16 iomap_base;
} __attribute__((packed)); // CR2=0000000800002138

typedef struct tss_entry_struct tss_entry_t;

tss_entry_t tss_entry;

uint64 gdt[8];

struct gdtp {
    uint16 size;
    uint64 addr;
} __attribute__((packed)) _gdtp;

void write_tss(gdt_entry_bits *g) {
  // Compute the base and limit of the TSS for use in the GDT entry.
  uint32 base = (uint32) &tss_entry;
  uint32 limit = sizeof tss_entry;

  // Add a TSS descriptor to the GDT.
  g->limit_low = limit;
  g->base_low = base;
  g->accessed = 1; // With a system entry (`code_data_segment` = 0), 1 indicates TSS and 0 indicates LDT
  g->read_write = 0; // For a TSS, indicates busy (1) or not busy (0).
  g->conforming_expand_down = 0; // always 0 for TSS
  g->code = 1; // For a TSS, 1 indicates 32-bit (1) or 16-bit (0).
  g->code_data_segment=0; // indicates TSS/LDT (see also `accessed`)
  g->DPL = 0; // ring 0, see the comments below
  g->present = 1;
  g->limit_high = (limit & (0xf << 16)) >> 16; // isolate top nibble
  g->available = 0; // 0 for a TSS
  g->long_mode = 1;
  g->big = 0; // should leave zero according to manuals.
  g->gran = 0; // limit is in bytes, not pages
  g->base_high = (base & (0xff << 24)) >> 24; //isolate top byte

  // Ensure the TSS is initially zero'd.
  memset(&tss_entry, 0, sizeof tss_entry);

  tss_entry.ss0 = 0x00000000;  // kernel high bit
  tss_entry.esp0 = task_struct[0].knlStk; // kernel low bit
}

void updateKernelStack(uint32 addr) {
  tss_entry.esp0 = addr;
}

void jmpUsermode() {
  // jmp usermode
  gdt[0] = 0UL;                   // dummy
  gdt[1] = 0x00a0980000000000UL;  // kernel code
  gdt[2] = 0x00c0920000000000UL;  // kernel data
  gdt[3] = 0UL;                   // reserved
  gdt[4] = 0x00c0f20000000000UL;  // user data
  gdt[5] = 0x00a0f80000000000UL;  // user code //0x42 ie 66?
  //gdt[4] = 0x00c0920000000000UL;  // user data
  //gdt[5] = 0x00a0980000000000UL;  // user code //0x42 ie 66?
  write_tss(&gdt[6]);
  gdt[7] = 0x0000000000000000UL;
  _gdtp.size = 8 * 8 - 1; // with one
  _gdtp.addr = (uint64)&gdt[0];

  __asm__ ("lgdt %0" : : "m" (_gdtp) : "memory");
  flush_tss();
  usermode(); // the issue should be related to tss?
}