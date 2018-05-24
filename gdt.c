#include "gdt.h"
extern struct segment_desc gdt[NUMSEG];

void
init_segments(void){
	gdt[0] = SEG(0x0, 0x0, 0x0, 0x0);
	gdt[1] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
	gdt[2] = SEG(STA_W, 0, 0xffffffff, 0);
}
//from xv6 source; x86.h 
inline void
lgdt(struct segment_desc *gdtd, int size){
	volatile uint16_t pd[3];
	pd[0] = size-1;
	pd[1] = (uint32_t)gdtd;
	pd[2] = (uint32_t)gdtd>>16;

	asm("lgdt %0" :: "m"(pd));
}
//end from xv6 source

inline void
loadsegs(void){
	//can only load segment value from register
	uint16_t seg = 0x10;
	//can only reload code segment by jmp
	asm volatile("jmp $0x08, $__here__");
	asm volatile("__here__:");
	//reload the rest of them
	asm volatile(""
	"movw %0, %%ds\n\t"
	"movw %0, %%es\n\t"
	"movw %0, %%fs\n\t"
	"movw %0, %%gs\n\t"
	"movw %0, %%ss\n\t"
	 :: "r" (seg) );
}

