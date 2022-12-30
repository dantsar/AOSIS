#!/bin/bash

if [ -f "build/aosis.iso" ]; then
    qemu-system-i386 -m 1G -cdrom build/aosis.iso
else
    echo "ISO not found. Please build the ISO"
fi
