#include <stdint.h>
#define NENTRIES 512
#define pde_t uint64_t

//masks for virtual addrs (4kb pages)
#define PM_SEX 0xFFFF000000000000
#define PML4_I 0x0000FF8000000000
#define PDPT_I 0x0000007FC0000000
#define PDO9_I 0x000000003FE00000
#define PTO9_I 0x00000000001FF000
#define PHYS_I 0x0000000000000FFF

#define PHYS_2M PTO9_I | PHYS_I
#define PHYS_1G PDO9_I | PTO9_I | PHYS_I

//flags for pages
#define PTE_P  0x0000000000000001	//present
#define PTE_W  0x0000000000000002	//writable
#define PTE_U  0x0000000000000004	//user
#define PTE_WT 0x0000000000000008	//write-through
#define PTE_CD 0x0000000000000010	//cache-disable
#define PTE_A  0x0000000000000020	//accessed
#define PTE_D  0x0000000000000040	//dirty
#define PTE_PS 0x0000000000000080	//page size

