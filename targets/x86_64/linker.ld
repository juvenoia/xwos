ENTRY(start)

SECTIONS
{
    . = 0x7000000;

	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.text :
	{
		*(.text)
	}
}