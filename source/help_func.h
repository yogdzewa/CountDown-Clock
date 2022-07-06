#ifndef _HELP_FUNC_
#define _HELP_FUNC_
#include "common_include.h"
typedef struct
{
    uchar hour;
    uchar minute;
    uchar second;
} struct_time_diff;


void time_diff_count_down();
void delay_ms(uint n);

#endif