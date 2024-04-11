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
  // SYSCALL
  // 64-bit syscall.
  /*
   * syscall loads CS from STAR 47:32, SS .store RIP in RCX(we need to store rcx)
   * loads RIP from LSTAR.
   * */
  /*
  TODO:
  pseudo: save current registers in task_struct
  rcx should be current + get rip, write rcx, sycall.
  Kernel rsp is Set in TSS. you should need this, because each proc should have different kernel stack.
  __asm__ __volatile__ ("syscall");
  write_serial(a);
  change rcx and r11. you do not change others, because syscalls are called only-once,
  i.e intr are ignored!
  __asm__ __volatile__ ("sysret");
  restore registers.
  */
  uint32 cid = task_struct[0].id; // current working on debug, tss[0].knlStk should be current rsp/rbp.
  INTR_SAVE_ALL_REGS_TO_TASK_STRUCT(cid); // you dont save rip here. it is meaning less.
  uint64 *stk = task_struct[cid].knlStk;
  //test mode
  stk = task_struct[0].ctx.rsp;
  updateKernelStack(stk);
  //TODO: change LSTAR. get rip, mov lstar, syscall, then it should be where it works.
  __asm__ __volatile__ ("syscall");
  //TODO: privileged inst..
  write_serial(a);
  //TODO: load r11, read rip, load it into rcx, sysret.
  // save current in task_struct i
   //SYSRET
  /*
   * loads RIP from RCX. we do not need these datas, because syscall is done every-time-called.
   * and registers are each-hart specific.
   * */
}