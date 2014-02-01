ENTRY(hal_start)
SECTIONS
{
  .text 0x100000 :
  {
    kernel_code = .;
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     kernel_data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    kernel_bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  kernel_end = .;
}