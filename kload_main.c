#include "multiboot.h"
#include "gdt.h"
#include "elf.h"

extern struct segment_desc gdt[NUMSEG];
#define NULL (void *)0x0
void kpanic(char *);
void dump(char *);
int strcmp(char *, char *);
inline void bochs_bp(void);
void memcpy(char *, char *, int);
extern int check_longmode();

void
enter_x64_kern(void *entry){
	void (*efptr)() = entry;
	if(!check_longmode()){
		dump("longmode ok!!!!!!!");
		asm volatile("xchg %bx, %bx");
		efptr();
	} else {
		dump("64-bit not available on this processor!!");
	}
	return;
}

void
kload_main(const void* mboot_struct){
	//set up 32bitGDT (GDTR is undefined according to multiboot spec)
	init_segments();
	lgdt(gdt, sizeof(gdt));
	loadsegs();
	//following several lines courtesy of OSDEV WIKI
	const multiboot_info_t* mb_info = mboot_struct;
	multiboot_uint32_t mb_flags = mb_info->flags;
	void *main64 = NULL;
	multiboot_module_t* module = (multiboot_module_t*) 0x0;
	if (mb_flags & MULTIBOOT_INFO_MODS){
		multiboot_uint32_t mods_count = mb_info->mods_count;
		multiboot_uint32_t mods_addr = mb_info->mods_addr;
		for (uint32_t mod = 0; mod < mods_count; mod++){
			 module = 
				(multiboot_module_t*)(mods_addr + (mod * sizeof(multiboot_module_t)));
				if (strcmp((char *)module->cmdline, "KERN64.BIN")){
					main64 = load_elf(module->mod_start, module->mod_end);
				}
		} 
	}
	asm volatile("xchg %bx, %bx");
	enter_x64_kern(main64);
	return;
}

int
strcmp(char *l, char *r){
	unsigned char *p1 = l, *p2 = r;
	while (*p1 == *p2){
		p1++;
		p2++;
	}
	return (*p1)-(*p2);
}

void
memcpy(char* dest, char *src, int len){
	for (int i = 0; i < len; i++){
		dest[i] = src[i];
	 }
return;
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

inline void
bochs_bp(void){
	asm("xchg %bx, %bx");
	return;
}
