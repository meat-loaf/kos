#include <stdint.h>
#define NUMSEG 3
//from xv6 source; mmu.h
#define SEG(type, base, lim, dpl) (struct segment_desc)		\
{ ((lim) >> 12) & 0xffff, (uint32_t)(base) & 0xffff,		\
  ((uint32_t)(base) >> 16) & 0xff, type, 1, dpl, 1,		\
  (uint32_t)(lim) >> 28, 0, 0, 1, 1, (uint32_t)(base) >> 24 }
#define STA_X	0x8	// executable
#define STA_E	0x4	// expand down (nonexec)
#define STA_C	0x4	// conforming code seg (exec only)
#define STA_W	0x2	// writable (nonexec)
#define STA_R	0x2	// readable (exec)
#define STA_A	0x1	//  accessed

struct segment_desc{
	uint32_t lim_15_0 : 16;
	uint32_t base_15_0 : 16;
	uint32_t base_16_23 : 8;
	uint32_t type : 4;
	//following 3 fields are access bits
	uint32_t sys_lvl : 1;	//0 = sys, 1 = application
	uint32_t dpl : 2;	//descriptor privilege level
	uint32_t p : 1; 	//present
	uint32_t lim_19_16 : 4;
	uint32_t avl : 1; 	//unused; avail for software
	uint32_t rsv1 : 1;	//reserved
	uint32_t db : 1;	//0 - 16 bit seg; 1 - 32 bit seg
	uint32_t gran : 1;	//granularity -- byte (0) vs page (1) scaling
	uint32_t base_31_24 : 8;
};
//end from xv6 source
struct segment_desc gdt[NUMSEG];

void lgdt(struct segment_desc *, int);
void init_segments();
void loadsegs();
