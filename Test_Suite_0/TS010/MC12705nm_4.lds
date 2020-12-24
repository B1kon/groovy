OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)

MEMORY
{
	IMB0	: ORIGIN = 0x40000<<2, LENGTH = 0x80000
}

ENTRY(_start)

SECTIONS
{      
	.text : 
	{	KEEP ( *(.text_vectors) )
		. = ALIGN(8);
		*(.text)
	} > IMB0

	.data :
	{	. = ALIGN(8);
		*(.data)
	} > IMB0         

	.bss :
	{	. = ALIGN(8);
		*(.bss)
	} > IMB0

	.stack (NOLOAD) : 
	{	. = ALIGN(8);
		*(.stack) 
	}  > IMB0
}
