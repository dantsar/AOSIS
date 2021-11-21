#!/bin/bash

# to debug with gdb, open up gdb $(path to iso)
# and type remote target localhost:1234
if [[ $1 == 'qemu' ]]
then 
    if [ -f "build/aosis.iso" ]; then
        qemu-system-i386 -s -S -cdrom build/aosis.iso
    else 
        echo "ISO not found. Please build the ISO"
    fi
elif [[ $1 == 'gdb' ]]
then 
    gdb -ex "target remote localhost:1234" -ex "break kmain" \
    -ex "set disassemble-next-line on" -ex "show disassemble-next-line" \
    ./kernel/aosis.bin
fi
