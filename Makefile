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

.PHONY: clean
clean:
	@rm -rf $(DIR_BUILD) 
	@make --directory=$(DIR_KERNEL) clean
