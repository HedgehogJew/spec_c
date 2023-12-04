#ifndef __EXAMPLE__
#define __EXAMPLE__

#include "example.h"
// Функция для генерации массива
// Не рекомендуется её читать т.к. она захардкожена целиком и полностью

void generate_array(uint8_t *array)
{
    for (int i = 0; i < 2114; i++)
    {
        array[i] = 0;
    }

    // Задаем пример первой секции
    //  Первый байт равен 0
    array[0] = 0;

    uint32_t value32 = 48;
    memcpy(&array[1], &value32, sizeof(uint32_t));

    uint8_t name[32] = "John Doe";
    for (int i = 8; i < 32; i++)
    {
        name[i] = 0;
    }

    memcpy(&array[7], name, strlen(name));

    array[39] = 1;

    uint64_t timestamp = (uint64_t)time(NULL) + 1000000000;
    memcpy(&array[40], &timestamp, sizeof(uint64_t));

    //
    // Задаем секцию данных
    //
    array[48] = 1;

    // Со второго по пятый байты - число uint32_t равное 48
    value32 = 2055;
    memcpy(&array[49], &value32, sizeof(uint32_t));

    // Задаём дату

    uint8_t *data = (uint8_t *)calloc(2048, sizeof(uint8_t));

    data[0] = 'H';
    data[1] = 'e';
    data[2] = 'l';
    data[3] = 'l';
    data[4] = 'o';
    data[5] = '\0';

    for (int i = 6; i < 2048; i++)
    {
        data[i] = (uint8_t)i;
    }

    memcpy(&array[55], data, 2048);

    // Задаём секцию контрольной суммы

    array[2103] = 2;

    value32 = 11;
    memcpy(&array[2104], &value32, sizeof(uint32_t));

    uint32_t crc32 = crc32_calculate(array, 2114);

    memcpy(&array[2110], &crc32, sizeof(uint32_t));

    // Шестой и седьмой байты - число uint16_t, полученное из calculate_crc
    uint16_t crcValue = crc16_calculate(array, 48);
    memcpy(&array[5], &crcValue, sizeof(uint16_t));

    crcValue = crc16_calculate(&array[48], 2055);
    memcpy(&array[53], &crcValue, sizeof(uint16_t));

    crcValue = crc16_calculate(&array[2103], 11);
    memcpy(&array[2108], &crcValue, sizeof(uint16_t));
}

void make_example(char *filename)
{
    uint8_t array[2114];

    generate_array(array);

    write_array_to_file(filename, array, 2114);

    uint8_t access_array[21];

    access_array[0] = 2;
    access_array[7] = 1;
    access_array[14] = 1;

    access_array[1] = ',';
    access_array[8] = ',';
    access_array[15] = ',';

    access_array[6] = '\n';
    access_array[13] = '\n';
    access_array[20] = '\n';

    uint32_t name_crc32 = crc32_calculate("Ivan DME", 8);
    memcpy(&access_array[2], &name_crc32, sizeof(uint32_t));

    name_crc32 = crc32_calculate("John Doe", 8);
    memcpy(&access_array[9], &name_crc32, sizeof(uint32_t));

    name_crc32 = crc32_calculate("Albert Einstein", 15);
    memcpy(&access_array[16], &name_crc32, sizeof(uint32_t));

    write_array_to_file("access.bin", access_array, 21);
}

#ifndef __MAIN__
#define __MAIN__
int main()
{
    make_example("example1.bin");
    return 0;
}

#endif
#endif // __EXAMPLE__