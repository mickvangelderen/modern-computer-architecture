#include "rvex/rvex.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

const char RVEX_START = '1';
const char RVEX_CLEAR = '2';
const char RVEX_DONE = '3';

#ifdef DEBUG
#define D_PRINTF(fmt, args...) printf(fmt, ## args)
#else
#define D_PRINTF(fmt, args...)
#endif

Rvex rvex0;

// works like open(const char * path, int oflag, ...) but only supports path and flag parameters. Prints errors if any.
static
int rvexOpen(const char * path, int oflag) {
	const size_t BUFFER_SIZE = 1024;
	char buffer[1024];
	// Ints to save errno and file descriptor
	int e, fd;

	fd = open(path, oflag);
	if (fd < 0) {
		e = errno;
		// Try to convert error code to string and place it in the buffer
		if (strerror_r(e, buffer, BUFFER_SIZE) == 0) {
			printf("rvex: Error while opening \"%s\": %s\n", path, buffer);
		} else {
			printf("rvex: Error while opening \"%s\": errno %d\n", path, e);
		}
		return -1;
	}
	return fd;
}

int rvexInit(Rvex * rvex, const unsigned char bytecode[], size_t size,
	const char * instr, const char * mem, const char * ctl, const char * stat) {

	D_PRINTF("rvex: Opening files...\n");
	rvex->instr = rvexOpen(instr, O_WRONLY);
	rvex->mem = rvexOpen(mem, O_RDWR);
	rvex->ctl = rvexOpen(ctl, O_WRONLY);
	rvex->stat = rvexOpen(stat, O_RDONLY);
	if (rvex->instr < 0 || rvex->mem < 0 || rvex->ctl < 0 || rvex->stat < 0) {
		return -1;
	}
	D_PRINTF("rvex: Files opened...\n");

	D_PRINTF("rvex: writing bytecode...%d\n", (int) size);
	write(rvex->instr, bytecode, size);
	D_PRINTF("rvex: bytecode written...\n");

	return 0;
}

void rvexDispose(Rvex * rvex) {
	D_PRINTF("rvex: Closing files...\n");
	close(rvex->mem); rvex->mem = -1;
	close(rvex->ctl); rvex->ctl = -1;
	close(rvex->stat); rvex->stat = -1;
	close(rvex->instr); rvex->instr = -1;
	D_PRINTF("rvex: Files closing...\n");
}
