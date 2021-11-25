include ../config.mk

LINKER_LD 	:= linker.ld
CFLAGS		:=$(CFLAGS) -ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -lgcc -O0 -ggdb

KBIN_DEP 	:= 	kernel.o 			  	\
			 	boot/boot.o 		  	\
				terminal/tty.o 		  	\
				terminal/vga.o		  	\
				interrupt/init_idt.o 	\
				interrupt/interrupt.o 	\
				interrupt/keyboard.o	\
				interrupt/pic.o			\


# ---------------------------------------------- #

all: aosis.bin 

%.o: %.asm
	$(AS) -felf32 $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

aosis.bin: linker.ld $(KBIN_DEP)
	$(CC) -T $< -o $@ $(CFLAGS) $(KBIN_DEP)

.PHONY: clean
clean: 
	$(RM) $(KBIN_DEP) *.o *.bin *.iso aosis
