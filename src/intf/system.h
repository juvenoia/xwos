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
extern int strncmp(const char *p, const char *q, uint32 n);
extern char* strncpy(char *s, const char *t, int n);

/* IDT.C */
extern void idt_set_gate(unsigned char num, unsigned long long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* ISRS.C */
extern void isrs_install();

/* IRQ.C */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_install();

/* TIMER.C */
extern void timer_wait(int ticks);
extern void timer_install();

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

#define NOFILE       16  // open files per process

typedef struct {
    int id; // proc id
    ctx_t ctx;
    pagetable_t *pgtbl, *kpgtbl;
    uint64 *knlStk;
    int state;
    struct inode* cwd;
    int killed;
    void *chan; // use for wait and sleep
    struct file *ofile[NOFILE];  // Open files
}proc;

#define NPROC 256 // currently only 256 tasks are allowed
extern proc task_struct[];
extern void procinit();
extern int killed(proc *pr);
extern void wakeup(void *chan);
extern void sleep(void *chan);

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
extern int either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
extern int either_copyin(void *dst, int user_src, uint64 src, uint64 len);
extern int copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len);
extern int copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len);


/* DISK.C */
extern void read_sectors(uint8 *buffer, uint32 sector, uint8 count, int is_master);
extern void write_sectors(uint8 *buffer, uint32 sector, uint8 count, int is_master);

/* BUFFER.C */
#define BSIZE 512
struct buf {
    int valid;
    int disk;
    uint32 dev;
    uint32 blockno;
    uint32 refcnt;
    struct buf *prev;
    struct buf *next;
    uint8 data[BSIZE];
};
extern void binit();
extern struct buf* bread(uint32 dev, uint32 blockno); // get a buf and read
extern void bwrite(struct buf *b); // write a buf to disk
extern void brelse(struct buf *b); // release a buf after usage

/* FS.C */

// block per bitmap = BSIZE * 8
#define BPB BSIZE * 8
// block b所对应的bitmap所在的磁盘块
#define BBLOCK(b, sb) ((b) / BPB + sb.bmapstart)
// inode 所在的磁盘块
#define IBLOCK(i, sb) ((i) / IPB + sb.inodestart)
// 每个 block的disk inode个数
#define IPB (BSIZE / sizeof(struct dinode))
// inode对应的文件block 表。
#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint32))
#define MAXFILE (NDIRECT + NINDIRECT)

// in-memory copy of an inode
struct inode {
    uint32 dev;           // Device number
    uint32 inum;          // Inode number
    int ref;            // Reference count
    int valid;          // inode has been read from disk?

    uint16 type;         // copy of disk inode
    uint16 major;
    uint16 minor;
    uint16 nlink;
    uint32 size;
    uint32 addrs[NDIRECT+1];
};

// On-disk inode structure
struct dinode {
    uint16 type;           // File type
    uint16 major;          // Major device number (T_DEVICE only)
    uint16 minor;          // Minor device number (T_DEVICE only)
    uint16 nlink;          // Number of links to inode in file system
    uint32 size;            // Size of file (bytes)
    uint32 addrs[NDIRECT+1];   // Data block addresses
};

#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

struct stat {
    int dev;     // File system's disk device
    uint32 ino;    // Inode number
    short type;  // Type of file
    short nlink; // Number of links to file
    uint64 size; // Size of file in bytes
};

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

#define ROOTINO 1   // root i-number
#define ROOTDEV 1

extern int readi(struct inode *ip, int user_dst, uint64 dst, uint32 off, uint32 n);
extern int writei(struct inode *ip, int user_src, uint64 src, uint32 off, uint32 n);
/* FILE.C */
#define NDEV         10  // maximum major device number
#define NFILE       100  // open files per system
// map major device number to device functions.
struct devsw {
    int (*read)(int, uint64, int);
    int (*write)(int, uint64, int);
};

struct file {
    enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;
    int ref; // reference count
    char readable;
    char writable;
    struct pipe *pipe; // FD_PIPE
    struct inode *ip;  // FD_INODE and FD_DEVICE
    uint32 off;          // FD_INODE
    short major;       // FD_DEVICE
};

extern void fileclose(struct file *f);
extern struct file* filedup(struct file *f);
extern int fileread(struct file *f, uint64 addr, int n);
extern int filewrite(struct file *f, uint64 addr, int n);
/* PIPE.C */
extern void pipeclose(struct pipe *pi, int writable);
extern int pipewrite(struct pipe *pi, uint64 addr, int n);
extern int piperead(struct pipe *pi, uint64 addr, int n);

#endif
