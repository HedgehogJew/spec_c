#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "filework.h"
#include "crc.h"

// Функции для генерации массива
// Не рекомендуется её читать т.к. она захардкожена целиком и полностью

void generate_array(uint8_t *array);

void make_example(char *filename);

int main();

#endif