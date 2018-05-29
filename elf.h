#include <stdint.h>
#ifndef MULTIBOOT_HEADER
#include "multiboot.h"
#endif

#ifndef _KOS_ELF_H_
#define _KOS_ELF_H_
#endif 
#define ELF_MAGIC	0x464C457FU	//0x7FELF (little endian)

#define EL_MAG0		0	
#define	EL_MAG1		1
#define	EL_MAG2		2
#define	EL_MAG3		3
#define EL_CLASS	4
#define EL_DATA		5
#define EL_VERSION	6
#define	EL_OSABI	7
#define EL_ABIVERSION	8
#define EL_PAD		9
#define EI_NIDENT	16

typedef uint64_t Elf64_Addr;
typedef uint16_t Elf64_Half;
typedef int16_t  Elf64_SHalf;
typedef uint64_t Elf64_Off;
typedef int32_t  Elf64_Sword;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

typedef struct {
	unsigned char e_ident[EI_NIDENT];	// ELF magic number (\x7FELF)
	Elf64_Half e_type;
	Elf64_Half e_machine;
	Elf64_Word e_version;
	Elf64_Addr e_entry;		/* Entry point virtual address */
	Elf64_Off e_phoff;		/* Program header table file offset */
	Elf64_Off e_shoff;		/* Section header table file offset */
	Elf64_Word e_flags;
	Elf64_Half e_ehsize;
	Elf64_Half e_phentsize;
	Elf64_Half e_phnum;
	Elf64_Half e_shentsize;
	Elf64_Half e_shnum;
	Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct {
	Elf64_Word p_type;
	Elf64_Word p_flags;
	Elf64_Off p_offset;
	Elf64_Addr p_vaddr;
	Elf64_Addr p_addr;
	Elf64_Xword p_filesz;
	Elf64_Xword p_memsz;
	Elf64_Xword p_align;
} Elf64_Phdr;

void *load_elf(multiboot_uint32_t, multiboot_uint32_t);
int parse_elf(void *, Elf64_Ehdr*);

