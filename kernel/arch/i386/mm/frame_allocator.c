#include <stddef.h>
#include <stdint.h>
#include <string.h>

// The physical frame allocator is the first seriously memory-related portion
// of the kernel we initialize.
//
// Goldrush's memory allocator is currently using a bitmap as it is highly efficient.
// To deal with memory gaps, we also utilize a singly linked list per section
// of available memory.

// Defines a node in the physical memory allocator.
typedef struct page_node {
    // A reference to the next page node.
    struct page_node *next;

    // The address to start allocating from.
    uint32_t base;

    // How many pages are available to allocate from this range. This does not
    // represent the number of free pages - the bitmap must be checked.
    size_t pages_avail;

    // The page node bitmap.
    uint32_t *bitmap;
} page_node_t;

page_node_t *first_node;

#define STAGE1_PAGES 512
#define PAGE_BITMAP_SIZE(p) (p >> 5) // 2^5 = 32
extern char early_page_alloc_start[]; // defined in linker.ld

void initialize_page_alloc_stage1()
{
    // In stage 1 of initializing the page allocator, we use a static block of
    // memory at the end of the kernel image. We use one page to hold the page node
    // and the bitmap. This is sufficient for the early initialization of the kernel.
    // Later, with the ability to dynamically allocate memory, we will be able to
    // reinitialize the page allocator with the system's full physical address space
    // available.
    first_node = (page_node_t *) early_page_alloc_start;
    first_node->next = NULL;
    first_node->pages_avail = STAGE1_PAGES;
    // The base will begin after the node/bitmap page.
    first_node->base = (uint32_t) early_page_alloc_start + 0x1000;
    first_node->bitmap = (uint32_t *) early_page_alloc_start + sizeof(page_node_t);
    memset(first_node->bitmap, 0, PAGE_BITMAP_SIZE(STAGE1_PAGES));
}

void *allocate_page()
{
    page_node_t *curr = first_node;
    
    // Iterate the linked list of physical memory regions available.
    while (curr != NULL)
    {
        // Iterate through the bitmap of pages allocated or free.
        for (size_t bitmap_index = 0; bitmap_index < (curr->pages_avail / sizeof(uint32_t)) - 1; bitmap_index++)
        {
            uint32_t word = curr->bitmap[bitmap_index];
            for (size_t word_index = 0; word_index < sizeof(uint32_t) * 8; word_index++)
            {
                if ((word & (1 << word_index)) == 0)
                {
                    // We found a free page. Allocate it to the user.
                    curr->bitmap[bitmap_index] = word | (1 << word_index);
                    return ((unsigned char *) curr->base + ((bitmap_index + 1) * word_index * 4096));
                }
            }
        }

        curr = curr->next;
    }

    return NULL;
}

void free_page(void *addr)
{
    uint32_t addr_int = (uint32_t) addr;
    page_node_t *curr = first_node;

    // Iterate the linked list of physical memory regions available.
    while (curr != NULL)
    {
        size_t page_index = (addr_int - curr->base) >> 12;
        if (page_index > curr->pages_avail)
        {
            // The page is almost certainly not in this boundary. Try the next
            // space.
            curr = curr->next;
            continue;
        }

        uint32_t bitmap_index = page_index >> 5;
        uint32_t and_word = ~(1 << (page_index % 8));
        curr->bitmap[bitmap_index] &= and_word;
        break;
    }
}