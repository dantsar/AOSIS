all: boot.bin shit.iso

boot.bin: boot.asm
	nasm boot.asm -f bin -o boot.bin

shit.iso: boot.bin
	dd if=boot.bin of=shit.iso

.PHONY: run
run: boot.bin
	qemu-system-x86_64 -hda boot.bin

.PHONY: clean
clean:
	rm boot.bin shit.iso