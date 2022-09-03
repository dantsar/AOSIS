include ../config.mk

LINKER_LD 	:= 	linker.ld
CFLAGS		:=	$(CFLAGS) -I../include/kernel -I../include/libc -L../build \
				-ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -nostartfiles -lgcc -ggdb

KERNEL_SOURCE     :=  kernel.o                  \
                        boot/boot.o             \
                        terminal/tty.o          \
                        terminal/shell.o        \
                        terminal/vga.o          \
                        interrupt/init_idt.o    \
                        interrupt/interrupt.o   \
                        interrupt/keyboard.o    \
                        interrupt/pic.o         \
                        div_zero.o              \
                        branch_nowhere.o        \
                        common/bitmap.o         \
                        common/ringbuff.o       \
                        common/ports.o          \
                        common/cpu.o            \
                        memory/pmm.o            \
                        libk.a                  \
						# rust/librust.a			\

AOSIS_BIN 		:= ../build/aosis.bin

# ---------------------------------------------- #

all: $(AOSIS_BIN)

%.o: %.asm
	$(AS) -g -felf32 $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(AOSIS_BIN): linker.ld $(KERNEL_SOURCE)
	$(CC) -T $< -o $@ $(CFLAGS) $(KERNEL_SOURCE)

.PHONY: clean
clean:
	@echo "Kernel Clean Up"
	@$(RM) $(KERNEL_SOURCE) *.o *.bin *.iso aosis
