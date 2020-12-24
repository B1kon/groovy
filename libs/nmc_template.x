OUTPUT_FORMAT("elf32-nmc", "elf32-nmc", "elf32-nmc")
OUTPUT_ARCH(nmc)
ENTRY(start)

#if defined(RESET_ENTRY) && defined(USE_ONLY_EM)
#error "Configuration mismatch!"
#endif

#if defined(NMC_INDEX) && defined(USE_ONLY_IM)
#error "Configuration mismatch!"
#endif

#ifdef NMC_INDEX
#if (NMC_INDEX < 0) || (NMC_INDEX > 3)
#error "NMC index mismatch!"
#endif
#endif

#if defined(USE_ONLY_IM)
MEMORY
{
#if defined(RESET_ENTRY)
	IMB (rwxa):	 ORIGIN = 0x00000000<<2, LENGTH = 0x00020000<<2
#else
	IMB (rwxa):	 ORIGIN = 0x00000200<<2, LENGTH = 0x0001FE00<<2
#endif
}
#elif defined(USE_ONLY_EM)
MEMORY
{
#if defined(NMC_INDEX)
	EMI (rwxa):	ORIGIN = (0x000D0000 + NMC_INDEX * 0x01FCC000)<<2, LENGTH = 0x01FCC000<<2
#else
	EMI (rwxa):	ORIGIN = 0x000D0000<<2, LENGTH = 0x07f30000<<2
#endif
}
#else
MEMORY
{
#if defined(RESET_ENTRY)
	IMB (rx):	ORIGIN = 0x00000000<<2, LENGTH = 0x00020000<<2
#else
	IMB (rx):	ORIGIN = 0x00000200<<2, LENGTH = 0x0001FE00<<2
#endif

#if defined(NMC_INDEX)
	EMI (wa!rx):	ORIGIN = (0x000D0000 + NMC_INDEX * 0x01FCC000)<<2, LENGTH = 0x01FCC000<<2
#else
	EMI (wa!rx):	ORIGIN = 0x000D0000<<2, LENGTH = 0x07f30000<<2
#endif
}
#endif

/* Supported only ablosute files */

SECTIONS
{
/* Read-only sections, merged into text segment: */
#if defined(USE_ONLY_EM)
	PROVIDE (__executable_start = ORIGIN(EMI));
	. = ORIGIN(EMI);
#else
	PROVIDE (__executable_start = ORIGIN(IMB));
	. = ORIGIN(IMB);
#endif

#if defined(RESET_ENTRY)
	.text.reset_stub :
	{
		*(.text.reset_stub)
		*crt0.o(.text .text.* .gnu.linkonce.t.*)
	}
#endif

	.init :
	{
		KEEP (*(SORT_NONE(.init)))
	}

	.plt : { *(.plt) *(.iplt) }

	.text :
	{
#if defined(RESET_ENTRY)
		*(EXCLUDE_FILE (*crt0.o) .text .text.* .gnu.linkonce.t.*)
#else
		*(.text .text.* .gnu.linkonce.t.*)
#endif

		*(.stub .stub.*)
		*(.gnu.warning)
	}

	.fini :
	{
		KEEP (*(SORT_NONE(.fini)))
	}

	PROVIDE (__etext = .);
	PROVIDE (_etext = .);
	PROVIDE (etext = .);

	.rodata :
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		KEEP (*(.bsst))
	}

	.eh_frame_hdr :
	{
		*(.eh_frame_hdr)
		*(.eh_frame_entry .eh_frame_entry.*)
	}

	.eh_frame : ONLY_IF_RO
	{
		KEEP (*(.eh_frame))
		*(.eh_frame.*)
	}

	.gcc_except_table : ONLY_IF_RO
	{
		*(.gcc_except_table .gcc_except_table.*)
	}

	.gnu_extab : ONLY_IF_RO
	{
		*(.gnu_extab*)
	}

	.exception_ranges : ONLY_IF_RO
	{
		*(.exception_ranges*)
	}

	. = .;

	.eh_frame : ONLY_IF_RW
	{
		KEEP (*(.eh_frame))
		*(.eh_frame.*)
	}

	.gnu_extab : ONLY_IF_RW
	{
		*(.gnu_extab)
	}

	.gcc_except_table : ONLY_IF_RW
	{
		*(.gcc_except_table .gcc_except_table.*)
	}

	.exception_ranges : ONLY_IF_RW
	{
		*(.exception_ranges*)
	}

	.preinit_array :
	{
		PROVIDE_HIDDEN (__preinit_array_start = ABSOLUTE(.));
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = ABSOLUTE(.));
	}

	__fix_1__ = .;  /* Sometimes linker ignore not empty section. */

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = ABSOLUTE(.));
		KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = ABSOLUTE(.));
	}

	__fix_2__ = .;  /* It is occur then use -nostartfiles flag. */

	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = ABSOLUTE(.));
		KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
		KEEP (*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = ABSOLUTE(.));
	}

	__fix_3__ = .;  /* As result after that VMA and LMA is mismatched. */

	.ctors :
	{
		KEEP (*crtbegin.o(.ctors))
		KEEP (*crtbegin?.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
	}

	.dtors :
	{
		KEEP (*crtbegin.o(.dtors))
		KEEP (*crtbegin?.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
	}

	.got : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }

	.data :
	{
		*(.data .data.* .gnu.linkonce.d.*)
		SORT(CONSTRUCTORS)
	}

	_edata = .;
	PROVIDE (edata = .);

	. = .;

	.bss (NOLOAD) :
	{
		__bss_start__ = ABSOLUTE(.);
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		__bss_end__ = ABSOLUTE(.);
	}

	__end__ = .;
	_end = .;
	PROVIDE (end = .);

	. = .;

#ifdef USE_ONLY_IM
	.heap (NOLOAD) : ALIGN(2<<2)
	{
		*(.heap)
		*(.heap1)
		*(.heap2)
		*(.heap3)
		*(.heap4)
		*(.heap5)
		*(.heap6)
		*(.heap7)
		*(.heap8)
		*(.heap9)
		*(.heap10)
		*(.heap11)
		*(.heap12)
		*(.heap13)
		*(.heap14)
		*(.heap15)

		. = 0x2000 << 2;
	}
#else
	.heap (NOLOAD) : ALIGN(2<<2)
	{
		*(.heap)
		*(.heap1)

		*(.heap2)
		. += 0x1000 << 2;
		__HEAP_2_END = .;

		*(.heap3)
		. += 0x1000 << 2;
		__HEAP_3_END = .;

		*(.heap4)
		*(.heap5)
		*(.heap6)
		*(.heap7)
		*(.heap8)
		*(.heap9)
		*(.heap10)
		*(.heap11)
		*(.heap12)
		*(.heap13)
		*(.heap14)
		*(.heap15)

		. = 0x4000 << 2;
	}
#endif

#ifdef USE_ONLY_IM
	.stack (NOLOAD) : ALIGN(2<<2)
	{
		*(.stack)

		. = 0x2000 << 2;

	}
#else
	.stack (NOLOAD) : ALIGN(2<<2)
	{
		*(.stack)

		. = 0x4000 << 2;
	}
#endif

	/* Stabs debugging sections. */
	.stab		0 : { *(.stab) }
	.stabstr	0 : { *(.stabstr) }
	.stab.excl	0 : { *(.stab.excl) }
	.stab.exclstr	0 : { *(.stab.exclstr) }
	.stab.index	0 : { *(.stab.index) }
	.stab.indexstr	0 : { *(.stab.indexstr) }
	.comment	0 : { *(.comment) }

	/* DWARF debug sections.
	   Symbols in the DWARF debugging sections are relative to the beginning
	   of the section so we begin them at 0.
	*/
	/* DWARF 1 */
	.debug		0 : { *(.debug) }
	.line		0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo	0 : { *(.debug_srcinfo) }
	.debug_sfnames	0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges	0 : { *(.debug_aranges) }
	.debug_pubnames	0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info	0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev	0 : { *(.debug_abbrev) }
	.debug_line	0 : { *(.debug_line .debug_line.* .debug_line_end ) }
	.debug_frame	0 : { *(.debug_frame) }
	.debug_str	0 : { *(.debug_str) }
	.debug_loc	0 : { *(.debug_loc) }
	.debug_macinfo	0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	/* DWARF 3 */
	.debug_pubtypes	0 : { *(.debug_pubtypes) }
	.debug_ranges	0 : { *(.debug_ranges) }
	/* DWARF Extension.  */
	.debug_macro	0 : { *(.debug_macro) }

	.gnu.attributes 0:
	{
		KEEP (*(.gnu.attributes))
	}

	/DISCARD/ :
	{
		*(.interp)
		*(.note.gnu.build-id)
		*(.hash)
		*(.gnu.hash)
		*(.dynsym)
		*(.dynstr)
		*(.gnu.version)
		*(.gnu.version_d)
		*(.gnu.version_r)
		*(.rel.*)
		*(.rela.*)
		*(.rodata1)
		*(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
		*(.data1)
		*(.sdata .sdata.* .gnu.linkonce.s.*)
		*(.tdata .tdata.* .gnu.linkonce.td.*)
		*(.tbss .tbss.* .gnu.linkonce.tb.*)
		*(.tcommon)
		*(.jcr)
		*(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*)
		*(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*)
		*(.dynamic)
		*(.dynbss)
		*(.dynsbss)
		*(.sbss .sbss.* .gnu.linkonce.sb.*)
		*(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
		*(.scommon)
		*(.dynlbss)
		*(.lbss .lbss.* .gnu.linkonce.lb.*)
		*(.lrodata .lrodata.* .gnu.linkonce.lr.*)
		*(.ldata .ldata.* .gnu.linkonce.l.*)

		*(.note.GNU-stack)
		*(.gnu_debuglink)
		*(.gnu.lto_*)
	}
}

