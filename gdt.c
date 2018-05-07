#include "gdt.h"
//TODO maybe we can just make these macros instead
void
encode_gdt_entry(struct gdt* e, uint32_t limit, uint32_t base, uint8_t access){
	//flags will be 0b0100
	uint8_t flags = 0x04;	//0100
	//limit is really 20 bits
/*	if (limit < 0x10000 || limit > 0xFFFFF){
		panic("GDT limit must be between 0x10000 and 0xFFFFF");
	}
*/
	limit &= 0x000FFFFF;
	e->limit_low16 = limit & 0x0000FFFF;
	e->base_low16 = base & 0x0000FFFF;
	e->base_mid8 = (base & 0x00FF0000) >> 16;
	e->access = access;
	e->limit_mid8_flags = ((limit & 0x00FF0000) >> 16) | (flags << 4);
	e->base_high8 = base & 0xFF000000 >> 24;
	return;
}
		
