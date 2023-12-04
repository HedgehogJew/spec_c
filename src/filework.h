#ifndef FILE_WORK_H
#define FILE_WORK_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void write_array_to_file(const char *filename, const uint8_t *data, size_t size);

uint8_t *read_array_from_file(const char *filename, size_t size);

#endif // FILE_WORK_H
