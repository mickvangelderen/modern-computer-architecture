#ifndef RVEX_FILES_H
#define RVEX_FILES_H

#include <stddef.h>
#include <unistd.h>

// Opens files and loads bytecode
int rvexInit();

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
