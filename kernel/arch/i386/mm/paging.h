#ifndef ARCH_I386_PAGING_H
#define ARCH_I386_PAGING_H

// Page Directory bits

// If this bit is set, the page referenced by this directory is present in
// physical memory. If it is not set, then if the page is accessed, the CPU
// will raise a page fault exception. We are not currently prepared for this.
#define PAGE_DIRECTORY_PRESENT (1 << 0)

// If this bit is set, we can write to the range covered by this page directory
// entry. Otherwise, it is read only.
#define PAGE_DIRECTORY_WRITABLE (1 << 1)

// If this bit is set, then user-mode can access this page. Otherwise, only the
// supervisor (the kernel) may access the page.
#define PAGE_DIRECTORY_USER (1 << 2)

// Controls write-thru behavior.
#define PAGE_DIRECTORY_WRITE_THRU (1 << 3)

// If this bit is set, the processor will never cache this entry.
#define PAGE_DIRECTORY_DISABLE_CACHE (1 << 4)

// Whether or not this page entry has been accessed.
#define PAGE_DIRECTORY_ACCESSED (1 << 5)

// If this bit is set, then this page directory entry is treated as covering 4MiB
// of memory. This also means you need to use a 4MiB-aligned base address for the
// page table you'd like to use.
#define PAGE_DIRECTORY_LARGE_PAGES (1 << 7)

static inline uint32_t new_page_directory_entry(uint32_t table_address, uint8_t flags)
{
    if (flags & PAGE_DIRECTORY_LARGE_PAGES)
    {
        // Align the address to 4MiB.
        return (table_address & 0xffc00000) | flags;
    }
    return (table_address & 0xfffff000) | flags;
}

// Page Table Bits

// If this bit is set, the page referenced by this table is present in
// physical memory. If it is not set, then if the page is accessed, the CPU
// will raise a page fault exception. We are not currently prepared for this.
#define PAGE_TABLE_PRESENT (1 << 0)

// If this bit is set, we can write to the range covered by this page table
// entry. Otherwise, it is read only.
#define PAGE_TABLE_WRITABLE (1 << 1)

// If this bit is set, then user-mode can access this page. Otherwise, only the
// supervisor (the kernel) may access the page.
#define PAGE_TABLE_USER (1 << 2)

// Controls write-thru behavior.
#define PAGE_TABLE_WRITE_THRU (1 << 3)

// If this bit is set, the processor will never cache this entry.
#define PAGE_TABLE_DISABLE_CACHE (1 << 4)

// Whether or not this page entry has been accessed.
#define PAGE_TABLE_ACCESSED (1 << 5)

// This page is dirty: it has been modified.
#define PAGE_TABLE_DIRTY (1 << 6)

// Whether or not address updates do not update the TLB cache.
#define PAGE_TABLE_GLOBAL (1 << 8)

static inline uint32_t new_page_table_entry(uint32_t page_address, uint8_t flags)
{
    return (table_address & 0xfffff000) | flags;
}

#endif