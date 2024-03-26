#ifndef _CRC32_H_
#define _CRC32_H_

#include <stdint.h>
#include <stddef.h>

uint32_t crc32(const void *buf, size_t size);
uint32_t file_crc(const char* file_name);

#endif


