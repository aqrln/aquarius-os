org 0x80000000

[BITS 32]

start:
  mov	esp,stack_end
  mov	ebp,esp

  push	0x23	; ss
  push	esp
  pushf
  pop	eax
  or	eax,0x200 ; IF
  push	eax
  push	0x1b	; cs
  push	usermode
  iret
usermode:
  mov	edx,.return
  mov	ecx,esp
  mov	esi,exec
  mov	edi,0
  mov	eax,1
  int	80h
.return:
  jmp $

data:
  exec db "/initrd/autostart", 0

stack:
  times 0x1000 db 0
stack_end:
  times 4 db 0
