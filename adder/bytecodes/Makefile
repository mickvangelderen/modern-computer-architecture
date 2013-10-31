BINUTILS = ~/tools/binutils-rovex
AS = $(BINUTILS)/gas/as-new
LD = $(BINUTILS)/ld/ld-new
ELF2VHD = $(BINUTILS)/binutils/elf2vhd
CC = ~/tools/vex-3.43/bin/cc
CFLAGS = -fno-xnop -autoinline -fexpand-div -O3 -fmm=pipe_1_4_fw.mm
SIM = ~/tools/xstsim-ST231-r-VEX-0.3.15-fixed
SIMFLAGS = --ips='"[r-VEX c]"'  --c.trace=5 --c.trace_regs=3
SRCDIR = ./src
PWD = $(shell pwd)

EXECUTABLES = sum adder x264
HEADERS = $(EXECUTABLES:%=headers/%.h)

.SUFFIXES:
.PRECIOUS: %.o %.s

.PHONY: all
all: $(EXECUTABLES)

headers: $(HEADERS)

headers/%.h: byte-%
	../../bin2c/bin/bin2c bytecode $@

_start.s: $(SRCDIR)/_start.s
	cp $^ $@

%.s: $(SRCDIR)/%.c
ifdef UART
ifeq (${UART}, sim)
	$(CC) $(CFLAGS) -D_UART=2 -S $<
else
	$(CC) $(CFLAGS) -D_UART=1 -S $<
endif
else
	$(CC) $(CFLAGS) -S $<
endif

%.o: %.s
	$(AS) $< -o $@

ifdef UART
%: %.o _start.o uart.o
else
%: %.o _start.o
	$(LD) $^ -o $@
endif
	$(LD) $^ -Map $@.map -o $@

.PHONY: run
run: $(EXECUTABLES:%=run-%)

run-% : %
	$(SIM) $(SIMFLAGS) --c.target_exec='"$^"'

vhd-% : %
	$(ELF2VHD) --vhd $^

byte-% : %
	$(ELF2VHD) --bytecode --bytedata $^

.PHONY: clean
clean:
	$(RM) $(EXECUTABLES) *.o *.s *.vhd *.h *.hex *.map
