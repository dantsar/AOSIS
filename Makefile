# Credit should go to Brett Vickers' project MonkOS
# for the idea of the build environment

DIR_ROOT 	:= .

include $(DIR_ROOT)/config.mk

# -----------------------------------------------------------------------------
# Build targets
# -----------------------------------------------------------------------------

all: iso 

.PHONY: docker
docker: 
	@$(DIR_DOCKER)/build.sh iso

.PHONY: iso 
iso: 
	@echo "CREATING ISO"
	@$(DIR_SCRIPTS)/build.sh
	@$(DIR_SCRIPTS)/iso.sh

.PHONY: run
run: 
	@$(DIR_SCRIPTS)/run.sh

.PHONY: o_debug
o_debug: 
	@$(DIR_SCRIPTS)/debug.sh qemu

.PHONY: g_debug
g_debug: 
	@$(DIR_SCRIPTS)/debug.sh gdb 

.PHONY: clean
clean:
	@$(RM) $(DIR_BUILD) 
	@make --directory=$(DIR_KERNEL) clean
