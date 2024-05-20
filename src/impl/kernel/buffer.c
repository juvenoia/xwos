#include "system.h"

#define NBUF 30
// a disk block is 1024B
// idk why NBUF is associated with

/*
 * BUFFER LAYER
 * Interface:
 * bread(dev, blockno): allocate buf and read disk
 * bwrite(buf): write it back to disk
 * brelse(buf): release refcnt and nobody use it.
 * since everyone is using the same buf, synchronization is since done.
 * */

struct {
    struct buf buf[NBUF];
    // a linkedlist for all buffers. head-node is invalid.
    struct buf head;
} bcache;

void binit(void) {
  // initially, it is a empty linkedlist
  bcache.head.prev = &bcache.head;
  bcache.head.next = &bcache.head;
  for (struct buf *b = bcache.buf; b != bcache.buf + NBUF; b ++) {
    // insert h right after bcache.head
    b->prev = &bcache.head;
    b->next = &bcache.head.next;
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
}

/*
 * find a buffer on dev->blockno.
 * */
struct buf* bget(uint32 dev, uint32 blockno) {
  // find any allocated buf
  for (struct buf *b = bcache.buf; b != bcache.buf + NBUF; b ++) {
    if (b->dev == dev && b->blockno == blockno)
      return b;
  }
  // not find, then you allocate one
  for (struct buf *b = bcache.buf; b != bcache.buf + NBUF; b ++) {
    if (b->refcnt == 0) {
      // not used
      b->refcnt = 1;
      b->dev = dev;
      b->blockno = blockno;
      b->valid = 0;
      return b;
    }
  }
  printk("buf if full\n");
  for (;;) ;
}

struct buf* bread(uint32 dev, uint32 blockno) {
  // our fs dev no. is always 1.
  struct buf* b = bget(dev, blockno);
  if (!b->valid) {
    // now read!
    read_sectors(b->data, blockno, 1, dev);
    b->valid = 1;
  }
  return b;
}

/*
 * write a buffer back to disk.
 * */
void bwrite(struct buf *b) {
  write_sectors(b->data, b->blockno, 1, b->dev);
}

void brelse(struct buf *b) {
  b->refcnt --;
  if (b->refcnt == 0) {
    // 1. write back to disk
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
}

void bpin(struct buf *b) {
  b->refcnt ++;
}

void bunpin(struct buf *b) {
  b->refcnt --;
}