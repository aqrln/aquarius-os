[GLOBAL new_irq0]
[EXTERN timer_ticks]
[EXTERN switch_task]

new_irq0:
	cli

	push	eax
	push	ebx
	push	ecx
	push	edx
	push	ebp
	push	esi
	push	edi

	;mov 	ax,ds
	;push	eax
	mov 	ax,0x10
	mov 	ds,ax
	mov 	es,ax

	mov 	al,0x20
	out 	0x20,al

	mov 	eax,[timer_ticks]
	inc 	eax
	mov 	[timer_ticks],eax

	mov 	ebp,esp
	call	switch_task

	test	eax,eax
	jz		x
	mov		cr3,eax
x:
	
	;pop 	eax
	mov		ax,0x23 ; user data segment (0x20 | 0x3)
	mov 	ds,ax
	mov 	es,ax

	pop 	edi
	pop 	esi
	pop 	ebp
	pop 	edx
	pop 	ecx
	pop 	ebx
	pop 	eax

	sti
	iret
