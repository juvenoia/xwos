; note: this code is for 64-bit long mode only.
;       it is unknown if it works in protected mode.
;       using intel assembly style
global jump_usermode
extern user_test
jump_usermode:
;enable system call extensions that enables sysret and syscall
	; enable syscall
    mov rcx, 0xc0000082
    wrmsr
    mov rcx, 0xc0000080
    rdmsr
    or eax, 1
    wrmsr
    mov rcx, 0xc0000081
    rdmsr
    mov edx, 0x00180008
    wrmsr
	mov rcx, user_test ; to be loaded into RIP
	mov r11, 0x202 ; to be loaded into EFLAGS
	;ret
	;sysret ;use "o64 sysret" if you assemble with NASM
	o64 sysret