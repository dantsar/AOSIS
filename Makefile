# https://stackoverflow.com/a/12099167
ifeq ($(OS),Windows_NT)
    RM=del -fR
else
    RM=rm -fr
endif


all: boot.bin

boot.bin: boot.asm
	nasm boot.asm -f bin -o boot.bin

.PHONY: run
run: boot.bin
	qemu-system-x86_64 -hda boot.bin

.PHONY: clean
clean:
	$(RM) boot.bin 


# shit.iso
# shit.iso: boot.bin
# dd if=boot.bin of=shit.iso