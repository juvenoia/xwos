#include "../intf/sysc.h"
.global fork
fork:
 movq $SYS_fork, %r9
 int $0x80
 retq
.global exit
exit:
 movq $SYS_exit, %r9
 int $0x80
 retq
.global wait
wait:
 movq $SYS_wait, %r9
 int $0x80
 retq
.global pipe
pipe:
 movq $SYS_pipe, %r9
 int $0x80
 retq
.global read
read:
 movq $SYS_read, %r9
 int $0x80
 retq
.global write
write:
 movq $SYS_write, %r9
 int $0x80
 retq
.global close
close:
 movq $SYS_close, %r9
 int $0x80
 retq
.global kill
kill:
 movq $SYS_kill, %r9
 int $0x80
 retq
.global exec
exec:
 movq $SYS_exec, %r9
 int $0x80
 retq
.global open
open:
 movq $SYS_open, %r9
 int $0x80
 retq
.global mknod
mknod:
 movq $SYS_mknod, %r9
 int $0x80
 retq
.global unlink
unlink:
 movq $SYS_unlink, %r9
 int $0x80
 retq
.global fstat
fstat:
 movq $SYS_fstat, %r9
 int $0x80
 retq
.global link
link:
 movq $SYS_link, %r9
 int $0x80
 retq
.global mkdir
mkdir:
 movq $SYS_mkdir, %r9
 int $0x80
 retq
.global chdir
chdir:
 movq $SYS_chdir, %r9
 int $0x80
 retq
.global dup
dup:
 movq $SYS_dup, %r9
 int $0x80
 retq
.global getpid
getpid:
 movq $SYS_getpid, %r9
 int $0x80
 retq
.global sbrk
sbrk:
 movq $SYS_sbrk, %r9
 int $0x80
 retq
.global sleep
sleep:
 movq $SYS_sleep, %r9
 int $0x80
 retq
.global uptime
uptime:
 movq $SYS_uptime, %r9
 int $0x80
 retq
.global putc
putc:
 movq $SYS_putc, %r9
 int $0x80
 retq
