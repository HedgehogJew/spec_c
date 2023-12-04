#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include "traces.h"
#include "filework.h"
#include "parser.h"

#ifndef __MAIN__
#define __MAIN__

#define READ_FROM "example3.bin"

/*
OpenFile()  Ошибка 2.1
    \/ _out (TRUE)
    \/ _out (FALSE)
*/
int func()
{
    uint8_t *input = read_array_from_file(READ_FROM, 2114);
    if (input != NULL)
    {
        g_trace.is_input_file_open = PASSED;

        Parsed_data *res = parse_package(input);
        print_parsed_data(res);
        parse_data_delete(res);
    }
    else
    {
        g_trace.is_input_file_open = ERROR;
    }
}

int main()
{
    g_trace.program_begin = PASSED;

    func();

    g_trace.program_end = PASSED;

    print_trace(&g_trace);

    return 0;
}

#endif