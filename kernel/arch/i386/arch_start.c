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

#include <stdint.h>
#include <stdio.h>
#include <kernel/kernel.h>
#include <kernel/tty.h>

#include "mm/frame_allocator.h"
#include "multiboot.h"

static multiboot_info_t *multiboot_info;

// kernel_arch_early_start is a special entry point, called before kernel_arch_start.
// It is responsible for early initialization and is subject to a bare C environment.
// Only the boot stack is available.
void kernel_arch_early_start(uint32_t magic, uint32_t multiboot_info_ptr)
{
    // Initialize early print support.
    terminal_initialize();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printf("magic %x doesn't match multiboot 0.6.96 magic %x\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
        panic("not booted with a multiboot-compliant boot loader");
    }

    multiboot_info = (multiboot_info_t*) multiboot_info_ptr;

    // The very first thing we've got to do is initialize the memory manager so
    // that we'll be able to do all sorts of fun stuff. Here's how we do it:
    //
    // 1. Parse the memory tables passed to us from the multiboot loader. If there
    //    is no table, we will panic.
    // 2. Use those memory tables to initialize the page frame allocator. We can now
    //    allocate our first few physical pages of memory.
    // 3. Set up an initial page table (initially just identity-mapping) allocated
    //    by the physical page allocator, and activate it. Paging is now active.
    // 4. Set up and activate the GDT, TSS, and IDT. We now approach something close
    //    to a normal protected mode environment.
    // 5. Initialize the kernel's heap manager. We can now dynamically allocate objects,
    //    which will give our kernel form.
    initialize_page_alloc_stage1();

    printf("page 1: 0x%x\n", allocate_page());
    printf("page 2: 0x%x\n", allocate_page());
}

void kernel_arch_start()
{
    // For now, just print hello world.
    printf("Hello, world!\n");

    printf("xkcd says that a good random number is %d\n", 4);

    // hmm, what can we gleam from our multiboot?
    if (multiboot_info->flags & MULTIBOOT_INFO_MEMORY) {
        printf("basic memory information: low: %dKiB, high: %dKiB\n", multiboot_info->mem_lower, multiboot_info->mem_upper);
    }

    if (multiboot_info->flags & MULTIBOOT_INFO_MEM_MAP) {
        printf("we have full memory mappings!\n");
        printf("the mappings are %d bytes long\n", multiboot_info->mmap_length);
        multiboot_memory_map_t *mmap = multiboot_info->mmap_addr;
        unsigned int available_pages;
        while (mmap < multiboot_info->mmap_addr + multiboot_info->mmap_length) {
            printf("- at 0x%x%x: 0x%x%x bytes (type %d)\n", mmap->addr_low, mmap->addr_high, mmap->len_low, mmap->len_high, mmap->type);
            if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
                available_pages += mmap->len_low >> 12;
            }
            mmap = (multiboot_memory_map_t*) ((unsigned long) mmap + mmap->size + sizeof(mmap->size));
        }
        printf("We have %d pages to work with\n", available_pages);
    }

    // Run kernel_main().
    kernel_main();

    // We shouldn't exit from our eventual kernel_main(), nor should
    // we exit from kernel_arch_start() itself. (We're doing so because
    // very little actual functionality has been written.)
    panic("Returned from kernel_main()\n");
}