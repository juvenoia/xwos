#include "system.h"

#define SAVE_REG_TO_TASK_STRUCT(reg, id) \
    __asm__ __volatile__ ("movq %%" #reg ", %0" : "=m" (task_struct[id].ctx.reg))

#define INTR_SAVE_ALL_REGS_TO_TASK_STRUCT(id) \
        SAVE_REG_TO_TASK_STRUCT(rax, id); \
        SAVE_REG_TO_TASK_STRUCT(rbx, id); \
        SAVE_REG_TO_TASK_STRUCT(rcx, id); \
        SAVE_REG_TO_TASK_STRUCT(rdx, id); \
        SAVE_REG_TO_TASK_STRUCT(rsi, id); \
        SAVE_REG_TO_TASK_STRUCT(rdi, id); \
        SAVE_REG_TO_TASK_STRUCT(rbp, id); \
        SAVE_REG_TO_TASK_STRUCT(rsp, id); \
        SAVE_REG_TO_TASK_STRUCT(r8, id); \
        SAVE_REG_TO_TASK_STRUCT(r9, id); \
        SAVE_REG_TO_TASK_STRUCT(r10, id); \
        SAVE_REG_TO_TASK_STRUCT(r11, id); \
        SAVE_REG_TO_TASK_STRUCT(r12, id); \
        SAVE_REG_TO_TASK_STRUCT(r13, id); \
        SAVE_REG_TO_TASK_STRUCT(r14, id); \
        SAVE_REG_TO_TASK_STRUCT(r15, id);

void uwrite_serial(char a) {
  __asm__ ("ud2"); // this will cause a syscall. we will jmp to kernel mode after that.
  printk("i have come to kernel mode. hello!");
  write_serial(a);
  //TODO: load r11, read rip, load it into rcx, sysret.
}