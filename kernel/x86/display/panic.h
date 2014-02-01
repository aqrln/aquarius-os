#ifndef PANIC_H
#define PANIC_H

void panic_vanilla(char*, char*, u32);
void assert_panic(char*, u32, char*);

void warning(char*);

#define panic(a) (panic_vanilla(a, __FILE__, __LINE__))
#define assert(a) ((a) ? (void)0 : assert_panic(__FILE__, __LINE__, #a))

#endif
