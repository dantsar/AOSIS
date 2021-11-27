include ../config.mk

CFLAGS 		:= -lgcc -ggdb

LIBK_BIN_DEP 		:= 	stdio/printf.o		\
						stdio/putchar.o 	\
						stdio/puts.o		\
						string/memcmp.o 	\
						string/memcpy.o 	\
						string/memset.o 	\
						string/memmove.o	\
						string/memset.o		\
						string/strlen.o		\
						string/strnlen.o

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
