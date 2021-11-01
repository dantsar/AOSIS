include ../config.mk

LINKER_LD 	:= linker.ld
CFLAGS		:=$(CFLAGS) -O2 -ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -lgcc

all: aosis.bin 

boot.o: boot.asm
	nasm -felf32 boot.asm -o boot.o

kernel.o: kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o $(CFLAGS) 

aosis.bin: linker.ld kernel.o boot.o 
	i686-elf-gcc -T $< -o $@ $(CFLAGS) boot.o kernel.o 

.PHONY: clean
clean: 
	rm -rf *.o *.bin *.iso aosis
