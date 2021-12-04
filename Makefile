# Credit should go to Brett Vickers' project MonkOS
# for the idea of the build environment

DIR_ROOT 	:= .

include $(DIR_ROOT)/config.mk

# -----------------------------------------------------------------------------
# Build targets
# -----------------------------------------------------------------------------

.PHONY: all docker iso run o_debug g_debug clean

all: iso 

docker: 
	@$(DIR_DOCKER)/build.sh iso

iso: 
	@echo "CREATING ISO"
	@$(DIR_SCRIPTS)/build.sh
	@$(DIR_SCRIPTS)/iso.sh

run: 
	@$(DIR_SCRIPTS)/run.sh

# lanuches qemu with remote gdb stub and waits
qdb:
	@$(DIR_SCRIPTS)/debug.sh qemu

# launches and connects gdb to qemu stub
gdb:
	@$(DIR_SCRIPTS)/debug.sh gdb 

clean:
	@$(RM) $(DIR_BUILD) 
	@$(DIR_SCRIPTS)/clean.sh
