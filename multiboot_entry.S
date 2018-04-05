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
.skip 4096 
stack_top:
#note: all this code is run in 32-bit protected mode, as that
#is how grub hands off the machine...need to check for long mode
#and set up page table, interrupt table, etc
.section .text
.global _start
.type _start, @function
_start:
	xchgw	%bx, %bx	#bochs magic breakpoint
	movl	$stack_top, %esp	#set up the stack
	movl	$stack_top, %ebp
	jmp	chk_x64
_kmain:
	call	kmain		#first call to C code here
	cli
_hang:
	jmp	_hang
.size _start, . - _start
#cpuid is available if we can flip bit 21 in the flags register.
.type chk_x64, @function
chk_x64:
	pushf
	pushf
	xorl $0x00200000, (%esp)
	popf
	pushf
	pop %eax
	xor (%esp), %eax
	popf			# restore proper flags
	and $0x00200000, %eax
	jz no_cpuid		# if the flags didn't change, there's no cpuid
	mov $0x80000000, %eax	# need to check if can use extended cpuid functions
	cpuid
	cmp $0x80000001, %eax
	jb no_ext_cpuid		# if less, we do not have extended cpuid
	mov $0x80000001, %eax
	cpuid			# call cpuid with 0x80000001
	test 1 << 29, %edx	# if LM-bit (29) set in edx after call, we can use long mode
	jz no_x64_long
	popf
	jmp enter_x64_long
#we are in prot 32-bit, args pushed onto stack here...
no_cpuid:
	push $msg_no_cpuid
	jmp dmp
no_ext_cpuid:
	push $msg_no_ext_cpuid
	jmp dmp
no_x64_long:
	push $msg_no_x64_support
dmp:
	call term_dump_error
	cli
	jmp	_hang
.type enter_x64_long, @function
enter_x64_long:
	push $msg_ok_x64_support
	call term_dump_error
	jmp _kmain
.data
msg_no_cpuid:
	.asciz "CPUID is not supported on this processor..."
msg_no_ext_cpuid:
	.asciz "This CPU does not support extended CPUID functions."
msg_no_x64_support:
	.asciz "x86_64 is not supported on this processor..."
msg_ok_x64_support:
	.asciz "x86_64 support ok!"

