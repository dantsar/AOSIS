include ../config.mk

LINKER_LD 	:= 	linker.ld
CFLAGS		:=	$(CFLAGS) -I../include -I../include/libc -L../build \
				-ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -lgcc -ggdb

KBIN_DEP 	:= 	kernel.o 			  	\
			 	boot/boot.o 		  	\
				terminal/tty.o 		  	\
				terminal/vga.o		  	\
				interrupt/init_idt.o 	\
				interrupt/interrupt.o 	\
				interrupt/keyboard.o	\
				interrupt/pic.o			\
				libk.a					\
				div_zero.o				\
				branch_nowhere.o		\

AOSIS_BIN 		:= ../build/aosis.bin

# ---------------------------------------------- #

.PHONY: clean

all: $(AOSIS_BIN)

%.o: %.asm
	$(AS) -g -felf32 $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

$(AOSIS_BIN): linker.ld $(KBIN_DEP)
	$(CC) -T $< -o $@ $(CFLAGS) $(KBIN_DEP)

clean: 
	@echo "Kernel Clean Up"
	@$(RM) $(KBIN_DEP) *.o *.bin *.iso aosis
