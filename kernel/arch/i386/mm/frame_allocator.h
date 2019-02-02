#ifndef ARCH_I386_MM_FRAME_ALLOCATOR_H
#define ARCH_I386_MM_FRAME_ALLOCATOR_H

void initialize_page_alloc_stage1();

void *allocate_page();

#endif