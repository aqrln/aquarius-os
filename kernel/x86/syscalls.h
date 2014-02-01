#ifndef SYSCALLS_H
#define SYSCALLS_H

#define SYS_PUTS 0
#define SYS_EXEC 1

void InitSyscalls(void);
void SystemCall(u32 id, void *param, void *result);
void SyscallEntry(void);
void Int80hSyscall(registers_t *regs);

#endif
