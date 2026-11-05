# Change this to change the compiler used
# CC = clang
CC = gcc

# Use C11, debugging and warnings flags
define CFLAGS
-std=c11 \
-g3 \
-O3 \
-fsanitize=address \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
-Wstrict-prototypes
endef

# Turn on the address sanitizer
LDFLAGS = -fsanitize=address

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = opdracht3

.PHONY: all clean tarball check

all: $(PROG)

valgrind: LDFLAGS=
valgrind: CFLAGS=-Wall
valgrind: $(PROG)

opdracht3: opdracht3.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ *.o $(PROG)

tarball: opdracht3_submit.tar.gz

opdracht3_submit.tar.gz: $(PROG).c Makefile
	tar -czf $@ $^
