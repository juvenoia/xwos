#include "system.h"

#define SAVE_REGS \
    __asm__ __volatile__ ( \
        "pushq %%rdi\n\t" \
        "pushq %%rsi\n\t" \
        "pushq %%rdx\n\t" \
        "pushq %%rcx\n\t" \
        "pushq %%r8\n\t" \
        "pushq %%r9\n\t" \
        : \
        : \
        : "memory" \
    )

#define RESTORE_REGS \
    __asm__ __volatile__ ( \
        "popq %%r9\n\t" \
        "popq %%r8\n\t" \
        "popq %%rcx\n\t" \
        "popq %%rdx\n\t" \
        "popq %%rsi\n\t" \
        "popq %%rdi\n\t" \
        : \
        : \
        : "memory" \
    )

#define LOAD_REG(reg, val) \
    __asm__ __volatile__ ("movq %0, %%" #reg :: "r" (val))

#define TRAP \
    __asm__ __volatile__ ("int $0x80")


//int fork(void);
//int exit(int) __attribute__((noreturn));
//int wait(int*);
//int pipe(int*);
//int write(int, const void*, int);
//int read(int, void*, int);
//int close(int);
//int kill(int);
//int exec(const char*, char**);
//int open(const char*, int);
//int mknod(const char*, short, short);
//int unlink(const char*);
//int fstat(int fd, struct stat*);
//int link(const char*, const char*);
//int mkdir(const char*);
//int chdir(const char*);
//int dup(int);
//int getpid(void);
//char* sbrk(int);
//int sleep(int);
//int uptime(void);

#define SAVE_CONTEXT(reg, val) \
    __asm__ __volatile__ ( \
        "movq %%" #reg ", %0\n\t" \
        : "=m" (val) \
        : \
        : \
    )
int putc(char c) {
  // 1. push your params in userstack
  uint64 arg0 = (uint64)c;
  SAVE_REGS;
  LOAD_REG(rdi, arg0); // first param.
  LOAD_REG(r9, (uint64)SYS_putc);
  // 2. change your TSS. KnlStk
  // updateKernelStack This process should be done when you switch your process, but not in usermode.
  // 3. int 0x80
  TRAP;
  // 4. load your return value?
  // isrs.asm' iret goes here.
  // rax carries the return value.
  uint64 ret;
  SAVE_CONTEXT(r9, ret); // you donot need change rax anymore.
  // you pop your return address..
  RESTORE_REGS;
  return ret;
}

int fork() {
  SAVE_REGS;
  LOAD_REG(r9, (uint64)SYS_fork);
  TRAP;
  uint64 ret;
  SAVE_CONTEXT(r9, ret);
  RESTORE_REGS;
  uprintf("once?\n");
  return ret;
  // now, we step a backup sa
}