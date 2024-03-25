global long_mode_start

extern main

section .text
bits 64
long_mode_start:
    ; load null into all data segment registers.
    ; why i cannot load > 0x4 segment into ss?
    ; it will be changed to 0x18, why?
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    call main
	;;mov dword [0xb8000], 0x2f4b2f4f
	; hlt