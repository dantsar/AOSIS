#!/bin/bash

# export SYSTEM_PROJECTS="libc kernel/rust kernel"
export SYSTEM_PROJECTS="libc kernel user"

export PROJ_NAME="aosis"
export DIR_ROOT=`realpath .`
export DIR_BUILD=$DIR_ROOT/build
export DIR_KERNEL=$DIR_ROOT/kernel
export DIR_LIBC=$DIR_ROOT/libc
