#ifndef _ARCH_INTERNAL_H
#define _ARCH_INTERNAL_H

#include <stddef.h>

/**
 * _goldrush_arch_halt() is called by the panic() function if the system enters
 * an unrecoverable state. The system will be halted.
 */
void _goldrush_arch_halt();

#endif
