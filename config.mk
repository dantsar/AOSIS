# config.mk
# Makefile configuration settings 

DIR_DOCKER		:= $(DIR_ROOT)/docker
DIR_KERNEL		:= $(DIR_ROOT)/kernel
DIR_BUILD		:= $(DIR_ROOT)/build
DIR_SCRIPTS		:= $(DIR_ROOT)/scripts
DIR_LIBC		:= $(DIR_ROOT)/libc

CC				:= i686-elf-gcc

AR 				:= ar
AS 				:= nasm
QEMU 			:= qemu-system-i386
RM				:= rm -rf

# --------------------- 
# Makefile color macros
# https://gist.github.com/rsperl/d2dfe88a520968fbc1f49db0a29345b9
# ---------------------

# 
# stuff here
#
