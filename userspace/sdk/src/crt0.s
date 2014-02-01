[BITS 32]

[GLOBAL _start]
[GLOBAL SystemCall]
[EXTERN main]
[EXTERN _cstart]

[SECTION .text]

_start:
	mov esp,stack_end
	mov ebp,esp
	call main
	jmp $

SystemCall:
	push ebp
	mov  ebp,esp
	mov  edi,[ebp+16]
	mov  esi,[ebp+12]
	mov  eax,[ebp+8]
	mov  ecx,esp
	mov  edx,.return
	int  80h
.return:
	pop  ebp
	ret

[SECTION .data]

stack:
  times 0x2000 db 0
stack_end:
  times 4 db 0

[GLOBAL __stack]
__stack dd stack_end
