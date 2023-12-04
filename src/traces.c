#ifndef __TRACES__
#define __TRACES__

#include <stdint.h>
#include <stdio.h>

#include "traces.h"

struct trace_p g_trace = {
    .program_begin = NONE,
    .program_end = NONE,

    .is_input_file_open = NONE,

    .parse_package_correction_args = NONE,
    .parse_package_correction_author_type = NONE,
    .parse_package_memory_allocate_for_structure = NONE,

    .parse_author_section_correction_args = NONE,
    .parse_author_section_correction_section_type = NONE,
    .parse_author_section_crc16_control = NONE,

    .parse_data_section_correction_args = NONE,
    .parse_data_section_correction_section_type = NONE,
    .parse_data_section_memory_allocate = NONE,
    .parse_data_section_crc16_control = NONE,

    .parse_control_section_correction_args = NONE,
    .parse_control_section_correction_section_type = NONE,
    .parse_control_section_crc16_control = NONE,

    .is_acess_file_open = NONE,
    .author_have_access = NONE,

    .crc32_control = NONE,
    .timestamp_control = NONE};

// Функция для вывода полей структуры trace_p
void print_trace(struct trace_p *trace)
{
    printf("NONE = %d\nERROR = %d\nPASSED = %d\n\n", NONE, ERROR, PASSED);

    printf("program_begin: %u\n", trace->program_begin);
    printf("program_end: %u\n", trace->program_end);

    printf("is_input_file_open: %u\n", trace->is_input_file_open);
    printf("parse_package_correction_args: %u\n", trace->parse_package_correction_args);
    printf("parse_package_correction_author_type: %u\n", trace->parse_package_correction_author_type);
    printf("parse_package_memory_allocate_for_structure: %u\n", trace->parse_package_memory_allocate_for_structure);

    printf("parse_author_section_correction_args: %u\n", trace->parse_author_section_correction_args);
    printf("parse_author_section_correction_section_type: %u\n", trace->parse_author_section_correction_section_type);
    printf("parse_author_section_crc16_control: %u\n", trace->parse_author_section_crc16_control);

    printf("parse_data_section_correction_args: %u\n", trace->parse_data_section_correction_args);
    printf("parse_data_section_correction_section_type: %u\n", trace->parse_data_section_correction_section_type);
    printf("parse_data_section_memory_allocate: %u\n", trace->parse_data_section_memory_allocate);
    printf("parse_data_section_crc16_control: %u\n", trace->parse_data_section_crc16_control);

    printf("parse_control_section_correction_args: %u\n", trace->parse_control_section_correction_args);
    printf("parse_control_section_correction_section_type: %u\n", trace->parse_control_section_correction_section_type);
    printf("parse_control_section_crc16_control: %u\n", trace->parse_control_section_crc16_control);

    printf("is_acess_file_open: %u\n", trace->is_acess_file_open);
    printf("author_have_access: %u\n", trace->author_have_access);

    printf("crc32_control: %u\n", trace->crc32_control);
    printf("timestamp_control: %u\n", trace->timestamp_control);
}

#endif