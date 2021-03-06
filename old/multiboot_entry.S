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
#smol stack; we'll set up a proper stack when we get to long mode
stack_bottom:
	.skip 128
stack_top:
#set up blank space for page tables; set these up at runtime
#pml4:
	#.lcomm p4, 512*8
#	.skip 512*8
#pdpt:
#	.lcomm p3, 512*8
#	.skip 512*8
#pdp:
#	.lcomm p2, 512*8
#	.skip 512*8
#pt:
#	.lcomm p1, 512*8
#	.SKIP 512*8
#note: all this code is run in 32-bit protected mode, as that
#is how grub hands off the machine...need to check for long mode
#and set up page table, interrupt table, etc
.section .text
.global _start
.type _start, @function
_start:
#	xchgw	%bx, %bx		#bochs magic breakpoint
	movl	$stack_top, %esp	#set up the stack
	movl	$stack_top, %ebp
	jmp chk_x64
ex64:	jmp enter_x64_long		#set up paging, lgdt
_kmain:	call	kmain			#need to load x64-compiled actual kernel and jump to it
	cli
_hang:
	jmp	_hang
.size _start, . - _start
#cpuid is available if we can flip bit 21 in the flags register.
.type chk_x64, @function
chk_x64:
	push	%ebp
	mov	%esp, %ebp
	pushf
	pushf
	xorl	$0x00200000, (%esp)
	popf
	pushf
	pop	%eax
	xor	(%esp), %eax
	popf				# restore proper flags
	and	$0x00200000, %eax
	jz	no_cpuid		# if the flags didn't change, there's no cpuid
	mov	$0x80000000, %eax	# need to check if can use extended cpuid functions
	cpuid
	cmp	$0x80000001, %eax
	jb	no_ext_cpuid		# if less, we do not have extended cpuid
	mov	$0x80000001, %eax
	cpuid				# call cpuid with 0x80000001
	test	$(1 << 29), %edx		# if LM-bit (29) set in edx after call, we can use long mode
	jz	no_x64_long
	popf
#	jmp enter_x64_long
	mov	%ebp, %esp
	pop	%ebp
#	ret
	jmp ex64
#we are in prot 32-bit, args pushed onto stack here...
no_cpuid:
	push	$msg_no_cpuid
	jmp	dmp
no_ext_cpuid:
	push	$msg_no_ext_cpuid
	jmp	dmp
no_x64_long:
	push	$msg_no_x64_support
dmp:
	call	term_dump_error
	cli
	jmp	_hang
.type enter_x64_long, @function
enter_x64_long:
	#push	%ebp
	#mov	%esp, %ebp
	# set up page tables (badly) for now
	# can probably just do this with a macro or something(?)
	# figure it out later...
	# for now identity map first GB of address space with hyooge (2MiB) pages
	# make a proper mapping later but this works for now
set_up_paging:
	# identity map first 2 megabytes	
	# eventually do memcheck/move this to C but this is ok for now...
	# courtesy of OSDEV Wiki
#	std
#	mov	$4096, %ecx
	xchgw	%bx, %bx		#bochs magic breakpoint
	mov	%cr0, %eax
	and	$0b01111111111111111111111111111111, %eax
	mov	%eax, %cr0
	xor	%ecx, %ecx
	mov	$0x1000, %edi
	mov	%edi, %cr3
	xor	%eax, %eax
.nullLoop:
	movl 	$0x0, (%edi, %ecx)
	add	$0x4, %ecx
	cmp	$4096, %ecx
	jnz .nullLoop
#	rep	stosl
	mov	%cr3, %edi
	movl	$0x2003, (%edi)
	add	$0x1000, %edi
	movl	$0x3003, (%edi)
	add	$0x1000, %edi
	movl	$0x4003, (%edi)
	add	$0x1000, %edi
	
	mov 	$0x00000003, %ebx
	mov	$512, %ecx
.SetEntry:
	mov	%ebx, (%edi)
	add	$0x1000, %ebx
	add	$8, %edi
	loop	.SetEntry
	xchgw	%bx, %bx		#bochs magic breakpoint
	mov 	%cr4, %ebx
	or	$(1 << 5), %eax
	mov	%eax, %cr4

	mov	$0xC0000080, %ecx
	rdmsr
	or	$(1 << 8), %eax
	wrmsr
	
	mov	%cr0, %eax
	or	$(1 << 31), %eax
	mov	%eax, %cr0
	#push	$msg_ok_x64_support
	#call	term_dump_error
	#mov	%ebp, %esp
	#pop	%ebp
	jmp _kmain
#	ret
.data

gdtdesc64:
    .Null:
	.size n, . - gdtdesc64
    .word 0xFFFF                    # Limit (low).
    .word 0                         # Base (low).
    .byte 0                         # Base (middle)
    .byte 0                         # Access.
    .byte 1                         # Granularity.
    .byte 0                         # Base (high).
    .Code:
	.size c, . - gdtdesc64         # The code descriptor.
    .word 0                         # Limit (low).
    .word 0                         # Base (low).
    .byte 0                         # Base (middle)
    .byte 0b10011010                 # Access (exec/read).
    .byte 0b10101111                 # Granularity, 64 bits flag, limit19:16.
    .byte 0                         # Base (high).
    .Data:
	.size d, . - gdtdesc64         # The data descriptor.
    .word 0                         # Limit (low).
    .word 0                         # Base (low).
    .byte 0                         # Base (middle)
    .byte 0b10010010                 # Access (read/write).
    .byte 0b00000000                 # Granularity.
    .byte 0                         # Base (high).
    .Pointer:                    # The GDT-pointer.
    .word . - gdtdesc64 - 1             # Limit.
#    .quad gdtdesc64                     # Base.
	
msg_no_cpuid:
	.asciz "CPUID is not supported on this processor..."
msg_no_ext_cpuid:
	.asciz "This CPU does not support extended CPUID functions."
msg_no_x64_support:
	.asciz "x86_64 is not supported on this processor..."
msg_ok_x64_support:
	.asciz "x86_64 support ok!"

