#include "multiboot.h"
#include "gdt.h"

void kpanic(char *);
void dump(char *);
int strcmp(char *, char *);
void
kload_main(const void* mboot_struct){
	//set up 32bitGDT
	//following several lines courtesy of OSDEV WIKI
	const multiboot_info_t* mb_info = mboot_struct;
	multiboot_uint32_t mb_flags = mb_info->flags;
	dump("hello!");	
//	void *main64 = 0x0;
	multiboot_module_t* module = (multiboot_module_t*) 0x0;
	if (mb_flags & MULTIBOOT_INFO_MODS){
		multiboot_uint32_t mods_count = mb_info->mods_count;
		multiboot_uint32_t mods_addr = mb_info->mods_addr;
		for (uint32_t mod = 0; mod < mods_count; mod++){
			 module = 
				(multiboot_module_t*)(mods_addr + (mod * sizeof(multiboot_module_t)));
				dump((char *)module->cmdline);
				if (strcmp((char *)module->cmdline, "KERN64.BIN")){
					//load the kernel...
								
				}
		} 
	}
	//check cpuid
	return;
}

/*void
lgdt(struct gdtdesc *gdtd){
	asm("lgdt %0" :: "m"(gdtd));
}*/
int strcmp(char *l, char *r){
	unsigned char * p1 = l, *p2 = r;
	while (*p1 == *p2){
		p1++;
		p2++;
	}
	return (*p1)-(*p2);
}
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
