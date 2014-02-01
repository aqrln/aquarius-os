#ifndef MSR_H
#define MSR_H

#define MSR_IA32_SYSENTER_CS  0x174
#define MSR_IA32_SYSENTER_ESP 0x175
#define MSR_IA32_SYSENTER_EIP 0x176

void ReadMSR(u32 msr, u32 *hi, u32 *lo);
void WriteMSR(u32 msr, u32 hi, u32 lo);

#endif
