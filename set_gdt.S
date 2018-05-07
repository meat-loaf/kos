.section .text
.global set_gdt
set_gdt:
	xchg	%bx, %bx
	lgdt	4(%esp)
reload_segments:
	ljmp	$0x08,$reload_CS
reload_CS:
	mov	$0x10, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss
	ret

