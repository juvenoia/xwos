#include "system.h"
#include "sysc.h"

// Prototypes for the functions that handle system calls.
extern uint64 sys_fork(void);
//extern uint64 sys_exit(void);
//extern uint64 sys_wait(void);
//extern uint64 sys_pipe(void);
extern uint64 sys_read(void);
//extern uint64 sys_kill(void);
//extern uint64 sys_exec(void);
//extern uint64 sys_fstat(void);
//extern uint64 sys_chdir(void);
extern uint64 sys_dup(void);
//extern uint64 sys_getpid(void);
//extern uint64 sys_sbrk(void);
//extern uint64 sys_sleep(void);
//extern uint64 sys_uptime(void);
//extern uint64 sys_open(void);
extern uint64 sys_write(void);
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
        [SYS_read]    sys_read,
//        [SYS_kill]    sys_kill,
//        [SYS_exec]    sys_exec,
//        [SYS_fstat]   sys_fstat,
//        [SYS_chdir]   sys_chdir,
        [SYS_dup]     sys_dup,
//        [SYS_getpid]  sys_getpid,
//        [SYS_sbrk]    sys_sbrk,
//        [SYS_sleep]   sys_sleep,
//        [SYS_uptime]  sys_uptime,
//        [SYS_open]    sys_open,
        [SYS_write]   sys_write,
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
        LOAD_REG(rbx, (context)->rbx); \
        LOAD_REG(rcx, (context)->rcx); \
        LOAD_REG(rsi, (context)->rsi); \
        LOAD_REG(r8, (context)->r8); \
        LOAD_REG(r9, (context)->r9); \
        LOAD_REG(r10, (context)->r10); \
        LOAD_REG(r11, (context)->r11); \
        LOAD_REG(r12, (context)->r12); \
        LOAD_REG(r13, (context)->r13); \
        LOAD_REG(r14, (context)->r14); \
        LOAD_REG(r15, (context)->r15); \
        LOAD_REG(rdx, (context)->rdx); \
        LOAD_REG(rdi, (context)->rdi); \
        LOAD_REG(rax, (context)->rax); \
    }

void sys_stub() {
  // you first save your programs' runnin ctx!
  // only those in regs.
  // int c = task_struct[0].id; // current running process
  SAVE_ALL_CONTEXT(&task_struct[0].ctx);
  for (uint64 *va = task_struct[task_struct[0].id].knlStk - 5, i = 0; i < 5; i ++) {
    task_struct[0].ctx.stk[i] = va[i];
  }
  uint64 *rbp = task_struct[task_struct[0].id].knlStk - 6;
  task_struct[0].ctx.rbp = *rbp; // real rbp is saved at knlStk[-6]
  task_struct[task_struct[0].id].ctx = task_struct[0].ctx;
  swtchPgtbl(task_struct[task_struct[0].id].kpgtbl); // change pgtbl to kernel, if you need to modify everything.
  // find syscall number
  // rdi, rsi, rdx, rcx, r8, r9
  syscalls[task_struct[0].ctx.r9]();
  swtchPgtbl(task_struct[task_struct[0].id].pgtbl); // change back.
  *rbp = task_struct[task_struct[0].id].ctx.rbp;
  task_struct[0].ctx = task_struct[task_struct[0].id].ctx;
  for (uint64 *va = task_struct[task_struct[0].id].knlStk - 5, i = 0; i < 5; i ++) {
    va[i] = task_struct[0].ctx.stk[i];
  }
  LOAD_ALL_CONTEXT(&(task_struct[0].ctx)); // load the original ctx. load rax at last.
  return; // now go to isrs.asm
  // e=0138
  // 0000 0001 0011 1 000
  // internal, GDT, 7 + 32 = 39. the last one?
}

int ticks = 0;

void timer_stub() {
  //a call 'timer_stub' will add rsp. we need to restore that.
  //rsp
  // you first change to kernel pgtbl, but i am not sure if this will cause trouble?
  SAVE_ALL_CONTEXT(&task_struct[0].ctx);
  for (uint64 *va = task_struct[task_struct[0].id].knlStk - 5, i = 0; i < 5; i ++) {
    task_struct[0].ctx.stk[i] = va[i];
  }
  uint64 *rbp = task_struct[task_struct[0].id].knlStk - 6;
  task_struct[0].ctx.rbp = *rbp; // real rbp is saved at knlStk[-6]
  task_struct[task_struct[0].id].ctx = task_struct[0].ctx;
  swtchPgtbl(task_struct[task_struct[0].id].kpgtbl); // change pgtbl to kernel, if you need to modify everything.
  // you save current ctx in 0, and copy into corres.
  ticks ++;
  int p = task_struct[0].id;
  int done = 0;
  if (ticks % 18 == 0) {
    done = sched();
    ticks = 0;
  }
  // which knlStk should it be? should be the current running knlstk.
  outportb(0x20, 0x20);
  swtchPgtbl(task_struct[task_struct[0].id].pgtbl); // change back.
  *rbp = task_struct[task_struct[0].id].ctx.rbp;
  task_struct[0].ctx = task_struct[task_struct[0].id].ctx;
  // printk("%p\n", task_struct[0].ctx.stk[0]); //what caused the trouble?
  // a stop at push rbp will cause the trouble. i donot have idea why now.
  for (uint64 *va = task_struct[p].knlStk - 5, i = 0; i < 5; i ++) {
    va[i] = task_struct[0].ctx.stk[i];
  }
  // you should, restore rbp here, but not below.
  LOAD_ALL_CONTEXT(&(task_struct[0].ctx)); // load the original ctx.
  return; // now go to isrs.asm/timer_stub(isrs0)
}

int swtchCnt = 0;

int sched() {
  int done = 0;
  for (int i = 1; i < NPROC; i ++) {
    if (i != task_struct[0].id && task_struct[i].state == PROC_RUNNABLE) {
      task_struct[task_struct[0].id].state = PROC_RUNNABLE; // previous task paused
      task_struct[0].id = i; // this indicates current running PROC.
      task_struct[i].state = PROC_RUNNING; // change it to running, the ctx will be loaded later.
      updateKernelStack(task_struct[i].knlStk);
      done = 1;
      swtchCnt ++;
      break;
    }
  }
  return done;
}