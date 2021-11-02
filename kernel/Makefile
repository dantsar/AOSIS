include ../config.mk

LINKER_LD 	:= linker.ld
CFLAGS		:=$(CFLAGS) -O2 -ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -lgcc

AOSIS_BIN_DEP 	:= 	kernel.o \
				 	boot.o

all: aosis.bin 

%.o: %.asm
	nasm -felf32 $< -o $@

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS) 

aosis.bin: linker.ld $(AOSIS_BIN_DEP)
	$(CC) -T $< -o $@ $(CFLAGS) $(AOSIS_BIN_DEP)

.PHONY: clean
clean: 
	rm -rf *.o *.bin *.iso aosis
