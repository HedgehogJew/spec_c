#ifndef __PARSER__
#define __PARSER__

#include "parser.h"

// Вывод данных
void print_parsed_data(Parsed_data *src)
{
    if (src == NULL)
    {
        printf("Arguments error in printf_parsed_data function.\n");
        return;
    }
    printf("Author title:\nSection type: %d | Section size: %d | Crc16 control sum: %u\n",
           src->author_section_title.section_type, src->author_section_title.section_data_size, src->author_section_title.crc16_control_sum);
    printf("Name: ");

    for (int i = 0; i < AUTHOR_NAME_SIZE; i++)
    {
        printf("%c", src->author_name[i]);
    }

    printf(" | Author type: %d | Timestamp: %lu\n", src->author_type, src->timestamp);

    printf("\nData title:\nSection type: %d | Section size: %d | Crc16 control sum: %u\n\nData:\n",
           src->data_section_title.section_type, src->data_section_title.section_data_size, src->data_section_title.crc16_control_sum);
    if (src->data != NULL)
    {
        for (int i = 0; i < ADMIN_DATA_SIZE; i++)
        {
            printf("%c", src->data[i]);
        }
    }

    printf("\n\nControl title:\nSection type: %d | Section size: %d | Crc16 control sum: %u\n",
           src->control_section_title.section_type, src->control_section_title.section_data_size, src->control_section_title.crc16_control_sum);
    printf("CRC32: %u\n\n", src->crc32_control_sum);
}

// Очистка памяти после работы со структурой
void parse_data_delete(Parsed_data *src)
{
    if (src != NULL)
    {
        if (src->data != NULL)
        {
            free(src->data);
        }
        else
        {
            printf("Src.data is already deleted\n");
        }
        free(src);
    }
    else
    {
        printf("Src is already deleted\n");
    }
    return;
}

// Проверка контрольной суммы секции.
int check_section_by_crc16(const uint8_t *raw_data, size_t section_len, uint16_t crc16)
{
    // Копирование входных данных
    uint8_t local_data[section_len];
    memcpy(local_data, raw_data, section_len);

    // При проверке контрольной суммы, поля, предназначенные для её хранения, должны быть равны нулю
    local_data[SRC16_IN_TITLE_POS] = 0;
    local_data[SRC16_IN_TITLE_POS + 1] = 0;

    if (crc16 == crc16_calculate(local_data, section_len))
    {
        return 1;
    }
    else
    {
        printf("Crc16 calculation error\n");
        return 0;
    }
}

// Проверка контрольнойй суммы всего файла
int check_by_crc32(const uint8_t *raw_data, size_t total_len, size_t data_len, uint32_t crc32)
{
    uint8_t local_data[total_len];
    memcpy(local_data, raw_data, total_len);

    // При проверке контрольной суммы, поля, предназначенные для хранения сумм секций, должны быть равны нулю
    uint16_t zero_16 = 0;
    memcpy(&local_data[SRC16_IN_TITLE_POS], &zero_16, sizeof(zero_16));
    memcpy(&local_data[TOTAL_AUTHOR_SECTION_LEN + TOTAL_TITLE_LEN + SRC16_IN_TITLE_POS], &zero_16, sizeof(zero_16));
    memcpy(&local_data[TOTAL_AUTHOR_SECTION_LEN + data_len + 2 * TOTAL_TITLE_LEN + SRC16_IN_TITLE_POS], &zero_16, sizeof(zero_16));

    // При проверке контрольной суммы, поле, предназначенные для её хранения, должно быть равным нулю
    uint32_t zero_32 = 0;
    memcpy(&local_data[TOTAL_TITLE_LEN * 3 + TOTAL_AUTHOR_SECTION_LEN + data_len], &zero_32, sizeof(zero_32));

    if (crc32 == crc32_calculate(local_data, total_len))
    {
        return 1;
    }
    else
    {
        printf("Crc32 calculation error\n");
        return 0;
    }
}

// Функция для парсинга массива в заголовок
Section_title parse_title(const uint8_t *raw_data)
{
    Section_title res;
    res.section_type = raw_data[TYPE_SECTION_IN_TITLE_POS];

    // Копирование размера данных в структуру с использованием memcpy
    memcpy(&res.section_data_size, &raw_data[SECTION_SYZE_IN_TITLE_POS], sizeof(section_data_size_t));

    memcpy(&res.crc16_control_sum, &raw_data[SRC16_IN_TITLE_POS], sizeof(uint16_t));

    return res;
}

// Парсинг секции автора
Parsed_data *parse_author_section(Parsed_data *src, const uint8_t *unparsed_section_1)
{
    if (src == NULL || unparsed_section_1 == NULL)
    {
        printf("Arguments error in parse author section. Return NULL\n");
        g_trace.parse_author_section_correction_args = ERROR;
        return NULL;
    }
    g_trace.parse_author_section_correction_args = PASSED;

    // Проверка типа секции
    if (unparsed_section_1[TYPE_SECTION_IN_TITLE_POS] != AUTHOR_SECTION_TYPE)
    {
        printf("Incorrect type section. Return NULL.\n");
        g_trace.parse_author_section_correction_section_type = ERROR;
        return NULL;
    }
    g_trace.parse_author_section_correction_section_type = PASSED;

    // Копирование данных из массива в структуру
    src->author_section_title = parse_title(unparsed_section_1);
    memcpy(src->author_name, unparsed_section_1 + TOTAL_TITLE_LEN + AUTHOR_NAME_IN_SECTION_POS, AUTHOR_NAME_SIZE);
    src->author_type = unparsed_section_1[TOTAL_TITLE_LEN + AUTHOR_TYPE_IN_SECTION_POSE];
    memcpy(&src->timestamp, &unparsed_section_1[TOTAL_TITLE_LEN + AUTHOR_TIMESTAMP_IN_SECTION_POSE], sizeof(timestamp_t));

    // проверка контрольной суммы первой секции
    if (!check_section_by_crc16(unparsed_section_1, TOTAL_AUTHOR_SECTION_LEN + TOTAL_TITLE_LEN, src->author_section_title.crc16_control_sum))
    {
        g_trace.parse_author_section_crc16_control = ERROR;
        printf("First section crc16 error. Return NULL\n");
        return NULL;
    }
    g_trace.parse_author_section_crc16_control = PASSED;

    return src;
}

// Парсинг секции данных
Parsed_data *parse_data_section(Parsed_data *src, const uint8_t *unparsed_section_2, size_t data_size)
{
    if (src == NULL || unparsed_section_2 == NULL)
    {
        printf("Arguments error in parse data section. Return NULL\n");
        g_trace.parse_data_section_correction_args = ERROR;
        return NULL;
    }
    g_trace.parse_data_section_correction_args = PASSED;

    // Проверка типа секции
    if (unparsed_section_2[TYPE_SECTION_IN_TITLE_POS] != DATA_SECTION_TYPE)
    {
        g_trace.parse_data_section_correction_section_type = ERROR;
        printf("Incorrect type section. Return NULL.\n");
        return NULL;
    }
    g_trace.parse_data_section_correction_section_type = PASSED;

    src->data_section_title = parse_title(unparsed_section_2);

    // ВЫделение памяти для массива, хранящего данные
    src->data = (uint8_t *)calloc(data_size, sizeof(uint8_t));
    if (src->data == NULL)
    {
        g_trace.parse_data_section_memory_allocate = ERROR;
        printf("Memory allocation error. Return NULL\n");
        return NULL;
    }
    g_trace.parse_data_section_memory_allocate = PASSED;

    if (!check_section_by_crc16(unparsed_section_2, TOTAL_TITLE_LEN + data_size, src->data_section_title.crc16_control_sum))
    {
        g_trace.parse_data_section_crc16_control = ERROR;
        printf("Second section crc16 error. Return NULL\n");
        return NULL;
    }
    g_trace.parse_data_section_crc16_control = PASSED;

    memcpy(src->data, &unparsed_section_2[TOTAL_TITLE_LEN], data_size);

    return src;
}

// Парсинг секции проверки
Parsed_data *parse_control_section(Parsed_data *src, const uint8_t *unparsed_section_3)
{
    if (src == NULL || unparsed_section_3 == NULL)
    {
        g_trace.parse_control_section_correction_args = ERROR;
        printf("Arguments error in parse control section. Return NULL\n");
        return NULL;
    }
    g_trace.parse_control_section_correction_args = PASSED;

    // Проверка типа секции
    if (unparsed_section_3[TYPE_SECTION_IN_TITLE_POS] != CONTROL_SECTION_TYPE)
    {
        g_trace.parse_control_section_correction_section_type = ERROR;
        printf("Incorrect type section. Return NULL.\n");
        return NULL;
    }
    g_trace.parse_control_section_correction_section_type = PASSED;

    src->control_section_title = parse_title(unparsed_section_3);

    // Проверка контрольной суммы третьей секции
    if (!check_section_by_crc16(unparsed_section_3, TOTAL_CONTROL_SUM_LEN + TOTAL_TITLE_LEN, src->control_section_title.crc16_control_sum))
    {
        g_trace.parse_control_section_crc16_control = ERROR;
        printf("Third section parse error. Return NULL\n");
        return NULL;
    }
    g_trace.parse_control_section_crc16_control = PASSED;

    memcpy(&src->crc32_control_sum, &unparsed_section_3[TOTAL_TITLE_LEN], sizeof(uint32_t));

    return src;
}

// Функция проверки прав доступа
int check_access(uint8_t src_author_type, uint8_t *src_name)
{
    FILE *file = fopen(ACCESS_FILE_NAME, "rb");

    if (file == NULL)
    {
        g_trace.is_acess_file_open = ERROR;
        printf("Error opening file\n");
        return 0;
    }
    g_trace.is_acess_file_open = PASSED;

    size_t entries = 0;
    size_t buf_size = sizeof(uint8_t) + sizeof(char) + sizeof(uint32_t) + sizeof(char); // = 7
    uint8_t buf[buf_size];                                                              // Максимальная длина строки (1 байт на тип, 1 байт на запятую, 4 байт на CRC32, 1 байт на '\n')

    size_t name_len = strlen((char *)src_name);
    // printf("Current \nname: %s\nauthor type: %u\n", src_name, src_author_type);

    uint32_t src_crc32 = crc32_calculate(src_name, name_len);

    while (fread(buf, sizeof(uint8_t), 7, file) != 0 && entries < MAX_ENTRIES)
    {

        uint8_t type = buf[0];
        uint32_t crc32 = 0;

        memcpy(&crc32, &buf[2], sizeof(uint32_t));

        // printf("type = %u, crc32 = %u ", type, crc32);

        if (type == src_author_type && crc32 == src_crc32)
        {
            fclose(file);
            g_trace.author_have_access = PASSED;
            return 1;
        }

        entries++;
    }

    g_trace.author_have_access = ERROR;
    fclose(file);
    return 0;
}

// Основная функция
Parsed_data *parse_package(const uint8_t *src_array)
{
    if (src_array == NULL)
    {
        printf("Argument error in parse data function\n. Return NULL\n");
        g_trace.parse_package_correction_args = ERROR;
        return NULL;
    }
    g_trace.parse_package_correction_args = PASSED;

    // Извлечение типа автора для рассчёта размера файла
    uint8_t author_type = src_array[TOTAL_TITLE_LEN + AUTHOR_TYPE_IN_SECTION_POSE];
    size_t data_size = 0;

    if (author_type == (uint8_t)ADMIN_TYPE)
    {
        data_size = ADMIN_DATA_SIZE;
    }
    else if (author_type == (uint8_t)USER_TYPE)
    {
        data_size = USER_DATA_SIZE;
    }
    else
    {
        g_trace.parse_package_correction_author_type = ERROR;
        printf("Incorrect author type or wrong adress. Return NULL\n");
        return NULL;
    }

    g_trace.parse_package_correction_author_type = PASSED;

    // Разбиение и копирование исходного блока данных на секции в соответствии с определёнными в начале константами
    size_t shift = 0; // Переменная введена для упрощения записи
    uint8_t unparsed_section_1[TOTAL_TITLE_LEN + TOTAL_AUTHOR_SECTION_LEN];
    memcpy(unparsed_section_1, src_array, TOTAL_TITLE_LEN + TOTAL_AUTHOR_SECTION_LEN);

    shift += TOTAL_TITLE_LEN + TOTAL_AUTHOR_SECTION_LEN;

    uint8_t unparsed_section_2[TOTAL_TITLE_LEN + data_size];
    memcpy(unparsed_section_2, &src_array[shift], TOTAL_TITLE_LEN + data_size);

    shift += TOTAL_TITLE_LEN + data_size;

    uint8_t unparsed_section_3[TOTAL_TITLE_LEN + TOTAL_CONTROL_SUM_LEN];
    memcpy(unparsed_section_3, &src_array[shift], TOTAL_TITLE_LEN + TOTAL_CONTROL_SUM_LEN);

    size_t total_len = shift + TOTAL_TITLE_LEN + TOTAL_CONTROL_SUM_LEN;

    // Создание экземпляра структуры и проверка выделения памяти
    Parsed_data *res = (Parsed_data *)calloc(1, sizeof(Parsed_data));
    if (res == NULL)
    {
        g_trace.parse_package_memory_allocate_for_structure = ERROR;
        printf("Memory allocation error in parse data function. Return NULL\n");
        return NULL;
    }

    g_trace.parse_package_memory_allocate_for_structure = PASSED;

    if (parse_author_section(res, unparsed_section_1) == NULL)
    {
        printf("Parse author section error. Return NULL\n");
        parse_data_delete(res);
        return NULL;
    }

    if (parse_data_section(res, unparsed_section_2, data_size) == NULL)
    {
        printf("Parse data section error. Return NULL\n");
        parse_data_delete(res);
        return NULL;
    }

    if (parse_control_section(res, unparsed_section_3) == NULL)
    {
        printf("Parse control section error. Return NULL\n");
        parse_data_delete(res);
        return NULL;
    }

    if (!check_access(res->author_type, res->author_name))
    {
        printf("Access was denied. Return NULL\n");
        parse_data_delete(res);
        return NULL;
    }

    if (!check_by_crc32(src_array, total_len, data_size, res->crc32_control_sum))
    {
        g_trace.crc32_control = ERROR;
        printf("Incorrect CRC32. Return NULL\n");
        parse_data_delete(res);
        return NULL;
    }
    g_trace.crc32_control = PASSED;

    timestamp_t current_time = (timestamp_t)time(NULL);

    // printf("Time check: current %lu, input %lu\n", current_time, res->timestamp);

    if (res->timestamp < current_time)
    {
        g_trace.timestamp_control = ERROR;
        printf("Timestamp has expired. Return NULL\n");
        parse_data_delete(res);
        return NULL;
    }
    g_trace.timestamp_control = PASSED;

    return res;
}

#endif