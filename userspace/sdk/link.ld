OUTPUT_FORMAT(binary)

SEARCH_DIR(sdk/lib)

STARTUP(crt0.o)
ENTRY(_start)

SECTIONS
{
	.text.start (0x80000000) : {
		crt0.o( .text )
	}
	.text : ALIGN(4) {
		*(.text)
	}
	.data : ALIGN(4) {
		*(.data)
	}
	.bss : ALIGN(4) {
		*(.bss)
	}
}
