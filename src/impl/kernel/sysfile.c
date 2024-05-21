#include "system.h"


// rdi, rsi, rdx, rcx, r8, r9
uint64 arg(int i) {
  switch (i) {
    case 1:
      return task_struct[0].ctx.rdi;
    case 2:
      return task_struct[0].ctx.rsi;
    case 3:
      return task_struct[0].ctx.rdx;
    case 4:
      return task_struct[0].ctx.rcx;
    case 5:
      return task_struct[0].ctx.r8;
    case 6:
      return task_struct[0].ctx.r9;
    default:
      return 0;
  }
}

int argfd(int n, int *pfd, struct file **pf) {
  int fd;
  struct file *f;

  fd = arg(n);
  if (fd < 0 || fd >= NOFILE || (f = task_struct[task_struct[0].id].ofile[fd]) == 0)
    return -1;
  if (pfd)
    *pfd = fd;
  if (pf)
    *pf = f;
  return 0;
}

int fdalloc(struct file *f) {
  int fd;
  proc *p = &task_struct[task_struct[0].id];

  for (fd = 0; fd < NOFILE; fd ++) {
    if (p->ofile[fd] == 0) {
      p->ofile[fd] = f;
      return fd;
    }
  }
  return -1;
}

// file dup syscall
uint64 sys_dup() {
  int fd;
  struct file *f;
  if (argfd(1, 0, &f) < 0) {
    task_struct[task_struct[0].id].ctx.rax = -1;
    return 0;
  }
  if ((fd = fdalloc(f)) < 0) {
    task_struct[task_struct[0].id].ctx.rax = -1;
    return 0;
  }
  filedup(f);
  task_struct[task_struct[0].id].ctx.rax = fd;
  return 0;
}

uint64 sys_read(void) {
  struct file *f;
  int n = arg(2);
  uint64 p = arg(3);
  if (argfd(0, 0, &f) < 0) {
    task_struct[task_struct[0].id].ctx.rax = -1;
    return 0;
  }
  task_struct[task_struct[0].id].ctx.rax = fileread(f, p, n);
  return 0;
}

uint64 sys_write(void) {
  struct file *f;
  int n = arg(2);
  uint64 p = arg(3);
  if (argfd(0, 0, &f) < 0) {
    task_struct[task_struct[0].id].ctx.rax = -1;
    return 0;
  }
  task_struct[task_struct[0].id].ctx.rax = filewrite(f, p, n);
  return 0;
}

uint64 sys_close(void) {
  int fd;
  struct file *f;
  if (argfd(0, &fd, &f) < 0) {
    task_struct[task_struct[0].id].ctx.rax = -1;
    return 0;
  }
  task_struct[task_struct[0].id].ofile[fd] = 0;
  fileclose(f);
  task_struct[task_struct[0].id].ctx.rax = 0;
  return 0;
}

uint64 sys_link(void) {

}