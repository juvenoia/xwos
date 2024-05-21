# xwos
a x86-64 arch 64-bit kernel, for personal study.

## usage:
docker build buildenv -t myos-buildenv \
docker run --rm -it -v $(pwd):/root/env  myos-buildenv \
make build-x86_64 \
qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso -nographic

### already done:
64-bit mode, \
idt, \
intr & irq process, \
8250 based printk, \
kalloc & kfree, \
syscall stub, \
k/upgtbl, \
task_struct, \
sched,
###  TODO:
wakeup & sleep (i.e. sched), \
adding fs syscall, \




