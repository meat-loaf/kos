#include "multiboot.h"
#include "gdt.h"

void kpanic(char *);
void dump(char *);
void
kload_main(const void* mboot_struct){

//following several lines courtesy of OSDEV WIKI
	const multiboot_info_t* mb_info = mboot_struct;
	multiboot_uint32_t mb_flags = mb_info->flags;
	dump("hello!");	
//	void *main64 = 0x0;
	if (mb_flags & MULTIBOOT_INFO_MODS){
		multiboot_uint32_t mods_count = mb_info->mods_count;
		multiboot_uint32_t mods_addr = mb_info->mods_addr;
		for (uint32_t mod = 0; mod < mods_count; mod++){
			multiboot_module_t* module = 
				(multiboot_module_t*)(mods_addr + (mod * sizeof(multiboot_module_t)));
				dump((char *)module->cmdline);
		} 
	}
	//check cpuid
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
	char * vga = (char *) 0xB8000;
	int i = 0;
	while (msg[i]){
		vga[i*2] = msg[i];
		i++;
	}
	return;
}
