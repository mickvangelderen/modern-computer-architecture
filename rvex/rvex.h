#ifndef RVEX_FILES_H
#define RVEX_FILES_H

#include <stddef.h>
#include <unistd.h>

#define RVEX_CORE_CTL_FILE "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_ctl"
#define RVEX_CORE_STATUS_FILE "/sys/devices/virtual/rvex_core/rvex-smemory.0/rvex_core_status"
#define RVEX_DATA_MEMORY_FILE "/dev/rvex-dmemory.0"
#define RVEX_INSTRUCTION_MEMORY_FILE "/dev/rvex-imemory.0"

#define changeEndianess32(d) \
	(((d) & 0xff000000) >> 24) | \
	(((d) & 0x00ff0000) >> 8) | \
	(((d) & 0x0000ff00) << 8) | \
	(((d) & 0x000000ff) << 24)

#define changeEndianess16(d) \
	(((d) & 0xff00) >> 8) | \
	(((d) & 0x00ff) << 8)

// Opens files and loads bytecode
int rvexInit(char bytecode[]);

// Closes files
void rvexDeInit();

int rvexSeek(int offset);

// Write to rvex data memory
ssize_t rvexWrite(const void *buf, size_t count);

// Read from rvex data memory
ssize_t rvexRead(void *buf, size_t count);

// Writes clear and start to rvex and returns when the computation is done
void rvexGo();

#endif
