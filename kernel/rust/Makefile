include ../../config.mk

RUST_FLAGS :=  -O -C panic=abort -C link-arg=-nostdlib --target=i686-unknown-linux-gnu

KERNEL_SOURCE_RUST	:=	test_rust.o

# ---------------------------------------------- #

all: build_rust

%.o: %.rs
	rustc $(RUST_FLAGS) --emit obj $< -o $@

.PHONY: build_rust
build_rust:
	cd ../../ && $(DIR_SCRIPTS)/rust.sh

librust.a: $(KERNEL_SOURCE_RUST)
	ar rc $@ $<

.PHONY: clean
clean:
	@rm -rf *.o *.a


