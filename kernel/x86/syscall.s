[GLOBAL SyscallEntry]
[EXTERN SystemCall]

SyscallEntry:
	cli
	push	edx
	push	ecx
	push	ebp
	push	edi
	push	esi
	push	eax
	call	SystemCall
	add		esp,12
	pop		ebp
	pop		ecx
	pop		edx
	sti
	sysexit
