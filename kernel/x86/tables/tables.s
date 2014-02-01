[GLOBAL gdt_flush]

gdt_flush:
	mov eax, [esp + 4]
	lgdt [eax]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x8:.flush
.flush:
	ret

[GLOBAL idt_flush]

idt_flush:
   mov eax, [esp+4]
   lidt [eax]
   ret

[GLOBAL tss_flush]

tss_flush:
	; Load the index of our TSS structure - The index is
	; 0x28, as it is the 5th selector and each is 8 bytes
	; long, but we set the bottom two bits (making 0x2B)
	; so that it has an RPL of 3, not zero.

	mov ax, 0x2B
	ltr ax
	ret
