include ../config.mk

LINKER_LD 	:= 	linker.ld
CFLAGS		:=	$(CFLAGS) -I../include/kernel -I../include/libc -L../build \
				-ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -lgcc -ggdb

KERNEL_SOURCE_C 	:= 	kernel.o 			  	\
						boot/boot.o 		  	\
						terminal/tty.o 		  	\
						terminal/shell.o		\
						terminal/vga.o		  	\
						interrupt/init_idt.o 	\
						interrupt/interrupt.o 	\
						interrupt/keyboard.o	\
						interrupt/pic.o			\
						div_zero.o				\
						branch_nowhere.o		\
						common/bitmap.o			\
						common/ringbuff.o		\
						common/ports.o			\
						common/cpu.o			\
						memory/pmm.o			\
						libk.a					\


KERNEL_SOURCE_RUST	:=	test.rs

AOSIS_BIN 		:= ../build/aosis.bin

# ---------------------------------------------- #

.PHONY: clean

all: $(AOSIS_BIN)

%.o: %.asm
	$(AS) -g -felf32 $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

rust:


$(AOSIS_BIN): linker.ld $(KERNEL_SOURCE_C)
	$(CC) -T $< -o $@ $(CFLAGS) $(KERNEL_SOURCE_C)

clean:
	@echo "Kernel Clean Up"
	@$(RM) $(KERNEL_SOURCE_C) *.o *.bin *.iso aosis
