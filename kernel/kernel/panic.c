#include <stdio.h>
#include <arch/internal.h>

/**
 * The system has run into a condition that is unsafe to recover from.
 * 
 * Tells the user to reboot the system and dumps diagnostic information
 * that may assist in debugging.
 */
__attribute__((noreturn))
void panic(const char *msg)
{
    printf("[PANIC] %s", msg);

    // Halt the system.
    _goldrush_arch_halt();

    __builtin_unreachable();
}