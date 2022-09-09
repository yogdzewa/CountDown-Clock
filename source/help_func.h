#ifndef _HELP_FUNC_
#define _HELP_FUNC_
#include "common_include.h"
typedef struct
{
    uchar hour;
    uchar minute;
    uchar second;
} struct_time_diff;
#define CLOCK_CUR 0
#define CLOCK_BASE 1

void time_diff_count_down();
void clock_read_2sec(bit t);
void delay_ms(uint n);

#endif