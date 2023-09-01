#!/bin/bash

# current structure:
# start with simple shell scripts {Baby steps}
# config.sh -> header.sh -> build.sh -> iso.sh

# DIR_KERNEL="./kernel"
# DIR_BUILD="./build"
set -e
. ./scripts/config.sh

# mkdir -p "./build"
mkdir -p $DIR_BUILD/grub/boot/grub &&
cp $DIR_BUILD/aosis.bin $DIR_BUILD/grub/boot/aosis.bin &&
cp $DIR_KERNEL/boot/grub.cfg $DIR_BUILD/grub/boot/grub/grub.cfg &&
mkdir -p $DIR_BUILD/grub/boot/modules &&
cp $DIR_USER/user.bin $DIR_BUILD/grub/boot/modules/user.mod &&
grub-mkrescue -o $DIR_BUILD/aosis.iso $DIR_BUILD/grub # 2> /dev/null
