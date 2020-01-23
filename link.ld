ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS {
	. = 1M; /* load at 1MB. */

	/* ensure the boot header won't get moved/optimized. */
	.boot : {
		*(.boot)
		KEEP(*(.boot))
	}

	. = ALIGN(1K);

	.text : {
		*(.text)
	}

	.rodata : {
		*(.rodata*)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(COMMON)
		*(.bss)
	}
}
