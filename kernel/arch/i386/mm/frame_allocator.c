#include <stddef.h>
#include <stdint.h>

// For the moment, this page frame allocator uses a very simple but inefficient
// implementation based on bitmaps. One page can address 128MiB of memory, as we
// have 32,768 pages for each entry.

// The bitmap to use. All values are 32-bit.
uint32_t assigned_pages[1024];

// The address to begin allocating physical frames from(?)