#!/bin/bash

# current structure:
# start with simple shell scripts {Baby steps}
# config.sh -> header.sh -> build.sh -> iso.sh

# DIR_KERNEL="./kernel"
# DIR_BUILD="./build"
set -e
. ./scripts/config.sh

# mkdir -p "./build"
mkdir -p $DIR_BUILD/aosis/boot/grub && 
cp $DIR_BUILD/aosis.bin $DIR_BUILD/aosis/boot/aosis.bin && 
cp $DIR_KERNEL/boot/grub.cfg $DIR_BUILD/aosis/boot/grub/grub.cfg &&
grub-mkrescue -o $DIR_BUILD/aosis.iso $DIR_BUILD/aosis # 2> /dev/null
