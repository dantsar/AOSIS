#!/bin/bash

# to debug with gdb, open up gdb $(path to iso)
# and type remote target localhost:1234

if [ -f "build/aosis.iso" ]; then
    qemu-system-i386 -s -S -cdrom build/aosis.iso
else 
    echo "ISO not found. Please build the ISO"
fi
