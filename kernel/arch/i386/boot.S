# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function

# The Multiboot loader will transfer control to _start once the file is loaded
# and the environment set up.
_start:
	# Capture the Multiboot boot information in ebx.
	movl $stack_top, %esp

	# Call the early start C logic.
	push %ebx # has the multiboot magic
	push %eax # has a pointer to the multiboot boot information struct
	call kernel_arch_early_start

	# Call all constructors.
	call _init

	# Transfer control to the arch-specific kernel entry.
	call kernel_arch_start

	# Hang if kernel_arch_start unexpectedly returns.=
	jmp _goldrush_arch_halt

.size _start, . - _start

.global _goldrush_arch_halt
.type _goldrush_arch_halt, @function
_goldrush_arch_halt:
	cli
1:	hlt
	jmp 1b