#ifndef RVEX_HEADER_GUARD
#define RVEX_HEADER_GUARD

#include <stddef.h>
#include <unistd.h>

#define RVEX_0_INSTRUCTION_MEMORY_FILE "/dev/rvex-imemory.0"
#define RVEX_0_DATA_MEMORY_FILE "/dev/rvex-dmemory.0"
#define RVEX_0_CORE_CTL_FILE "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl"
#define RVEX_0_CORE_STATUS_FILE "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status"

extern const char RVEX_START;
extern const char RVEX_CLEAR;
extern const char RVEX_DONE;

typedef struct Rvex {
	int instr;
	int mem;
	int ctl;
	int stat;
} Rvex;

extern Rvex rvex0;

// Opens files and loads bytecode
int rvexInit(Rvex * rvex, const unsigned char bytecode[], size_t size, const char * instr, const char * mem, const char * ctl, const char * stat);

// Closes files
void rvexDispose(Rvex * rvex);

static inline
int rvexSeek(Rvex * rvex, int offset) {
	return lseek(rvex->mem, offset, SEEK_SET);
}

static inline
int rvexSeekRel(Rvex * rvex, int offset) {
	return lseek(rvex->mem, offset, SEEK_CUR);
}

// Write to rvex data memory
static inline
ssize_t rvexWrite(Rvex * rvex, const void * buf, size_t count) {
	return write(rvex->mem, buf, count);
}

// Read from rvex data memory
static inline
ssize_t rvexRead(Rvex * rvex, void * buf, size_t count) {
	return read(rvex->mem, buf, count);
}

// Writes clear and start to rvex and returns when the computation is done
static inline
void rvexGo(Rvex * rvex) {
	write(rvex->ctl, &RVEX_CLEAR, sizeof(unsigned char));
	write(rvex->ctl, &RVEX_START, sizeof(unsigned char));
	unsigned char status;
	do {
		lseek(rvex->stat, 0, SEEK_SET);
		read(rvex->stat, &status, sizeof(unsigned char));
	} while (status != '3');
}

#endif
