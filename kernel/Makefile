include ../config.mk

all: aosis.bin aosis.iso

boot.o: boot.asm
	nasm -felf32 boot.asm -o boot.o

kernel.o: kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

aosis.bin: kernel.o boot.o linker.ld
	i686-elf-gcc -T linker.ld -o aosis.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

aosis.iso: aosis.bin grub.cfg
	mkdir -p aosis/boot/grub
	cp aosis.bin aosis/boot/aosis.bin
	cp grub.cfg aosis/boot/grub/grub.cfg
	grub-mkrescue -o aosis.iso aosis 

.PHONY: clean
clean: 
	rm -rf *.o *.bin *.iso aosis

.PHONY: run
run: aosis.iso
	qemu-system-i386 -cdrom aosis.iso
