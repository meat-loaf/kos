CC = x86_64-elf-gcc
CFLAGS = -Wall -Wextra -ffreestanding -std=gnu99 -nostdlib -mno-red-zone -O2
32BIT = -m32
CFLAGS32 = $(CFLAGS) $(32BIT) -Xlinker '-melf_i386' -Xlinker '-zmax-page-size=0x1000'
KDIR = ./klib/
ULIB = ./ulib/
SRCFILES = \
	./klib/memcpy.c \
	./klib/strcmp.c 
OBJFILES = $(patsubst %.c, %.o, $(SRCFILES))


%.o: %.c
	$(CC) $(CFLAGS32) $< -c -o $@ 

default: $(OBJFILES)

clean:
	rm ./klib/*.o

