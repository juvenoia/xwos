#include "system.h"

struct devsw devsw[NDEV];

struct {
    struct file file[NFILE];
} ftable;


/*
 * allocate a new stub for file structure
 * */
struct file* filealloc() {
  for (struct file *f = ftable.file; f < ftable.file + NFILE; f ++) {
    if (f->ref == 0) {
      f->ref = 1;
      return f;
    }
  }
  return 0;
}

struct file* filedup(struct file *f) {
  if (f->ref < 1) {
    printk("filedup");
    for (;;) ;
  }
  f->ref ++;
  return f;
}

void fileclose(struct file *f) {
  if (f->ref < 1) {
    printk("fileclose");
    for (;;) ;
  }
  if (--f->ref > 0)
    return;
  struct file ff = *f;
  f->ref = 0;
  f->type = FD_NONE;
  if (ff.type == FD_PIPE) {
    pipeclose(ff.pipe, ff.writable);
  } else if (ff.type == FD_PIPE || ff.type == FD_DEVICE) {
    iput(ff.ip);
  }
}

/*
 * stat file into addr(user va)
 * */
int filestat(struct file *f, uint64 addr) {
  proc *p = &task_struct[task_struct[0].id];
  struct stat st;
  if (f->type == FD_INODE || f->type == FD_DEVICE) {
    stati(f->ip, &st);
    if (copyout(p->pgtbl, addr, (char *)&st, sizeof (st)) < 0)
      return -1;
    return 0;
  }
  return -1;
}

/*
 * read from file f
 * */
int fileread(struct file *f, uint64 addr, int n) {
  int r = 0;
  if (f->readable == 0)
    return -1; // read an unreadable file
  if (f->type == FD_PIPE) {
    r = piperead(f->pipe, addr, n);
  } else if (f->type == FD_DEVICE) {
    if (f->major < 0 || f->major >= NDEV || !devsw[f->major].read)
      return -1;
    r = devsw[f->major].read(1, addr, n);
  } else if (f->type == FD_INODE) {
    if ((r = readi(f->ip, 1, addr, f->off, n)) > 0)
      f->off += r;
  } else {
    printk("fileread");
    for (;;) ;
  }
}

int filewrite(struct file *f, uint64 addr, int n) {
  int r, ret = 0;
  if (f->writable == 0)
    return -1;
  if (f->type == FD_PIPE) {
    ret = pipewrite(f->pipe, addr, n);
  } else if (f->type == FD_DEVICE) {
    if (f->major < 0 || f->major >= NDEV || !devsw[f->major].write)
      return -1;
    devsw[f->major].write(1, addr, n);
  } else if (f->type == FD_INODE) {
    // write in progress, or it will overwhelm the log level ability
    // since we do not have log level, we just simply ignore it
    if ((r = writei(f->ip, 1, addr, f->off, n)) > 0)
      f->off += r;
    ret = (r == n)? n: -1;
  } else {
    printk("filewrite");
    for (;;) ;
  }
  return ret;
}