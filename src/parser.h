// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "traces.h"
#include "crc.h"

// Параметры заголовка
#define TOTAL_TITLE_LEN 7
#define TYPE_SECTION_IN_TITLE_POS 0
#define SECTION_SYZE_IN_TITLE_POS 1
#define SRC16_IN_TITLE_POS 5

#define AUTHOR_SECTION_TYPE 0
#define DATA_SECTION_TYPE 1
#define CONTROL_SECTION_TYPE 2

// Параметры секции автора данных
#define TOTAL_AUTHOR_SECTION_LEN 41
#define AUTHOR_NAME_IN_SECTION_POS 0
#define AUTHOR_NAME_SIZE 32
#define AUTHOR_TYPE_IN_SECTION_POSE 32
#define AUTHOR_TIMESTAMP_IN_SECTION_POSE 33

// Параметры секции данных
#define ADMIN_TYPE 1
#define USER_TYPE 2
#define USER_DATA_SIZE 1024
#define ADMIN_DATA_SIZE 2048

// Параметры секции контролльной суммы
#define TOTAL_CONTROL_SUM_LEN 4

// Имя файла, хранящего права доступа
#define ACCESS_FILE_NAME "access.bin"
#define MAX_ENTRIES 1000 // Максимальное количество записей в файле

// Абстрагирование типов данных, которые могут быть изменены в будущем
typedef uint64_t timestamp_t;
typedef uint32_t section_data_size_t;

// Структура для представления заголовка секции
typedef struct section_title
{
    uint8_t section_type;
    section_data_size_t section_data_size;
    uint16_t crc16_control_sum;
} Section_title;

// Структура для представления всего сообщения в удобном формате для удобной дальнейшей работы
typedef struct parsed_data
{
    Section_title author_section_title;
    Section_title data_section_title;
    Section_title control_section_title;

    // Секция автора данных
    uint8_t author_name[AUTHOR_NAME_SIZE];
    uint8_t author_type;
    timestamp_t timestamp;

    // Секция данных
    uint8_t *data;

    // Секция контрольной суммы
    uint32_t crc32_control_sum;

} Parsed_data;

// Функция не определена в спецификации, служит для вывода данных
void print_parsed_data(Parsed_data *src);

// функция не определена в спецификации, выполняет очистку выделенной памяти
void parse_data_delete(Parsed_data *src);

/*
CRC16 (state)  Ошибка 2.6
    /\ IF state == CORRECT
        THEN
            /\ _out (TRUE)
        ELSE
            /\ _out (FALSE)
*/
int check_section_by_crc16(const uint8_t *raw_data, size_t section_len, uint16_t crc16);

/*
CRC32 (dataStatus)  Ошибка 2.7
    /\ IF dataStatus == CORRECT
        THEN
            /\ _out (TRUE)
        ELSE
            /\ _out (FALSE)
*/
int check_by_crc32(const uint8_t *raw_data, size_t total_len, size_t data_len, uint32_t crc32);

/* Не выполняет какую-либо проверку, только записывает данные в структуру */
Section_title parse_title(const uint8_t *raw_data);

/* Вызывает функции parse_title и check_section_by_crc16 и выполняет проверку
    SectionTypeCheck(title, titleType)  Ошибка 2.5
    /\  IF
            /\
                \/ title != titleType
                \/ title == WRONGTITLETYPE    /* Возможно, что это условие избыточно, так как titleType однозначно представлен в Next
            /\ _out (FALSE)
        ELSE
            /\ _out (TRUE)
*/
Parsed_data *parse_author_section(Parsed_data *src, const uint8_t *unparsed_section_1);

/* Вызывает функции parse_title, check_section_by_crc16 и выполняет проверки:
    SectionTypeCheck(title, titleType)  Ошибка 2.5
    /\  IF
            /\
                \/ title != titleType
                \/ title == WRONGTITLETYPE    /* Возможно, что это условие избыточно, так как titleType однозначно представлен в Next
            /\ _out (FALSE)
        ELSE
            /\ _out (TRUE)

    MemoryAlloc ()   Ошибка 2.2
    \/ _out (TRUE)
    \/ _out (FALSE)
*/
Parsed_data *parse_data_section(Parsed_data *src, const uint8_t *unparsed_section_2, size_t data_size);

/* Вызывает функции parse_title и check_section_by_crc16 и выполняет проверку
    SectionTypeCheck(title, titleType)  Ошибка 2.5
    /\  IF
            /\
                \/ title != titleType
                \/ title == WRONGTITLETYPE    /* Возможно, что это условие избыточно, так как titleType однозначно представлен в Next
            /\ _out (FALSE)
        ELSE
            /\ _out (TRUE)
*/
Parsed_data *parse_control_section(Parsed_data *src, const uint8_t *unparsed_section_3);

/*
AccessCheck(authorType, authorName, accessList) Ошибка 2.9
    /\  IF
            /\ OpenFile() Извлечение базы с правами доступа
            /\ authorType == \in accessList.type
            /\ authorName == \in accessList.name
        THEN
            /\ _out (TRUE)
        ELSE
            /\ _out (FALSE)
*/
int check_access(uint8_t src_author_type, uint8_t *src_name);

/*Основная функция, выполняющая большую часть проверок:

    /\ AuthorTypeCheck ( package.authorType)  Проверка типа автора для определения размера блока памяти перед выделением памяти

    /\ MemoryAlloc() // Выделение памяти для экземпляра структуры

     Парсинг первой секции

    /\ SectionTypeCheck(package.titlesArr[0].titleType, AUTHORTITLE) /\ CRC16(package.titlesArr[0].state)

     Парсинг второй секции

    /\ SectionTypeCheck(package.titlesArr[1].titleType, DATATITLE) /\ MemoryAlloc()  Выделение памяти для хранения данных внутри структуры
    /\ CRC16(package.titlesArr[1].state)

     Парсинг третьей секции

    /\ SectionTypeCheck(package.titlesArr[2].titleType, CONTROLTITLE) /\ CRC16(package.titlesArr[2].state)

     Проверка доступа
    /\ AccessCheck(package.authorType, package.authorName, accessList)

     Проверка Контрольной суммы crc32
    /\ CRC32(package.dataStatus)

     Проверка времени доступа
    /\ TimeStampCheck(package.timeStamp)
*/
Parsed_data *parse_package(const uint8_t *src_array);

#endif // PARSER_H
