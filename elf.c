#include "elf.h"
//TODO this is actually the worst thing imaginable
//make a freestanding version of libc for God's sake
void
memcpy2(char* dest, char *src, int len){
	for (int i = 0; i < len; i++){
		dest[i] = src[i];
	 }
return;
}

int
parse_elf(void *start, Elf64_Ehdr *header){
	return  header->e_ident[EL_MAG0] == '\x7F' &&
		header->e_ident[EL_MAG1] == 'E' &&
		header->e_ident[EL_MAG2] == 'L' &&
		header->e_ident[EL_MAG3] == 'F';
}

void *
load_elf(multiboot_uint32_t mod_start, multiboot_uint32_t mod_end){
	char elf_header[sizeof(Elf64_Ehdr)];
	Elf64_Ehdr *kern_head = (Elf64_Ehdr *)elf_header;
	memcpy2(elf_header, (char *)mod_start, sizeof(Elf64_Ehdr));
	if(parse_elf((void *)mod_start, kern_head)){
		//'vaddr' of entrypoint
		return (void *)mod_start + kern_head->e_entry;
	}
	return (void *)0x0; 
}
