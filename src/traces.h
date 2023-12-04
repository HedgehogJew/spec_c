#ifndef TRACES_H
#define TRACES_H

#include <stdint.h>

#define NONE 0
#define ERROR 1
#define PASSED 2

/* Структура для отслеживания контрольных точек. Не определена в спецификации.*/

struct trace_p
{
    uint8_t program_begin;
    uint8_t program_end;

    uint8_t is_input_file_open;
    uint8_t open_file_memory_allocation;

    uint8_t parse_package_correction_args;
    uint8_t parse_package_correction_author_type;
    uint8_t parse_package_memory_allocate_for_structure;

    uint8_t parse_author_section_correction_args;
    uint8_t parse_author_section_correction_section_type;
    uint8_t parse_author_section_crc16_control;

    uint8_t parse_data_section_correction_args;
    uint8_t parse_data_section_correction_section_type;
    uint8_t parse_data_section_memory_allocate;
    uint8_t parse_data_section_crc16_control;

    uint8_t parse_control_section_correction_args;
    uint8_t parse_control_section_correction_section_type;
    uint8_t parse_control_section_crc16_control;

    uint8_t is_acess_file_open;
    uint8_t author_have_access;

    uint8_t crc32_control;
    uint8_t timestamp_control;
};

extern struct trace_p g_trace;

void print_trace(struct trace_p *trace);

#endif // TRACES_H