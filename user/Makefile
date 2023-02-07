include ../config.mk


CFLAGS	:= $(CFLAGS) -ffreestanding -Wall -Wextra -std=gnu11 -nostdlib -nostartfiles -lgcc -ggdb

all: user.bin
	@echo "hello from userspace"

user.bin: linker.ld start.o main.o
	$(CC) -T $< -o $@ $(CFLAGS) start.o main.o

%.o: %.asm
	$(AS) -g -felf32  $< -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(RM) *.o *.bin
