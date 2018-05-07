.set ALIGN,	1<<0
.set MEMINFO,	1<<1
.set FLAGS,	ALIGN | MEMINFO
.set MAGIC, 	0x1BADB002
.set CHECKSUM,	-(MAGIC+FLAGS)
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.global _start

.section .bss
stack_bottom:
	.skip 256
stack_top:

.section .text
_start:
	cli
	xchg	%bx, %bx
	mov	%esp, stack_top
	mov	%ebp, stack_top
	#cmp %eax, $0x2BADB002	#magic number, means we were loaded by GRUB
	push	%ebx		#pointer to multiboot structure (GRUB)
	call	kload_main	