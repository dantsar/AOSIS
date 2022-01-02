include ../config.mk

CFLAGS 		:= -lgcc -ggdb -std=gnu11 -ffreestanding -nostdlib -Wall -Wextra

LIBK_BIN_DEP 		:= 	stdio/printf.o		\
						stdio/putchar.o 	\
						stdio/puts.o		\
						string/strcmp.o		\
						string/memcmp.o 	\
						string/memcpy.o 	\
						string/memset.o 	\
						string/memmove.o	\
						string/memset.o		\
						string/strlen.o		\
						string/strnlen.o	\
						ctype/isalnum.o		\
						ctype/isalpha.o		\
						ctype/isascii.o		\
						ctype/isblank.o		\
						ctype/iscntrl.o		\
						ctype/isgraph.o		\
						ctype/isdigit.o		\
						ctype/islower.o		\
						ctype/isprint.o		\
						ctype/ispunct.o		\
						ctype/isspace.o		\
						ctype/isupper.o		\
						ctype/isxdigit.o	\

# ---------------------------------------------- #

all: libk.a

%.o: %.c
	$(CC)  -I../include/libc -c $< -o $@

libk.a: $(LIBK_BIN_DEP)
	$(AR) $@ $^
	@echo "LIBK SUCCESSFULLY CREATED"
	@cp $@ ../kernel

clean:
	@$(RM) $(LIBK_BIN_DEP) libk.a
