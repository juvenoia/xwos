global long_mode_start

extern main

section .text
bits 64
long_mode_start:
    ; load null into all data segment registers.
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call main
	;;mov dword [0xb8000], 0x2f4b2f4f
	; hlt