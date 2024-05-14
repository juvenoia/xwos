#include "src/intf/sysc.h"
global fork
fork:
 mov r9, SYS_fork
 int 0x80
 iretq
global exit
exit:
 mov r9, SYS_exit
 int 0x80
 iretq
global waite
waite:
 mov r9, SYS_waite
 int 0x80
 iretq
global pipe
pipe:
 mov r9, SYS_pipe
 int 0x80
 iretq
global read
read:
 mov r9, SYS_read
 int 0x80
 iretq
global write
write:
 mov r9, SYS_write
 int 0x80
 iretq
global close
close:
 mov r9, SYS_close
 int 0x80
 iretq
global kill
kill:
 mov r9, SYS_kill
 int 0x80
 iretq
global exec
exec:
 mov r9, SYS_exec
 int 0x80
 iretq
global open
open:
 mov r9, SYS_open
 int 0x80
 iretq
global mknod
mknod:
 mov r9, SYS_mknod
 int 0x80
 iretq
global unlink
unlink:
 mov r9, SYS_unlink
 int 0x80
 iretq
global fstat
fstat:
 mov r9, SYS_fstat
 int 0x80
 iretq
global link
link:
 mov r9, SYS_link
 int 0x80
 iretq
global mkdir
mkdir:
 mov r9, SYS_mkdir
 int 0x80
 iretq
global chdir
chdir:
 mov r9, SYS_chdir
 int 0x80
 iretq
global dup
dup:
 mov r9, SYS_dup
 int 0x80
 iretq
global getpid
getpid:
 mov r9, SYS_getpid
 int 0x80
 iretq
global sbrk
sbrk:
 mov r9, SYS_sbrk
 int 0x80
 iretq
global sleep
sleep:
 mov r9, SYS_sleep
 int 0x80
 iretq
global uptime
uptime:
 mov r9, SYS_uptime
 int 0x80
 iretq
