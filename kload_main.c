#include "multiboot.h"
#include "gdt.h"

void kpanic(char *);
void dump(char *);
//TODO extern?
void
kload_main(const void* mboot_struct){
	struct gdt gdt_entries[NUMGDTENTRIES];
	struct gdtdesc gdtd;
//	kpanic("test!");
	//null descriptor
	encode_gdt_entry(&(gdt_entries[0]), 0x0, 0x0, 0x0);
	encode_gdt_entry(&(gdt_entries[1]), 0x0, 0xFFFFFFFF, 0x9A);
	encode_gdt_entry(&(gdt_entries[2]), 0x0, 0xFFFFFFFF, 0x92);
//	encode_gdt_entry(&gdt_entries[3], 0x0, 0xFFFFFFFF, 0x89); //TSS?
	gdtd.size = sizeof(struct gdt) * NUMGDTENTRIES;
	gdtd.offset = &gdt_entries;
	set_gdt(&gdtd);
	kpanic("GDT LOADED; SEGMENTS RESET!"); 
	return;
}

/*void
lgdt(struct gdtdesc *gdtd){
	asm("lgdt %0" :: "m"(gdtd));
}*/

void
kpanic(char *msg){
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
