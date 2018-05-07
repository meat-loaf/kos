#include <stdint.h>
#define NUMGDTENTRIES 3
struct gdt{
	uint16_t limit_low16;
	uint16_t base_low16;
	uint8_t	base_mid8;
	uint8_t access;
	//low nibble is 16:19 of limit
	//high nibble is flags field
	uint8_t limit_mid8_flags;
	uint8_t base_high8;
} __attribute__ ((packed));

struct gdtdesc{
	//valid gdtsizes are [1, 65536],
	//so we store size as size-1
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

void set_gdt(struct gdtdesc *);
void encode_gdt_entry(struct gdt *, uint32_t, uint32_t, uint8_t);
