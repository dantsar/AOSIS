#!/bin/bash

set -e
. ./scripts/config.sh

# echo $DIR_ROOT

if [[ $1 == 'qemu' ]]
then 
    if [ -f "build/aosis.iso" ]; then
        qemu-system-i386 -s -S -cdrom $DIR_BUILD/aosis.iso
    else 
        echo "ISO not found. Please build the ISO"
    fi
elif [[ $1 == 'gdb' ]]
then 
    gdb -tui -q -cd=$DIR_ROOT -iex "set auto-load safe-path $DIR_ROOT" \
        -iex "set auto-load local-gdbinit on" $DIR_BUILD/aosis.bin
fi
# -tui
