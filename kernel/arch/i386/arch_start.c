/**
 * The primary C entrypoint of the Goldrush kernel is known as kernel_arch_start.
 * 
 * On i386, Goldrush is a Multiboot 0.6.96-compliant kernel, and by the time that
 * kernel_arch_start() is called, a stack has been set up and all global constructors
 * have been called, initalizing a very minimal C environment. Otherwise, the system is
 * in the state specified by the Multiboot specification.
 * 
 * It feels lonely. It's cold. But like the planet of Jakku, if a tiny little pitty
 * flower can survive here, we can survive here, too. kernel_arch_start() is designed
 * to take the very basic environment provided by the assembly startup and turn it into
 * something interesting.
 **/

#include <stdio.h>
#include <kernel/kernel.h>
#include <kernel/tty.h>

void kernel_arch_start()
{
    // Initialize early print support.
    terminal_initialize();

    // For now, just print hello world.
    printf("Hello, world!\n");

    // Run kernel_main().
    kernel_main();

    // We shouldn't exit from our eventual kernel_main(), nor should
    // we exit from kernel_arch_start() itself. (We're doing so because
    // very little actual functionality has been written.)
    printf("Returned from kernel_main\n");
}