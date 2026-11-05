# Change this to change the compiler used
# CC = clang
CC = gcc

# Use C11, debugging and warnings flags
define CFLAGS
-std=c11 \
-g3 \
-Og \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
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

PROG = opdracht5

all: $(PROG)

opdracht5: opdracht5.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ *.o $(PROG) $(TESTS)

check: all $(TESTS)
	@echo "No checks implemented"

tarball: opdracht5_submit.tar.gz

opdracht5_submit.tar.gz: $(PROG:=.c) Makefile
	tar -czf $@ $^
