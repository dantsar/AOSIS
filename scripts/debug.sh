#!/bin/bash

set -e
. ./scripts/config.sh

if [[ $1 == 'qemu' ]]
then 
    if [ -f "build/aosis.iso" ]; then
        qemu-system-i386 -s -S -cdrom $DIR_BUILD/aosis.iso
    else 
        echo "ISO not found. Please build the ISO"
    fi
elif [[ $1 == 'gdb' ]]
then 
    gdb -tui -q -ex "target remote localhost:1234" -ex "set disassembly-flavor intel" \
    -ex "set disassemble-next-line on" -ex "show disassemble-next-line" \
    -ex "dir kernel" -ex "break kmain" $DIR_BUILD/aosis.bin
fi
