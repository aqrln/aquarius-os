[GLOBAL read_cr0]
[GLOBAL write_cr0]
[GLOBAL read_cr3]
[GLOBAL write_cr3]
[GLOBAL read_cr2]
[GLOBAL align_address]
[GLOBAL tlb_flush]

read_cr0:
	mov  eax, cr0
	ret

read_cr3:
	mov  eax, cr3
	ret

write_cr0:
	push ebp
	mov  ebp, esp
	mov  eax, [ebp + 8]
	mov  cr0, eax
	pop  ebp
	ret

write_cr3:
	push ebp
	mov  ebp, esp
	mov  eax, [ebp + 8]
	mov  cr3, eax
	pop  ebp
	ret

read_cr2:
	mov  eax, cr2
	ret

align_address:
	push ebp
	mov  ebp, esp
	mov  eax, [ebp + 8]
	test eax, 0xFFF
	jz   .ok
	and  eax, 0xFFFFF000
	add  eax, 0x1000
.ok:
	pop  ebp
	ret

tlb_flush:
	mov  eax, cr3
	mov  cr3, eax
	ret
