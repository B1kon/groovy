OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
	IMB0	: ORIGIN = 0x00000800, LENGTH = 0x00060000

}

ENTRY(_start)

SECTIONS
{      
	PROVIDE (__executable_start = ORIGIN(IMB0));
	. = ORIGIN(IMB0);

      
	.text : 
	{
		PROVIDE_HIDDEN (__vectors_begin = .);
		KEEP (*(.text.vectors_reset))
		PROVIDE_HIDDEN (__vectors_end = .);

		. = ALIGN(8);
		*(.text)
	} > IMB0

	.data :
	{
		. = ALIGN(8);
		*(.data)
	} > IMB0

	.bss :
	{
		. = ALIGN(8);
		__bss_start__ = .;
		*(.bss)
		__bss_end__ = .;
	} > IMB0
	
	.stack (NOLOAD) : 
	{	
		. = ALIGN(8);
		__stack_end = .;
		. += 0x2000;
		__stack_usr = .;
		. += 0x4000;
		__stack_svc = .;
		. += 0x400;
		__stack_und = .;
		. += 0x400;
		__stack_abt = .;
		. += 0x400;
		__stack_fiq = .;
		. += 0x1000;
		__stack_irq = .;
		. += 0x400;
		__stack_mon = .;
		__stack_start = ABSOLUTE(.);
/*		__stack = .; */
	}  > IMB0
}
