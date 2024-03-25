global flush_tss
flush_tss:
	mov ax, (6 * 8) | 0 ; sixth 8-byte selector, symbolically OR-ed with 0 to set the RPL (requested privilege level).
	ltr ax
	ret