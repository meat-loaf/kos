#include "multiboot.h"
#include "gdt.h"
#include "elf.h"
#include "paging_defs.h"
#include "./libc/klib/string.h"
#define CONST_TMP_ADDR 0x200000
extern struct segment_desc gdt[NUMSEG];
#define NULL (void *)0x0
void kpanic(char *);
void dump(char *);
int strcmp(char *, char *);
extern int check_longmode();

void
enter_x64_kern(void * entry, pde_t *pgtab_tmp){
	//void (*efptr)() = entry;
	void (*efptr)();
	efptr = (void (*)(void))entry;
	asm ("xchg %bx, %bx");
	if(!check_longmode()){
		dump("longmode ok!!!!!!!");
		asm ("xchg %bx, %bx");
		efptr();
	} else {
		dump("64-bit not available on this processor!!");
	}
	return;
}

void
kload_main(const void* mboot_struct){
	pde_t pml4_tmp[NENTRIES] = {0};
	pml4_tmp[0] = 0 & PTE_P;
	pde_t pdpe = 0 & PTE_P;
	pde_t pde = 0 & PTE_P;
	pde_t pte[NENTRIES]
	init_segments();
	lgdt(gdt, sizeof(gdt));
	loadsegs();
	set_tmp_paging();
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
	//end
	enter_x64_kern(main64, pml4_tmp);
	return;
}
//lazy dump, write proper term controller for x64 code
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

