#include <stdint.h>
#define NUMGDTENTRIES 3
//xv6 source helped with struct definition
struct gdt{
	uint32_t lim_15_0 : 16;
	uint32_t base_15_0 : 16;
	uint32_t base_16_23 : 8;
	uint32_t type : 4;
	//following 3 fields are access bits
	uint32_t sys_lvl : 1;	//0 = sys, 1 = application
	uint32_t dpl : 2	//descriptor privilege level
	uint32_t p : 1; 	//present
	uint32_t lim_19_16 : 4;
	uint32_t avl : 1; 	//unused; avail for software
	uint32_t rsv1 : 1;	//reserved
	uint32_t db : 1;	//0 - 16 bit seg; 1 - 32 bit seg
	uint32_t gran : 1;	//granularity -- byte (0) vs page (1) scaling
	uint32_t base_31_24 : 8;
};

void set_gdt(struct gdtdesc *);
void encode_gdt_entry(struct gdt *, uint32_t, uint32_t, uint8_t);
