#ifndef _KERNEL_KERNEL_H
#define _KERNEL_KERNEL_H

#include <stddef.h>

void kernel_main();
__attribute__((noreturn))
void panic(const char *msg);

#endif
