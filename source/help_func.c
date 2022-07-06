#include "help_func.h"
#include "callback.h"

// used for countdown process
int TIME_LIMIT_ALLSEC;
// tmp var
int diff;
int diff_t;

void time_diff_count_down()
{
    diff = clock_cur_totalsec - clock_base_totalsec;
    diff_t = TIME_LIMIT_ALLSEC - diff;
    if (diff_t < 0)
    {
        diff_t = (60 + diff_t) % 60;
        TIME_LIMIT_ALLSEC = diff_t + diff;
    }
    time_diff_tmp.hour = diff_t / 3600;
    time_diff_tmp.minute = diff_t % 3600 / 60;
    time_diff_tmp.second = diff_t % 60;
}

void delay_ms(uint n)
{
    while (n)
    {
        uchar i, j;
        i = 11;
        j = 190;
        do
        {
            while (--j)
                ;
        } while (--i);
        n--;
    }
}