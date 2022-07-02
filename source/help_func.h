#ifndef _HELP_FUNC_
#define _HELP_FUNC_
#include "common_include.h"
typedef struct
{
    uchar hour;
    uchar minute;
    uchar second;
} struct_time_diff;
extern xdata uchar TIME_REST_M;

void time_diff_count_down(struct_DS1302_RTC, struct_DS1302_RTC);
void delay_ms(uint n);

#endif