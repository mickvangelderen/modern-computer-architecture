CC := gcc
CFLAGS := -std=gnu99 -m32
CFLAGSSB2 := -std=gnu99 -DWORDS_BIGENDIAN
SOURCEDIR := src/
BUILDDIR :=  build/
BINDIR := bin/

.PHONY: main
main: src
	$(CC) $(SOURCEDIR)main.c -o $(BINDIR)adder $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)* $(BINDIR)*

# Build for microblaze

.PHONY: microblaze
microblaze: src
	sb2 $(CC) $(SOURCEDIR)main.c -o $(BINDIR)adder $(CFLAGSSB2)
