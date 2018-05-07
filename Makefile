CC = x86_64-elf-gcc
AS = x86_64-elf-as

CFLAGS = -Wall -Wextra -ffreestanding -std=gnu99 -nostdlib -mno-red-zone -O2
CLINKSCRIPT = -T link64.ld
C32BIT = -m32
CFLAGS32 = $(CFLAGS) $(32BIT) -Xlinker '-melf_i386' -Xlinker '-zmax-page-size=0x1000'
ASFLAGS32 = --32


#target name for the bootstrapping binary
LOADER_TARGET = kos.bin
GRUB_ROOT_DIR =  ./runimage
GRUB_HIERARCHY = $(GRUB_ROOT_DIR)/boot/grub

default: grub_mkiso

grub_mkiso: grub_setup
	cp kos.bin ./runimage/boot/kos.bin;	\
	grub-mkrescue -o kos.iso $(GRUB_ROOT_DIR)/

grub_setup: loader
	mkdir -p $(GRUB_HIERARCHY); \
	if [ ! -f $(GRUB_HIERARCHY)/grub.cfg ]; then \
	echo -e 'set default = 0\nset timeout = 2\nmenuentry "KOS" {\nmultiboot /boot/kos.bin\n}' > \
	$(GRUB_HIERARCHY)/grub.cfg; \
	fi; 
 

loader:	bootasm.o kload_main.o
	$(CC) $(CLINKSCRIPT) $(CFLAGS32) -o $(LOADER_TARGET) bootasm.o kload_main.o

kload_main.o: kload_main.c
	$(CC) -c kload_main.c -o kload_main.o $(CFLAGS)	$(C32BIT)

bootasm.o: entryasm.S
	$(AS) $(ASFLAGS32) -o bootasm.o entryasm.S

bochs: default
	bochs -f bochsrc.conf -q
clean:
	rm *.o kos.bin
