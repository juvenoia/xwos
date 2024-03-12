ENTRY(start)

SECTIONS
{
	. = 32M;

	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.text :
	{
		*(.text)
	}

	PROVIDE(end = .);
}