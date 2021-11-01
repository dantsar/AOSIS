#!/bin/bash

if [ -f "build/aosis.iso" ]; then
    qemu-system-i386 -cdrom build/aosis.iso
else 
    echo "ISO not found. Please build the ISO"
fi
