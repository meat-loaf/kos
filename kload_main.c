#include "multiboot.h"
#include "gdt.h"

void lgdt(struct gdtdesc *);
void panic(char *);
void dump(char *);

static struct gdt gdt_entries[4];

void
kload_main(const void* mboot_struct){
	panic("test!");
	return;
}

void
lgdt(struct gdtdesc *gdtd){
	asm("lgdt %0" :: "m"(gdtd));
}

void
panic(char *msg){
	dump(msg);
	while(1);
}
//TODO write a goddamn terminal controller
void
dump(char *msg){
	char * vga = 0xB8000;
	int i = 0;
	while (msg[i]){
		vga[i*2] = msg[i];
		i++;
	}
	return;
}
