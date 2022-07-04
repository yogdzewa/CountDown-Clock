#ifndef _HELP_FUNC_
#define _HELP_FUNC_
#include "common_include.h"
typedef struct
{
    uchar hour;
    uchar minute;
    uchar second;
} struct_time_diff;
extern XDATA uchar TIME_RELD_H;
extern XDATA uchar TIME_RELD_M;
extern XDATA uchar TIME_RELD_S;
extern XDATA uchar TIME_REST_M;

void time_diff_count_down();
void delay_ms(uint n);

#endif