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

.section .bss
.align 16

stack_bottom:
	.skip 128
stack_top:

.section .text
.global _start
_start:		movl	$stack_top, %esp
		movl	$stack_top, %ebp
		call	chk_cpuid
		call	chk_long
		call	paging_tmp
		xchg	%bx, %bx
		lgdt	GDTPointer + 0x100000
		ljmp	*(%esp)
		hlt
.here:		jmp .here

chk_cpuid:	pushf
		pop	%eax
		mov	%eax, %ecx
		xor	$(1 << 21), %eax
		push	%eax
		popf

		pushf
		pop	%eax
		push	%ecx
		popf
		
		xor	%eax, %ecx
		jz	.noCPUID
		ret
.noCPUID:	jmp .noCPUID

chk_long:	mov	$0x80000000, %eax
		cpuid
		cmp	$0x80000001, %eax
		jb	.noLongMode
		mov	$0x80000001, %eax
		cpuid
		test	$(1 << 29), %edx
		jz	.noLongMode
		ret
.noLongMode:	jmp .noLongMode
		
paging_tmp:
		mov	%cr0, %eax
		and	$0b01111111111111111111111111111111, %eax
		mov	%eax, %cr0
		
		mov	$0x1000, %edi
		mov	%edi, %cr3
		xor	%eax, %eax
		mov	$4096, %ecx
		rep	stosl
		mov	%cr3, %edi

		movl	$0x2003, (%edi)
		add	$0x1000, %edi
		movl	$0x3003, (%edi)
		add	$0x1000, %edi
		movl	$0x4003, (%edi)
		add	$0x1000, %edi

		mov	$0x00000003, %ebx
		mov	$512, %ecx
.setEntry:
		mov	%ebx, (%edi)
		add	$0x1000, %ebx
		add	$8, %edi
		loop	.setEntry

		mov	%cr4, %eax
		or	$(1<<5), %eax
		mov	%eax, %cr4
		
		mov	$0xC0000080, %ecx
		rdmsr
		or	$(1<<8), %eax
		wrmsr
		mov	%cr0, %eax
		or	$(1<<31), %eax
		mov	%eax, %cr0
		ret
Realm64:
		hlt
#.section .data
GDT64:
	.Null: .int .Null - GDT64
	.int	0xFFFF
	.int	0
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.Code: .int .Code - GDT64
	.int	0
	.int	0
	.byte	0
	.byte	0b10011010
	.byte	0b10101111
	.byte	0
	.Data: .int .Data - GDT64
	.int	0
	.int	0
	.byte	0
	.byte	0b10011010
	.byte	0b10101111
	.byte	0
GDTPointer:
	.int	GDTPointer - GDT64 - 1
	.long	GDT64

