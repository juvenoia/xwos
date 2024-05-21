#include "system.h"

#define FSMAGIC 0x10203040
// a question: log block is for concurrent disk usage
// in a single cpu version can we simply ignore them? i assume yes.

// Disk layout:
// [ boot block | super block | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
    uint32 magic;        // Must be FSMAGIC
    uint32 size;         // Size of file system image (blocks)
    uint32 nblocks;      // Number of data blocks
    uint32 ninodes;      // Number of inodes.
    uint32 inodestart;   // Block number of first inode block
    uint32 bmapstart;    // Block number of first free map block
};
struct superblock sb;

struct inode* iget(uint32 dev, uint32 inum);

void readsb(int dev, struct superblock *sb) {
  struct buf *bp = bread(dev, 1); // we ignore boot block(bn=0) and find the superblock(bn=1)
  memcpy(sb, bp->data, sizeof (sb));
  brelse(bp);
}

void fsinit(int dev) {
  readsb(dev, &sb);
  if (sb.magic != FSMAGIC) {
    printk("unknown fs system");
    for (;;) ;
  }
}

/*
 * zero a block on disk
 * */
void bzero(int dev, int bno) {
  struct buf *bp = bread(dev, bno);
  memset(bp->data, 0, BSIZE);
  bwrite(bp);
  brelse(bp);
}


/*
 * alloc a block on dev
 * */
uint32 balloc(uint32 dev) {
  for (int b = 0; b < sb.size; b += BPB) { // for each block
    struct buf *bp = bread(dev, BBLOCK(b, sb));
    for (int bi = 0; bi < BPB && b + bi < sb.size; bi ++) {
      int m = 1 << (bi % 8);
      if ((bp->data[bi / 8] & m) == 0) { // this indicate an empty blk
        bp->data[bi / 8] |= m;
        bwrite(bp); // if several proc is using the same buf, which version, should we use?
        brelse(bp);
        bzero(dev, b + bi);
        return b + bi; // the corresponding blk no.
      }
    }
  }
  return 0; // 0 is the boot blk, return 0 indicates this caused a failure.
}

/*
 * free blk b on dev
 * */
void bfree(int dev, uint32 b) {
  struct buf *bp = bread(dev, b);
  int bi = b % BPB;
  int m = 1 << (bi % 8);
  if ((bp->data[bi / 8] & m) != 0) {
    printk("free an unallocated blk");
    for (;;) ;
  }
  bp->data[bi] &= ~m;
  bwrite(bp);
  brelse(bp);
}

// Inodes.
#define NINODE 50
struct {
    struct inode inode[NINODE];
} itable;

/*
 * Usage: allocate an disk-inode on dev with type 'type'
 * return minode pointer to this dinode
 * */
struct inode* ialloc(uint32 dev, uint16 type) {
  for (int inum = 1; inum < sb.ninodes; inum ++) {
    struct buf *bp = bread(dev, IBLOCK(inum, sb));
    struct dinode *dip = (struct dinode*)bp->data + inum % IPB;
    if (dip->type == 0) {
      memset(dip, 0, sizeof (*dip));
      dip->type = type;
      bwrite(bp);
      return iget(dev, inum);
    }
    brelse(bp);
  }
  printk("ialloc: no inodes\n");
  return 0;
}

/*
 * write inode back to disk
 * */
void iupdate(struct inode *ip) {
  struct buf *bp = bread(ip->dev, IBLOCK(ip->inum, sb));
  struct dinode *dip = (struct dinode*)bp->data + ip->inum % IPB;
  dip->type = ip->type;
  dip->major = ip->major;
  dip->minor = ip->minor;
  dip->nlink = ip->nlink;
  dip->size = ip->size;
  memcpy(dip->addrs, ip->addrs, sizeof (ip->addrs));
  bwrite(bp);
  brelse(bp);
}

/*
 * Read inode
 * */
void iread(struct inode *ip) {
  if (ip->valid == 0) {
    // invalid then read
    struct buf *bp = bread(ip->dev, IBLOCK(ip->inum, sb));
    struct dinode *dip = (struct dinode*)bp->data + ip->inum % IPB;
    ip->type = dip->type;
    ip->major = dip->major;
    ip->minor = dip->minor;
    ip->nlink = dip->nlink;
    ip->size = dip->size;
    memcpy(ip->addrs, dip->addrs, sizeof (ip->addrs));
    brelse(bp);
    ip->valid = 1;
    if (ip->type == 0) {
      printk("iread: no type\n");
      for (;;) ;
    }
  }
}

/*
 * find an dinode with no.inum on device dev,
 * read it,
 * return minode
 * */
struct inode* iget(uint32 dev, uint32 inum) {
  struct inode* empty = 0;
  for (struct inode* ip = &itable.inode[0]; ip < &itable.inode[NINODE]; ip ++) {
    if (ip->ref > 0 && ip->dev == dev && ip->inum == inum) {
      // already in mm.
      ip->ref ++;
      return ip;
    }
    if (ip->ref == 0 && empty == 0) {
      empty = ip; // an empty slot
    }
  }
  if (empty == 0) {
    printk("iget: no inodes");
    for (;;) ;
  }

  struct inode* ip = empty;

  ip->dev = dev;
  ip->inum = inum;
  ip->ref = 1;
  ip->valid = 0;

  iread(ip);

  return ip;
}

void iput(struct inode* ip) {
  if (ip->ref == 1 && ip->valid && ip->nlink == 0) {
    itrunc(ip);
    ip->type = 0;
    iupdate(ip);
    ip->valid = 0;
  }
  ip->ref --;
}

/*
 * release inode related disk
 * */
void itrunc(struct inode *ip) {
  for (int i = 0; i < NDIRECT; i ++) {
    if (ip->addrs[i]) { // this indicates a block
      bfree(ip->dev, ip->addrs[i]);
      ip->addrs[i] = 0;
    }
  }
  if (ip->addrs[NDIRECT]) {
    struct buf *bp = bread(ip->dev, ip->addrs[NDIRECT]);
    uint32 *a = (uint32 *)bp->data;
    for (int i = 0; i < NINDIRECT; i ++) {
      if (a[i])
        bfree(ip->dev, a[i]);
    }
    brelse(bp);
    bfree(ip->dev, ip->addrs[NDIRECT]);
    ip->addrs[NDIRECT] = 0;
  }
  ip->size = 0;
  iupdate(ip);
}

/*
 * return the blk number for nth blk in inode ip
 * */
uint32 bmap(struct inode *ip, uint32 bn) {
  if (bn < NDIRECT) { // Direct part is in minode so we donot need to write it back
    uint32 addr = ip->addrs[bn];
    if (addr == 0) {
      addr = balloc(ip->dev);
      if (addr == 0)
        return 0;
      ip->addrs[bn] = addr;
    }
    return addr;
  }
  bn -= NDIRECT;
  if (bn < NINDIRECT) { // however, the NDIRECT part is stored in the disk so we need to manipulate
    uint32 addr = ip->addrs[bn];
    if (addr == 0) {
      addr = balloc(ip->dev);
      if (addr == 0)
        return 0;
      ip->addrs[NDIRECT] = addr;
    }
    struct buf *bp = bread(ip->dev, addr);
    uint32 *a = (uint32 *)bp->data;
    if ((addr = a[bn]) == 0) {
      addr = balloc(ip->dev);
      if (addr) {
        a[bn] = addr;
        bwrite(bp);
      }
    }
    brelse(bp);
    return addr;
  }
  printk("bmap: out of range");
  for (;;) ;
}

void stati(struct inode *ip, struct stat *st) {
  st->dev = ip->dev;
  st->ino = ip->inum;
  st->type = ip->type;
  st->nlink = ip->nlink;
  st->size = ip->size;
}

#define min(a, b) ((a) < (b) ? (a) : (b))

/*
 * read from inode, from the offset-byte, read n bytes, and copy to mm dst
 * return copied bytes
 * */
int readi(struct inode *ip, int user_dst, uint64 dst, uint32 off, uint32 n) {
  uint32 tot = 0;
  if (off > ip->size || off + n < off) {
    // this indicates an invalid bound or a negative param n
    return 0;
  }
  if (off + n > ip->size) {
    n = ip->size - off;
  }
  for (uint64 m = 0; tot < n; tot += m, off += m, dst += m) {
    uint32 addr = bmap(ip, off / BSIZE); // offset byte -> offset-th BLK, at which ip->dev blk?
    if (addr == 0)
      break;
    struct buf *bp = bread(ip->dev, addr);
    m = min(n - tot, BSIZE - off % BSIZE);
    if (either_copyout(user_dst, dst, bp->data + (off % BSIZE), m) == -1) {
      brelse(bp);
      tot = -1;
      break;
    }
    brelse(bp);
  }
  return tot;
}

/*
 * write n bytes to the offset'th byte of inode ip, content at src.
 * user_src=1 indicates a user pgtbl; 0 for kernel space.
 * */
int writei(struct inode *ip, int user_src, uint64 src, uint32 off, uint32 n) {
  if (off > ip->size || off + n < off)
    return -1;
  if (off + n > MAXFILE * BSIZE) // bigger than our fs could stand
    return -1;
  uint32 tot = 0;
  for (uint64 m = 0; tot < n; tot += m, off += m, src += m) {
    uint32 addr = bmap(ip, off / BSIZE); // blk number
    if (addr == 0)
      break;
    struct buf *bp = bread(ip->dev, addr);
    m = min(n - tot, BSIZE - off % BSIZE);
    if (either_copyin(bp->data + (off % BSIZE), user_src, src, m) == -1) {
      brelse(bp);
      break;
    }
    bwrite(bp);
    brelse(bp);
  }
  if (off > ip->size)
    ip->size = off;
  iupdate(ip);
  return tot;
}

// < --- DIRECTORIES --- >
int namecmp(const char *s, const char *t) {
  return strncmp(s, t, DIRSIZ);
}

struct dirent {
    uint16 inum;
    char name[DIRSIZ];
};

/*
 * lookup files in given dir inode
 * dir inode contains many dirent struct
 * */
struct inode* dirloopup(struct inode *dp, char *name, uint64 *poff) {
  if (dp->type != T_DIR) {
    printk("dirlookup not DIR");
    for (;;) ;
  }
  struct dirent de;
  for (uint32 off = 0; off < dp->size; off += sizeof (de)) {
    if (readi(dp, 0, (uint64)&de, off, sizeof(de)) != sizeof (de)) {
      printk("dirlook readi");
      for (;;) ;
    }
    if (de.inum == 0)
      continue; // this stub is empty or deleted
    if (namecmp(name, de.name) == 0) {
      if (poff)
        *poff = off;
      return iget(dp->dev, de.inum);
    }
  }
  return 0;
}

int dirlink(struct inode *dp, char *name, uint32 inum) {
  struct inode *ip;
  if ((ip = dirloopup(dp, name, 0)) != 0) {
    // found this name. you should not mkdir this twice
    iput(ip);
    return -1;
  }
  int off;
  struct dirent de;
  for (off = 0; off < dp->size; off += sizeof (de)) {
    if (readi(dp, 0, (uint64) &de, off, sizeof (de)) != sizeof (de)) {
      printk("dirlink read");
      for (;;) ;
    }
    if (de.inum == 0)
      break;
  }
  // found an empty dir content stub
  strncpy(de.name, name, DIRSIZ);
  de.inum = inum;
  // write this inode back
  if (writei(dp, 0, (uint64)&de, off, sizeof(de)) != sizeof (de))
    return -1;
  return 0;
}

// < --- Paths --- >

/*
 * return next level char,
 * name-> return current dir name
 * Examples:
 * skipelem("a/bb/c", name) = "bb/c", name = a
 * */
char* skipelem(char *path, char *name) {
  while (*path == '/')
    path ++;
  if (*path == 0)
    return 0; // '\0' indiacates an end
  char *s = path;
  while (*path != '/' && *path != 0)
    path ++;
  uint32 len = path - s;
  if (len >= DIRSIZ)
    memcpy(name, s, DIRSIZ);
  else {
    memcpy(name, s, len);
    name[len] = 0;
  }
  while (*path == '/')
    path ++;
  return path;
}

struct inode *idup(struct inode *ip) {
  ip->ref ++;
  return ip;
}

struct inode* namex(char *path, int nameiparent, char *name) {
  struct inode *ip, *next;
  if (*path == '/')
    ip = iget(ROOTDEV, ROOTINO);
  else
    ip = idup(task_struct[task_struct[0].id].cwd);

  while ((path = skipelem(path, name)) != 0) {
    iread(ip); // read current ip
    if (ip->type != T_DIR) {
      iput(ip);
      return 0; // not a DIR. stop
    }
    if (nameiparent && *path == '\0') {
      // early stop. ip from parent
      iput(ip);
      return ip;
      // examples:
      // b/c: path=c, name=b.
      // c: path='', name=c.
    }
    if ((next = dirloopup(ip, name, 0)) == 0) {
      iput(ip); // next dir donot exist
      return 0;
    }
    iput(ip);
    ip = next;
  }
  if (nameiparent) {
    // you come here only when you do not have ny parent
    iput(ip);
    return 0;
  }
  return ip;
}

struct inode* namei(char *path) {
  char name[DIRSIZ];
  return namex(path, 0, name);
}

struct inode* nameiparent(char *path, char *name) {
  return namex(path, 1, name);
}