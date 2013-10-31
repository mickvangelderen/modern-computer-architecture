#include "rvex/rvex.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "rvex/bytecode.binc"

#define RVEX_CORE_CTL_FILE "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl"
#define RVEX_CORE_STATUS_FILE "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status"
#define RVEX_DATA_MEMORY_FILE "/dev/rvex-dmemory.0"
#define RVEX_INSTRUCTION_MEMORY_FILE "/dev/rvex-imemory.0"

const unsigned char RVEX_START = '1';
const unsigned char RVEX_CLEAR = '2';
const unsigned char RVEX_DONE = '3';

int fd_instr;
int fd_mem;
int fd_ctl;
int fd_stat;

// works like open(const char * path, int oflag, ...) but only supports path and flag parameters. Prints errors if any.
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

int rvexInit() {

	printf("rvex: Opening files...\n");
	fd_instr = rvexOpen(RVEX_INSTRUCTION_MEMORY_FILE, O_WRONLY);
	fd_mem = rvexOpen(RVEX_DATA_MEMORY_FILE, O_RDWR);
	fd_ctl = rvexOpen(RVEX_CORE_CTL_FILE, O_WRONLY);
	fd_stat = rvexOpen(RVEX_CORE_STATUS_FILE, O_RDONLY);

	if (fd_instr < 0 || fd_mem < 0 || fd_ctl < 0 || fd_stat < 0) {
		return -1;
	}

	// Incorporated the bytecode into the executable ;D
	write(fd_instr, bytecode, sizeof(bytecode));

	return 0;
}

void rvexDeInit() {
	close(fd_mem);
	close(fd_ctl);
	close(fd_stat);
	close(fd_instr);
}

ssize_t rvexWrite(const void *buf, size_t count) {
	return write(fd_mem, buf, count);	
}

ssize_t rvexRead(off_t offset, void *buf, size_t count) {
	lseek(fd_mem, offset, SEEK_SET);
	return read(fd_mem, buf, count);
}

void rvexGo() {
	write(fd_ctl, &RVEX_CLEAR, sizeof(unsigned char));
	write(fd_ctl, &RVEX_START, sizeof(unsigned char));
	// printf("Wrote clear and start to \"%s\"\n", RVEX_CORE_CTL_FILE);
	unsigned char status;
	do {
		lseek(fd_stat, 0, SEEK_SET);
		read(fd_stat, &status, sizeof(unsigned char));
		// printf("Status %d", status);
	} while (status != '3');
}