# Credit should go to Brett Vickers' project MonkOS
# for the idea of the build environment

DIR_ROOT := .
include $(DIR_ROOT)/config.mk

# -----------------------------------------------------------------------------
# Build targets
# -----------------------------------------------------------------------------

all: mkbuild iso

docker: .force
	rm -rf iso
	@$(DIR_DOCKER)/build.sh iso

.PHONY: mkbuild
mkbuild:
	@mkdir -p $(DIR_BUILD)

iso: mkbuild
	@echo "CREATING ISO"
	make --directory=$(DIR_KERNEL)

run: .force
	make --directory=$(DIR_KERNEL) run

.PHONY: clean
clean:
	rm -rf $(DIR_BUILD) 
	make --directory=$(DIR_KERNEL) clean

.force: