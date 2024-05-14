/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Global function declarations and type definitions
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef int size_t;
typedef int uint8_t;
/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned long long rdi, rsi, rbp, rbx, rdx, rcx, rax;
    unsigned long long int_no, err_code;
    unsigned long long rip, cs, rflags, rsp, ss;
};

/* SYSTEM.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern void sti();
extern void cli();
extern void usermode();

/* IDT.C */
extern void idt_set_gate(unsigned char num, unsigned long long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* ISRS.C */
extern void isrs_install();

/* IRQ.C */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();
//
/* TIMER.C */
extern void timer_wait(int ticks);
extern void timer_install();
//

/* KMALLOC.C */
typedef uint64 pagetable_t;

extern void kinit();
extern void kfree(void *);
extern void *kalloc();

/* SERIAL.C */
extern void init_serial();
extern void write_serial(char);

/* PRINTK.C */
extern void printk(char *fmt, ...);

/* PROC.C */
typedef struct {
    uint64 rax, rbx, rcx, rdx, rsi, rdi, rbp;
    uint64 r8, r9, r10, r11, r12, r13, r14, r15;
    uint64 stk[5]; // though we dont need these, but we still store it.
    // 0: rip, 1: cs, 2: rflags, 3: rsp, 4:ss;
} ctx_t;

enum {
    PROC_UNUSED, PROC_RUNNING, PROC_WAITING, PROC_RUNNABLE,
};

typedef struct {
    int id; // proc id
    ctx_t ctx;
    pagetable_t *pgtbl, *kpgtbl;
    uint64 *knlStk;
    int state;
}proc;

#define NPROC 256 // currently only 256 tasks are allowed
extern proc task_struct[];
extern void procinit();

/* USERMODE.C */
extern void updateKernelStack(uint32);
extern void jmpUsermode();

/* MM.C */
extern uint64 PGSIZE;
extern uint64 PGROUNDDOWN(void *);
extern uint64 PGROUNDUP(void *);
extern uint64 allocPgtbl();
extern uint64 kallocPgtbl();
extern void swtchPgtbl(uint64);
extern int mappages(uint64 *, uint64, uint64);

#endif
