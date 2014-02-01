MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_VIDEO			equ 1<<2
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

KERNEL_STACK_SIZE equ 8192

[BITS 32]

[GLOBAL mboot]
[EXTERN kernel_code]
[EXTERN kernel_bss]
[EXTERN kernel_end]

mboot:
	dd  MBOOT_HEADER_MAGIC
	dd  MBOOT_HEADER_FLAGS
	dd  MBOOT_CHECKSUM
	
	dd  mboot
	dd  kernel_code
	dd  kernel_bss
	dd  kernel_end
	dd  hal_start

[GLOBAL hal_start]
[EXTERN hal_main]

hal_start:
	cli
	mov     esp, kernel_stack + KERNEL_STACK_SIZE
	push    ebx
	call    hal_main
	cli
	hlt
	jmp $

[SECTION .bss]
[GLOBAL kernel_stack]
[GLOBAL kernel_stack_top]
kernel_stack:
	resb KERNEL_STACK_SIZE
kernel_stack_top:
	resb 4
