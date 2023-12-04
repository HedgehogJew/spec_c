#ifndef __FILEWORK__
#define __FILEWORK__

#include "filework.h"
void write_array_to_file(const char *filename, const uint8_t *data, size_t size)
{
    FILE *file = fopen(filename, "wb"); // Открываем файл для записи в бинарном режиме ("wb")

    if (file != NULL)
    {
        fwrite(data, sizeof(uint8_t), size, file); // Записываем массив данных в файл
        fclose(file);                              // Закрываем файл
        printf("Данные успешно записаны в файл %s\n", filename);
    }
    else
    {
        perror("Ошибка открытия файла");
    }
}

uint8_t *read_array_from_file(const char *filename, size_t size)
{
    FILE *file = fopen(filename, "rb"); // Открываем файл для чтения в бинарном режиме ("rb")

    if (file != NULL)
    {

        uint8_t *data = (uint8_t *)malloc(size); // Выделяем память под массив
        if (data != NULL)
        {

            fread(data, sizeof(uint8_t), size, file); // Считываем массив из файла
            fclose(file);                             // Закрываем файл
            return data;                              // Возвращаем указатель на считанный массив
        }
        else
        {
            printf("Ошибка выделения памяти\n");
        }
    }
    else
    {
        printf("Ошибка открытия файла\n");
    }

    return NULL; // В случае ошибки возвращаем NULL
}

#endif