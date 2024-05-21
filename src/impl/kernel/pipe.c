#include "system.h"

#define PIPESIZE 512 // byte

struct pipe {
    char data[PIPESIZE];
    uint32 nread; // bytes read
    uint32 nwrite; // bytes written
    int readopen; // fd-read is open?
    int writeopen; // fd-write is open?
};

int pipealloc(struct file **f0, struct file **f1) {
  struct pipe *pi = 0;
  *f0 = *f1 = 0;
  if ((*f0 = filealloc()) == 0 || (*f1 = filealloc()) == 0)
    goto bad;
  if ((pi = (struct pipe*)kalloc()) == 0)
    goto bad;
  pi->readopen = 1;
  pi->writeopen = 1;
  pi->nwrite = 0;
  pi->nread = 0;
  (*f0)->type = FD_PIPE;
  (*f0)->readable = 1;
  (*f0)->writable = 0;
  (*f0)->pipe = pi;
  (*f1)->type = FD_PIPE;
  (*f1)->readable = 0;
  (*f1)->writable = 1;
  (*f1)->pipe = pi;
  return 0;

  bad:
    if (pi)
      kfree((char *)pi);
    if (*f0)
      fileclose(*f0);
    if (*f1)
      fileclose(*f1);
    return -1;
}

// i donot see a better way to do sleep and wake.
// that is pretty bad.
// we must do some loop shit.
void pipeclose(struct pipe *pi, int writable) {
  if (writable) {
    pi->writeopen = 0;
  } else {
    pi->readopen = 0;
  }
  if (pi->readopen == 0 && pi->writeopen == 0) {
    kfree((char *)pi);
  }
}

/*
 * return bytes successfully written to pipe *pi
 * user, must try to try this in loop, i assume.
 * i donot see a better way to achieve this, our architecture is so different from riscv
 * */
int pipewrite(struct pipe *pi, uint64 addr, int n) {
  int i = 0;
  proc *pr = &task_struct[task_struct[0].id];
  while (i < n) {
    if (pi->readopen == 0 || killed(pr)) {
      return -1;
    }
    if (pi->nwrite == pi->nread + PIPESIZE) { // loop char. full
      return i; // already written..
    }
    char ch;
    if (copyin(pr->pgtbl, &ch, addr + i, 1) == -1)
      break;
    pi->data[pi->nwrite ++ % PIPESIZE] = ch;
  }
  return i;
}

/*
 * same as pipewrite. user call this in a loop way.
 * might have to change read/write syscall module.
 * */
int piperead(struct pipe *pi, uint64 addr, int n) {
  int i = 0;
  proc *pr = &task_struct[task_struct[0].id];
  if (pi->nread == pi->nwrite && pi->writeopen) // pipe empty
    return -1;
  for (i = 0; i < n; i ++) {
    if (pi->nread == pi->nwrite) // pipe empty
      break;
    char ch = pi->data[pi->nread ++ % PIPESIZE];
    if (copyout(pr->pgtbl, addr + i, &ch, i) == -1)
      break;
  }
  return i;
}

