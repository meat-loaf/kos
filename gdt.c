#include "gdt.h"
//TODO maybe we can just make these macros instead
/*void
encode_gdt_entry(struct gdt* e, uint32_t limit, uint32_t base, uint8_t access){
	//flags will be 0b0100
	uint8_t flags = 0x04;	//0100
	//limit is really 20 bits
	limit &= 0x000FFFFF;
	e->limit_low16 = limit & 0x0000FFFF;
	e->base_low16 = base & 0x0000FFFF;
	e->base_mid8 = (base & 0x00FF0000) >> 16;
	e->access = access;
	e->limit_mid8_flags = ((limit & 0x00FF0000) >> 16) | (flags << 4);
	e->base_high8 = base & 0xFF000000 >> 24;
	return;
}*/
extern struct segment_desc gdt[NUMSEG];

void
init_segments(void){
	gdt[0] = SEG(0x0, 0x0, 0x0, 0x0);
	gdt[1] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
	gdt[2] = SEG(STA_W, 0, 0xffffffff, 0);
}
 
inline void
lgdt(struct segment_desc *gdtd, int size){
	volatile uint16_t pd[3];
	pd[0] = size-1;
	pd[1] = (uint32_t)gdtd;
	pd[2] = (uint32_t)gdtd>>16;

	asm("lgdt %0" :: "m"(pd));
}

inline void
loadsegs(void){
	uint16_t seg = 0x10;
	asm volatile(""
	"jmp $0x8, _here_");
_here_:
	asm volatile(""
	"movw %ds, %0"
     	"movw %cs, %0\n\t"
	"movw %es, %0\n\t"
	"movw %fs, %0\n\t"
	"movw %gs, %0\n\t"
	"movw %ss, %0\n\t"
	:: "r" (seg) );
}

