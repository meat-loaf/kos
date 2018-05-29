.section .text
.globl check_longmode
.type check_longmode, @function
check_longmode:
	push    %ebp
        mov     %esp, %ebp
        pushf
        pushf
        xorl    $0x00200000, (%esp)
        popf
        pushf
        pop     %eax
        xor     (%esp), %eax
        popf				# restore proper flags
        and     $0x00200000, %eax
        jz      no_cpuid		# if the flags didn't change, there's no cpuid
        mov     $0x80000000, %eax	# need to check if can use extended cpuid functions
        cpuid
        cmp     $0x80000001, %eax
        jb      no_ext_cpuid		# if less, we do not have extended cpuid
        mov     $0x80000001, %eax
        cpuid				# call cpuid with 0x80000001
        test    $(1 << 29), %edx	# if LM-bit (29) set in edx after call, we can use long mode
        jz      no_x64_long
        popf
	mov	%eax, 0x0
        mov     %ebp, %esp
        pop     %ebp
	ret
no_cpuid:
	mov	%eax, 0x1
        mov     %ebp, %esp
        pop     %ebp
	ret
no_ext_cpuid:
	mov	%eax, 0x2
        mov     %ebp, %esp
        pop     %ebp
	ret
no_x64_long:
	mov	%eax, 0x4
        mov     %ebp, %esp
        pop     %ebp
	ret
