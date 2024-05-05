#include "system.h"

// Prototypes for the functions that handle system calls.
extern uint64 sys_fork(void);
//extern uint64 sys_exit(void);
//extern uint64 sys_wait(void);
//extern uint64 sys_pipe(void);
//extern uint64 sys_read(void);
//extern uint64 sys_kill(void);
//extern uint64 sys_exec(void);
//extern uint64 sys_fstat(void);
//extern uint64 sys_chdir(void);
//extern uint64 sys_dup(void);
//extern uint64 sys_getpid(void);
//extern uint64 sys_sbrk(void);
//extern uint64 sys_sleep(void);
//extern uint64 sys_uptime(void);
//extern uint64 sys_open(void);
//extern uint64 sys_write(void);
//extern uint64 sys_mknod(void);
//extern uint64 sys_unlink(void);
//extern uint64 sys_link(void);
//extern uint64 sys_mkdir(void);
//extern uint64 sys_close(void);
extern uint64 sys_putc(void);

// An array mapping syscall numbers from syscall.h
// to the function that handles the system call.
static uint64 (*syscalls[])(void) = {
        [SYS_fork]    sys_fork,
//        [SYS_exit]    sys_exit,
//        [SYS_wait]    sys_wait,
//        [SYS_pipe]    sys_pipe,
//        [SYS_read]    sys_read,
//        [SYS_kill]    sys_kill,
//        [SYS_exec]    sys_exec,
//        [SYS_fstat]   sys_fstat,
//        [SYS_chdir]   sys_chdir,
//        [SYS_dup]     sys_dup,
//        [SYS_getpid]  sys_getpid,
//        [SYS_sbrk]    sys_sbrk,
//        [SYS_sleep]   sys_sleep,
//        [SYS_uptime]  sys_uptime,
//        [SYS_open]    sys_open,
//        [SYS_write]   sys_write,
//        [SYS_mknod]   sys_mknod,
//        [SYS_unlink]  sys_unlink,
//        [SYS_link]    sys_link,
//        [SYS_mkdir]   sys_mkdir,
//        [SYS_close]   sys_close,
        [SYS_putc]    sys_putc,
};

#define SAVE_CONTEXT(reg, val) \
    __asm__ __volatile__ ( \
        "movq %%" #reg ", %0\n\t" \
        : "=m" (val) \
        : \
        : \
    )

#define SAVE_ALL_CONTEXT(context) \
    { \
        SAVE_CONTEXT(rax, (context)->rax); \
        SAVE_CONTEXT(rbx, (context)->rbx); \
        SAVE_CONTEXT(rcx, (context)->rcx); \
        SAVE_CONTEXT(rdx, (context)->rdx); \
        SAVE_CONTEXT(rsi, (context)->rsi); \
        SAVE_CONTEXT(rdi, (context)->rdi); \
        SAVE_CONTEXT(rbp, (context)->rbp); \
        SAVE_CONTEXT(r8, (context)->r8); \
        SAVE_CONTEXT(r9, (context)->r9); \
        SAVE_CONTEXT(r10, (context)->r10); \
        SAVE_CONTEXT(r11, (context)->r11); \
        SAVE_CONTEXT(r12, (context)->r12); \
        SAVE_CONTEXT(r13, (context)->r13); \
        SAVE_CONTEXT(r14, (context)->r14); \
        SAVE_CONTEXT(r15, (context)->r15); \
    }

#define LOAD_REG(reg, val) \
    __asm__ __volatile__ ("movq %0, %%" #reg :: "r" (val))

#define LOAD_ALL_CONTEXT(context) \
    { \
        LOAD_REG(rax, (context)->rax); \
        LOAD_REG(rbx, (context)->rbx); \
        LOAD_REG(rcx, (context)->rcx); \
        LOAD_REG(rdx, (context)->rdx); \
        LOAD_REG(rsi, (context)->rsi); \
        LOAD_REG(rdi, (context)->rdi); \
        LOAD_REG(rbp, (context)->rbp); \
        LOAD_REG(r8, (context)->r8); \
        LOAD_REG(r9, (context)->r9); \
        LOAD_REG(r10, (context)->r10); \
        LOAD_REG(r11, (context)->r11); \
        LOAD_REG(r12, (context)->r12); \
        LOAD_REG(r13, (context)->r13); \
        LOAD_REG(r14, (context)->r14); \
        LOAD_REG(r15, (context)->r15); \
    }

void sys_stub() {
  // you first save your programs' runnin ctx!
  // only those in regs.
  // int c = task_struct[0].id; // current running process
  SAVE_ALL_CONTEXT(&task_struct[0].ctx);
  for (uint64 *va = task_struct[task_struct[0].id].knlStk - 5, i = 0; i < 5; i ++) {
    task_struct[0].ctx.stk[i] = va[i];
  }
  task_struct[task_struct[0].id].ctx = task_struct[0].ctx;
  // find syscall number
  int cn;
  SAVE_CONTEXT(r9, cn);
  LOAD_REG(r9, syscalls[cn]);
  __asm__ __volatile__ (
          "call *%%r9\n\t"
          :
          :
          : "memory"
          );
  task_struct[0].ctx = task_struct[task_struct[0].id].ctx;
  LOAD_ALL_CONTEXT(&(task_struct[0].ctx)); // load the original ctx.
  return; // now go to isrs.asm
}

int ticks = 0;

void timer_stub() {
  //a call 'timer_stub' will add rsp. we need to restore that.
  //rsp
  SAVE_ALL_CONTEXT(&task_struct[0].ctx);
  uint64 *knlStk = task_struct[task_struct[0].id].knlStk;
  for (uint64 *va = knlStk - 5, i = 0; i < 5; i ++) {
    task_struct[0].ctx.stk[i] = va[i];
  }
  task_struct[task_struct[0].id].ctx = task_struct[0].ctx;
  // you save current ctx in 0, and copy into corres.
  ticks ++;
  int done = 0;
  if (ticks % 18 == 0) {
    done = sched();
    ticks = 0;
  }
  task_struct[0].ctx = task_struct[task_struct[0].id].ctx;
  outportb(0x20, 0x20);
  // change all that saved in stk..
  // which knlStk should it be? should be the current running knlstk.
  for (uint64 *va = knlStk - 5, i = 0; i < 5; i ++) {
    va[i] = task_struct[task_struct[0].id].ctx.stk[i];
  }
  LOAD_ALL_CONTEXT(&(task_struct[0].ctx)); // load the original ctx.
  return; // now go to isrs.asm/timer_stub(isrs0)
}

int sched() {
  int done = 0;
  for (int i = 1; i < NPROC; i ++) {
    if (i != task_struct[0].id && task_struct[i].state == PROC_RUNNABLE) {
      //task_struct[task_struct[0].id].state = PROC_RUNNABLE; // previous task paused
      task_struct[0].id = i; // this indicates current running PROC.
      task_struct[0].state = PROC_RUNNING; // change it to running, the ctx will be loaded later.
      // swtchPgtbl(task_struct[i].pgtbl); // change pgtbl first you try donot change pgtbl
      done = 1;
      break;
    }
  }
  return done;
}